#include <Adafruit_Sensor.h>
#include <LiquidCrystal.h>
#include "DHT.h"

#define DHTPIN 8 //nem-ısı sensörü
#define Buzzer_pin 53 
#define stepPin 51 //step motor kontrol
#define dirPin 49 //step motor yönü
#define Pin_switch_open 47
#define Pin_switch_close 45 
#define Pin_lazer 43  //lazeri aç kapa için
#define Pin_sayac A0 //LDR okuma için
#define DHTTYPE DHT11

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
DHT dht(DHTPIN, DHTTYPE);

int deger=2;
int sayac=0;
int state=0;
boolean kapi_durumu=false; //false:kapalı     true:açık
boolean engel=false; //false:engele takılmadı     true:engel var.

void setup() {
  pinMode(Buzzer_pin,OUTPUT);
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(Pin_switch_open,INPUT);
  pinMode(Pin_switch_close,INPUT);
  pinMode(Pin_lazer,OUTPUT);
  pinMode(Pin_sayac,INPUT);
     
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("SETUP");
  lcd.begin(16, 2);
  dht.begin();
  lcd.setCursor(0,0);
  lcd.print("S=");
  lcd.setCursor(9,0);
  lcd.print("N=");
  lcd.setCursor(0,1);
  lcd.print("SAYAC=0");
  
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
    engel=false;
    deger=Serial1.read();
    
    if( (deger=='1') && (kapi_durumu==false) ){//Kapıyı aç
      Serial.println("Kapi aciliyor");
      digitalWrite(dirPin,LOW);
      while (digitalRead(Pin_switch_open) == 1){//kapı tamamen açılana kadar
        digitalWrite(stepPin,HIGH); 
        delayMicroseconds(500); 
        digitalWrite(stepPin,LOW); 
        delayMicroseconds(500); 
      }
      Serial.println("Kapi acildi");
      digitalWrite(Pin_lazer,HIGH);
      delayMicroseconds(1000);
      kapi_durumu=true;
    
    }//kapıyı aç
    
    
    if((deger=='0') && (kapi_durumu==true) ){//Kapıyı kapat
      Serial.println("Kapi kapaniyor");
      digitalWrite(dirPin,HIGH);
      while (digitalRead(Pin_switch_close) == 1){//kapı tamamen kapanana kadar
        if(analogRead(A0)>1024/2){
          engel=true;
          break;  
        }
        digitalWrite(stepPin,HIGH); 
        delayMicroseconds(500); 
        digitalWrite(stepPin,LOW); 
        delayMicroseconds(500); 
      }
      if(engel==true){
        Serial.println("Kapi engele takıldı");
        digitalWrite(dirPin,LOW);//motor yönünü ters çevir
        while (digitalRead(Pin_switch_open) == 1){//kapı tamamen açılana kadar
          digitalWrite(stepPin,HIGH); 
          delayMicroseconds(500); 
          digitalWrite(stepPin,LOW); 
          delayMicroseconds(500); 
        }
        Serial.println("Kapi tekrar acildi");
        digitalWrite(Pin_lazer,HIGH);
        delayMicroseconds(1000);
        engel==false;
        
      }
      else{
        Serial.println("Kapi kapandi");
        digitalWrite(Pin_lazer,LOW);
        delayMicroseconds(1000);
        kapi_durumu=false;
      }
      
      
     }//kapıyı kapat
     
    
  }// serial avaiable kapat
  
  if (kapi_durumu==true){//kapı açık sayım yap
  //0-1-0
    
    if(analogRead(A0)>1024/2){
      engel=true;
    }
    else{
      engel=false;
    }
    if(state==0 && engel ==true){//0 -> 1
      state=1;
    }
    else if(state==1 && engel ==false){//1 -> 0
      state=2;
      sayac=sayac+1;
      lcd.setCursor(6, 1);
      lcd.print(sayac);
      state=0;
    }
    
    
  }  
  
}//main loop kapat




