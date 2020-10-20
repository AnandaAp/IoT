// Set seluruh LED pin
const int ledPinRed =  26;
const int ledPinYellow = 27;
const int ledPinGreen = 23;

/* Input led aku set GND
   Jadi ketika pertama kali dijalankan,maka 
   seluruh led akan menyala terlebih dahulu
*/
int ledStateRed = LOW;             
int ledStateYellow = LOW;
int ledStateGreen = LOW;

/* unsigned long berfungsi untuk menyimpan data yang bersifat "menyimpan waktu"
   karena int tidak mampu untuk menyimpan waktu
*/
unsigned long previousMillis = 0;        // akan menyimpah waktu terakhir led dinyalakan

// interval yang aku gunakan adalah 3 detik
const long interval = 3000;           // interval dalam satuan milisecond

//method yg berfungsi untuk merubah status led
void runTrafficLight(){
  digitalWrite(ledPinRed, ledStateRed);
  digitalWrite(ledPinYellow, ledStateYellow);
  digitalWrite(ledPinGreen, ledStateGreen);
}

void trafficLight(long currentMillis){
  if (currentMillis - previousMillis >= interval) {
    // simpan detik terakhir led menyala
    previousMillis = currentMillis;

    /*
      Posisi awal led adalah semua menyala selama 3 detik untuk permulaan
      kemudian led merah akan menyala sendiri selama 3 detik
      kemudian pada detik ke-3, led merah mati dan led kuning menyala selama 3 detik
      lalu setelah detik ke-6, led kuning mati dan led hijau menyala selama 3 detik
      dan kemudian pada detik ke-9, led merah kembali menyala dan led hijau mati
    */
    if (ledStateRed == LOW && ledStateYellow == LOW && ledStateGreen == LOW) {
      ledStateRed = LOW;
      ledStateYellow = HIGH;
      ledStateGreen = HIGH;
    }else if (ledStateRed == LOW && ledStateYellow == HIGH && ledStateGreen == HIGH) {
      ledStateRed = HIGH;
      ledStateYellow = LOW;
      ledStateGreen = HIGH;
    } else if(ledStateRed == HIGH && ledStateYellow == LOW && ledStateGreen == HIGH){
      ledStateRed = HIGH;
      ledStateYellow = HIGH;
      ledStateGreen = LOW;
    }
    else if(ledStateRed == HIGH && ledStateYellow == HIGH && ledStateGreen == LOW){
      ledStateRed = LOW;
      ledStateYellow = HIGH;
      ledStateGreen = HIGH;
    }
    runTrafficLight();
  }
}


void setup() {
  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinYellow, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  /* 
    Variable digunakan untuk mengambil detik sekarang sebagai patokan
  */
  unsigned long currentMillis = millis();
  trafficLight(currentMillis);
}
