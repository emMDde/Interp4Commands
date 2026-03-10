#ifndef CUBOID_HH
#define CUBOID_HH

#include "AbstractMobileObj.hh"
#include <mutex>

class Cuboid : public AbstractMobileObj {

    std::mutex _Guard;
    std::string _Name;  
    Vector3D _Trans_m;  
    Vector3D _Shift;    
    Vector3D _Scale;   
    VectorRGB _Color;    
    
    double _Ang_Roll_deg;
    double _Ang_Pitch_deg; 
    double _Ang_Yaw_deg;
    
  public:
        
    Cuboid();
      
    virtual double GetAng_Roll_deg() const override;

    virtual double GetAng_Pitch_deg() const override;

    virtual double GetAng_Yaw_deg() const override;

    virtual void SetAng_Roll_deg(double Ang_Roll_deg) override;

    virtual void SetAng_Pitch_deg(double Ang_Pitch_deg) override;

    virtual void SetAng_Yaw_deg(double Ang_Yaw_deg) override;

    virtual const Vector3D & GetTrans_m() const override;

    virtual void SetTrans_m(const Vector3D &trans_m) override;

    virtual const std::string & GetName() const override;

    virtual void SetName(const char* sName) override;

    virtual const Vector3D& GetScale() const override;

    virtual void SetScale(const Vector3D& scale) override;

    virtual const Vector3D& GetShift() const override;

    virtual void SetShift(const Vector3D& shift) override;

    virtual const VectorRGB& GetColor() const override;

    virtual void SetColor(const VectorRGB& color) override;

    virtual void LockObj() override;

    virtual void UnlockObj() override;

};



#endif