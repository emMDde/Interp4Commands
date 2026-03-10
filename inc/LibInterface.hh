#ifndef  LIBINT_HH
#define  LIBINT_HH

#include "AbstractInterp4Command.hh"
#include <map>
#include <dlfcn.h>

/*!
	Definicja klasy LibInterface
 */

class LibInterface {

  void * _LibHandler;
  std::string _CmdName;
  AbstractInterp4Command *(*_pCreateInterp)(void);
  
 public:
 
  LibInterface(const std::string& LibName);

  ~LibInterface();

  const std::string& GetCmdName() const;
  
  AbstractInterp4Command * CreateCmd() const;
 
 
 };



#endif
