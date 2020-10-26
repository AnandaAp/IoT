#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11

/*
 Program ini merupakan program otomasi untuk mengukur suhu dengan sensor, 
 dimana untuk menyalakan Perangkat hanya perlu untuk menekan tombol
 dan untuk mematikan sistem hanya perlu dengan menekan dan menahan tombol 
 selama beberapa detik.
 Catatan, ketika proses mengukur suhu berlangsung, sistem tidak dapat dimatikan
 sampai sistem menunjukan hasil pengukuran suhu

 
 Dibuat dan disusun oleh Ananda Apriliansyah
 Credit for Ananda Apriliansyah Cahya Utama, a Informatic Student of Duta Wacana Christian University Yogyakarta, Indonesia
*/


//membuat object DHT bernama dht dengan pin dan tipenya
DHT dht(DHTPIN, DHTTYPE);

// Set seluruh LED pin
const int ledPinRed =  26;
const int ledPinYellow = 27;
const int ledPinGreen = 23;
const int btn = 0;

int btnState = 0; //logic button low
int run; //variable untuk menentukan otomasi berjalan atau tidak

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
  //memberikan value dari button kepada btnState
  btnState = digitalRead(btn);
  //mengecek apakah program sedang berjalan dengan nilai run lebih dari 0
  if(run > 0){
    //mengecek apakah button ditekan, jika ditekan maka akan mematikan sistem
    if(btnState == LOW){
      run = 0;
      Serial.println("Mematikan Sistem");        
      delay(4000);
      digitalWrite(ledPinRed,HIGH);
      digitalWrite(ledPinYellow,HIGH);
      digitalWrite(ledPinGreen,HIGH);
      Serial.println("Berhasil Mematikan Sistem");
      return;
    }
    //jika tidak maka kembali menjalankan method tempDetect
    else{
      tempDetect();
    }
  }
  /*jika aplikasi belum berjalan dan tombol ditekan, 
  maka sistem akan mejalankan pengukuran suhu*/
  else if(run == 0 && btnState == LOW){
    Serial.println("Button Ditekan, Memulai Sistem");
    Serial.println("sistem membutuhkan waktu 4 detik untuk memulai menjalankan otomasi");
    //sistem membutuhkan waktu 4 detik untuk memulai menjalankan otomasi
    delay(4000);
    Serial.println("MEMULAI MENGUKUR SUHU!!!!!!");
    //merubah nilai run menjadi 255 jika nilai run masih 0
    if(run == 0){
      run = 255;
    }
  }
}

void tempDetect(){
    //memberikan delay 2 detik untuk memulai pengukuran suhu
    delay(2000);
    //membaca temperatur menjadi celcius
    temperaturCelcius = dht.readTemperature();
    //mengecek apakah gagal mengukur suhu
    if(isnan(temperaturCelcius)){
      Serial.println("gagal mengukur suhu");
      Serial.println("kembali melakukan pengukuran mengukur");
      tempDetect();
    }
    //jika sukses mengukur suhu
    else{
      Serial.print(F("Temperature: "));
      Serial.print(temperaturCelcius);
      Serial.print("°C \n");
      checkTemp();
      //memberi delay 2 detik untuk mengecek apakah button kembali ditekan atau tidak
      delay(2000);
    }
}

//method untuk mengecek suhu dan menyalakan led
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
