#include <iostream>
#include "Scene.hh"


Scene::Scene()
{}

std::shared_ptr<AbstractMobileObj>  Scene::FindMobileObj(const char *sName)
{
  std::map<std::string,std::shared_ptr<AbstractMobileObj>>::iterator iter = _Set_MobileObjs.find(sName);
  if(iter == _Set_MobileObjs.end())
  {
    std::cout << " Nie znaleziono obiektu! " << std::endl;
    return nullptr;
  }
  //cout << " Znalazlem obiekt: " << sName << endl;
  return iter->second;
}

void Scene::AddMobileObj(AbstractMobileObj *pMobObj)
{
    std::shared_ptr<AbstractMobileObj> pObj(pMobObj);
    _Set_MobileObjs.emplace(pObj->GetName(),pObj);
    //cout << " Dodano obiekt: "<< *pObj << endl;
}

const std::map<std::string, std::shared_ptr<AbstractMobileObj>>& Scene::GetObjects() const
{
  return _Set_MobileObjs;
}