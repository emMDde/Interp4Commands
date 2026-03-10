#include <iostream>
#include "Interp4Set.hh"


using std::cout;
using std::cerr;
using std::endl;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Set"; }
}




/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Set::CreateCmd();
}


/*!
 *
 */
Interp4Set::Interp4Set(): _X(0), _Y(0), _Z(0), _XAngle(0), _YAngle(0), _ZAngle(0)
{}


/*!
 *
 */
void Interp4Set::PrintCmd() const
{
  cout << GetCmdName() << " " << _X  << " " << _Y << " " << _Z << " " << _XAngle << " " << _YAngle << " " << _ZAngle << endl;
}


/*!
 *
 */
const char* Interp4Set::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
bool Interp4Set::ExecCmd( AbstractScene      &rScn, 
                           const char         *sMobObjName,
			   AbstractComChannel &rComChann
			 )
{
  std::shared_ptr<AbstractMobileObj> pObj=rScn.FindMobileObj(sMobObjName);
  if(pObj==nullptr) return false;

  pObj->LockObj();

  Vector3D position;
  position[0]=_X; position[1]=_Y; position[2]=_Z;
  pObj->SetTrans_m(position);
  pObj->SetAng_Roll_deg(_XAngle);
  pObj->SetAng_Pitch_deg(_YAngle);
  pObj->SetAng_Yaw_deg(_ZAngle);
  
  rComChann.UpdateServerObject(*pObj);
  
  pObj->UnlockObj();

  return true;
}


/*!
 *
 */
bool Interp4Set::ReadParams(std::istream& Strm_CmdsList)
{
  if(!(Strm_CmdsList >> _X)){
    cerr << "Blad: Parametr 'Wsp_X' musi byc liczba!" << endl;
    return false;
  }

  if(!(Strm_CmdsList >> _Y)){
    cerr << "Blad: Parametr 'Wsp_Y' musi byc liczba!" << endl;
    return false;
  }

  if(!(Strm_CmdsList >> _Z)){
    cerr << "Blad: Parametr 'Wsp_Z' musi byc liczba!" << endl;
    return false;
  }

  if(!(Strm_CmdsList >> _XAngle)){
    cerr << "Blad: Parametr 'Kat_OX' musi byc liczba!" << endl;
    return false;
  }

  if(!(Strm_CmdsList >> _YAngle)){
    cerr << "Blad: Parametr 'Kat_OY' musi byc liczba!" << endl;
    return false;
  }

  if(!(Strm_CmdsList >> _ZAngle)){
    cerr << "Blad: Parametr 'Kat_OZ' musi byc liczba!" << endl;
    return false;
  }

  return true;
}

/*!
 *
 */
void Interp4Set::PrintParams() const
{
  cout << "Parametry polecenia " << GetCmdName() << ": Wsp_X:" << _X << " Wsp_Y:" << _Y << " Wsp_Z:" << _Z << " Kat_OX:" << _XAngle << " Kat_OY:" 
       << _YAngle << " Kat_OZ:" << _ZAngle << endl;
}

/*!
 *
 */
AbstractInterp4Command* Interp4Set::CreateCmd()
{
  return new Interp4Set();
}


/*!
 *
 */
void Interp4Set::PrintSyntax() const
{
  cout << "   Set NazwaObiektu Wsp_X Wsp_Y Wsp_Z Kat_OX Kat_OY Kat_OZ" << endl;
}
