#include <SoftwareSerial.h>
#include<dht.h>
dht DHT11;
#define RX 2 // TX of esp8266 in connected with Arduino pin 2

#define TX 3 // RX of esp8266 in connected with Arduino pin 3


// dht DHT11;

#define DHT11_PIN A5

String WIFI_SSID = "rijul";// WIFI NAME

String WIFI_PASS = "11223344"; // WIFI PASSWORD

String API = "LT9C63YTC91A0OPF";// Write API KEY

String HOST = "api.thingspeak.com";

String PORT = "80";

int countTrueCommand;

int countTimeCommand; 

boolean found = false;   

SoftwareSerial esp8266(RX,TX); 



#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

//  Variables
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value. 
                               
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"


void setup() {

  Serial.begin(9600);

  esp8266.begin(115200);

  sendCommand("AT",5,"OK");

  sendCommand("AT+CWMODE=1",5,"OK");

  sendCommand("AT+CWJAP=\""+ WIFI_SSID +"\",\""+ WIFI_PASS +"\"",20,"OK");


  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);   

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }

}

void loop() {
int chk = DHT11.read11(DHT11_PIN);
Serial.print(chk);
Serial.print(" Humidity " );

Serial.print(DHT11.humidity, 1);

Serial.print(" ");

Serial.print(" Temparature ");

Serial.println(DHT11.temperature,1);
String getData="GET /update?api_key="+ API+"&field2="+DHT11.temperature;
sendCommand("AT+CIPMUX=1",5,"OK");

 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");

 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");

 esp8266.println(getData);

 delay(1500);

 countTrueCommand++;

 sendCommand("AT+CIPCLOSE=0",5,"OK");


if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".
int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
 Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
 Serial.print("BPM: ");                        // Print phrase "BPM: " 
 Serial.println(myBPM);   
 
 int tem=random(20,30); // int tem= analogRead(A0)

 int hum=random(70,80); //int tem= digitalRead(11)

 int light=random(0,100); //int tem= AnalogRead(A0)




 String getData="GET /update?api_key="+ API+"&field1="+myBPM;
//  String getData="GET /update?api_key="+ API+"&field2="+DHT11.temperature;

 
 sendCommand("AT+CIPMUX=1",5,"OK");

 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");

 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");

 esp8266.println(getData);

 delay(1500);

 countTrueCommand++;

 sendCommand("AT+CIPCLOSE=0",5,"OK");                     // Print the value inside of myBPM. 
}







  delay(20);                    // considered best practice in a simple sketch.

//  int tem=random(20,30); // int tem= analogRead(A0)

//  int hum=random(70,80); //int tem= digitalRead(11)

//  int light=random(0,100); //int tem= AnalogRead(A0)

//  String getData="GET /update?api_key="+ API+"&field1="+1000;

 
//  sendCommand("AT+CIPMUX=1",5,"OK");

//  sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");

//  sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");

//  esp8266.println(getData);

//  delay(1500);

//  countTrueCommand++;

//  sendCommand("AT+CIPCLOSE=0",5,"OK");

}

void sendCommand(String command, int maxTime, char readReplay[]) {

  Serial.print(countTrueCommand);

  Serial.print(". at command => ");

  Serial.print(command);

  Serial.print(" ");

  while(countTimeCommand < (maxTime*1))

  {

    esp8266.println(command);//at+cipsend

    if(esp8266.find(readReplay))//ok

    {

      found = true;

      break;

    }

    countTimeCommand++;

  } 

  if(found == true)

  {

    Serial.println("OK");

    countTrueCommand++;

    countTimeCommand = 0;

  }

  if(found == false)

  {

    Serial.println("Fail");

    countTrueCommand = 0;

    countTimeCommand = 0;

  } 

  found = false;

 }