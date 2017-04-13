//LCD 2x16
//
//
//ısı+nem sensörünü ekrana yazdır
//
//if (kapı aç and kapı kapalı)
//  switche kadar aç
//    kapı açık
//
//
//if (kapıyı kapat and kapı açık)
//  araya engel gelirse kapıyı aç
//    kapı kapıtıldı, lazeri kapat
//
//
//if kapı açık
//  sayım yap 0-1-0

#include <Adafruit_Sensor.h>
#include <LiquidCrystal.h>
#include "DHT.h"
#define DHTPIN 8
#define Buzzer_pin 53 
#define stepPin 51 
#define dirPin 49
#define Pin_switch_open 47
#define Pin_switch_close 45
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);






int deger=2;
boolean kapi_acik = false;
int lazer_engel;

void setup() {
  pinMode(Buzzer_pin,OUTPUT);
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(Pin_switch_open,INPUT);
  pinMode(Pin_switch_close,INPUT);
  
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("SETUP");
  lcd.begin(16, 2);
  dht.begin();
  lcd.setCursor(0,0);
  lcd.print("S=");
  lcd.setCursor(9,0);
  lcd.print("N=");
  
}

void loop() {
  
  // Isı nem sensörünü oku ekrana yazdır.
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    lcd.setCursor(0,0);
    lcd.print("ERROR");
    return;
  }
  lcd.setCursor(2, 0);
  lcd.print(t);
  lcd.setCursor(11,0);
  lcd.print(h);
  
  if (t > 40.0){//sıcaklık yüksek ise buzzer ötsün
      digitalWrite(Buzzer_pin,HIGH);
      delay(500);
      digitalWrite(Buzzer_pin,LOW);
  }

  
  
  
  if (Serial1.available()){
    deger=Serial1.read();
    Serial.println(deger);
    
    if(deger=='1'){//Kapıyı aç
      Serial.println("Kapi aciliyor");
      digitalWrite(dirPin,LOW);
      while (digitalRead(Pin_switch_open) == 1){//kapı tamamen açılana kadar
        digitalWrite(stepPin,HIGH); 
        delayMicroseconds(500); 
        digitalWrite(stepPin,LOW); 
        delayMicroseconds(500); 
      }
      Serial.println("Kapi acildi");
      
      
    }//kapıyı aç
    
    
    if(deger=='0'){//Kapıyı aç
      Serial.println("Kapi kapaniyor");
      digitalWrite(dirPin,HIGH);
      while (digitalRead(Pin_switch_close) == 1){//kapı tamamen açılana kadar
        digitalWrite(stepPin,HIGH); 
        delayMicroseconds(500); 
        digitalWrite(stepPin,LOW); 
        delayMicroseconds(500); 
      }
      Serial.println("Kapi kapandi");
    
     }//kapıyı aç
    
  }//
  
  
  
  
}//main loop kapat



