
#include <Array.h>

//pwm
int pwm = 0;
boolean pwmOn = false;

// deciding when to turn heater on and off
int LeftBound = 0;
int RightBound = 200;
boolean TurnOn = true;
int PhaseShift;
int DutyCycle;

// led timer
unsigned long previousMillis = 1000; 
long currentMillis = 0;
const long interval = 500; 
int Threshold = 0; // where the timer starts; syncs fridge's signal with heater's

// ports
const int Signal = A0; 
const int Green = 11;
const int PotOne = A1;
const int PotTwo = A2;
const int PotThree = A3;

// averaging filter
float Voltage;
float AverageVoltage = 0;
int MeasurementsToAverage = 16;

// averaging filter 2
float AverageVoltage2 = 0;
int MeasurementsToAverage2 = 16;

// compression 
float CompressedVoltage = 0;

int VoltageVal[500];
int i = 0;
int MaxVoltage = 0;
int MinVoltage = 0;

void setup() {
  Serial.begin(9600);

  pinMode(Signal,INPUT);
  pinMode(Green,OUTPUT);
  pinMode(PotOne,INPUT);
  pinMode(PotTwo,INPUT);
  pinMode(PotThree, INPUT);

}

void loop() {
  
  Voltage = analogRead(Signal);
  
  // averaging filter
  average();       

  /*  finds the signal threshold  
   *  1) determine the peak voltages (excluding outliers)
   *  2) calculate their average
   */ 
  VoltageVal[i] = AverageVoltage;
  if (MaxVoltage == 0) {
    MaxVoltage = VoltageVal[i];
  }
  if (MinVoltage == 0) {
    MinVoltage = VoltageVal[i];
  }
  if (AverageVoltage > MaxVoltage && AverageVoltage < (MaxVoltage * 1.2)) {
    MaxVoltage = VoltageVal[i];
  }
  if (AverageVoltage < MinVoltage && AverageVoltage > (MinVoltage / 1.2)) {
    MinVoltage = VoltageVal[i];
  }
  Threshold = (MaxVoltage + MinVoltage)/2;
  
  //graphs voltage vs. time in the plotter 
  Serial.println(Voltage);

  

  // compressor function reduces outlier voltage spikes  
 // compressor();

  // averaging filter 2
 // average2();

  // reads the potentiometer position and makes it a usable integer for our purposes
  PhaseShift = analogRead(PotOne);           // 0-600
  PhaseShift = map(PhaseShift, 0, 1023, 0, 500);
  DutyCycle = analogRead(PotTwo);          // 50-650
  DutyCycle = map(DutyCycle, 0, 1023, 50, 650);
  //DutyCycle = 200;

  pwm = analogRead(PotThree);
  pwm = map(pwm, 0, 1023, 0, 255);
   //  analogWrite(Green, pwm);
  
  //prevents user from overlapping cycles (we want the heater to turn on once during each period)
  RightBound = DutyCycle + PhaseShift;
  if (RightBound > 650) {
      RightBound = 650;
  }

   // records the time when the voltage crosses the threshold
  if ((AverageVoltage > Threshold) &&  millis() - previousMillis >= interval){                      
      previousMillis = millis();
      TurnOn = true;
  }
    
  // sends voltage from arduino after an amount of time (determined by PhaseShift) has ellapsed
  if ((millis() - previousMillis > LeftBound + PhaseShift) && TurnOn) {   
//X   analogWrite(Green, pwm);
   digitalWrite(Green,HIGH);
     TurnOn = false;
  }

  // stops sending voltage after an amount of time (determined by DutyCycle) has ellapsed
  if (millis() - previousMillis > RightBound) {
    digitalWrite(Green,LOW);

   

    i++;
  }
}

// averaging filter functions
void average() {
  for(int p = 0; p < MeasurementsToAverage; ++p)   
  {                                               
    AverageVoltage += Voltage;                    
    delay(1);                                      
  }                                                 
  AverageVoltage /= MeasurementsToAverage; 
}


void average2() {
  for(int q = 0; q < MeasurementsToAverage2; ++q)   
  {                                               
    AverageVoltage2 += CompressedVoltage;                    
    delay(1);                                      
  }                                                 
  AverageVoltage2 /= MeasurementsToAverage; 
}
