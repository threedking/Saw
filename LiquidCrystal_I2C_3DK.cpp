#include "LiquidCrystal_I2C_3DK.h"
//------------------------0x27, 16, 2
LiquidCrystal_I2C_3DK::LiquidCrystal_I2C_3DK(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize = LCD_5x8DOTS):lcd_(lcd_addr, lcd_cols, lcd_rows, charsize){
  this->lcd_.begin();
  this->lcd_.backlight();  
}
//------------------------
void LiquidCrystal_I2C_3DK::Print(String str_a,String str_b){
  this->lcd_.setCursor(0,0);
  this->lcd_.print(str_a);
  this->lcd_.setCursor(0,1);
  this->lcd_.print(str_b);
}
//------------------------
void LiquidCrystal_I2C_3DK::Print(String str_text, uint8_t str_num, uint8_t place = 0){
  this->lcd_.setCursor(place,str_num);
  this->lcd_.print(str_text);
}
//------------------------
void LiquidCrystal_I2C_3DK::DisplayBlink(uint8_t count = 3, int delay_ms = 150){
  for(int i=0;i<count;i++){
    this->lcd_.noDisplay();
    delay(delay_ms);
    this->lcd_.display();  
    delay(delay_ms);
  }
}
//------------------------
void LiquidCrystal_I2C_3DK::DisplayBlinkAndSound(uint8_t buzzer_pin, uint16_t frequency, uint8_t count = 3, int delay_ms = 150){
  for(int i=0;i<count;i++){
    this->lcd_.noDisplay();
    TimerFreeTone(buzzer_pin, frequency/2, delay_ms);
    delay(delay_ms);
    this->lcd_.display();
    TimerFreeTone(buzzer_pin, frequency, delay_ms);
    delay(delay_ms);
  }
} 
//------------------------
