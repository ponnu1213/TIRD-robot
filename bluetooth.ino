
#include <SoftwareSerial.h>               
#include <LiquidCrystal.h>
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>  

LiquidCrystal lcd(4, 5, 6, 7, 8, 9);
SoftwareSerial mySerial(2, 3);
#define echoPin A5
#define trigPin A4 

char Incoming_value = 0; 
long duration; 
int distance; 
unsigned long previousMillis = 0;  //will store last time LED was blinked
const long period = 1000;
unsigned long previousMillis2 = 0;  //will store last time LED was blinked

char namedb[15] = "";
char phonedb[15] = "";
const int PulseWire = 0; 
int Threshold = 700; 
int serialflushcounter =0;
  int counter1 =0;
  int counter2 =0;
  int sensorValue =0;
  int myBPM = 72;
  
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"


void setup() 
{
  Serial.begin(9600);    
  mySerial.begin(9600); 
   
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
    
  lcd.begin(20, 4);
  lcd.print("Covid Robo 2021 CEP");
  delay(1000);
  while(1)
  {
    if (Serial.available())
   {
     Incoming_value = Serial.read();
     if(Incoming_value == 'D')
     {
        lcd.setCursor(0, 1);
        lcd.print("DB - Connect - OK");
        counter1 = 1;
        
     }
     if(Incoming_value == 'P')
     {
        lcd.setCursor(0, 2);
        lcd.print("Pi - Connect - OK");
        counter2 = 1;
     }
     if((Incoming_value !='D' ) || (Incoming_value !='P' ))
     {
        Serial.write("SEND DP   ");
     }
   }
     if((Incoming_value !='D' ) || (Incoming_value !='P' ))
     {
        Serial.write("SEND DP   ");
     }
     if((counter2 == 1)&&(counter1 ==1 ))
     {
      lcd.setCursor(0, 3);
      lcd.print("Rover - Connect - OK");
      break;
     }
  }

  pulseSensor.analogInput(PulseWire);   
  pulseSensor.setThreshold(Threshold);    
  if (pulseSensor.begin()) {
    Serial.print("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
   
  pinMode(13, OUTPUT);        
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);                       // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(12, LOW); 
  digitalWrite(11, LOW); 
  digitalWrite(10, LOW); 
  delay(5000);   
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("NAME : ");
  lcd.setCursor(0,1);
  lcd.print("PHONE: ");
  lcd.setCursor(0,2);
  lcd.print("HB:   ");
  lcd.setCursor(5,2);
  lcd.print(" TMP:    ");
  lcd.setCursor(14,2);
  lcd.print("US:   ");
  lcd.setCursor(0,3);
  lcd.print("SYS :   ");
  lcd.setCursor(10,3);
  lcd.print("RovD:    ");
  
}
void loop()
{

  Ultrasound();
  ultrastop();
  if (mySerial.available())
   {
      Rover_direction();
   }
  lcddisplay1sec();
  nameandphone();
  HBTemp();

}  


void HBTemp()
{
     if(counter2 == 3)
   {
    myBPM = 72;
    
    while(1)
    {
      
      int sensorValue1 = analogRead(A0);
      sensorValue = (sensorValue + sensorValue1)/2;

      int myBPM1 = pulseSensor.getBeatsPerMinute();                                                  
      if (pulseSensor.sawStartOfBeat()) {                                
              myBPM = (myBPM1+myBPM)/2; 
              unsigned long currentMillis = millis();
              if(currentMillis - previousMillis2 >= period) 
              { 
                counter1 = counter1+1;
                      Serial.print("BP");
                      Serial.print(myBPM);
                      Serial.print("TEMP");
                      Serial.print(sensorValue);
              }
                                 
          }
      if(counter1 > 10 )
      {
        lcd.setCursor(10,2);
        lcd.print("    ");
        lcd.setCursor(10,2);
        lcd.print(sensorValue);
        lcd.setCursor(3,2);
        lcd.print("   ");
        lcd.setCursor(3,2);
        lcd.print(myBPM);
        Serial.print( String(myBPM) +"."+String(sensorValue)+"...");
        Serial.print("Terminated");
        counter1= 1;
        counter2 =1;
        delay(2000);
        break;
      }
    }
   }
}



void nameandphone()
{
  Serial.flush();
   if(counter2 == 2)
   {
    int i=0;
    counter2 =3;
    while(1)
        {
           if (Serial.available())
           {
            char checkbyte = Serial.read();
            Serial.print(checkbyte);
            if(checkbyte =='#')
            {
              Serial.print("NameSuccess");
              break;
            }
             namedb[i] = checkbyte;
             lcd.setCursor(7+i,0);
             lcd.print(checkbyte);
             i++;
           }
       }
       int j=0;
        while(1)
        {
          
           if (Serial.available())
           {
            char checkbyte = Serial.read();
            Serial.print(checkbyte);
            if(checkbyte =='#')
            {
              Serial.print("PhoneSucess");
              break;
            }
             phonedb[j] = checkbyte;
             lcd.setCursor(8+jgf,1);
             lcd.print(checkbyte);
             j++;
           }
       }
   }
}


void ultrastop()
{
    if( distance < 10)
  {
      digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
      digitalWrite(12, LOW); 
      digitalWrite(11, LOW); 
      digitalWrite(10, LOW); 
      lcd.setCursor(15,3);
      lcd.print("Stop ");
      if(counter2 == 1)
      {
        Serial.print("URoverStop");
        lcd.setCursor(17,2);
        lcd.print("   ");
        lcd.setCursor(17,2);
        lcd.print(distance);
        counter2 =2;
      }
  }
}


void lcddisplay1sec()
{
  unsigned long currentMillis = millis(); // store the current time
  if(currentMillis - previousMillis >= period) 
  { 
      previousMillis = currentMillis;
      lcd.setCursor(17,2);
      lcd.print("   ");
      lcd.setCursor(17,2);
      lcd.print(distance);

  }
}

void flushmyserial()
{
   if(serialflushcounter == 1)
   {
    mySerial.flush();
    serialflushcounter =0;
   }
}

void Ultrasound()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
}




void Rover_direction()
{
   Incoming_value = mySerial.read();      
    //Serial.print(Incoming_value);        
    if((Incoming_value == '1')||(Incoming_value == '2')||(Incoming_value == '3')||(Incoming_value == '4')||(Incoming_value == '5'))
    {
      serialflushcounter =1;
    }
    if(Incoming_value == '2')
    {
      
      digitalWrite(13, HIGH);    
      digitalWrite(12, LOW); 
      digitalWrite(11, HIGH); 
      digitalWrite(10, LOW);
      lcd.setCursor(15,3);
      lcd.print("Right");
       
    }
    if(Incoming_value == '1')
    {
     digitalWrite(13, LOW);    
     digitalWrite(12, HIGH); 
     digitalWrite(11, LOW); 
     digitalWrite(10, HIGH); 
     lcd.setCursor(15,3);
     lcd.print("Left ");
     
    }
    if(Incoming_value == '4')
    {
      digitalWrite(13, HIGH);   
      digitalWrite(12, LOW); 
      digitalWrite(11, LOW); 
      digitalWrite(10, HIGH); 
      lcd.setCursor(15,3);
      lcd.print("Back ");
      
    }
    if(Incoming_value == '3')
    {
      digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
      digitalWrite(12, HIGH); 
      digitalWrite(11, HIGH); 
      digitalWrite(10, LOW); 
      lcd.setCursor(15,3);
      lcd.print("Front");
      
    }
    if(Incoming_value == '5')
    {
      digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
      digitalWrite(12, LOW); 
      digitalWrite(11, LOW); 
      digitalWrite(10, LOW); 
      lcd.setCursor(15,3);
      lcd.print("Stop ");
      
    }
}
