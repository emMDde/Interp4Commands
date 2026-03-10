#ifndef  PROGINTERP_HH
#define  PROGINTERP_HH

#define LINE_SIZE 500

#include <list>
#include <thread>
#include <atomic>
#include "Set4LibInterface.hh"
#include "Scene.hh"
#include "ComChannel.hh"
#include "XMLInterp4Config.hh"

/*!
	Definicja klasy ProgramInterpreter
 */

class ProgramInterpreter {

  Set4LibInterface _LibManager;
  Scene _Scn;
  ComChannel _Chann2Serv;
  std::atomic<bool> _tException;

  bool ReadXmlFile(const char* sFileName, Configuration &rConfig);

  bool ReadCmdFile(const char* FileName_Prog, std::ostringstream & OTmpStrm);

  void InitServerScene();

  std::string LoadBlock(std::istream& IStrm, const std::string& EndKey, const std::string& StartKey);

  void ProcessCommands(std::istream& ITmpStrm, bool isParallel);

  void ExecCommand(std::istream& IStrm, const std::string& cmdName, bool isParallel, std::list<std::thread>& threads);
  
 public:
 
  ProgramInterpreter();

  bool Read_XML_Config(const char* FileName);

  bool ExecProgram(const char* FileName_Prog);
 
 };



#endif
