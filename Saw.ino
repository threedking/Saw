 #include "Mediator.h"
#include "TheGame.h"
#include "softUART.h"
#include "GBUS.h"
//----------------

//SendingData::ArduinoOfSaw kCenter kSideL  kSideR  kLed
SendingData::ArduinoOfSaw Type(SendingData::ArduinoOfSaw::kCenter);

softUART<4, GBUS_FULL > sUART(1000); // пин 4, скорость 1000
GBUS rx_tx(&sUART, Type, 100); // обработчик UART, адрес , буфер

//----------------
//----------------
//----------------
class UART: public ConcreteColleagueMain{
  public:    
  protected:  
    SendingData::ArduinoOfSaw type_;

    void (*ptr_Send_)(SendingData*) = NULL;
    bool (*ptr_Read_)(SendingData*) = NULL;  
  public:  
    SendingData read_data;
     
    UART(SendingData::ArduinoOfSaw new_type, void (*new_ptr_Send)(SendingData&), bool (*new_ptr_Read)(SendingData&)): type_(new_type), ptr_Send_(new_ptr_Send), ptr_Read_(new_ptr_Read) {
    }
    //------------------------
    void Notify(SendingData data) override{
      if(data.is_need_send){
        this->Send(&data); 
      }
    }
    //------------------------
    void Send(SendingData *data){
      if(this->ptr_Send_!=NULL){ 
        this->ptr_Send_(data);
      }
    }
    //------------------------
    void Read(){
      if(this->ptr_Read_!=NULL){
        if( this->ptr_Read_(&this->read_data) ){          
          this->SendToMediator(this->read_data); 
        }
      }
    }  
};
//----------------
//----------------
//----------------

void Send(SendingData *data);
bool Read(SendingData *data);

ConcreteMediatorMainGame mediator;
UART uart(Type, Send, Read);
GameManagerSide *manager;
//----------------
void setup()
{
  Serial.begin(9600);
  
  switch(Type){
  case SendingData::ArduinoOfSaw::kCenter :
    manager = new GameManagerCenter(Type);
  break;
  case SendingData::ArduinoOfSaw::kSideL :
    manager = new GameManagerSide(Type);
  break;
  case SendingData::ArduinoOfSaw::kSideR :      
    manager = new GameManagerSide(Type);
  break;
  case SendingData::ArduinoOfSaw::kLed :  
    manager = new GameManagerLED(Type);   
  break;
  }
  
  uart.SetMediator(&mediator);
  manager->SetMediator(&mediator);
  mediator.SetColleagueGame(manager);
  mediator.SetColleagueMain(&uart);
}
//----------------
void loop(){  
  rx_tx.tick();
  uart.Read();
  if(!rx_tx.isBusy()){  
    manager->Play();
  }
}
//----------------
void Send(SendingData *data){    
  rx_tx.sendData(data->receiver, *data);
}
//----------------
bool Read(SendingData *data){//Вернет тру, если получил и изменил дату  
  if(rx_tx.gotData()){
    rx_tx.readData(*data);
    return true;
  }
  return false;
}  
//----------------
