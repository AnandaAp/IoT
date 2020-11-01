#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

//mendefinisikan pin out DHT dengan GPIO 4
#define DHTPIN 4
//Mendefinisikan tipe DHT dengan DHT 11
#define DHTTYPE DHT11
//mendefinisikan pin untuk ldr
#define CENSORPIN 36 //A0
#define BLYNK_PRINT Serial
//menginisialisasikan variable warna
#define BLYNK_GREEN     "#23C48E"
#define BLYNK_RED       "#D3435C"

/*
 Program ini merupakan otomasi Sistem monitoring pencahayaan dan suhu kandang burung menggunakan ESP32.
 Pada program ini bertujuan untuk melakukan otomasi dan remote sistem menggunakan Blynk.
 
 Dibuat dan disusun oleh Ananda Apriliansyah
 Credit for Ananda Apriliansyah Cahya Utama, a Informatic Student of Duta Wacana Christian University Yogyakarta, Indonesia
*/


//membuat object DHT bernama dht dengan pin dan tipenya
DHT dht(DHTPIN, DHTTYPE);
//membuat variable dari BlynkTimer
BlynkTimer timer;
//membuat variable untuk led dari WidgetLED
WidgetLED ledCahaya(V6),ledSuhu(V7);

// Set seluruh LED pin
const int ledPinRed =  26,ledPinYellow = 27;

//membaca temperatur menjadi celcius
int Celcius;
//membaca ldr awal-awal
float ldr;
//membaca intensitas cahaya
float ldrVal;

/*
 sliderCahaya berfungsi untuk menampung nilai dari V4 dan 
 sliderCahayaValue berfungsi untuk menampung hasil konversi ke satuan lux
*/
float sliderCahaya,sliderCahayaValue;
/*
 sliderSuhu berfungsi untuk menampung nilai dari V5
 btn berfungsi untuk menampung niladi dari V8
*/
int sliderSuhu,btn;

//Token untuk terhubung dengan aplikasi
char auth[] = "LvFdSuEMghJf0ox9L3ylanji-SED_T0I";

//authentikasi ssid dan password theatering wifi
char ssid[] = "realme 3";
char pass[] = "test123q";

//untuk mengambil data dari Virtual Pin V4
BLYNK_WRITE(V4)
{
  sliderCahaya = param.asFloat(); // assigning incoming value from pin V1 to a variable
  sliderCahayaValue = (0.009758 * sliderCahaya) + 10;
  Serial.print(F("Nilai Slider Cahaya : "));
  Serial.print(sliderCahayaValue);
  Serial.print(F(" lux \n"));
}

//untuk mengambil data dari Virtual Pin V5
BLYNK_WRITE(V5)
{
  sliderSuhu = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print(F("Nilai Slider Suhu : "));
  Serial.print(sliderSuhu);
  Serial.print(F("°C \n"));
}

//untuk mengambil data dari Virtual Pin V8
BLYNK_WRITE(V8){
  btn = param.asInt();
  if(btn == 0){
    Serial.println("Button Off");
  }
  else{
    Serial.println("Button On");
  }
}

void setup() {
  //untuk debug
  Serial.begin(115200);
  //memulai koneksi ke Blynk
  Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,8), 8080);
  
  //menginisialisasikan led sebagai output
  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinYellow, OUTPUT);

/*
  karena input berupa LOW, maka mematikan led 
  terlebih dahulu dengan membuatnya menjadi HIGH
*/
  digitalWrite(ledPinRed,HIGH);
  digitalWrite(ledPinYellow,HIGH);

  //men-setup method yang dipanggil setiap detiknya
  timer.setInterval(1000L, utsProgram);
  dht.begin();
}

void loop() {
  if(Blynk.connected()){
    //memulai Blink
    Serial.println("Sistem sudah terhubung dengan server");
    ul:
    Blynk.run();
    if(btn == 1){
      timer.run();
    }
    goto ul;
  }
  else{
    Serial.println("Sistem gagal terhubung dengan server");
  }
}

//method untuk memasukan method cahaya dan suhu
void utsProgram(){
  tempDetect();
  ldrDetect();
}

//method untuk membaca suhu
void tempDetect(){
    Serial.println("Membaca Suhu");
    //membaca temperatur menjadi celcius
    Celcius = dht.readTemperature();
    //mengecek apakah gagal mengukur suhu
    if(isnan(Celcius)){
      Serial.println("gagal mengukur suhu");
      Serial.println("kembali melakukan pengukuran suhu");
      tempDetect();
    }
    //jika sukses mengukur suhu
    else{
      Serial.print(F("Temperature: "));
      Serial.print(Celcius);
      Serial.print("°C \n");
      Blynk.virtualWrite(V1, Celcius);
      Blynk.virtualWrite(V3, Celcius);
      checkTemp();
    }
}

//method untuk mengecek suhu
void checkTemp(){
  if(Celcius < sliderSuhu){
    digitalWrite(ledPinYellow,LOW);
    Blynk.virtualWrite(V7,ledPinYellow);
    ledSuhu.setColor(BLYNK_RED);
    ledSuhu.on();
    Serial.println("Indikator Suhu menyala berwarna merah");
  }
  else{
    digitalWrite(ledPinYellow,HIGH);
    Blynk.virtualWrite(V7,ledPinYellow);
    ledSuhu.setColor(BLYNK_GREEN);
    ledSuhu.on();
    Serial.println("Indikator Suhu menyala berwarna hijau");
  }
}

//method untuk membaca cahaya
void ldrDetect(){
  ldr = analogRead(CENSORPIN);
  ldrVal = (0.009758 * ldr) + 10;
  Serial.println("Membaca Intensitas Cahaya");
  Serial.print(F("Cahaya :"));
  Serial.print(ldrVal);
  Serial.print(" lux \n");
  Blynk.virtualWrite(V0,ldrVal);
  Blynk.virtualWrite(V2,ldrVal);
  checkLdr();
}

//method untuk mengecek intensitas cahaya
void checkLdr(){
  if(ldrVal < sliderCahayaValue){
    digitalWrite(ledPinRed,LOW);
    ledCahaya.setColor(BLYNK_GREEN);
    ledCahaya.on();
    Serial.println("Indikator Cahaya Menyala");
  }
  else{
    digitalWrite(ledPinRed,HIGH);
    ledCahaya.setColor(BLYNK_RED);
    ledCahaya.on();
    Serial.println("Indikator Cahaya Mati");
  }
}
