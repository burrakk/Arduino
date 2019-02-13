//Kütüphaneler
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <SimpleDHT.h>

//Wifi  / Firebase Bağlantı Ayarları 
#define FIREBASE_HOST "arduino-592d0.firebaseio.com"
#define FIREBASE_AUTH "QNzkpa2jCn6qr3NbQhjaCXX0sX30NSxgiPAJs6Qw"
#define WIFI_SSID "pc"
#define WIFI_PASSWORD "1789b1995b54"

//Sensor Pinleri
#define DHTPIN  D1
#define PIRpin  D2

//Sensorler için Flag
int PIRstate = 0 ;

// DHT-11
byte temperature = 0;
byte humidity = 0;

SimpleDHT11 dht11(DHTPIN);

void setup() {
  Serial.begin(9600);
  delay(10);
  pinMode(PIRpin, INPUT);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {

  if(digitalRead(PIRpin) == HIGH && PIRstate == 0)
  {
    PIRstate = 1;
    Serial.println("Hareket Algılandı: ");
    Firebase.setBool("Motion", true);
    if (Firebase.failed()) 
    {
      Serial.println("setting /Motion failed:");
      Serial.println(Firebase.error());  
      return;
    }
    delay(1000);
  }else if (digitalRead(PIRpin)== LOW && PIRstate == 1){
    PIRstate = 0;
    Serial.println("Harekesiz: ");
    Firebase.setBool("Motion", false);
    if (Firebase.failed()) 
    {
      Serial.print("setting /Motion failed:");
      Serial.println(Firebase.error());  
      return;
    }
    
  delay(1000);
  }
  dht11.read(&temperature, &humidity, NULL);
  float h = (float)temperature;
  float t = (float)humidity;
  Firebase.setFloat("temp",t);
  Firebase.setFloat("hum",h);
  delay(200);
}
