#ifndef LiquidCrystal_I2C_3DK_h
#define LiquidCrystal_I2C_3DK_h

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TimerFreeTone.h>

#include <stdint.h>
#include <Arduino.h>

//----------------
class LiquidCrystal_I2C_3DK{
  protected:  
  public:    
  LiquidCrystal_I2C lcd_;
  
  LiquidCrystal_I2C_3DK(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize = LCD_5x8DOTS);
  void Print(String A,String B);
  void Print(String str_text, uint8_t str_num, uint8_t place = 0);
  void DisplayBlink(uint8_t count = 3, int delay_ms = 150);
  void DisplayBlinkAndSound(uint8_t buzzer_pin, uint16_t frequency, uint8_t count = 3, int delay_ms = 150);
};
//----------------
#endif
