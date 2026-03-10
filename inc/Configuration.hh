#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include "AbstractMobileObj.hh"
#include <string>
#include <list>

class Configuration {
  public:

    std::list<std::string> _LibraryList;
    std::list<AbstractMobileObj*> _MobileObjects;

};


#endif