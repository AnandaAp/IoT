#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Set seluruh LED pin
const int ledPinRed =  26;
const int ledPinYellow = 27;
const int ledPinGreen = 23;
const int btn = 0;

/* unsigned long berfungsi untuk menyimpan data yang bersifat "menyimpan waktu"
   karena int tidak mampu untuk menyimpan waktu
*/
unsigned long previousMillis = 0;        // akan menyimpah waktu terakhir led dinyalakan

// interval yang aku gunakan:
const long invRed = 10000;           // interval dalam satuan milisecond
const long invYellow = 3000;
const long invGreen = 6000;

//int arahLed = 0;
int btnState = 0;

//membaca temperatur menjadi celcius
float temperaturCelcius;

void setup() {
  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinYellow, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);
  digitalWrite(ledPinRed,HIGH);
  digitalWrite(ledPinYellow,HIGH);
  digitalWrite(ledPinGreen,HIGH);
  pinMode(btn,INPUT);
  Serial.begin(115200);
  Serial.println(F("DHT11 test!"));
  dht.begin();
}

void loop() {
  btnState = digitalRead(btn);
  if(btnState == LOW){
    digitalWrite(ledPinRed,HIGH);
    digitalWrite(ledPinYellow,HIGH);
    digitalWrite(ledPinGreen,HIGH);
    Serial.println("Button Ditekan");
    Serial.println("Memulai mengukur suhu, Waktu yang dibutuhkan untuk mengukur adalah 10 detik");
    HumidDetect();
  }
}

void HumidDetect(){
  delay(10000);
  //membaca temperatur menjadi celcius
  temperaturCelcius = dht.readTemperature();
  if(isnan(temperaturCelcius)){
    Serial.println("Failed to read from DHT Sensor");
    return;
  }
  Serial.print(F("Temperature: "));
  Serial.print(temperaturCelcius);
  Serial.print("°C \n");
  checkTemp();
 
}
void checkTemp(){
  delay(500);
  if(temperaturCelcius < 35){
    digitalWrite(ledPinGreen,LOW);
    Serial.println("Aman");
  }
  else if(temperaturCelcius >= 35.00 && temperaturCelcius <= 50.00){
    digitalWrite(ledPinGreen,LOW);
    digitalWrite(ledPinYellow,LOW);
    Serial.println("Siaga");
  }
  else  if(temperaturCelcius > 50.00){
    digitalWrite(ledPinRed,LOW);
    digitalWrite(ledPinYellow,LOW);
    digitalWrite(ledPinGreen,LOW);
    Serial.println("Darurat");
  }
}
