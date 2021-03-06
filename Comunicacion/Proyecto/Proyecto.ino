#include "ESP8266WiFi.h"
#include <WiFiClient.h> 
#include <Servo.h>

int fotores = A0;
int ledPin = 15;
int servoPin = 0;
int motorPinR = 16;
int motorPinL = 5;

#include <DHT.h>
 
// Definimos el pin digital donde se conecta el sensor
#define DHTPIN 4
// Dependiendo del tipo de sensor
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

Servo servo;

unsigned long previousMillis = 0;

//-------------------VARIABLES Necesarias para conexión--------------------------
int contconexion = 0;

const char *ssid = "iPhone";
const char *pass = "12345678";


char host[48];
String strhost = "172.20.10.7";
String strurl = "/enviardatos.php";


float tempSum, humSum, lightSum, startLight= 0, startTemp = 0, startHum = 0, relTemp, relHum, relLight;
int tempCount, humCount, lightCount;


//--------------------------------------------------------------------------------


//-------Función para Enviar Datos a la Base de Datos SQL--------

String enviardatos(String datos) {
  String linea = "error";
  WiFiClient client;
  strhost.toCharArray(host, 49);
  if (!client.connect(host, 80)) {
    Serial.println("Fallo de conexion");
    return linea;
  }

  client.print(String("POST ") + strurl + " HTTP/1.1" + "\r\n" + 
               "Host: " + strhost + "\r\n" +
               "Accept: */*" + "*\r\n" +
               "Content-Length: " + datos.length() + "\r\n" +
               "Content-Type: application/x-www-form-urlencoded" + "\r\n" +
               "\r\n" + datos);           
  delay(10);             
  
  Serial.print("Enviando datos a SQL...");
  
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println("Cliente fuera de tiempo!");
      client.stop();
      return linea;
    }
  }
  // Lee todas las lineas que recibe del servidro y las imprime por la terminal serial
  while(client.available()){
    linea = client.readStringUntil('\r');
  }  
  Serial.println(linea);
  return linea;
}
void setup()
{
  Serial.begin(9600);

  pinMode(fotores, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(motorPinR, OUTPUT);
  servo.attach(servoPin);

  // Conexión WIFI
   delay(10);     
   Serial.println("Connecting to ");
   Serial.println(ssid);
   WiFi.begin(ssid, pass);

int tries = 0;

  while (WiFi.status() != WL_CONNECTED && tries<20) 
 {
        delay(500);
        Serial.print(".");
        tries++;
 }
  Serial.println("");
  Serial.println("WiFi connected");

  dht.begin();

}



void loop()
{


  

  
  // Leemos la humedad relativa
  float hum = dht.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float temp = dht.readTemperature();

  float light = analogRead(fotores);

  tempSum+=temp;
  humSum+=hum;
  lightSum+=light;

  tempCount++;
  humCount++;
  lightCount++;

    if(startHum>0 && startTemp>0 && startLight>0){

    relHum = ((humSum/humCount)/startHum)*100;
    relTemp = ((tempSum/tempCount)/startTemp)*100;
    relLight = ((lightSum/lightCount)/startLight)*100;
    
   
  }

 

  if(relTemp > 101){
    digitalWrite(motorPinR, HIGH);
    digitalWrite(motorPinL, HIGH);
  } else {
    digitalWrite(motorPinR, LOW);
    digitalWrite(motorPinL, LOW);
  }

  if(relHum > 120){
    servo.write(120);

  } else {
    servo.write(60);
  }

  if(relLight < 50){
    digitalWrite(ledPin, LOW);
  } else {
    digitalWrite(ledPin, HIGH);
  }

  if(startHum == 0 && hum > 0){
    startHum = hum;
  }

  if(startTemp == 0 && temp > 0){
    startTemp = temp;
  }

  if(startLight == 0 && light > 0){
    startLight = light;
  }

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 3000) { //envia la temperatura cada 10 segundos
    previousMillis = currentMillis;
     Serial.print("Humdity = ");
  Serial.print(humSum/humCount); //Displays the integer bits of humidity;
  Serial.print("% (");
  Serial.print(relHum);
  Serial.print("%)\t");
  Serial.print("Temperature = ");
  Serial.print(tempSum/tempCount);
  Serial.print("°C (");
  Serial.print(relTemp); //Displays the integer bits of temperature;
  Serial.print("%)\t");
  Serial.print("Light: ");
  Serial.print(light);
  Serial.print("mV (");
  Serial.println(relLight);
  Serial.println("%)\n");

  
    enviardatos("&Luz=" + String(light/10, 2)+"&Temperatura=" + String(tempSum/tempCount, 2) + "&Humedad=" + String(humSum/humCount, 2) );
    tempSum = humSum = lightSum = tempCount = humCount = lightCount = 0;
  }
}
