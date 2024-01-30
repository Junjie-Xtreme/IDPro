#include <DS18B20.h>
#include <LiquidCrystal_I2C.h>

DS18B20 ds(2);  //digital port2
LiquidCrystal_I2C lcd(0x27,16,2);  // 定义LCD的地址为0x27，16列2行

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop() {
  while (ds.selectNext()) {
    Serial.println(ds.getTempC());
  }
  lcd.print("PUMP IS WORKING");
  delay (500);
  lcd.clear();
}