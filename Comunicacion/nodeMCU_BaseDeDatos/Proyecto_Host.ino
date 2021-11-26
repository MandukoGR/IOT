#include "ESP8266WiFi.h"
#include <WiFiClient.h> 
#include <Servo.h>
int DHpin = 4; // input/output pin
int fotores = A0;
int ledPin = 0;
int servoPin = 2;
int motorPinR = 16;
int motorPinL = 5;

#include <DHT.h>
 
// Definimos el pin digital donde se conecta el sensor
#define DHTPIN 4
// Dependiendo del tipo de sensor
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

Servo servo;

//-------------------VARIABLES Necesarias para conexión--------------------------
int contconexion = 0;

const char *ssid = "INFINITUM3DB3_2.4";
const char *password = "4988521072";

unsigned long previousMillis = 0;

char *host = "145.14.145.94";
//char *host = "145.14.145.150"; Por si no sirve la otra
String strhost = "mosquittos.000webhostapp.com";
String strurl = "/enviardatos.php";
String chipid = "";
//--------------------------------------------------------------------------------


//-------Función para Enviar Datos a la Base de Datos SQL--------

String enviardatos(String datos) {
  String linea = "error";
  WiFiClient client;
  //strhost.toCharArray(host, 49);
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
    Serial.print(linea);
  }  
  //Serial.println(linea);
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
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED and contconexion <50) { //Cuenta hasta 50 si no se puede conectar lo cancela
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion <50) {
      //para usar con ip fija
      IPAddress ip(192,168,1,156); 
      IPAddress gateway(192,168,1,1); 
      IPAddress subnet(255,255,255,0); 
      WiFi.config(ip, gateway, subnet); 
      
      Serial.println("");
      Serial.println("WiFi conectado");
      Serial.println(WiFi.localIP());
  }
  else { 
      Serial.println("");
      Serial.println("Error de conexion");
  }

  dht.begin();
}



void loop()
{
  // Leemos la humedad relativa
  float h = dht.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  
  
  Serial.print("Humdity = ");
  Serial.print(h); //Displays the integer bits of humidity;
  Serial.println('%');
  Serial.print("Temperature = ");
  Serial.print(t); //Displays the integer bits of temperature;
  Serial.println('C');


  float luz = analogRead(fotores);
  Serial.print("Light: ");
  Serial.println(luz);
  if (luz < 60)
  {
    digitalWrite(ledPin, HIGH);
  }

  else
  {

    digitalWrite(ledPin, LOW);
  }

  if (isnan(h) || isnan(t)) {
      Serial.println("Error obteniendo los datos del sensor DHT11");
      return;
    } else {
        if (h > 50)
  {
    servo.write(180);
  }
  else
  {
    servo.write(0);
  }

  if (t > 22.1)
  {
    digitalWrite(motorPinR, HIGH);
  }
  else
  {
    digitalWrite(motorPinR, LOW);
  }

    }


  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 10000) { //envia la temperatura cada 10 segundos
    previousMillis = currentMillis;
    //int analog = analogRead(17);
    //float temp = analog*0.322265625;
    //Serial.println(temp);
    enviardatos("&Temperatura=" + String(t, 2) + "&Humedad=" + String(h, 2) + "&Luz=" + String(luz, 2));
  }
}
