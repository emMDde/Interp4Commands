#ifndef COMCHANNEL_HH
#define COMCHANNEL_HH

#include "AbstractComChannel.hh"
#include <mutex>

/*!
	Definicja klasy ComChannel
 */

class ComChannel : public AbstractComChannel {

    int _Socket;
    std::mutex _Guard;

    int Send(const char* sMesg);

  public:

    ComChannel();
      
    virtual void Init(int Socket) override;

    virtual int GetSocket() const override;

    virtual void LockAccess() override;
 
    virtual void UnlockAccess() override;

    virtual std::mutex &UseGuard() override;

    bool OpenConnection(int PORT);

    void CloseConnection();

    virtual void AddServerObject(const AbstractMobileObj & Obj) override;

    virtual void UpdateServerObject(const AbstractMobileObj & Obj) override;

    void ClearServerScene();

};


#endif
