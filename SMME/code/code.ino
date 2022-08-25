
#define BLYNK_PRINT Serial
 
#include "EmonLib.h"   //https://github.com/openenergymonitor/EmonLib
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#define BLYNK_AUTH_TOKEN           "1dwMGTSQMUCUT4GiwwmgdgtJjCbHA1K3" //token gerado pelo Blynk

 
EnergyMonitor emon;
#define vCalibration 106.8
#define currCalibration 02.0
BlynkTimer timer;

 ///////////----Conexão com wifi----///////////
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "SSID WIFI";
char pass[] = "SENHA WIFI";

///////////----Temperatura----///////////
int val;
double Volt = 0;
int Celcius = 0;
const int pino_sensor = 34; // LM35 di pin 

///////////----Vibração----///////////
int pino_vibra = 35;
int Vibra;

 ///////////----variaveis para calcular potência----///////////
float kWh = 0;
unsigned long lastmillis = millis();

void vibra(){
  Vibra = analogRead(pino_vibra);
}

void Temperatura(){
  val = analogRead(pino_sensor);// Leitura sensor de temperatura
  Volt = (val / 2048.0) * 3300; // converte em valor de tensão
  Celcius = Volt * 0.1; // converte em temperatura
     
 }


 
void myTimerEvent() {
    
    emon.calcVI(20, 2000);
    Serial.print("Vrms: ");
    Serial.print(emon.Vrms, 2);
    Serial.print("V");
    Blynk.virtualWrite(V0, emon.Vrms);
    
    Serial.print("\tIrms: ");
    Serial.print(emon.Irms, 4);
    Serial.print("A");
    Blynk.virtualWrite(V1, emon.Irms);
    
    Serial.print("\tPower: ");
    Serial.print(emon.apparentPower, 4);
    Serial.print("W");
    Blynk.virtualWrite(V2, emon.apparentPower);
    
    kWh = kWh + emon.apparentPower*(millis()-lastmillis)/3600000000.0;
    Serial.print(kWh, 4);
    Serial.println("kWh");
    lastmillis = millis();
    Blynk.virtualWrite(V3, kWh);
    Serial.print("\tkWh: ");

    Serial.print("\tTemperatura:");
    Serial.println(Celcius);
    Blynk.virtualWrite(V4,Celcius);
    
    Serial.print("\tValor:");
    Serial.println(Vibra);
    Blynk.virtualWrite(V5,Vibra);
}
 
void setup() {
  Serial.begin(9600);
  pinMode(pino_vibra,INPUT);// 35 Sensor de vibração
  pinMode(pino_sensor,INPUT);//34 Sensor de temperatura
  emon.voltage(36, vCalibration, 1.7); // Tensão: pino de entrada, calibração, Raiz de 3.
  emon.current(39, currCalibration); // Corrente: pino de entrada, calibração.
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}
 
void loop() {
  vibra();
  Temperatura();
  Blynk.run();
  timer.run();
}
