//RGB LED: RED: 3, GREEN: 2, BLUE: 4

// defines pins numbers
const int REDPin = 3;// Initialize a variable for RED pin of the RGB LED
const int GREENPin = 2;// Initialize a variable for GREEN pin of the RGB LED
const int BLUEPin = 4;// Initialize a variable for BLUE pin of the RGB LED
const int buzzerPin = 11;// Initialize a variable for the buzzer
const int trigPin = 9;// Initialize a variable for the trigger pin of the Distance Measuring Ranging Transducer Sensor
const int echoPin = 10;// Initialize a variable for echo pin of the Distance Measuring Ranging Transducer Sensor
const int LightSensorPin = A0;// Initialize a variable for the light resistor
const int LEDPin = 8;// Initialize a variable for the ON LED
const int soilPin1 = A1;//Initialize a variable for the soil moisture sensor no. 1
const int soilPin2 = A2;//Initialize a variable for the soil moisture sensor no. 2
const int soilPin3 = A3;//Initialize a variable for the soil moisture sensor no. 3
const int soilPower = 7;//Initialize variable for Soil moisture Power
const int PumpPin = 6; // Initialize a variable for pump
const int errorPin = 5; // Initialize pint for error LED
int soilVal1 = 0; //Initialize value for storing moisture value
int soilVal2 = 0; //Initialize value for storing moisture value
int soilVal3 = 0; //Initialize value for storing moisture value 
int soilCooldown=4;//Initialize value for how many "rounds" the moisture sensor wait until getting the sensor data
int pumpCounter=10;//Initialize value for how many "rounds" the pump works before turning off
int pumpCooldown=5;//Initialize value for how many "rounds" the pump wait until starting to work
long duration;//declare value for the duration of the Distance Measuring Ranging Transducer Sensor
int distance;//declare value for the distance of the Distance Measuring Ranging Transducer Sensor
bool locsol=false;
int counter=0; //Simply a counter for how many "rounds" the loop has been working
bool stopItNow=true;//Bool value for interrupting the process

void setup()
{
  pinMode(buzzerPin, OUTPUT);//Set as an OUTPUT
  pinMode(trigPin, OUTPUT);//Set as an OUTPUT
  pinMode(echoPin, INPUT);//Set as an INPUT
  pinMode(errorPin, OUTPUT);//Set as an OUTPUT
  pinMode(soilPower, OUTPUT);//Set as an OUTPUT
  //The next two values must be set to HIGH, because the relay connects the circuit if it gets 0 on its control pin
  digitalWrite(PumpPin, HIGH);//Set the PumpPin to HIGH
  digitalWrite(soilPower, HIGH);//Set the soilPower pin to HIGH
  pinMode(LightSensorPin, INPUT);//Set as an INPUT
  pinMode(REDPin, OUTPUT);//Set as an OUTPUT
  pinMode(GREENPin, OUTPUT);//Set as an OUTPUT
  pinMode(BLUEPin, OUTPUT);//Set as an OUTPUT
  pinMode(LEDPin, OUTPUT);//Set as an OUTPUT
  pinMode(PumpPin, OUTPUT);//Set as an OUTPUT
  Serial.begin(9600);
  //beeps 3 times for start
  beep(50);
  beep(50);
  beep(50);
}

void loop()
{
 // csak akkor fut le, ha a stopItNow valtozo true 
 if(!stopItNow){
   
  if(counter==0){
        pumpCooldown=4;
        digitalWrite(PumpPin, LOW);
        delay(4000);
        digitalWrite(PumpPin, HIGH);
    }
  
  if(soilCooldown==1){
    digitalWrite(soilPower, LOW);//turn D7 "On"
    delay(100);//wait 100 milliseconds 
    soilVal1 = analogRead(soilPin1);//Read the SIG value form sensor 
    delay(100);//wait 100 milliseconds 
    soilVal2 = analogRead(soilPin2);//Read the SIG value form sensor 
    delay(100);//wait 100 milliseconds 
    soilVal3 = analogRead(soilPin3);//Read the SIG value form sensor 
    digitalWrite(soilPower, HIGH);//turn D7 "Off"
  }
  int soilAtlag=(soilVal1+soilVal2+soilVal3)/3;
 
  //A szivattyu akkor indul ha a mert nedvessegertekek atlaga lecsokken a tapasztalati minimumnak szamito 550-es ertekre
  //A locsolast akkor hagyja abba, ha a nedvessegertek eleri a tapasztalati 610-es erteket.
  //Az eppen megfelelo nedvessegerteket zold, a tul alacsony erteket piros, a ketto kozottit pedig kek szinnel jelzi.

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
   //Ha megallitja a locsolast, ha az ontozes legalabb 10 "korig" tartott.
  if(pumpCounter==0){
    ontozes();
    pumpCounter=10;
  }
  
  
  // read the value from the sensor
  int lightSensorValue = analogRead(LightSensorPin);
  
   //Ha a fenyerzekelo a tapasztalati 775-os ertekre lecsokken, bekapcsolja a lampat. A villodzas elkerulese miatt itt is, mint a nedvessegerzekelesnel hiszterezissel szamoltam.
  if (lightSensorValue < 775) {
    if(lightSensorValue > 675) {
      digitalWrite(LEDPin, HIGH);
    }
    else{
      digitalWrite(LEDPin, LOW);
    }
  else{
      digitalWrite(LEDPin, HIGH);
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
  // Calculating the distance in cm
  distance = duration * 0.034 / 2;
  // Az ures tartajra 18 cm-t mert a szenzor, igy amikor megkaphuk, hogy jelenleg mennyi a mert ertek, azt ha kivonjuk 18-bol, megkapjuk a vizszintet.
  // Ha 2 cm-nel kevesebb a viz, akkor leall
  distance=18-distance;
  if(distance<=2)
  {
    stopItNow=true;
  }

  // kiirjuk a mert ertekeket a console-ra, ennek a tesztek soran volt jelentossege
  serialprint(lightSensorValue, distance, soilVal1, soilVal2, soilVal3);

   Serial.print("soilCooldown: ");
   Serial.print(soilCooldown);
   Serial.print(", pumpCounter: ");
   Serial.print(pumpCounter);
   Serial.print(", pumpCooldown: ");
   Serial.println(pumpCooldown);
  // A "kor" vegen csokkentjuk a cooldown valtozokat. ha 0-ra csokken, visszakapja az eredeti erteket, ezzel szabalyozhato, hogy egy adott egyseg hany "korig" uzemeljen/ne uzemeljen.
   soilCooldown--;
  pumpCooldown--;
  if(soilCooldown==0){
    soilCooldown=4;
    }
  if(pumpCooldown==0){
      pumpCooldown=10;
    }
   delay(2000); // Wait for 2 second(s)
    
   counter++;
   Serial.print("Counter: ");
   Serial.println(counter);
  
  }
  else
  {
      // Ha a locsol valtozo false erteken van, a program csipogassal es az error led felvillanasaval jelzi a problemat
             
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
      //meghivas eseten a locsol logikai erteket negalja
      if(locsol){
          locsol=false;
        }
      else{
          locsol=true;
        }
  }
  //fuggveny a console-ra valo kiirashoz.
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
 //buzzer kezelo fuggveny
void beep(unsigned char delayms){
  analogWrite(buzzerPin, 20);      // Almost any value can be used except 0 and 255
   delay(100);   
   analogWrite(buzzerPin, 150);      // Almost any value can be used except 0 and 255
   
                           // experiment to get the best tone
  delay(delayms);          // wait for a delayms ms
  analogWrite(buzzerPin, 0);       // 0 turns it off
  delay(delayms);          // wait for a delayms ms   
}  
