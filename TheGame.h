#ifndef TheGame_h
#define TheGame_h

#include <stdint.h>
#include <Arduino.h>
#include <math.h>


#include <Arduino_APDS9960.h>
#include <GyverButton.h>
#include <ServoSmooth.h>

#include <TimerFreeTone.h>
#include <IRremote.h>
#include <ctype.h>
#include <Wire.h>
#include "LiquidCrystal_I2C_3DK.h"
#include "Mediator.h"
//----------------
class PinNumbersSide{
  public:  
  
  static const uint8_t kIR = 2;
  static const uint8_t kRGBr = 5;
  static const uint8_t kRGBg = 6;
  static const uint8_t kRGBb = 9;
  static const uint8_t kBuzzer = 3;
  static const uint8_t kButtonR = 7;
  static const uint8_t kButtonG = 8;
  static const uint8_t kButtonB = 10;
  static const uint8_t kSDA = A4;
  static const uint8_t kSCL = A5;
  static const uint8_t kJoy0 = A0;
  static const uint8_t kJoy1 = A1;
};
class PinNumbersCenter{
  public:  
    
  static const uint8_t kRGBr = 5;
  static const uint8_t kRGBg = 6;
  static const uint8_t kRGBb = 9;
  static const uint8_t kBuzzer = 3;
  static const uint8_t kLazer = 2;
  static const uint8_t kLazerSensor0 = A0;
  static const uint8_t kLazerSensor1 = A1;
  static const uint8_t kServoLazer0 = A2;
  static const uint8_t kServoLazer1 = 11;
  static const uint8_t kServoSecret0 = 7;
  static const uint8_t kServoSecret1 = 8;
  static const uint8_t kServoDoor = 10;
  static const uint8_t kRemote = 12;
  static const uint8_t kJoy0 = A6;//только для демонстрации
  static const uint8_t kJoy1 = A7;//только для демонстрации
  
};
class PinNumbersLed{
  public:    
  static const uint8_t kLED1 = 3;
  static const uint8_t kLED2 = 5;
  static const uint8_t kLED3 = 6;    
  static const uint8_t kLED4 = 9;
  static const uint8_t kLED5 = 10;
  static const uint8_t kLED6 = 11;      
};  

//----------------
class LED{
  protected:       
    double period_=1000.0;
    uint8_t pin_=255;
    uint8_t pwm_=0;
    bool is_breathing_=false;
  public:
    LED(uint8_t pin);
    void TurnOff();
    void TurnOn();
    void Breath();  
    bool BreathUntil(bool stop_at_max);    
    void SetPeriod(double new_period);
};
//----------------
class RGBLED{
  protected:       
    LED leds[3];
    bool stop_at_max[3]={true,true,true};
    uint8_t chosen_one=0;
  public:
    RGBLED(uint8_t pin_r,uint8_t pin_g,uint8_t pin_b);    
    void Breath();
    void TurnOff();
    //void TurnOn();
    //bool BreathUntil(bool stop_at_max);    
    //void SetPeriod(double new_period);
};
//----------------
class ColorDigital{
  protected:      
    bool r_=0;
    bool g_=0;
    bool b_=0;
  public:
    ColorDigital()=default;
    ColorDigital(bool new_r, bool new_g, bool new_b);
    void SetRGB(bool new_r, bool new_g, bool new_b);
    bool GetR();
    bool GetG();
    bool GetB();
    bool operator==(const ColorDigital &color);
    bool IsBlack();
};
//----------------
class ColorAnalog{
  protected:      
  public:
    uint8_t r_=0;
    uint8_t g_=0;
    uint8_t b_=0;
  public:
    ColorAnalog()=default;
};
//----------------
//----------------
//----------------
class ITheGame{//абстрактный в качестве интерфейса
  protected:  
    PinNumbersSide pins_;
    LiquidCrystal_I2C_3DK *lcd_;

    virtual void PinsMode();
    void AnimationSucsess();
    void AnimationFailure();
    void AnimationQuestSucsess();
    void RGBwrite(bool R, bool G, bool B);//Применить цвет к RGB
    void RGBwrite(uint8_t R, uint8_t G, uint8_t B);//Применить цвет к RGB ШИМ
    void RGBwrite(ColorDigital color);//Применить цвет к RGB
    void RGBwrite(ColorAnalog color);//Применить цвет к RGB
    //void ITheGame::ShowPassOnLCD(String secret_code);
  public:
    ITheGame(LiquidCrystal_I2C_3DK *new_lcd);
    virtual ~ITheGame()=default;
    virtual uint8_t Play()=0;
    virtual uint8_t Play(SendingData& data)=0;//обертка над Play, с обработкой ввода
};
//----------------    
class Game0: public ITheGame{
  protected:     
    
  public:
    Game0(LiquidCrystal_I2C_3DK *new_lcd);
    virtual ~Game0()=default;
    
    uint8_t Play() override;
    uint8_t Play(SendingData& data) override;
};
//----------------    
class Game1: public ITheGame{
  protected: 
      enum Constants{
        kColorsCount = 7,
        kTargetProgress = 8
      };
      ColorDigital colors_[kColorsCount];//Загаданные цвета      
      uint8_t current_color_n_;
      uint8_t progress_;
      
      GButton button_r_;
      GButton button_g_;
      GButton button_b_;
     
      void MakeRandColors();//Набить colors_ всеми вариациями, кроме черного, в случайном порядке      
  public:
    Game1(LiquidCrystal_I2C_3DK *new_lcd);
    virtual ~Game1()=default;
    
    uint8_t Play() override;   
    uint8_t Play(SendingData& data) override;
};
//----------------
class Game2: public ITheGame{
  protected:      
      enum JoystickDirections : uint8_t{
        kCenter = 0,
        kLeft = 1,
        kRight = 2,
        kUp = 3,
        kDown = 4,
        kOther = 5
      };
      enum Constants{
        kTargetProgress = 8
      };
      
      JoystickDirections target_direction=kOther;      
      uint8_t progress_;
          
  public:
    Game2(LiquidCrystal_I2C_3DK *new_lcd);
    virtual ~Game2()=default;
    
    uint8_t Play() override;
    uint8_t Play(SendingData& data) override;
};
//----------------
class Game3: public ITheGame{
  protected:    
  public:
    Game3(LiquidCrystal_I2C_3DK *new_lcd);
    virtual ~Game3()=default;
    
    uint8_t Play() override;
    uint8_t Play(SendingData& data) override;
};
//----------------
class Game1_2Center: public ITheGame{
  protected:      
    PinNumbersCenter pins_center_;
  public:
    Game1_2Center(LiquidCrystal_I2C_3DK *new_lcd);
    virtual ~Game1_2Center()=default;
    
    uint8_t Play() override;
    uint8_t Play(SendingData& data) override;
};
//----------------
class GameSleep: public ITheGame{
  protected:     
    PinNumbersCenter pins_center_;
    RGBLED rgb_;
  public:
    GameSleep(LiquidCrystal_I2C_3DK *new_lcd);
    virtual ~GameSleep()=default;
    
    uint8_t Play() override;
    uint8_t Play(SendingData& data) override;
};
//---------------- 
class Game3Center: public ITheGame{
  protected:     
    ServoSmooth servo_turret0_;
    ServoSmooth servo_turret1_;
    ServoSmooth servo_secret0_;
    ServoSmooth servo_secret1_;
    ServoSmooth servo_door_;
    uint8_t deg_servo_turret0_;
    uint8_t deg_servo_turret1_;
    uint8_t deg_servo_secret0_;
    uint8_t deg_servo_secret1_;    
    uint8_t deg_servo_door_;
    PinNumbersCenter pins_center_;
    bool secret0_open_=false, secret1_open_=false;
    
    void PinsMode() override;
  public:
    Game3Center(LiquidCrystal_I2C_3DK *new_lcd);
    virtual ~Game3Center()=default;
    
    uint8_t Play() override;
    uint8_t Play(SendingData& data) override;
};
//----------------
class Game3IR: public ITheGame{
  protected:         
    PinNumbersCenter pins_center_;        
    
    IRrecv irrecv_;
    String ircode_="";

    ServoSmooth servo_turret0_;
    ServoSmooth servo_turret1_;
    uint8_t deg_servo_turret0_;
    uint8_t deg_servo_turret1_;
    
    ServoSmooth servo_door_;
    uint8_t deg_servo_door_;    

    String secret_code_="120656";
    String input_code_="000000";

    void ChangeInputCode(String new_input);
    void GetIrCommand();
    void AnimationOpenDoor();

    void PinsMode() override;    
  public:
    Game3IR(LiquidCrystal_I2C_3DK *new_lcd);
    virtual ~Game3IR()=default;
    
    uint8_t Play() override;
    uint8_t Play(SendingData& data) override;
};
//---------------- 
class GameLED: public ITheGame{
  protected:       
    PinNumbersLed pins_led_;
    LED leds_[2][3];//0 - левый, 1 - правый
    uint8_t chosen_one[2]={0,0};//0 - никто, 1-3 - один из

    void PinsMode() override;
  public:
    GameLED(LiquidCrystal_I2C_3DK *new_lcd);
    virtual ~GameLED()=default;
    
    uint8_t Play() override;
    uint8_t Play(SendingData& data) override;
    void SetChosen(uint8_t num, uint8_t val);
};
//----------------
//----------------
//----------------
class GameManagerSide: public ConcreteColleagueGame{
  protected:    
    ITheGame *saw_game_=NULL;;
    LiquidCrystal_I2C_3DK lcd_;

    SendingData::ArduinoOfSaw arduino_type_;
    
    uint8_t Pass0=0;
    uint8_t Pass1=0;

    void SetSawGame(uint8_t game_n);
    void ShowPass(bool both);//Выводит свой кусок пароля на лсд
  public:
    GameManagerSide(SendingData::ArduinoOfSaw arduino_type);
    virtual void Play();

    void Notify(SendingData data) override;
};
//----------------
class GameManagerCenter: public GameManagerSide{
  protected:    
    bool is_quest3_started_=false;    
    //uint8_t Pass[2][2];//левый/правый, первый/второй
    void SetSawGame(uint8_t game_n);
  public:
    GameManagerCenter(SendingData::ArduinoOfSaw arduino_type);    
    void Play() override;

    void Notify(SendingData data) override;
};
//----------------
class GameManagerLED: public GameManagerSide{
  protected:    
  
    void SetSawGame(uint8_t game_n)=delete;
  public:
    GameManagerLED(SendingData::ArduinoOfSaw arduino_type);
    void Play() override;

    void Notify(SendingData data) override;
};
//----------------
#endif
