//---------------------------------------------------------------------------
#ifndef MediatorH
#define MediatorH

#include <stdint.h>
#include <Arduino.h>
//---------------------------------------------------------------------------
 struct SendingData{
   enum ArduinoOfSaw:  uint8_t{
    kCenter = 1,
    kSideL = 2,
    kSideR = 3,
    kLed = 4
  };
    SendingData::ArduinoOfSaw sender = kCenter;
    SendingData::ArduinoOfSaw receiver = kCenter; 
    bool is_need_send=false; 
    uint8_t quest_n=0;
    bool is_right=false;
    bool quest_end=false;
    bool quest_start=false;
    uint8_t Pass0=5;
    uint8_t Pass1=7;
  };
//---------------------------------------------------------------------------
class Colleague;
class Mediator;
class ConcreteMediatorMainGame;
class ConcreteColleagueMain;
class ConcreteColleagueGame;
//---------------------------------------------------------------------------
class Mediator{
public:
	virtual void Send(SendingData& data,Colleague *colleague) const = 0;
};
//---------------------------------------------------------------------------
class Colleague{
protected:
	Mediator* mediator_=NULL;
public:
	void SetMediator(Mediator* new_mediator);
};
//---------------------------------------------------------------------------
class ConcreteColleagueMain:public Colleague{
public:
	ConcreteColleagueMain()=default;

	void SendToMediator(SendingData data);
	virtual void Notify(SendingData data)=0;
};
//---------------------------------------------------------------------------
class ConcreteColleagueGame:public Colleague{
public:
	ConcreteColleagueGame()=default;

	void SendToMediator(SendingData data);
	virtual void Notify(SendingData data)=0;
};
//---------------------------------------------------------------------------
class ConcreteMediatorMainGame:public Mediator
{
protected:
	ConcreteColleagueGame* m_ColleagueGame;
	ConcreteColleagueMain* m_ColleagueMain;
public:
	void SetColleagueGame(ConcreteColleagueGame* c);
	void SetColleagueMain(ConcreteColleagueMain* c);
	virtual void Send(SendingData& data,Colleague *colleague) const;
};
//---------------------------------------------------------------------------
#endif
