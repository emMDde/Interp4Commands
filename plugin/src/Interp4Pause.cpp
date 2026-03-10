#include <iostream>
#include "Interp4Pause.hh"


using std::cout;
using std::cerr;
using std::endl;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Pause"; }
}




/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Pause::CreateCmd();
}


/*!
 *
 */
Interp4Pause::Interp4Pause(): _Time_ms(0)
{}


/*!
 *
 */
void Interp4Pause::PrintCmd() const
{
  cout << GetCmdName() << " " << _Time_ms << endl;
}


/*!
 *
 */
const char* Interp4Pause::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
bool Interp4Pause::ExecCmd( AbstractScene      &rScn, 
                           const char         *sMobObjName,
			   AbstractComChannel &rComChann
			 )
{
  rComChann.LockAccess();
  usleep(_Time_ms*1000);
  rComChann.UnlockAccess();

  return true;
}


/*!
 *
 */
bool Interp4Pause::ReadParams(std::istream& Strm_CmdsList)
{
  if(!(Strm_CmdsList >> _Time_ms) || _Time_ms<=0){
    cerr << "Blad: Parametr 'CzasPauzy[ms]' musi byc liczba dodatnia!" << endl;
    return false;
  }
  
  return true;
}

/*!
 *
 */
void Interp4Pause::PrintParams() const
{
  cout << "Parametry polecenia " << GetCmdName() << ": CzasPauzy[ms]:" << _Time_ms << endl;
}



/*!
 *
 */
AbstractInterp4Command* Interp4Pause::CreateCmd()
{
  return new Interp4Pause();
}


/*!
 *
 */
void Interp4Pause::PrintSyntax() const
{
  cout << "   Pause  CzasPauzy[ms]" << endl;
}
