#ifndef  SCENE_HH
#define  SCENE_HH

#include "AbstractScene.hh"
#include <map>
#include <memory>

/*!
	Definicja klasy Scene
 */

class Scene : public AbstractScene {

  std::map<std::string,std::shared_ptr<AbstractMobileObj>> _Set_MobileObjs;
   
 public:

  Scene();
 
  virtual std::shared_ptr<AbstractMobileObj>  FindMobileObj(const char *sName) override;

  virtual void AddMobileObj(AbstractMobileObj *pMobObj) override;
 
  const std::map<std::string, std::shared_ptr<AbstractMobileObj>>& GetObjects() const;

 };


#endif