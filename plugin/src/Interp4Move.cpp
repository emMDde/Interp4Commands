#include <iostream>
#include "Interp4Move.hh"
#include "Fps.hh"


using std::cout;
using std::cerr;
using std::endl;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Move"; }
}




/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Move::CreateCmd();
}


/*!
 *
 */
Interp4Move::Interp4Move(): _Speed_mmS(0), _Length_m(0)
{}


/*!
 *
 */
void Interp4Move::PrintCmd() const
{
  cout << GetCmdName() << " " << _Speed_mmS << " " << _Length_m << endl;
}


/*!
 *
 */
const char* Interp4Move::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
bool Interp4Move::ExecCmd( AbstractScene      &rScn, 
                           const char         *sMobObjName,
			   AbstractComChannel &rComChann
			 )
{
  std::shared_ptr<AbstractMobileObj> pObj=rScn.FindMobileObj(sMobObjName);
  if(pObj==nullptr) return false;
  if(_Length_m==0 || _Speed_mmS==0) return true;

  double time=_Length_m/std::abs((double)_Speed_mmS);
  int steps=(int)(time*FPS);
  if(steps==0) steps=1;
  double step_len=_Length_m/steps;
  if(_Speed_mmS<0) step_len=-step_len;

  for(int i=0; i<steps; ++i)
  {
    pObj->LockObj();

    double pitch=DEG2RAD(pObj->GetAng_Pitch_deg());
    double yaw=DEG2RAD(pObj->GetAng_Yaw_deg());
    Vector3D position=pObj->GetTrans_m();
    position[0]+=step_len*cos(pitch)*cos(yaw);
    position[1]+=step_len*cos(pitch)*sin(yaw);
    position[2]+=step_len*sin(pitch);
    pObj->SetTrans_m(position);

    rComChann.UpdateServerObject(*pObj);

    pObj->UnlockObj();
    usleep(WAIT_TIME);
  }

  return true;
}


/*!
 *
 */
bool Interp4Move::ReadParams(std::istream& Strm_CmdsList)
{
  if(!(Strm_CmdsList >> _Speed_mmS)){
    cerr << "Blad: Parametr 'Szybkosc[m/s]' musi byc liczba!" << endl;
    return false;
  }

  if(!(Strm_CmdsList >> _Length_m) || _Length_m<0){
    cerr << "Blad: Parametr 'DlugoscDrogi[m]' musi byc liczba dodatnia!" << endl;
    return false;
  }

  return true;
}

/*!
 *
 */
void Interp4Move::PrintParams() const
{
  cout << "Parametry polecenia " << GetCmdName() << ": Szybkosc[m/s]:" << _Speed_mmS << " DlugoscDrogi[m]:" << _Length_m << endl;
}


/*!
 *
 */
AbstractInterp4Command* Interp4Move::CreateCmd()
{
  return new Interp4Move();
}


/*!
 *
 */
void Interp4Move::PrintSyntax() const
{
  cout << "   Move  NazwaObiektu  Szybkosc[m/s]  DlugoscDrogi[m]" << endl;
}
