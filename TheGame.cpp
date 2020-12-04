#include "TheGame.h"
//----------------------
LED::LED(uint8_t pin):pin_(pin){
  pinMode(this->pin_, OUTPUT);
  this->TurnOff();
}
//------------------------
void LED::TurnOff(){
  digitalWrite(this->pin_,LOW);
}
//------------------------
void LED::TurnOn(){
  digitalWrite(this->pin_,HIGH);
}
//------------------------
void LED::Breath(){
  //analogWrite(this->pin_, uint8_t( sin( millis()/this->period_*PI )  *128.0 +127 ) );
  this->pwm_ = uint8_t( sin( exp( sin( millis()/this->period_*PI ) ) -0.36787944 ) * 255.0 );
  analogWrite(this->pin_, this->pwm_);
}
//------------------------
bool LED::BreathUntil(bool stop_at_max){
  this->Breath();
  return this->is_breathing_= stop_at_max? this->pwm_<250 : this->pwm_>75 ;
}
//------------------------
void LED::SetPeriod(double new_period){
  if(new_period > 1e-8){
    this->period_=new_period;
  }
}
//------------------------
//------------------------
//------------------------
RGBLED::RGBLED(uint8_t pin_r,uint8_t pin_g,uint8_t pin_b):leds{pin_r,pin_g,pin_b}{
}
//------------------------
void RGBLED::Breath(){
  if(!this->leds[this->chosen_one].BreathUntil(this->stop_at_max[this->chosen_one])){    
    this->stop_at_max[this->chosen_one]=!this->stop_at_max[this->chosen_one];
    this->chosen_one = this->chosen_one < 2 ? this->chosen_one+1 : 0;
  }  
}
//------------------------
void RGBLED::TurnOff(){
  this->leds[0].TurnOff();
  this->leds[1].TurnOff();
  this->leds[2].TurnOff();
}
//------------------------
//------------------------
//------------------------
ColorDigital::ColorDigital(bool new_r, bool new_g, bool new_b):r_(new_r),g_(new_g),b_(new_b){
}
//------------------------
void ColorDigital::SetRGB(bool new_r, bool new_g, bool new_b){
  this->r_=new_r;
  this->g_=new_g;
  this->b_=new_b;
}   
//------------------------    
bool ColorDigital::GetR(){
  return this->r_;
}
//------------------------
bool ColorDigital::GetG(){  
  return this->g_;
}
//------------------------
bool ColorDigital::GetB(){
  return this->b_;  
}
//------------------------
bool ColorDigital::operator==(const ColorDigital &color){
  bool rez =  (this->r_ == color.r_ &&
            this->g_ == color.g_ &&
            this->b_ == color.b_ );
    return rez;
}
//------------------------
bool ColorDigital::IsBlack(){
  return !(this->r_ ||
          this->g_ ||
          this->b_ );
}
//------------------------
//------------------------
//------------------------
ITheGame::ITheGame(LiquidCrystal_I2C_3DK *new_lcd): lcd_(new_lcd){
  this->PinsMode();
}
//------------------------
void ITheGame::PinsMode(){
  pinMode(this->pins_.kIR, INPUT);
  pinMode(this->pins_.kRGBr, OUTPUT);
  pinMode(this->pins_.kRGBg, OUTPUT);
  pinMode(this->pins_.kRGBb, OUTPUT);
  pinMode(this->pins_.kBuzzer, OUTPUT);
  pinMode(this->pins_.kButtonR, INPUT);
  pinMode(this->pins_.kButtonG, INPUT);
  pinMode(this->pins_.kButtonB, INPUT);
}
//------------------------
void ITheGame::AnimationSucsess(){
  this->RGBwrite(false,false,false);
  this->lcd_->DisplayBlinkAndSound(this->pins_.kBuzzer,5000,2,50);
}
//------------------------
void ITheGame::AnimationFailure(){
  this->RGBwrite(false,false,false);
  this->lcd_->DisplayBlinkAndSound(this->pins_.kBuzzer,1000,1,200);
}
//------------------------
void ITheGame::AnimationQuestSucsess(){
  this->RGBwrite(false,false,false);
  this->lcd_->DisplayBlinkAndSound(this->pins_.kBuzzer,4000,2,100);
}
//------------------------
void ITheGame::RGBwrite(ColorDigital color){
  digitalWrite(this->pins_.kRGBr,color.GetR());
  digitalWrite(this->pins_.kRGBg,color.GetG());
  digitalWrite(this->pins_.kRGBb,color.GetB());  
}
//------------------------
void ITheGame::RGBwrite(bool R, bool G, bool B){
  digitalWrite(this->pins_.kRGBr,R);
  digitalWrite(this->pins_.kRGBg,G);
  digitalWrite(this->pins_.kRGBb,B);  
}
//------------------------
void ITheGame::RGBwrite(uint8_t R, uint8_t G, uint8_t B){
  analogWrite(this->pins_.kRGBr,R);
  analogWrite(this->pins_.kRGBg,G);
  analogWrite(this->pins_.kRGBb,B);    
}
//------------------------
void ITheGame::RGBwrite(ColorAnalog color){
  analogWrite(this->pins_.kRGBr,color.r_);
  analogWrite(this->pins_.kRGBg,color.g_);
  analogWrite(this->pins_.kRGBb,color.b_);
}
//------------------------
//------------------------
//------------------------
Game0::Game0(LiquidCrystal_I2C_3DK *new_lcd):ITheGame(new_lcd){   
  this->lcd_->lcd_.clear();     
  this->lcd_->Print("X",0,7);     
  this->lcd_->Print("marks the spot",1,1);
}
//------------------------    
uint8_t Game0::Play(SendingData& data){
  return this->Play();
}
//------------------------    
uint8_t Game0::Play(){
  Serial.println("TEST GAME0-PLAY");
 if(!digitalRead(this->pins_.kIR)){
  this->lcd_->lcd_.clear();
  this->lcd_->Print("Welcome",0,4);
  this->lcd_->DisplayBlink(5);  
  this->lcd_->lcd_.clear();
  this->lcd_->Print("I",0,4);
  delay(500);
  this->lcd_->Print("want",0,6);
  delay(500);
  this->lcd_->Print("to play a game",1,1);
  delay(2000);
  return 0;
 }else{
  return 255;
 }
}
//------------------------
//------------------------
//------------------------
Game1::Game1(LiquidCrystal_I2C_3DK *new_lcd):ITheGame(new_lcd),button_r_(pins_.kButtonR), button_g_(pins_.kButtonG), button_b_(pins_.kButtonB){
  this->button_r_.setType(LOW_PULL);
  this->button_g_.setType(LOW_PULL);
  this->button_b_.setType(LOW_PULL);
  this->progress_=0;
  this->MakeRandColors();
  this->current_color_n_=0;  
  this->RGBwrite( this->colors_[this->current_color_n_] );  
  this->lcd_->lcd_.clear();
  this->lcd_->Print("   RGButtons",0,0);
}
//------------------------    
uint8_t Game1::Play(SendingData& data){
  return this->Play();
}
//------------------------    
uint8_t Game1::Play(){
  this->button_r_.tick();
  this->button_g_.tick();
  this->button_b_.tick();
  
  static ColorDigital color_from_buttons;
  
  if(this->button_r_.isPress() ||
  this->button_g_.isPress() ||
  this->button_b_.isPress()){  //Если хоть одна кнопка была нажата

    this->button_r_.isPress();//Нужно вызвать для сброса флагов
    this->button_g_.isPress();
    this->button_b_.isPress();
    
    delay(300);//Даем возможность нажать все нужные
    color_from_buttons.SetRGB( this->button_r_.state(), this->button_g_.state(), this->button_b_.state() ); //взяли цвет с кнопок
    if(this->colors_[this->current_color_n_]==color_from_buttons){//Если цвета совадаютыч
      this->progress_++;
       
      this->current_color_n_++;
      if(this->current_color_n_ >= this->kColorsCount){//Если цвета закончились - новый рандом
        this->MakeRandColors();
        this->current_color_n_=0;
      }
      
      this->AnimationSucsess();
    }else{ //цвета не совпали
      if(this->progress_ > 0){
        this->progress_--;
      }
      this->AnimationFailure();
    }
    constrain(this->progress_,0,this->kTargetProgress);
    
    for( int i=0; i<this->kTargetProgress*2, i<16; i++ ){ //Вывод прогрессбара
      this->lcd_->Print( i+1 <= 2*this->progress_ ? "#" : " ",1,i);
    }
   
    if(this->progress_ >= this->kTargetProgress){//Если нужные баллы набраны - выходим
      this->AnimationQuestSucsess();
      return 1;
    }else{//Продолжаем
      this->RGBwrite( this->colors_[this->current_color_n_] );
    }
  }
  return 255;
}
//------------------------
void Game1::MakeRandColors(){ //Набить colors_ всеми вариациями, кроме черного, в случайном порядке
  static auto IsRepeatsUntil=[this](uint8_t i)->bool{
    for(uint8_t j=0;j<i;j++){
      if(this->colors_[j]==this->colors_[i]){
        return true;
      }
    }
    return false;
  };
  
  for(int i=0; i < this->kColorsCount; i++){
    do{
      this->colors_[i].SetRGB(rand()%2==1?true:false, rand()%2==1?true:false, rand()%2==1?true:false);
    }while(this->colors_[i].IsBlack() || IsRepeatsUntil(i));
  }  
}
//------------------------
//------------------------
//------------------------
Game2::Game2(LiquidCrystal_I2C_3DK *new_lcd):ITheGame(new_lcd){
  this->progress_=0;
  this->target_direction=rand()%4+1;
  this->lcd_->lcd_.clear();  
  this->lcd_->Print(
    this->target_direction==kCenter?"     Center ":
    this->target_direction==kLeft  ?"      Left  ":
    this->target_direction==kRight ?"     Right  ":
    this->target_direction==kUp    ?"       Up   ":
    this->target_direction==kDown  ?"      Down  ":
                                    "     ERROR  ",0);
}
//------------------------    
uint8_t Game2::Play(SendingData& data){
  return this->Play();
}
//------------------------    
uint8_t Game2::Play(){  

  static JoystickDirections joystick_direction=kOther;

  uint16_t X=analogRead(this->pins_.kJoy0);
  uint16_t Y=analogRead(this->pins_.kJoy1);

  Serial.print(X); Serial.print("____"); Serial.println(Y);

  if( X>384 && X<640 && Y>384 && Y<640 ){
    joystick_direction=kCenter;
  }else if( X<=384 && Y>384 && Y<640 ){
    joystick_direction=kRight;
  }else if( X>=640 && Y>384 && Y<640 ){
    joystick_direction=kLeft;
  }else if( X>384 && X<640 && Y>=640 ){
    joystick_direction=kDown;
  }else if( X>384 && X<640 && Y<=384 ){
    joystick_direction=kUp;
  }else{
    joystick_direction=kOther;
  }

  if(joystick_direction!=kOther){
    if(joystick_direction == this->target_direction){
      this->progress_++;

      do{
        this->target_direction=rand()%4+1;      
      }while(joystick_direction == this->target_direction);
      
      this->lcd_->Print(
        this->target_direction==kCenter?"     Center ":
        this->target_direction==kLeft  ?"      Left  ":
        this->target_direction==kRight ?"     Right  ":
        this->target_direction==kUp    ?"       Up   ":
        this->target_direction==kDown  ?"      Down  ":
                                        "     ERROR  ",0);      
      
      this->AnimationSucsess();                                  
    }else{
      if(joystick_direction!=kCenter){
        if(this->progress_ > 0){
          this->progress_--;
        }
        this->AnimationFailure();
      }
    }
    constrain(this->progress_,0,this->kTargetProgress);

    for( int i=0; i<this->kTargetProgress*2, i<16; i++ ){ //Вывод прогрессбара
      this->lcd_->Print( i+1 <= 2*this->progress_ ? "#" : " ",1,i);
    }
    
    if(this->progress_ >= this->kTargetProgress){//Если нужные баллы набраны - выходим
      this->AnimationQuestSucsess();
      return 2;
    }else{//Продолжаем
      
    }
  }
  return 255;
}
//------------------------
//------------------------
//------------------------
Game3::Game3(LiquidCrystal_I2C_3DK *new_lcd):ITheGame(new_lcd){ 
  this->lcd_->lcd_.clear();  
  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor.");
  }
}
//------------------------    
uint8_t Game3::Play(SendingData& data){
  return this->Play();
}
//------------------------    
uint8_t Game3::Play(){    
  if (APDS.gestureAvailable())
  {
    static int result;
    result = APDS.readGesture(); //0 UP; 1 DOWN; 2 LEFT; 3 RIGHT; NONE 
    if(result == 2 || result == 3){
      return result*10;
    }
  }
  return 255;
}
//------------------------
//------------------------
//------------------------
GameSleep::GameSleep(LiquidCrystal_I2C_3DK *new_lcd):ITheGame(new_lcd), rgb_(this->pins_center_.kRGBr, this->pins_center_.kRGBg, this->pins_center_.kRGBb){   
  this->lcd_->lcd_.clear();
}
//------------------------
uint8_t GameSleep::Play(SendingData& data){
  return this->Play();
} 
//------------------------    
uint8_t GameSleep::Play(){
  rgb_.Breath();
  return 255;
}
//------------------------
//------------------------
//------------------------
Game3IR::Game3IR(LiquidCrystal_I2C_3DK *new_lcd):ITheGame(new_lcd),irrecv_(this->pins_center_.kRemote){
  this->deg_servo_door_=90;//90
  this->deg_servo_turret0_=90;//90
  this->deg_servo_turret1_=0;//0
  
  this->servo_door_.attach(this->pins_center_.kServoDoor, 544, 2400, this->deg_servo_door_);
  this->servo_door_.setSpeed(50);
  this->servo_door_.setAccel(0.3);
  this->servo_door_.setAutoDetach(false);  
  
  this->servo_turret0_.attach(this->pins_center_.kServoLazer0, 544, 2400, this->deg_servo_turret0_); 
  this->servo_turret1_.attach(this->pins_center_.kServoLazer1, 544, 2400, this->deg_servo_turret1_); 
  
  this->servo_turret0_.setSpeed(50);
  this->servo_turret0_.setAccel(0.3);
  this->servo_turret0_.setAutoDetach(false);
  this->servo_turret1_.setSpeed(50);
  this->servo_turret1_.setAccel(0.3);    
  this->servo_turret1_.setAutoDetach(false);  
  
  digitalWrite(this->pins_center_.kLazer,LOW);  
  
  this->irrecv_.enableIRIn();
  this->irrecv_.resume();  
  
  this->secret_code_.setCharAt(0,String(rand()%10).charAt(0));
  this->secret_code_.setCharAt(1,String(rand()%10).charAt(0));
  this->secret_code_.setCharAt(4,String(rand()%10).charAt(0));
  this->secret_code_.setCharAt(5,String(rand()%10).charAt(0));
}
//------------------------    
uint8_t Game3IR::Play(){

  if(this->secret_code_.length()==6){
    for(int i=0; i<6; i++){            
      this->lcd_->Print(String(this->secret_code_[i]),1, 2+i*2);
    }
  }
  
  this->servo_turret0_.setTargetDeg( constrain(this->deg_servo_turret0_,0,180) );
  this->servo_turret1_.setTargetDeg( constrain(this->deg_servo_turret1_,0,180) );
  this->servo_door_.setTargetDeg( constrain(this->deg_servo_door_,0,180) );
  
  this->servo_turret0_.tick();
  this->servo_turret1_.tick();
  this->servo_door_.tick();  
   
  
  do{
    this->GetIrCommand();
  }while(this->ircode_.length() == 0);  
  
  
  if(this->ircode_.length() == 6){
    if(this->ircode_.equals("FF9867")){//Кнопка 0
      this->ChangeInputCode(String("0"));      
      this->lcd_->DisplayBlink(1);
    }else if(this->ircode_.equals("FFA25D")){//Кнопка 1
      this->ChangeInputCode(String("1"));      
      this->lcd_->DisplayBlink(1);
    }else if(this->ircode_.equals("FF629D")){//Кнопка 2
      this->ChangeInputCode(String("2"));      
      this->lcd_->DisplayBlink(1);
    }else if(this->ircode_.equals("FFE21D")){//Кнопка 3
      this->ChangeInputCode(String("3"));      
      this->lcd_->DisplayBlink(1);
    }else if(this->ircode_.equals("FF22DD")){//Кнопка 4
      this->ChangeInputCode(String("4"));      
      this->lcd_->DisplayBlink(1);
    }else if(this->ircode_.equals("FF02FD")){//Кнопка 5
      this->ChangeInputCode(String("5"));      
      this->lcd_->DisplayBlink(1);
    }else if(this->ircode_.equals("FFC23D")){//Кнопка 6
      this->ChangeInputCode(String("6"));      
      this->lcd_->DisplayBlink(1);
    }else if(this->ircode_.equals("FFE01F")){//Кнопка 7
      this->ChangeInputCode(String("7"));      
      this->lcd_->DisplayBlink(1);
    }else if(this->ircode_.equals("FFA857")){//Кнопка 8
      this->ChangeInputCode(String("8"));      
      this->lcd_->DisplayBlink(1);
    }else if(this->ircode_.equals("FF906F")){//Кнопка 9
      this->ChangeInputCode(String("9"));
      this->lcd_->DisplayBlink(1);
    }
  }  
  
  Serial.println(this->input_code_);  
  
  if(this->secret_code_.equals(this->input_code_)){
    
    this->servo_turret0_.detach();
    this->servo_turret1_.detach();    
    this->servo_turret0_.attach(this->pins_center_.kServoSecret0, 544, 2400, 170); //Закрыть секретки
    this->servo_turret1_.attach(this->pins_center_.kServoSecret1, 544, 2400, 10); 
    delay(500);
    this->servo_turret0_.detach();
    this->servo_turret1_.detach();    
    
    this->AnimationOpenDoor();
    return 4;
  }    
  return 255;
}
//------------------------
uint8_t Game3IR::Play(SendingData& data){
  if(data.quest_n == 0){  //если пришел кусок секретного кода от сайда
    if(data.quest_start){
      if(this->secret_code_.length() == 6){
        if(data.sender == SendingData::ArduinoOfSaw::kSideL){
          this->secret_code_.setCharAt(0,String(data.Pass0).charAt(0));
          this->secret_code_.setCharAt(1,String(data.Pass1).charAt(0));
        }else if(data.sender == SendingData::ArduinoOfSaw::kSideR){          
          this->secret_code_.setCharAt(4,String(data.Pass0).charAt(0));
          this->secret_code_.setCharAt(5,String(data.Pass1).charAt(0));
        }
      }
    }
  }
  return 255;
}
//------------------------
void Game3IR::ChangeInputCode(String new_input){  
    if(new_input.length() == 1){
      this->input_code_.remove(0, 1);
      this->input_code_+=new_input;
    }
    if(this->input_code_.length() != 6){
      this->input_code_="ERRROR";
    }
}
//------------------------    
void Game3IR::GetIrCommand(){
  decode_results result;
  if (this->irrecv_.decode(&result)){
    this->ircode_ = String(result.value, HEX);
    this->ircode_.toUpperCase();
    this->irrecv_.resume();
  }else{
    this->ircode_ = "";
  }  
}
//------------------------
void Game3IR::AnimationOpenDoor(){
  this->deg_servo_door_=0;
  this->servo_door_.setTargetDeg( constrain(this->deg_servo_door_,0,180) );
  do{
    this->servo_door_.tick(); 
  }while(
    abs(this->servo_door_.getCurrentDeg() - this->deg_servo_door_) > 2
   );      
  Serial.println("Door is open!");  
  this->lcd_->lcd_.clear();  
  this->lcd_->Print("You Win!",0,4);
  delay(500);
  this->lcd_->DisplayBlink(3);
  this->lcd_->Print("Congratulations!",0,0);  
  this->lcd_->DisplayBlink(5);
  this->lcd_->Print("You free!",1,3);
  delay(1000);
}
//------------------------
void Game3IR::PinsMode(){    
  pinMode(this->pins_center_.kRGBr, OUTPUT);
  pinMode(this->pins_center_.kRGBg, OUTPUT);
  pinMode(this->pins_center_.kRGBb, OUTPUT);
  pinMode(this->pins_center_.kBuzzer, OUTPUT);
  pinMode(this->pins_center_.kLazer, OUTPUT);
  pinMode(this->pins_center_.kRemote, INPUT);
}
//------------------------
//------------------------
//------------------------
Game3Center::Game3Center(LiquidCrystal_I2C_3DK *new_lcd):ITheGame(new_lcd){
  this->PinsMode();
  this->deg_servo_turret0_=90;//90
  this->deg_servo_turret1_=0;//0
  this->deg_servo_secret0_=170;//170
  this->deg_servo_secret1_=10;//10
  this->deg_servo_door_=90;//90
  this->secret0_open_=false;
  this->secret1_open_=false;
  
  this->servo_turret0_.attach(this->pins_center_.kServoLazer0, 544, 2400, this->deg_servo_turret0_); 
  this->servo_turret1_.attach(this->pins_center_.kServoLazer1, 544, 2400, this->deg_servo_turret1_); 
  this->servo_secret0_.attach(this->pins_center_.kServoSecret0, 544, 2400, this->deg_servo_secret0_); 
  this->servo_secret1_.attach(this->pins_center_.kServoSecret1, 544, 2400, this->deg_servo_secret1_); 
  this->servo_door_.attach(this->pins_center_.kServoDoor, 544, 2400, this->deg_servo_door_);   
  
  this->servo_turret0_.setSpeed(50);
  this->servo_turret0_.setAccel(0.3);
  this->servo_turret0_.setAutoDetach(false);
  this->servo_turret1_.setSpeed(50);
  this->servo_turret1_.setAccel(0.3);
  this->servo_turret1_.setAutoDetach(false);
  this->servo_secret0_.setSpeed(50);
  this->servo_secret0_.setAccel(0.3);
  this->servo_secret0_.setAutoDetach(false);
  this->servo_secret1_.setSpeed(50);
  this->servo_secret1_.setAccel(0.3);
  this->servo_secret1_.setAutoDetach(false);
  this->servo_door_.setSpeed(50);
  this->servo_door_.setAccel(0.3);
  this->servo_door_.setAutoDetach(false);    
      
  this->lcd_->lcd_.clear();  
  this->lcd_->Print("Laser helps you",0,0);
  delay(2000);
  this->lcd_->Print("find last",1,3);
  delay(2000);
  this->lcd_->lcd_.clear();  
  this->lcd_->Print("puzzle pieces",0,1);  
  this->lcd_->DisplayBlink(5);
  delay(1000);
  
  digitalWrite(this->pins_center_.kLazer,HIGH);
}
//------------------------
uint8_t Game3Center::Play(){  

  uint16_t X=analogRead(this->pins_center_.kJoy0);
  uint16_t Y=analogRead(this->pins_center_.kJoy1);
  uint16_t jspeed=2;
  
  static uint32_t tmr = 0;
  if (millis() - tmr >= 100) {
    tmr = millis();
  
    if( X>384 && X<640 && Y>384 && Y<640 ){//Center
    }else if( X<=384 && Y>384 && Y<640 ){//Right
      this->deg_servo_turret0_+= this->deg_servo_turret0_ >= jspeed ? -jspeed : 0;
    }else if( X>=640 && Y>384 && Y<640 ){//Left
      this->deg_servo_turret0_+= this->deg_servo_turret0_ <= 180-jspeed ? jspeed : 0;
    }else if( X>384 && X<640 && Y>=640 ){//Down
      this->deg_servo_turret1_+= this->deg_servo_turret1_ <= 180-jspeed ? jspeed : 0;
    }else if( X>384 && X<640 && Y<=384 ){//Up
      this->deg_servo_turret1_+= this->deg_servo_turret1_ >= jspeed ? -jspeed : 0;
    }    
  }
  
  this->servo_turret0_.setTargetDeg( constrain(this->deg_servo_turret0_,0,180) );
  this->servo_turret1_.setTargetDeg( constrain(this->deg_servo_turret1_,0,180) );
  this->servo_secret0_.setTargetDeg( constrain(this->deg_servo_secret0_,0,180) );
  this->servo_secret1_.setTargetDeg( constrain(this->deg_servo_secret1_,0,180) );
  this->servo_door_.setTargetDeg( constrain(this->deg_servo_door_,0,180) );
  
  this->servo_turret0_.tick();
  this->servo_turret1_.tick(); 
  this->servo_secret0_.tick();
  this->servo_secret1_.tick();
  this->servo_door_.tick();  
  
  if(!this->secret0_open_ || !this->secret1_open_){//Если хоть в 1 еще не попали
    if(!this->secret0_open_){
      this->secret0_open_ = digitalRead(this->pins_center_.kLazerSensor0);      
    }else{      
      this->deg_servo_secret0_=70;
    }
    if(!this->secret1_open_){
      this->secret1_open_ = digitalRead(this->pins_center_.kLazerSensor1);      
    }else{      
      this->deg_servo_secret1_=105;
    }

    if(this->secret0_open_ && this->secret1_open_){//Если оба попадания, то открыть секретки до конца
      this->deg_servo_secret0_=70;
      this->deg_servo_secret1_=105;
      this->servo_secret0_.setTargetDeg( constrain(this->deg_servo_secret0_,0,180) );
      this->servo_secret1_.setTargetDeg( constrain(this->deg_servo_secret1_,0,180) );
      do{        
        this->servo_secret0_.tick();
        this->servo_secret1_.tick(); 
      }while(
        abs(this->servo_secret0_.getCurrentDeg() - this->deg_servo_secret0_) > 1 ||
        abs(this->servo_secret1_.getCurrentDeg() - this->deg_servo_secret1_) > 1
      );
    }
    
  }else{//Оба попадания, идем на ввод с пульта
    digitalWrite(this->pins_center_.kLazer,LOW);
    this->servo_turret0_.detach(); 
    this->servo_turret1_.detach(); 
    this->servo_secret0_.detach(); 
    this->servo_secret1_.detach(); 
    this->servo_door_.detach(); 
    return 3;
  }  
  return 255;
}
//------------------------
uint8_t Game3Center::Play(SendingData& data){//обертка над Play, с обработкой ввода
  if(data.quest_n == 3){  //если пришла команда управления от сайда
    if(!this->secret0_open_ || !this->secret1_open_){
      if(data.sender == SendingData::ArduinoOfSaw::kSideL){
        this->deg_servo_turret0_+=data.is_right?5:-5;
      }else if(data.sender == SendingData::ArduinoOfSaw::kSideR){
        this->deg_servo_turret1_+=data.is_right?5:-5;
      }    
    }
  }
  /*if(data.quest_n == 0){  //если пришел кусок секретного кода от сайда
    if(data.quest_start){
      if(this->secret_code_.length() == 6){
        if(data.sender == SendingData::ArduinoOfSaw::kSideL){
          this->secret_code_.setCharAt(0,String(data.Pass0).charAt(0));//itoa(x, myStr, 10);
          this->secret_code_.setCharAt(1,String(data.Pass1).charAt(0));
        }else if(data.sender == SendingData::ArduinoOfSaw::kSideR){
          this->secret_code_.setCharAt(4,String(data.Pass0).charAt(0));
          this->secret_code_.setCharAt(5,String(data.Pass1).charAt(0));
        }
      }
    }
  }*/
  return this->Play();
}
//------------------------
void Game3Center::PinsMode(){
  pinMode(this->pins_center_.kRGBr, OUTPUT);
  pinMode(this->pins_center_.kRGBg, OUTPUT);
  pinMode(this->pins_center_.kRGBb, OUTPUT);
  pinMode(this->pins_center_.kBuzzer, OUTPUT);
  pinMode(this->pins_center_.kLazer, OUTPUT);
  pinMode(this->pins_center_.kLazerSensor0, INPUT);
  pinMode(this->pins_center_.kLazerSensor1, INPUT);
  pinMode(this->pins_center_.kRemote, INPUT);
}
//------------------------
//------------------------
//------------------------
Game1_2Center::Game1_2Center(LiquidCrystal_I2C_3DK *new_lcd):ITheGame(new_lcd){    
  this->lcd_->lcd_.clear();
  this->lcd_->Print("Welcome",0,4);
  this->lcd_->DisplayBlink(5);  
  this->lcd_->lcd_.clear();
  this->lcd_->Print("Complete",0,4);
  this->lcd_->Print("personal quests",1,0);
  this->lcd_->DisplayBlink(3);
}
//------------------------
uint8_t Game1_2Center::Game1_2Center::Play(){
  
  uint16_t X=analogRead(this->pins_center_.kJoy0);
  uint16_t Y=analogRead(this->pins_center_.kJoy1);

  if( X<384 || X>640 || Y<384 || Y>640 ){
    return 2;
  }
  return 255;
}
//------------------------
uint8_t Game1_2Center::Play(SendingData& data){
  return this->Play();  
}
//------------------------
//------------------------
//------------------------
GameLED::GameLED(LiquidCrystal_I2C_3DK *new_lcd):ITheGame(new_lcd), leds_{{this->pins_led_.kLED1,this->pins_led_.kLED2,this->pins_led_.kLED3},{this->pins_led_.kLED4,this->pins_led_.kLED5,this->pins_led_.kLED6}}{
  this->PinsMode();
}
//------------------------
uint8_t GameLED::Play(SendingData& data){  
  if(data.sender == SendingData::ArduinoOfSaw::kSideL){    
    this->SetChosen(0,data.quest_n);
  }else if(data.sender == SendingData::ArduinoOfSaw::kSideR){
    this->SetChosen(1,data.quest_n);
  } 
  return this->Play();
}
//------------------------    
uint8_t GameLED::Play(){  
  for(int i=0;i<2;i++){
    for(int j=0;j<3;j++){ 
      if(this->chosen_one[i] == j+1){
        this->leds_[i][j].Breath(); 
      }else{
        this->leds_[i][j].TurnOff(); 
      }
    }
  }
  return 255;
}
//------------------------
void GameLED::SetChosen(uint8_t num, uint8_t val){
  if(num==0 || num==1){
    this->chosen_one[num]=val;
  }
}
//------------------------
void GameLED::PinsMode(){
  pinMode(this->pins_led_.kLED1, OUTPUT);
  pinMode(this->pins_led_.kLED2, OUTPUT);
  pinMode(this->pins_led_.kLED3, OUTPUT);
  pinMode(this->pins_led_.kLED4, OUTPUT);
  pinMode(this->pins_led_.kLED5, OUTPUT);
  pinMode(this->pins_led_.kLED6, OUTPUT);
}
//------------------------
//------------------------
//------------------------
GameManagerSide::GameManagerSide(SendingData::ArduinoOfSaw arduino_type):arduino_type_(arduino_type), lcd_(0x27, 16, 2){      
  srand(analogRead(A3));  
  this->Pass0 = rand()%10;
  this->Pass1 = rand()%10;
  if(this->arduino_type_ == SendingData::ArduinoOfSaw::kSideL ||
  this->arduino_type_ == SendingData::ArduinoOfSaw::kSideR){
    this->SetSawGame(4);
    
    SendingData data;
    data.sender = this->arduino_type_;
    data.receiver = SendingData::ArduinoOfSaw::kCenter;
    data.is_need_send = true;
    data.quest_n = 0;
    data.quest_start = true;
    data.Pass0 = this->Pass0;
    data.Pass1 = this->Pass1;    
    this->SendToMediator(data);   //шлем свой кусок кода центру
  }
}
//------------------------
void GameManagerSide::Play(){
  static uint8_t result;
  result = this->saw_game_->Play();
  
  if(result!=255){    
    if(result >= 0 || result <= 2){
      
      SendingData data; //отправка команд леду
      data.sender = this->arduino_type_;
      data.receiver = SendingData::ArduinoOfSaw::kLed;
      data.quest_n = result+1;
      data.is_need_send = true;
      this->SendToMediator(data);      

      if(result == 0){          
        data.sender = this->arduino_type_;  //отправка центру инфы о старте игры
        data.receiver = SendingData::ArduinoOfSaw::kCenter;
        data.quest_n = result;        
        data.quest_end=true;    
        data.quest_start=false;
        data.is_need_send = true;
        this->SendToMediator(data);        
      }else{//1 или 2
        this->ShowPass(result==2);
        if(result == 2){//начнется 3ий, оповестить центр          
          data.sender = this->arduino_type_;
          data.receiver = SendingData::ArduinoOfSaw::kCenter;
          data.quest_n = result+1;        
          data.quest_end=false;  
          data.quest_start=true;
          data.is_need_send = true;
          this->SendToMediator(data);          
        }
      }
    }else if(result == 20 || result == 30){//Квест 3, сигналы влево вправо, отправить центру
      SendingData data;
      data.sender = this->arduino_type_;
      data.receiver = SendingData::ArduinoOfSaw::kCenter;
      data.quest_n = 3;
      data.is_right = result==30;
      data.is_need_send = true;
      this->SendToMediator(data);
    }        
    this->SetSawGame(result);
  }
}
//------------------------
void GameManagerSide::ShowPass(bool both){//Выводит свой кусок пароля на лсд
  if(both){
    this->lcd_.Print(String(this->Pass0),0, this->arduino_type_ == SendingData::ArduinoOfSaw::kSideL ? 0 : 14 );
    this->lcd_.Print(String(this->Pass1),0, this->arduino_type_ == SendingData::ArduinoOfSaw::kSideL ? 1 : 15 );
  }else{
    this->lcd_.Print(String(this->Pass0),0, this->arduino_type_ == SendingData::ArduinoOfSaw::kSideL ? 0 : 14 );  
  }
}
//------------------------
void GameManagerSide::SetSawGame(uint8_t game_n){
  if(game_n == 0 ||
  game_n == 1 ||
  game_n == 2 ||
  game_n == 3 ||
  game_n == 4){
    if(this->saw_game_!=NULL){
      delete this->saw_game_;
      this->saw_game_=NULL;
    }
  }
  switch(game_n){
    case 4:    
      this->saw_game_ = new Game0(&this->lcd_);
    break;
    case 0:
      this->saw_game_ = new Game1(&this->lcd_);
    break;
    case 1:
      this->saw_game_ = new Game2(&this->lcd_);
    break;
    case 2:
      this->saw_game_ = new Game3(&this->lcd_);
    break;
    case 3:
      this->saw_game_ = new GameSleep(&this->lcd_);
    break;
  }
}
//------------------------
void GameManagerSide::Notify(SendingData data){
  if(data.receiver == this->arduino_type_){
    this->saw_game_->Play(data);
  }
}
//------------------------
//------------------------
//------------------------
GameManagerCenter::GameManagerCenter(SendingData::ArduinoOfSaw arduino_type):GameManagerSide(arduino_type){
  srand(analogRead(A3));
  this->SetSawGame(1);
}
//------------------------
void GameManagerCenter::SetSawGame(uint8_t game_n){  
  if(game_n!=255){
    if(this->saw_game_!=NULL){
      delete this->saw_game_;
      this->saw_game_=NULL;
    }
    
    switch(game_n){
      case 1:
        this->saw_game_ = new Game1_2Center(&this->lcd_);
      break;
      case 2:
        this->saw_game_ = new Game3Center(&this->lcd_);
      break;
      case 3:
        this->saw_game_ = new Game3IR(&this->lcd_);
      break;
      case 4:
        this->saw_game_ = new GameSleep(&this->lcd_);
      break;
    }  
  }
}
//------------------------    
void GameManagerCenter::Play(){
  static uint8_t result;
  result = this->saw_game_->Play();
  
  if(result!=255){
    
    if(result == 3){
      SendingData data;//Комната пройдена, всем спать
      data.sender = this->arduino_type_;
      data.quest_n = result;
      data.quest_end = true;
      data.receiver = SendingData::ArduinoOfSaw::kSideL;
      this->SendToMediator(data);
      data.receiver = SendingData::ArduinoOfSaw::kSideR;
      this->SendToMediator(data);
    }
  
    this->SetSawGame(result);
  }
}
//------------------------
void GameManagerCenter::Notify(SendingData data){
  if(data.receiver == this->arduino_type_){    
    if(data.sender == SendingData::ArduinoOfSaw::kSideL ||
    data.sender == SendingData::ArduinoOfSaw::kSideR){
      if(data.quest_end && data.quest_n == 0){  //пришла инфа о старте игры
        this->is_quest3_started_=false;
        this->SetSawGame(0);
        return;
      }else if(data.quest_start && data.quest_n == 3){  //пришла инфа о старте 3его квеста любым из сайдов        
        if(!this->is_quest3_started_){          
          this->is_quest3_started_=true;
          this->SetSawGame(2);
        }
        return;
      }
    }
    this->saw_game_->Play(data);
  }  
}
//------------------------
//------------------------
//------------------------
GameManagerLED::GameManagerLED(SendingData::ArduinoOfSaw arduino_type):GameManagerSide(arduino_type){  
  if(this->saw_game_!=NULL){
    delete this->saw_game_;
    this->saw_game_=NULL;
  }
  this->saw_game_ = new GameLED(&this->lcd_);
}
//------------------------
void GameManagerLED::Play(){
  this->saw_game_->Play();  
}
//------------------------
void GameManagerLED::Notify(SendingData data){   
  this->saw_game_->Play(data);
  
}
//------------------------
//------------------------
//------------------------
