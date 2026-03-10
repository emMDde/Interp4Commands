#include <iostream>
#include <fstream>
#include <string>   
#include <dlfcn.h>
#include <unistd.h>
#include <cassert>
#include "ProgramInterpreter.hh"


using namespace std;


int main(int argc, char* argv[])
{
  if(argc!=3)
  {
    cerr << "Blad wywolania programu!" << endl;
    cerr << "Poprawne uzycie: " << argv[0] << " [plik_konfiguracyjny.xml] [plik_skryptu]" << endl;
    return 1;
  }

  ProgramInterpreter Interp;
  if(!Interp.Read_XML_Config(argv[1])) return 1;
  if(!Interp.ExecProgram(argv[2])) return 1;

  return 0;
}
