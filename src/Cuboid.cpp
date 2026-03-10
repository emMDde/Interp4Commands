#include "Cuboid.hh"

Cuboid::Cuboid() : _Name(""), _Ang_Roll_deg(0), _Ang_Pitch_deg(0), _Ang_Yaw_deg(0)
{
    _Scale[0] = 1; _Scale[1] = 1; _Scale[2] = 1;
    _Color[0] = 128; _Color[1] = 128; _Color[2] = 128;
    _Shift[0] = 0; _Shift[1] = 0; _Shift[2] = 0;
    _Trans_m[0] = 0; _Trans_m[1] = 0; _Trans_m[2] = 0;
}

double Cuboid::GetAng_Roll_deg() const
{
    return _Ang_Roll_deg;
}

double Cuboid::GetAng_Pitch_deg() const
{
    return _Ang_Pitch_deg;
}

double Cuboid::GetAng_Yaw_deg() const 
{
    return _Ang_Yaw_deg;
}

void Cuboid::SetAng_Roll_deg(double Ang_Roll_deg) 
{
    _Ang_Roll_deg=Ang_Roll_deg;
}

void Cuboid::SetAng_Pitch_deg(double Ang_Pitch_deg)
{
    _Ang_Pitch_deg=Ang_Pitch_deg;
}

void Cuboid::SetAng_Yaw_deg(double Ang_Yaw_deg) 
{
    _Ang_Yaw_deg=Ang_Yaw_deg;
}

const Vector3D & Cuboid::GetTrans_m() const
{
    return _Trans_m;
}

void Cuboid::SetTrans_m(const Vector3D &trans_m)
{
    _Trans_m=trans_m;
}

const std::string & Cuboid::GetName() const
{
    return _Name;
}

void Cuboid::SetName(const char* sName)
{
    _Name=sName;
}

const Vector3D& Cuboid::GetScale() const
{
    return _Scale;
}

void Cuboid::SetScale(const Vector3D& scale)
{
    _Scale = scale;
}

const Vector3D& Cuboid::GetShift() const
{
    return _Shift;
}

void Cuboid::SetShift(const Vector3D& shift)
{
    _Shift = shift;
}

const VectorRGB& Cuboid::GetColor() const
{
    return _Color;
}

void Cuboid::SetColor(const VectorRGB& color)
{
    _Color = color;
}

void Cuboid::LockObj()
{
    _Guard.lock();
}

void Cuboid::UnlockObj()
{
    _Guard.unlock();
}