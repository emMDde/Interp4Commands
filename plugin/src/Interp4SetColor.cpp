#include <iostream>
#include "Interp4SetColor.hh"
#include "geomVector.hh"


using std::cout;
using std::cerr;
using std::endl;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "SetColor"; }
}




/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4SetColor::CreateCmd();
}


/*!
 *
 */
Interp4SetColor::Interp4SetColor()
{
  _Color[0]=0; _Color[1]=0; _Color[2]=0;
}


/*!
 *
 */
void Interp4SetColor::PrintCmd() const
{
  cout << GetCmdName() << " " << _Color << endl;
}


/*!
 *
 */
const char* Interp4SetColor::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
bool Interp4SetColor::ExecCmd( AbstractScene      &rScn, 
                           const char         *sMobObjName,
			   AbstractComChannel &rComChann
			 )
{
  std::shared_ptr<AbstractMobileObj> pObj=rScn.FindMobileObj(sMobObjName);
  if(pObj==nullptr) return false;

  pObj->LockObj();

  pObj->SetColor(_Color);
  rComChann.UpdateServerObject(*pObj);
  
  pObj->UnlockObj();

  return true;
}


/*!
 *
 */
bool Interp4SetColor::ReadParams(std::istream& Strm_CmdsList)
{
  if(!(Strm_CmdsList >> _Color)){
    cerr << "Blad: Parametr 'Color' musi byc wektorem liczb w formacie RGB!" << endl;
    return false;
  }
  
  if(_Color[0] > 255 || _Color[1] > 255 || _Color[2] > 255) {
     cerr << "Blad: Wartosci koloru musza byc z zakresu 0-255!" << endl;
     return false;
  }

  return true;
}

/*!
 *
 */
void Interp4SetColor::PrintParams() const
{
  cout << "Parametry polecenia " << GetCmdName() << ": Kolor:" << _Color << endl;
}



/*!
 *
 */
AbstractInterp4Command* Interp4SetColor::CreateCmd()
{
  return new Interp4SetColor();
}


/*!
 *
 */
void Interp4SetColor::PrintSyntax() const
{
  cout << "   SetColor  Kolor" << endl;
}
