#include "Mediator.h"
//---------------------------------------------------------------------------
void Colleague::SetMediator(Mediator* new_mediator){
this->mediator_=new_mediator;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void ConcreteColleagueMain::SendToMediator(SendingData data){  
if(this->mediator_!=NULL){
	this->mediator_->Send(data, this);
}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void ConcreteColleagueGame::SendToMediator(SendingData data){
if(this->mediator_!=NULL){
	this->mediator_->Send(data, this);
}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void ConcreteMediatorMainGame::SetColleagueGame(ConcreteColleagueGame* c){
this->m_ColleagueGame=c;
}
//---------------------------------------------------------------------------
void ConcreteMediatorMainGame::SetColleagueMain(ConcreteColleagueMain* c){
this->m_ColleagueMain=c;
}
//---------------------------------------------------------------------------
void ConcreteMediatorMainGame::Send(SendingData& data,Colleague *colleague) const{
if (colleague==this->m_ColleagueGame){ 
	m_ColleagueMain->Notify(data); 
}else if (colleague==this->m_ColleagueMain){  
	m_ColleagueGame->Notify(data); 
}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
