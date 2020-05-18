#include <Wire.h>

#include <LiquidCrystal_I2C.h>
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include "DHT.h"
#define Soil A0
#define DHTPIN D5 //14
#define fan1 D1
#define fan2 D4
#define water1 D7
#define water2 D8
#define DHTTYPE DHT11
#define FIREBASE_HOST "nodemcu-de986.firebaseio.com"
#define FIREBASE_AUTH "Im48JL2QGm03TJd1BpkhHotTyG14baS0Te134OW3"
#define WIFI_SSID "Smart"
#define WIFI_PASSWORD "Vukey_don123"
String stringH;
String stringT;
String stringS;
String Dry = "Dry";
String Wet = "Wet";
String Normal = "Normal";
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x3F, 16, 2);
void setup()
{
  Serial.begin(115200);
  Wire.begin(D2,D3);
  lcd.begin();
  lcd.home();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED)
      {
    Serial.print(".");
    delay(500);
      }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  dht.begin();
  pinMode(DHTPIN,INPUT);
  pinMode(fan1,OUTPUT);
  pinMode(fan2,OUTPUT);
  pinMode(water1,OUTPUT);
  pinMode(water2,OUTPUT);
  pinMode(Soil,INPUT);
 
}
 
void loop()
{
  int soil_value = analogRead(Soil);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  /*Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.println(t);
  Serial.print("Soil value: ");*/
  Serial.println(soil_value);
stringH = String(h);
stringT = String(t);
//stringS  = String(soil_value);
String fan = Firebase.getString("Fan");
if(fan == "ON")
{
  digitalWrite(fan1,HIGH);
  digitalWrite(fan2,LOW);
  Serial.println(" FAN ON!!!!!!");
}
else
{
  digitalWrite(fan1,LOW);
  digitalWrite(fan2,LOW);
  Serial.println("FAN OFF!!!!");
}
String water = Firebase.getString("Water_pump");
if(water == "ON")
{
  digitalWrite(water1,HIGH);
  digitalWrite(water2,LOW);
  Serial.println("Water pump is ON!!");
}
else
{
  digitalWrite(water1,LOW);
  digitalWrite(water2,LOW);
  Serial.println("Water pump is OFF!!");
} 

lcd.clear();
lcd.setCursor(0,0);
lcd.print("Temp = " + stringT + char(223) + "C" ); //°C
lcd.setCursor(0,1);
lcd.print("Humidity = " + stringH + "%");
Serial.println("Temp = " + stringT + char(223) + "C" ); //°C
Serial.println("Humidity = " + stringH + "%");
Firebase.setString("humidity/Value",stringH);
Firebase.setString("temperature/Value",stringT);
lcd.clear();
lcd.setCursor(0,0);
if(soil_value >= 850)
{
  Firebase.setString("Soil_moisture/Value",Dry);
  digitalWrite(water1,HIGH);
  digitalWrite(water2,LOW);
  lcd.print("I am thirsty");
  Serial.println("I am thirsty");
  Serial.println("Water pump is ON!!");
  delay(2000);
}
else if (soil_value<=400)
{
  Firebase.setString("Soil_moisture/Value",Wet);
  digitalWrite(water1,LOW);
  digitalWrite(water2,LOW);
  Serial.println("Too much water");
  lcd.print("Too much water");
  Serial.println("Water pump is OFF!!");
  delay(2000);
  
}
else 
{
  Firebase.setString("Soil_moisture/Value",Normal);
  lcd.print("I am comfortable");
  Serial.println("I am comfortable");
  Serial.println("Water pump is OFF!!");
  digitalWrite(water1,LOW);
  digitalWrite(water2,LOW);
  delay(2000);
}
}
