#include <Servo.h>
int DHpin = 4; // input/output pin
int fotores = A0;
int ledPin = 0;
byte dat[5];
int servoPin = 2;
int motorPinR = 16;
int motorPinL = 5;
Servo servo;

//-------------------VARIABLES Necesarias para conexión--------------------------
int contconexion = 0;

const char *ssid = "";
const char *password = "";

unsigned long previousMillis = 0;

char host[48];
String strhost = "";
String strurl = "/enviardatos.php";
String chipid = "";
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

//-------------------------------------------------------------------------


byte read_data()
{
	byte i = 0;
	byte result = 0;
	for (i = 0; i < 8; i++)
	{
		while (digitalRead(DHpin) == LOW)
			;				   // wait 50us
		delayMicroseconds(30); //The duration of the high level is judged to determine whether the data is '0' or '1'
		if (digitalRead(DHpin) == HIGH)
			result |= (1 << (8 - i)); //High in the former, low in the post
		while (digitalRead(DHpin) == HIGH)
			; //Data '1', waiting for the next bit of reception
	}
	return result;
}

void start_test()
{
	digitalWrite(DHpin, LOW); //Pull down the bus to send the start signal
	delay(30);				  //The delay is greater than 18 ms so that DHT 11 can detect the start signal
	digitalWrite(DHpin, HIGH);
	delayMicroseconds(40); //Wait for DHT11 to respond
	pinMode(DHpin, INPUT);
	while (digitalRead(DHpin) == HIGH)
		;
	delayMicroseconds(80); //The DHT11 responds by pulling the bus low for 80us;

	if (digitalRead(DHpin) == LOW)
		delayMicroseconds(80);	//DHT11 pulled up after the bus 80us to start sending data;
	for (int i = 0; i < 5; i++) //Receiving temperature and humidity data, check bits are not considered;
		dat[i] = read_data();
	pinMode(DHpin, OUTPUT);
	digitalWrite(DHpin, HIGH); //After the completion of a release of data bus, waiting for the host to start the next signal
}

void setup()
{
	Serial.begin(9600);
	pinMode(DHpin, OUTPUT);
	pinMode(fotores, INPUT);
	pinMode(ledPin, OUTPUT);
	pinMode(motorPin, OUTPUT);
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
}

}

void loop()
{
	// put your main code here, to run repeatedly:
	start_test();
	Serial.print("Humdity = ");
	Serial.print(dat[0], DEC); //Displays the integer bits of humidity;
	Serial.print('.');
	Serial.print(dat[1], DEC); //Displays the decimal places of the humidity;
	Serial.println('%');
	Serial.print("Temperature = ");
	Serial.print(dat[2], DEC); //Displays the integer bits of temperature;
	Serial.print('.');
	Serial.print(dat[3], DEC); //Displays the decimal places of the temperature;
	Serial.println('C');

	byte checksum = dat[0] + dat[1] + dat[2] + dat[3];

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

	double humidity = dat[0] + dat[1];
	double temperature = dat[2] + dat[3];

	if (humidity > 50)
	{
		servo.write(180);
	}
	else
	{
		servo.write(0);
	}

	if (temperature > 22.1)
	{
		digitalWrite(motorPin, HIGH);
	}
	else
	{
		digitalWrite(motorPin, LOW);
	}

	delay(1000);



  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 10000) { //envia la temperatura cada 10 segundos
    previousMillis = currentMillis;
    //int analog = analogRead(17);
    //float temp = analog*0.322265625;
    //Serial.println(temp);
    enviardatos("&Temperatura=" + String(temperature, 2) + "&Humedad=" + String(humidity, 2) + "&Luz=" + String(luz, 2));
  }
}
