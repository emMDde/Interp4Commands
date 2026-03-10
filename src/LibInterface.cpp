#include <iostream>
#include <stdexcept>
#include "LibInterface.hh"

using std::cout;
using std::cerr;
using std::endl;
using std::runtime_error;


LibInterface::LibInterface(const std::string& LibName)
{
  _LibHandler = dlopen(LibName.c_str(),RTLD_LAZY);
  void *pFun;

  if (!_LibHandler) {
    throw runtime_error("!!! Brak biblioteki: " + LibName);
  }

  pFun = dlsym(_LibHandler,"CreateCmd");
  if (!pFun) {
    dlclose(_LibHandler);
    throw runtime_error("!!! Nie znaleziono funkcji CreateCmd");
  }
  _pCreateInterp = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);

  pFun = dlsym(_LibHandler,"GetCmdName");
  if (!pFun) {
    dlclose(_LibHandler);
    throw runtime_error("!!! Nie znaleziono funkcji GetCmdName");
  }
  _CmdName = reinterpret_cast<const char* (*)(void)>(pFun)();
  
  cout << ">>> Wczytano biblioteke polecenia: "<< _CmdName << endl;
}

LibInterface::~LibInterface()
{
  if(_LibHandler) 
  {
     dlclose(_LibHandler);
  }
}

const std::string& LibInterface::GetCmdName() const 
{ 
  return _CmdName; 
}

AbstractInterp4Command * LibInterface::CreateCmd() const
{
  return _pCreateInterp();
}
