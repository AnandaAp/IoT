//This example shows how to read, store and update database using get, set, push and update functions.

#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ESP32Servo.h>
#define FIREBASE_HOST "https://iot-ukdw.firebaseio.com/"
#define FIREBASE_AUTH "e71jBfP3wr9xE8o9lyGADZr71Tkd8PXu4RUBXujD"
#define WIFI_SSID "SIZZLE-DESIGN"
#define WIFI_PASSWORD "sizzle256"
#define LDR 36
#define SERVO 18
//Define FirebaseESP32 data object
FirebaseData firebaseData;
Servo motorServo;
FirebaseJson json;
String path = "/esp32";
//void printResult(FirebaseData &data);

int oldRotation,newRotation,pos = 0;
float oldLdr = 12,newLdr;

void initWifi(){
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void initFirebase(){
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");

  //optional, set the decimal places for float and double data to be stored in database
  Firebase.setFloatDigits(2);
  Firebase.setDoubleDigits(6);

  /*
  This option allows get and delete functions (PUT and DELETE HTTP requests) works for device connected behind the
  Firewall that allows only GET and POST requests.
  
  Firebase.enableClassicRequest(firebaseData, true);
  */
}

//method untuk membaca cahaya
void ldrDetect(){
  float ldr = analogRead(LDR);
  newLdr = (0.009758 * ldr) + 10;
  Serial.println("Membaca Intensitas Cahaya");
  Serial.print(F("Cahaya :"));
  Serial.print(newLdr);
  Serial.print(" lux \n");
  Firebase.setFloat(firebaseData,path+"/ldr",newLdr);
}

void servo(){
  if(newLdr > oldLdr){
    String servo = "Servo is run anti-clockwise";
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      motorServo.write(pos);    // tell servo to go to position in variable 'pos'
    }
    Serial.println(servo);
    Firebase.setString(firebaseData,path+"/servo",servo);
  }
  else if(newLdr <= oldLdr){
    String servo = "Servo is run clockwise";
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      motorServo.write(pos);    // tell servo to go to position in variable 'pos'
    }
    Serial.println(servo);
    Firebase.setString(firebaseData,path+"/servo",servo);
  }
}

void setup()
{
  initWifi();
  initFirebase();
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  motorServo.setPeriodHertz(50);    // standard 50 hz servo
  motorServo.attach(SERVO, 500, 2400); // attaches the servo on pin 18 to the servo objec
  oldRotation = 0;
  Serial.println("------------------------------------");
  
}


void loop()
{
  if(Firebase.getString(firebaseData,path+"/status")){
    if(firebaseData.stringData() == "system is off"){
      Serial.println("Sistem belum menyala");  
      Serial.println(firebaseData.stringData());
      Firebase.setString(firebaseData,path+"/servo","servo is off");
      delay(1500);
    }
    else if(firebaseData.stringData() == "system is on"){
      Serial.println("Sistem sudah menyala"); 
      Serial.println(firebaseData.stringData());
      ldrDetect();
      servo();
    }
  }
}
