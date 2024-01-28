#include <LiquidCrystal.h> //call library
#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_BUS A0 //define data output port is A0
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup(void) {
  // put your setup code here, to run once:
Serial.begin(9600);
sensors.begin();
}

void loop(void) {
  // put your main code here, to run repeatedly:
sensors.requestTemperatures();
Serial.print("Temperature is:");
Serial.print(sensors.getTempCByIndex(0));
Serial.println("℃");
delay(1000); //refresh per sec
}
