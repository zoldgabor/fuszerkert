//RGB LED: RED: 3, GREEN: 2, BLUE: 4

// defines pins numbers
const int REDPin = 3;
const int GREENPin = 2;
const int BLUEPin = 4;
const int buzzerPin = 11;
const int trigPin = 9;
const int echoPin = 10;
const int LightSensorPin = A0;
const int LEDPin = 8;
const int soilPin1 = A1;//Declare a variable for the soil moisture sensor 
const int soilPin2 = A2;//Declare a variable for the soil moisture sensor 
const int soilPin3 = A3;//Declare a variable for the soil moisture sensor 
const int soilPower = 7;//Variable for Soil moisture Power
const int PumpPin = 6; // Declare a variable for pump
const int errorPin = 5; // declare pint for error
int soilVal1 = 0; //value for storing moisture value
int soilVal2 = 0; //value for storing moisture value
int soilVal3 = 0; //value for storing moisture value 
int soilCooldown=4;
int pumpCounter=10;
int pumpCooldown=5;
long duration;
int distance;
bool locsol=false;
int counter=0;
bool stopItNow=true;

void setup()
{
  pinMode(buzzerPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(errorPin, OUTPUT);//Set D5 as an OUTPUT
  pinMode(soilPower, OUTPUT);//Set D7 as an OUTPUT
  digitalWrite(PumpPin, HIGH);
  digitalWrite(soilPower, HIGH);
  pinMode(LightSensorPin, INPUT);
  pinMode(2, OUTPUT);
  pinMode(LEDPin, OUTPUT);
  pinMode(PumpPin, OUTPUT);
  Serial.begin(9600);
  beep(50);
  beep(50);
  beep(50);
}

void loop()
{
 if(!stopItNow){
   
  if(counter==0){
        pumpCooldown=4;
        digitalWrite(PumpPin, LOW);
        delay(4000);
        digitalWrite(PumpPin, HIGH);
    }
  
  if(soilCooldown==1){
    digitalWrite(soilPower, LOW);//turn D7 "On"
    delay(100);//wait 10 milliseconds 
    soilVal1 = analogRead(soilPin1);//Read the SIG value form sensor 
    delay(100);//wait 2 milliseconds 
    soilVal2 = analogRead(soilPin2);//Read the SIG value form sensor 
    delay(100);//wait 2 milliseconds 
    soilVal3 = analogRead(soilPin3);//Read the SIG value form sensor 
    digitalWrite(soilPower, HIGH);//turn D7 "Off"
  }
  int soilAtlag=(soilVal1+soilVal2+soilVal3)/3;
 
  //vizresz

  if(pumpCooldown==1){
    
    if(soilAtlag<550){
        if(locsol==false){
            analogWrite(REDPin, 255);
            analogWrite(GREENPin, 0);
            analogWrite(BLUEPin, 0);
            ontozes();
          }
      }
    else{
        if(soilAtlag>610){
             analogWrite(REDPin, 0);
            analogWrite(GREENPin, 255);
            analogWrite(BLUEPin, 0);
            ontozes();
            pumpCounter=10;
            pumpCooldown=10;
          } 
          else
          {
            analogWrite(REDPin, 0);
            analogWrite(GREENPin, 0);
            analogWrite(BLUEPin, 255);
            }
      }
  }
  if(locsol){
        digitalWrite(PumpPin, LOW);
        delay(5000);
        digitalWrite(PumpPin, HIGH);
        pumpCounter--;
    }
  if(pumpCounter==0){
    ontozes();
    pumpCounter=10;
  }
  
  
  // read the value from the sensor
  int lightSensorValue = analogRead(LightSensorPin);
  
  if (lightSensorValue < 775) {
    digitalWrite(LEDPin, LOW);
  }
  else{
      if(lightSensorValue > 675) {
        digitalWrite(LEDPin, HIGH);
      }
    }

  
   // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  distance=18-distance;
  if(distance<=2)
  {
    stopItNow=true;
  }

  
  serialprint(lightSensorValue, distance, soilVal1, soilVal2, soilVal3);

   Serial.print("soilCooldown: ");
   Serial.print(soilCooldown);
   Serial.print(", pumpCounter: ");
   Serial.print(pumpCounter);
   Serial.print(", pumpCooldown: ");
   Serial.println(pumpCooldown);
  soilCooldown--;
  pumpCooldown--;
  if(soilCooldown==0){
    soilCooldown=4;
    }
  if(pumpCooldown==0){
      pumpCooldown=10;
    }
   delay(2000); // Wait for 100 millisecond(s)
   counter++;
   Serial.print("Counter: ");
   Serial.println(counter);
  
  }
  else
  {
        
        // Clears the trigPin
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        // Sets the trigPin on HIGH state for 10 micro seconds
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        // Reads the echoPin, returns the sound wave travel time in microseconds
        duration = pulseIn(echoPin, HIGH);
        // Calculating the distance
        distance = duration * 0.034 / 2;
        distance=18-distance;
        Serial.print("Vizszint!!!: ");
        Serial.println(distance);
        if(distance>2)
        {
          stopItNow=false;
          digitalWrite(errorPin, LOW);
            
        }else
        {
           digitalWrite(errorPin, HIGH);
           beep(100);
        }
  }
}
void ontozes(){
      if(locsol){
          locsol=false;
        }
      else{
          locsol=true;
        }
  }
void serialprint(int lightval, int distval, int soil1, int soil2, int soil3)
{
  Serial.print("Locsolunk? ");
  if(locsol){
      Serial.print("igen");
    }
  else{
      Serial.print("nem");
  }
  Serial.print(", fenyero: ");
  Serial.print(lightval);
  Serial.print(", vizszint: ");
  Serial.print(distval);
  Serial.print(" cm, nedvesseg szenzor 1: ");
  Serial.print(soil1);
  Serial.print(", nedvesseg szenzor 2: ");
  Serial.print(soil2);
  Serial.print(", nedvesseg szenzor 3: ");
  Serial.print(soil3);
  Serial.print(", atlagos nedvesseg: ");
  int soilAtlag=(soil1+soil2+soil3)/3;
  Serial.println(soilAtlag);
}
void beep(unsigned char delayms){
  analogWrite(buzzerPin, 20);      // Almost any value can be used except 0 and 255
   delay(100);   
   analogWrite(buzzerPin, 150);      // Almost any value can be used except 0 and 255
   
                           // experiment to get the best tone
  delay(delayms);          // wait for a delayms ms
  analogWrite(buzzerPin, 0);       // 0 turns it off
  delay(delayms);          // wait for a delayms ms   
}  
