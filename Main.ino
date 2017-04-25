#include "string.h"
#include "ctype.h"
#include "SoftwareSerial.h"
#include "dGPS.h"
#include "HMC5983.h"
#include "Wire.h"

HMC5983 compass;

int ledPin = 13;                  // LED test pin
double desLat=0;                   //Destination Latitude filled by user in Serial Monitor Box
double desLon=0;                   //Destination Longitude filled by user in Serial Monitor Box
char fla[2];                      //flag (Y/N) whether to print checksum or not. Filled by user in Serial Monitor Box
dGPS dgps = dGPS();               // Construct dGPS class
double angleToTarget = 0;


double getdestcoord()
  // function to get the coordinates of the destination from user
  { double result;
    while (Serial.available()==0)
    {;}// do nothing until something comes into the serial buffer

    if (Serial.available()>0)
    {
     result=Serial.parseFloat(); //read a double value from serial monitor box, correct upto 2 decimal places
     result = result + ((double) Serial.parseInt()/10000);
     delay(10); // the processor needs a moment to process
    }
    return result;
  }
  
 void getflag(char *str)
   // function to read the flag character from the user 
  {  
    while (Serial.available()==0)
    {;}
    int index=0;
    if (Serial.available()>0)
    { if (index<2){
      str[index]=Serial.read();
      index++;
     }
      else str[index]='\0';
      delay(10);
    }  
  }

void setup() {
  pinMode(ledPin, OUTPUT);       // Initialize LED pin
  Serial.end();                  // Close any previously established connections
  Serial.begin(9600);            // Serial output back to computer.  On.
  dgps.init();// Run initialization routine for dGPS.
  compass.begin();
  pinMode(5, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(12, OUTPUT);
  delay(1000);  

  Serial.println("IMPORTANT: Make sure that the pin selector on the dGPS Shield is set to 10 before proceeding!");
  delay(1000);  
  Serial.print("Do you want to display checksum (Y/N): "); 
  delay(3000);
  getflag(fla);
  Serial.println(*fla);
  Serial.print("Enter Destination Latitude (in degrees): ");
  delay(3000);
  desLat=getdestcoord();
  Serial.println(desLat, 4);
  Serial.print("Enter Destination Longitude (in degrees): ");
  delay(3000);
  desLon=getdestcoord();
  Serial.println(desLon, 4);
}

void loop() {
  
  dgps.update(desLat, desLon);                  // Calling this updates the GPS data.  The data in dGPS variables stays the same unless
                                  // this function is called.  When this function is called, the data is updated.
  
  Serial.print("CheckSum: ");
  Serial.println(dgps.Checks());
  Serial.println("");
  Serial.print("Azimuth: ");
  Serial.println(dgps.Azim());
  Serial.println("");
  Serial.print("Mode: ");
  Serial.println(dgps.Mode());

  float c = -999;
  c = compass.read();
  if (c == -999) {
    Serial.println("Reading error, discarded");
  } else {
    Serial.println(c);
    double angleToTargetUnajusted =  (double) map(dgps.Azim(), -180.0, 180.0, 0.0, 360.0) - (double) c;
    if(angleToTargetUnajusted > 0){
      angleToTarget = angleToTargetUnajusted;
    }
    else {
      angleToTarget = 360 + angleToTargetUnajusted;
    }

    if (angleToTarget <= 30){
      digitalWrite(5, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(12, LOW);
    }
    else if (angleToTarget >= 330){
      digitalWrite(5, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(12, LOW);
    }
    else if (angleToTarget > 30 && angleToTarget <=60) {
      digitalWrite(5, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(12, LOW);
    }
    else if (angleToTarget > 60 && angleToTarget <=120){
      digitalWrite(5, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(12, LOW);
    }
    else if (angleToTarget > 120 && angleToTarget <=150){
      digitalWrite(5, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(12, LOW);
    }
    else if (angleToTarget > 150 && angleToTarget <= 210){
      digitalWrite(5, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(12, LOW);
    }
    else if (angleToTarget > 210 && angleToTarget <= 240){
      digitalWrite(5, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(12, HIGH);
    }
    else if (angleToTarget > 240 && angleToTarget <= 300){
      digitalWrite(5, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(12, HIGH);
    }
    else if (angleToTarget > 300 && angleToTarget <= 330){
      digitalWrite(5, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(12, HIGH);
    }
    Serial.print("angleToTarget: ");
    Serial.println(angleToTarget);
  }
}
