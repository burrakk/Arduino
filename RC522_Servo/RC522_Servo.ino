//Kütüphaneler
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Servo.h>
#include <MFRC522.h>
#include <SPI.h>

//Wifi  / Firebase Bağlantı Ayarları 
#define FIREBASE_HOST "arduino-592d0.firebaseio.com"
#define FIREBASE_AUTH "QNzkpa2jCn6qr3NbQhjaCXX0sX30NSxgiPAJs6Qw"
#define WIFI_SSID "pc"
#define WIFI_PASSWORD "1789b1995b54"

//Sensor Pinleri
#define SS_PIN D4 
#define RST_PIN D3

//RC522
MFRC522 mfrc522(SS_PIN, RST_PIN);
int doorState;

//Servo
Servo servo;
int servoLock=0;
int ackapa=0;

void setup() {
  Serial.begin(9600);
  delay(10);
  //Servo Bağlantı
  servo.attach(D1);
  servo.write(0);
  //RC522
  SPI.begin();
  mfrc522.PCD_Init();
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Bağlanılıyor ... ");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Şu adrese bağlandı : ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  if (content.substring(1) == "F4 60 D2 75") //change UID of the card that you want to give access
  {
    Serial.println(" Erişim Sağlandı ");
    Serial.println(" Merhaba Burak ");
    delay(1000);
    if(ackapa==0){
      servo.write(90);
      ackapa=1;
    }else{
      servo.write(0);
      ackapa=0;
    }
   
    Serial.println();
    servoLock = 10;
    Firebase.setInt("servoLock", servoLock);
    delay(1000);
  }
  else if (content.substring(1) == "CE AA 25 D9") //change UID of the card that you want to give access
  {
    Serial.println(" Erişim Sağlandı ");
    Serial.println(" Merhaba Engin ");
    delay(1000);
    if(ackapa==0){
      servo.write(90);
      ackapa=1;
    }else{
      servo.write(0);
      ackapa=0;
    }
    Serial.println();
    servoLock = 11;
    Firebase.setInt("servoLock", servoLock);
    delay(1000);
  }
  
  else   {
    Serial.println(" Erişim Engellendi ! ");
    servoLock = 1;
    Firebase.setInt("servoLock", servoLock);
    servo.write(0);
    delay(1000);
  }
  //Kapı Kilit Kontrolü Uzaktan Kullanıcı Tarafından Ele Geçirilir.
  int override = Firebase.getInt("DoorOverride");
  if (override == 1)
  {
    checkDL();
  }
}


void checkDL(){
  int dlock = Firebase.getInt("doorLock");
  if(dlock==1 && ackapa == 0){
    servo.write(90);
  }else if (dlock==0 && ackapa ==1){
    servo.write(0);
  }
  return;
}
