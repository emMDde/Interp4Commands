#include <iostream>
#include "Set4LibInterface.hh"

using std::cout;
using std::cerr;
using std::endl;

Set4LibInterface::Set4LibInterface()
{}
  
bool Set4LibInterface::AddPlugin(const std::string& LibName)
{
  try
  {
    std::shared_ptr<LibInterface> newLib = std::make_shared<LibInterface>(LibName);
    std::string cmdName = newLib->GetCmdName();
   
    _LibMap.emplace(cmdName,newLib);
    return true;
  }
  catch(const std::exception& error)
  {
    cerr << error.what() << endl;
    return false;
  }
}

std::shared_ptr<LibInterface> Set4LibInterface::FindPlugin(const std::string& CmdName)
{
  std::map<std::string,std::shared_ptr<LibInterface>>::iterator iter = _LibMap.find(CmdName);
  if(iter == _LibMap.end())
  {
    //cerr << " Nie znaleziono pluginu! " << endl;
    return nullptr;
  }
  //cout << " Znalazlem plugin: " << CmdName << endl;
  return iter->second;
}
