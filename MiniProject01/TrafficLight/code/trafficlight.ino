// Set seluruh LED pin
const int ledPinRed =  26;
const int ledPinYellow = 27;
const int ledPinGreen = 23;

/* Input led aku set GND
   Jadi ketika pertama kali dijalankan,maka 
   seluruh led akan menyala terlebih dahulu
*/

/* unsigned long berfungsi untuk menyimpan data yang bersifat "menyimpan waktu"
   karena int tidak mampu untuk menyimpan waktu
*/
unsigned long previousMillis = 0;        // akan menyimpah waktu terakhir led dinyalakan

// interval yang aku gunakan:
const long invRed = 10000;           // interval dalam satuan milisecond
const long invYellow = 3000;
const long invGreen = 6000;

int arahLed = 0;

void setup() {
  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinYellow, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  trafficLight();
}

void trafficLight(){
  /* 
    Variable digunakan untuk mengambil detik sekarang sebagai patokan
  */
  unsigned long currentMillis = millis();
  if(arahLed == 0){
    digitalWrite(ledPinRed,LOW);
    digitalWrite(ledPinYellow,HIGH);
    digitalWrite(ledPinGreen,HIGH);
    arahLed++;
    previousMillis += invRed;
  }
  else{
    if(arahLed == 1 && previousMillis - currentMillis == 0){
      digitalWrite(ledPinRed,HIGH);
      digitalWrite(ledPinYellow,LOW);
      arahLed++;
      previousMillis += invYellow;
    }
    else if(arahLed == 2 && previousMillis - currentMillis == 0){
      arahLed++;
      digitalWrite(ledPinYellow,HIGH);
      digitalWrite(ledPinGreen,LOW);
      previousMillis += invGreen;
    }
    else if(arahLed == 3 && previousMillis - currentMillis == 0){
      digitalWrite(ledPinRed,LOW);
      digitalWrite(ledPinGreen,HIGH);
      arahLed = 1;
      previousMillis += invRed;
    }
  }
}
