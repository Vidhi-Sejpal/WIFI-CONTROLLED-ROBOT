
#include<SoftwareSerial.h>  //include the software serial library

SoftwareSerial esp8266(3,2); //set the software pin RX=3 TX=2

//defination of variable
#define DEBUG true  //allow message between esp8266 and aurdino in serial monitor ,when set to true
#define SSID "AndroidAP65E1" //replace with your wifi network
#define PASS "vidhi3103"    //replace with your wifi password

int state=5;

//function to send AT commands to esp8266

String sendAT(String command,const int timeout,boolean debug)
{
  String response ="";
  esp8266.print(command);
  long int time = millis();
  while((time + timeout )>millis())
  {
    while(esp8266.available())
    {
      char c = esp8266.read();
      response +=c;
    }
  }
  if(debug)
  {
    Serial.print(response);
  }
  return response;
}


//function to initalize arduino and esp communication

void setup()
{
  Serial.begin(9600);  //begin th serial communication with baud rate of 9600
  esp8266.begin(9600); //begin the software serial communication with baud rate 9600

  sendAT("AT+RST\r\n",2000,DEBUG); //call sendAT function to send AT command
  sendAT("AT+CWMODE=1\r\n",1000,DEBUG); //call sendAT function to esp8266 to station mode
  sendAT("AT+CWJAP=\""SSID"\",\""PASS"\"\r\n",2000,DEBUG ); //AT command to connect with the wifi network
  while(!esp8266.find("OK"))
  {
    //wait for connection
  }
  sendAT("AT+CIFSR\r\n",1000,DEBUG); //AT command to print IP address on serial monitor
  sendAT("AT+CIPMUX=1\r\n",1000,DEBUG); ///AT command to set esp8266 to multiple connection
  sendAT("AT+CIPSERVER=1,80\r\n",1000,DEBUG); //AT command to start web server on port 80
  delay(4000);
  Serial.println("intialization done");
  digitalWrite(13,HIGH); //switch on LED on arduino

  //define motor pins
  pinMode(8,OUTPUT); //set digital pin 8,9,10,11 as output
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
 }

//function to read data from esp8266
void loop()
{
  if(esp8266.available())
  {
    //verify incoming data from esp8266
    if(esp8266.find("+IPD,"))
    {
     //if the message contains +IPD
     String msg;
     esp8266.find("?"); // look for ? in message
     msg = esp8266.readStringUntil(' ');  //read the whole message
     String command = msg.substring(0,3); //first 3 character = command 
     String valueStr = msg.substring(4); //next 4 character = value
     int value =valueStr.toInt(); //convert string value tp integer
       if(DEBUG)
      {
        Serial.println(command);
        Serial.println(value);
      }
      //move forward
      if(command=="cm1")
      {
        state=1;
        Serial.println("Forward");
      }
      
      //move back
      if(command=="cm2")
      {
        state=2;
        Serial.println("back");
      }

      if(command=="cm3")
      {
        state=3;
        Serial.println("left");
      }
      if(command=="cm4")
      {
        state=4;
        Serial.println("right");
      }
     if(command=="cm5")
      {
        state=5;
        Serial.println("stop");
      }

      if(command=="cm6")
      {
        state=6;
        Serial.println("connected to client");
        Serial.println("now use arrow keys to control you robot");
      }  
    }
   }

   if(state==1)
   {
    digitalWrite(9,HIGH);
    digitalWrite(11,HIGH);
   }

    if(state==2)
   {
    digitalWrite(8,HIGH);
    digitalWrite(10,HIGH);
   }

    if(state==3)
   {
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
   }
    if(state==4)
   {
    digitalWrite(8,HIGH);
    digitalWrite(11,HIGH);
   }
    if(state==5)
   {
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    
    
   }
  if(state == 6)
  {
    
  }
   
   
   
   
   
}


 
