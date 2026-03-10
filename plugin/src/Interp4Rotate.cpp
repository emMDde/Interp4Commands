#include <iostream>
#include "Interp4Rotate.hh"
#include "Fps.hh"


using std::cout;
using std::cerr;
using std::endl;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Rotate"; }
}




/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Rotate::CreateCmd();
}


/*!
 *  
 */
Interp4Rotate::Interp4Rotate(): _AxisName(""), _AxisVel(0), _RotAngle(0)
{}


/*!
 *
 */
void Interp4Rotate::PrintCmd() const
{
  cout << GetCmdName() << " " << _AxisName << " " << _AxisVel << " " << _RotAngle << endl;
}


/*!
 *
 */
const char* Interp4Rotate::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
bool Interp4Rotate::ExecCmd( AbstractScene      &rScn, 
                           const char         *sMobObjName,
			   AbstractComChannel &rComChann
			 )
{
  std::shared_ptr<AbstractMobileObj> pObj=rScn.FindMobileObj(sMobObjName);
  if(pObj==nullptr) return false;
  if(_RotAngle==0 || _AxisVel==0) return true;

  double time=_RotAngle/std::abs((double)_AxisVel);
  int steps=(int)(time*FPS);
  if(steps==0) steps=1;
  double step_angle=_RotAngle/steps;
  if(_AxisVel<0) step_angle=-step_angle;

  for(int i=0; i<steps; ++i)
  {
    pObj->LockObj();

    if(_AxisName=="OX") pObj->SetAng_Roll_deg(pObj->GetAng_Roll_deg()+step_angle);
    if(_AxisName=="OY") pObj->SetAng_Pitch_deg(pObj->GetAng_Pitch_deg()+step_angle);
    if(_AxisName=="OZ") pObj->SetAng_Yaw_deg(pObj->GetAng_Yaw_deg()+step_angle);
    
    rComChann.UpdateServerObject(*pObj);

    pObj->UnlockObj();
    usleep(WAIT_TIME);
  }
  
  return true;
}


/*!
 *
 */
bool Interp4Rotate::ReadParams(std::istream& Strm_CmdsList)
{
  if(!(Strm_CmdsList >> _AxisName) || (_AxisName!="OX" && _AxisName!="OY" && _AxisName!="OZ")){
    cerr << "Blad: Parametr 'NazwaOsi' musi byc tekstem!" << endl;
    return false;
  }

  if(!(Strm_CmdsList >> _AxisVel)){
    cerr << "Blad: Parametr 'SzybkoscKatowa' musi byc liczba!" << endl;
    return false;
  }

  if(!(Strm_CmdsList >> _RotAngle) || _RotAngle<0){
    cerr << "Blad: Parametr 'KatObrotu' musi byc liczba nieujemna!" << endl;
    return false;
  }

  return true;
}

/*!
 *
 */
void Interp4Rotate::PrintParams() const
{
  cout << "Parametry polecenia " << GetCmdName() << ": NazwaOsi:" << _AxisName << " SzybkoscKatowa:" << _AxisVel << " KatObrotu:" << _RotAngle << endl;
}


/*!
 *
 */
AbstractInterp4Command* Interp4Rotate::CreateCmd()
{
  return new Interp4Rotate();
}


/*!
 *
 */
void Interp4Rotate::PrintSyntax() const
{
  cout << "   Rotate  NazwaObiektu  NazwaOsi  SzybkoscKatowa  KatObrotu" << endl;
}
