#ifndef  SET4LIBINT_HH
#define  SET4LIBINT_HH

#include "AbstractInterp4Command.hh"
#include "LibInterface.hh"
#include <memory>

/*!
	Definicja klasy Set4LibInterface
 */

class Set4LibInterface {

  std::map<std::string,std::shared_ptr<LibInterface>> _LibMap;
  
 public:
 
  Set4LibInterface();

  bool AddPlugin(const std::string& LibName);

  std::shared_ptr<LibInterface> FindPlugin(const std::string& CmdName);
 
 };



#endif
