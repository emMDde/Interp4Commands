#include <iostream>
#include <iomanip>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <map>

#include "AbstractMobileObj.hh"
#include "ComChannel.hh"


using std::cout;
using std::cerr;
using std::endl;


int ComChannel::Send(const char* sMesg)
{
  ssize_t  IlWyslanych;
  ssize_t  IlDoWyslania = (ssize_t) strlen(sMesg);

  while((IlWyslanych = write(_Socket,sMesg,IlDoWyslania)) > 0) {
    IlDoWyslania -= IlWyslanych;
    sMesg += IlWyslanych;
  }
  if (IlWyslanych < 0) {
    cerr << "*** Blad przeslania napisu." << endl;
  }
  return 0;
}

ComChannel::ComChannel() : _Socket(0)
{}
      
void ComChannel::Init(int Socket)
{
    _Socket=Socket;
}

int ComChannel::GetSocket() const
{
    return _Socket;
}

void ComChannel::LockAccess()
{
    _Guard.lock();
}
 
void ComChannel::UnlockAccess()
{
    _Guard.unlock();
}

std::mutex & ComChannel::UseGuard()
{
    return _Guard;
}

bool ComChannel::OpenConnection(int PORT)
{
  struct sockaddr_in  DaneAdSerw;

  bzero((char *)&DaneAdSerw,sizeof(DaneAdSerw));

  DaneAdSerw.sin_family = AF_INET;
  DaneAdSerw.sin_addr.s_addr = inet_addr("127.0.0.1");
  DaneAdSerw.sin_port = htons(PORT);


  _Socket = socket(AF_INET,SOCK_STREAM,0);

  if(_Socket < 0) 
  {
    cerr << "*** Blad otwarcia gniazda." << endl;
    return false;
  }

  if(connect(_Socket,(struct sockaddr*)&DaneAdSerw,sizeof(DaneAdSerw)) < 0)
  {
    cerr << "*** Brak mozliwosci polaczenia do portu: " << PORT << endl;
    return false;
  }

  return true;
}

void ComChannel::CloseConnection()
{
  Send("Close \n");
  close(_Socket);
}

void ComChannel::AddServerObject(const AbstractMobileObj & Obj)
{
  std::ostringstream TmpStream;
  TmpStream << "AddObj ";
  TmpStream << Obj;
  TmpStream << "\n";
  //cout << TmpStream.str() << endl;
  _Guard.lock();
  Send(TmpStream.str().c_str());
  _Guard.unlock();
}

void ComChannel::UpdateServerObject(const AbstractMobileObj & Obj)
{
  std::ostringstream TmpStream;
  TmpStream << "UpdateObj ";
  TmpStream << Obj;
  TmpStream << "\n";
  //cout << TmpStream.str() << endl;
  _Guard.lock();
  Send(TmpStream.str().c_str());
  _Guard.unlock();
}

void ComChannel::ClearServerScene()
{
  Send("Clear \n");
}