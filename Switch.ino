#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 10);
int irmotionPin = 4;  // Pin of IR Motion Sensor
int relayPin = 8;  // Pin of Relay Module
char inchar;
int wait=1000;
int wait1m=60000;
int t=0;
int warn=0;


void setup()

{

  mySerial.begin(9600);   // Setting the baud rate of GSM Module 

  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  pinMode(relayPin, OUTPUT);  // Set Pin connected to Relay as an OUTPUT
  pinMode(irmotionPin, INPUT);

  delay(100);

}

void loop()
{
  //Serial.println("AT+CMGD=1,4"); // delete all SMS

  if(digitalRead(irmotionPin) == HIGH) {  // If Motion detected 
       digitalWrite(relayPin, HIGH);  // Turn Relay ON
       Serial.println("Relay is ON and time becomes 0");
       delay(wait);
       t=0;
       
}
else if(digitalRead(irmotionPin) == LOW){
      // digitalWrite(relayPin, LOW);  // Turn Relay OFF
       Serial.println("no motion detected");
       delay(wait);
       t=t+wait;
}
if (t==wait1m){
      SendMessage();
      t=0;
      warn=warn+1;
     Serial.println("sending wanrning ");
     
  }
  if (warn==2){
    digitalWrite(relayPin, LOW);  // Turn Relay OFF
    Serial.println("Relay off");
    wait1m=-5;
    
    }
   RecieveMessage();
  
  for(int i=0;i<200;i++){
    if(mySerial.available() >0)
  {
    inchar=mySerial.read();
  
      Serial.print(inchar);
      
   
    if (inchar=='$')
    {
      delay(10);

      inchar=mySerial.read();
      if (inchar=='o')
      {
        delay(10);
        inchar=mySerial.read();
        if (inchar=='f')
        {     
     digitalWrite(relayPin, LOW);  // Turn Relay OFF
    Serial.println("Relay off");
    delay(10000);
    wait1m=-5;
   
        }
        else if (inchar=='n')
        {
          Serial.println("Relay ON bro,chill");
        
        }
        delay(100);
       Serial.println("AT+CMGD=1,4"); // delete all SMS
       delay(2000);
      }
    }
  }
  }


}

 void SendMessage()


{

  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode

  delay(1000);  // Delay of 1000 milli seconds or 1 second

  mySerial.println("AT+CMGS=\"+8801711871931\"\r"); // Replace x with mobile number

  delay(1000);

  mySerial.println("I am SMS from GSM Module, your light is on, do you wanna turn it off...???");// The SMS text you want to send

  delay(100);

  mySerial.println((char)26);// ASCII code of CTRL+Z

  delay(1000);

}

 void RecieveMessage()

{

  mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to recieve a live SMS

  delay(1000);

}



  void DialCall()

 {

  mySerial.println("ATD+01711871931;"); // ATDxxxxxxxxxx; -- watch out here for semicolon at the end!!

  delay(100);

 }
