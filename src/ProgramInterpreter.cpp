#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdexcept>
#include <list>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>
#include <thread>
#include <filesystem>
#include <functional>

#include "ProgramInterpreter.hh"
#include "Port.hh"


using namespace std;
using namespace xercesc;


static SAX2XMLReader* BuildParser()
{
  SAX2XMLReader* pParser = XMLReaderFactory::createXMLReader();

  pParser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
  pParser->setFeature(XMLUni::fgSAX2CoreValidation, true);
  pParser->setFeature(XMLUni::fgXercesDynamic, false);
  pParser->setFeature(XMLUni::fgXercesSchema, true);
  pParser->setFeature(XMLUni::fgXercesSchemaFullChecking, true);
  pParser->setFeature(XMLUni::fgXercesValidationErrorAsFatal, true);

  return pParser;
}

bool ProgramInterpreter::ReadXmlFile(const char* sFileName, Configuration &rConfig)
{
  try { 
    XMLPlatformUtils::Initialize(); 
  }
  catch(const XMLException& error) {
    char* message = XMLString::transcode(error.getMessage());
    cerr << "Error during initialization! :\n";
    cerr << "Exception message is: \n"
      << message << "\n";
    XMLString::release(&message);
    return false;
  }

  SAX2XMLReader* pParser=BuildParser();
  XMLInterp4Config Handler(rConfig);
  pParser->setContentHandler(&Handler);
  pParser->setErrorHandler(&Handler);
  bool result=true;

  try {
    filesystem::path gramFile(sFileName);
    gramFile.replace_extension(".xsd");
    if(!pParser->loadGrammar(gramFile.string().c_str(), Grammar::SchemaGrammarType,true)) throw runtime_error("!!! Plik "+gramFile.string()+"\n !!! ktory zawiera opis gramatyki, nie moze zostac wczytany.");
    
    pParser->setFeature(XMLUni::fgXercesUseCachedGrammarInParse,true);
    pParser->parse(sFileName);
  }
  catch(const XMLException& Exception) {
    char* sMessage = XMLString::transcode(Exception.getMessage());
    cerr << "Informacja o wyjatku: \n"
      << "   " << sMessage << "\n";
    XMLString::release(&sMessage);
    result=false;
  }
  catch(const exception& error){
    cerr << error.what() << endl;
    result=false;
  }
  catch (...) {
    cout << "Zgloszony zostal nieoczekiwany wyjatek!\n" ;
    result=false;
  }

  delete pParser;
  return result;
}

bool ProgramInterpreter::ReadCmdFile(const char* FileName_Prog, ostringstream & OTmpStrm)
{
  if(!filesystem::exists(FileName_Prog)) 
  {
    cerr << "Blad: Plik '" << FileName_Prog << "' nie istnieje!" << endl;
    return false;
  }
  string Cmd4Preproc = "cpp -P ";
  char Line[LINE_SIZE];
  Cmd4Preproc += FileName_Prog;
  FILE* pProc = popen(Cmd4Preproc.c_str(),"r");
  
  if(!pProc)
  {
    cerr << "Blad deskryptora pliku!" << endl;
    return false;
  } 

  while(fgets(Line,LINE_SIZE,pProc)) 
  {
    OTmpStrm << Line;
  }

  pclose(pProc);
  return true;
}

void ProgramInterpreter::InitServerScene()
{
  _Chann2Serv.ClearServerScene();
  for(const std::pair<std::string,std::shared_ptr<AbstractMobileObj>> & iter : _Scn.GetObjects())
  {
    _Chann2Serv.AddServerObject(*(iter.second));
  }
}

string ProgramInterpreter::LoadBlock(std::istream& IStrm, const std::string& EndKey, const std::string& StartKey)
{
  std::string word;
  std::stringstream CStrm;
  int nesting = 1;

  while(IStrm >> word) {
    if (word == StartKey) nesting++;
    else if (word == EndKey) nesting--;

    if (nesting == 0) return CStrm.str();
    CStrm << word << " ";
  }
  throw runtime_error("Blad: Brak zamkniecia bloku "+EndKey+"!");
}

void ProgramInterpreter::ProcessCommands(std::istream& ITmpStrm, bool isParallel)
{
  string word;
  std::list<std::thread> localThreads;

  try 
  {
    while(ITmpStrm >> word && !_tException)
    {
      if(word=="Begin_Parallel_Actions" || word=="Begin_Sequential_Actions") 
      {
        bool nextModeIsParallel=(word=="Begin_Parallel_Actions");
        string endKey=nextModeIsParallel ? "End_Parallel_Actions" : "End_Sequential_Actions";
        string content=LoadBlock(ITmpStrm, endKey, word); 
        
        std::function<void()> task=[this, content, nextModeIsParallel](){
            try{
                std::stringstream CStrm(content);
                ProcessCommands(CStrm, nextModeIsParallel);
            } catch (...){
                _tException=true; 
            }
        };

        if(isParallel) localThreads.push_back(thread(task));
        else task();
        continue;
      }
      ExecCommand(ITmpStrm, word, isParallel, localThreads);
    }
  }
  catch(...) 
  {
    for(std::thread &thread : localThreads) if(thread.joinable()) thread.join();
    throw; 
  }

  for(std::thread &thread : localThreads) if(thread.joinable()) thread.join();
  if(_tException) throw invalid_argument("Wystapil blad w trakcie wykonywania programu!");
}

void ProgramInterpreter::ExecCommand(std::istream& IStrm, const std::string& cmdName, bool isParallel, std::list<std::thread>& threads)
{
  std::shared_ptr<LibInterface> LInter = _LibManager.FindPlugin(cmdName);
  if(LInter==nullptr) throw invalid_argument("Blad: Nie znaleziono wtyczki '"+cmdName+"'!");

  std::shared_ptr<AbstractInterp4Command> plugin(LInter->CreateCmd());
  if(plugin==nullptr) throw invalid_argument("Nie udalo sie utworzyc instancji interpretera!");

  string objName;
  if(cmdName!="Pause" && !(IStrm >> objName)) throw invalid_argument("Nie udalo sie wczytac nazwy obiektu!");
  if(!(plugin->ReadParams(IStrm))) throw invalid_argument("Nie udalo sie wczytac parametrow interpretera!");
  plugin->PrintCmd();

  if(isParallel)
  {
    threads.push_back(thread([plugin, this, objName](){ if(!(plugin->ExecCmd(_Scn, objName.c_str(), _Chann2Serv))) { _tException=true; } }));
    return;
  }

  if(!(plugin->ExecCmd(_Scn, objName.c_str(), _Chann2Serv))) throw invalid_argument("Nie udalo sie wykonac polecenia interpretera!");
}

ProgramInterpreter::ProgramInterpreter() : _tException(false)
{}

bool ProgramInterpreter::Read_XML_Config(const char* FileName)
{
  Configuration config;

  if(!(ReadXmlFile(FileName, config)))
  {
    cerr << "Nie udalo sie poprawnie wczytac pliku " << FileName << endl;
    return false;
  }

  for(const std::string& libName : config._LibraryList)
  {
    if(!(_LibManager.AddPlugin(libName)))
    {
      cerr << "Nie udalo sie dodac pluginu " << libName << endl;
      for(AbstractMobileObj * pObj : config._MobileObjects) delete pObj;
      return false;
    }
  }

  for(AbstractMobileObj * pObj : config._MobileObjects) _Scn.AddMobileObj(pObj);

  return true;
}

bool ProgramInterpreter::ExecProgram(const char* FileName_Prog)
{
  ostringstream OTmpStrm;
  if(!ReadCmdFile(FileName_Prog,OTmpStrm)) return false;

  if(!_Chann2Serv.OpenConnection(PORT))
  {
    cerr << ">>> Nie udalo sie skomunikowac z serwerem! <<<" << endl;
    return false;
  }

  InitServerScene();
  bool result=true;
  istringstream ITmpStrm;
  ITmpStrm.str(OTmpStrm.str());
  cout << "\nProcesowany program: " << FileName_Prog << endl;

  try
  {
    ProcessCommands(ITmpStrm, false);
  }
  catch(const exception& error)
  {
    cerr << error.what() << endl;
    result=false;
  }

  _Chann2Serv.CloseConnection();
  return result;
}