#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11
//membuat object DHT bernama dht dengan pin dan tipenya
DHT dht(DHTPIN, DHTTYPE);

// Set seluruh LED pin
const int ledPinRed =  26;
const int ledPinYellow = 27;
const int ledPinGreen = 23;
const int btn = 0;

int btnState = 0; //logic button low
int run;

//membaca temperatur menjadi celcius
float temperaturCelcius;

void setup() {
  run = 0; //memulai dan memberhentikan sistem
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
  if(run > 0){
    if(btnState == LOW){
      run = 0;
      Serial.println("Mematikan Sistem");        
      delay(4000);
      digitalWrite(ledPinRed,HIGH);
      digitalWrite(ledPinYellow,HIGH);
      digitalWrite(ledPinGreen,HIGH);
      return;
    }
    else{
      HumidDetect();
    }
  }
  else if(run == 0 && btnState == LOW){
    Serial.println("Button Ditekan, Memulai Sistem");
    delay(4000);
    Serial.println("MEMULAI MENGUKUR SUHU!!!!!!");
    if(run == 0){
      run = 255;
    }
  }
}

void HumidDetect(){
    delay(2000);
    //membaca temperatur menjadi celcius
    temperaturCelcius = dht.readTemperature();
    //mengecek apakah berhasil mengukur suhu
    if(isnan(temperaturCelcius)){
      Serial.println("gagal mengukur suhu");
      Serial.println("kembali melakukan pengukuran mengukur");
      HumidDetect();
    }
    Serial.print(F("Temperature: "));
    Serial.print(temperaturCelcius);
    Serial.print("°C \n");
    checkTemp();
    delay(2000);
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
