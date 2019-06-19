/* Goals
 * 
 *fake spikes
 *duty cycle
 *phase change
 * 
 */
int j = 0;

// led timer
unsigned long previousMillis = 1000; 
long currentMillis = 0;
const long interval = 500; 

// ports
const int Signal = A0; 
const int Green = 13;
const int Yellow = 12;

// averaging filter
float Voltage;
float AverageVoltage = 0;
int MeasurementsToAverage = 5;

// averaging filter 2
float AverageVoltage2 = 0;
int MeasurementsToAverage2 = 16;

// compression 
float CompressedVoltage = 0;

// duty cycle
int MaxVoltage = 330;
int MinVoltage = 310;
float MediumVoltage = 260;
float Threshold = MediumVoltage;
float PercentMax = 50; 
float PercentMin = 50;

// phase change
boolean PhaseInvert = false;
int PhaseShift = 0;


void setup() {
  Serial.begin(9600);
  
  pinMode(Signal,INPUT);
  pinMode(Green,OUTPUT);
  pinMode(Yellow,OUTPUT);

//duty cycle
  Threshold = (MaxVoltage * (PercentMax/100)) + (MinVoltage * (PercentMin/100)); 
}

void loop() {

  j++;
  
  digitalWrite(Yellow,HIGH);
  Voltage = analogRead(Signal);

  // averaging filter
   for(int i = 0; i < MeasurementsToAverage; ++i)   
  {                                               
    AverageVoltage += Voltage;                    
    delay(1);                                      
  }                                                 
  AverageVoltage /= MeasurementsToAverage;         

  // compressor function reduces outlier voltage spikes  
     compressor();

  // averaging filter 2
  for(int i = 0; i < MeasurementsToAverage2; ++i)   
  {                                               
    AverageVoltage2 += CompressedVoltage;                    
    delay(1);                                      
  }                                                 
  AverageVoltage2 /= MeasurementsToAverage;   

  // turns on light once a cycle
  if ((AverageVoltage > Threshold) &&  millis() - previousMillis >= interval){                      
      j = 0;
      previousMillis = millis();
  }

  if (j == 15) {
    digitalWrite(Green,HIGH);
  }
  Serial.println(AverageVoltage);

  if (j > 20) {
    digitalWrite(Green,LOW);
  }
}
// compressor function
void compressor() {

  int CompValues[] = {250,251,252,253,254,255,256,257,258,259,260,
                      261,262,263,264,265,266,267,268,269,270,271};
 
  if (AverageVoltage >= CompValues[21]) {
     CompressedVoltage = (CompValues[18] + ((AverageVoltage - CompValues[18]) * .2));
  }
  else if (AverageVoltage >= CompValues[20]) {
     CompressedVoltage = (CompValues[18] + ((AverageVoltage - CompValues[18]) * .2));
  }
  else if (AverageVoltage >= CompValues[19]) {
     CompressedVoltage = (CompValues[18] + ((AverageVoltage - CompValues[18]) * 1));
  }
  else if (AverageVoltage >= CompValues[18]) {
     CompressedVoltage = (CompValues[17] + ((AverageVoltage - CompValues[17]) * 1.7));
  }
  else if (AverageVoltage >= CompValues[17]) {
     CompressedVoltage = (CompValues[16] + ((AverageVoltage - CompValues[16]) * 1.7));
  }
  else if (AverageVoltage <= CompValues[0]) {
     CompressedVoltage = (CompValues[3] - ((CompValues[3] - AverageVoltage) * .2));
  }
  else if (AverageVoltage <= CompValues[1]) {
     CompressedVoltage = (CompValues[3] - ((CompValues[3] - AverageVoltage) * .2));
  }
  else if (AverageVoltage <= CompValues[2]) {
     CompressedVoltage = (CompValues[3] - ((CompValues[3] - AverageVoltage) * 1));
  }
  else if (AverageVoltage <= CompValues[3]) {
     CompressedVoltage = (CompValues[4] - ((CompValues[4] - AverageVoltage) * 1.5));
  }
  else if (AverageVoltage <= CompValues[4]) {
     CompressedVoltage = (CompValues[5] - ((CompValues[5] - AverageVoltage) * 2));
  }
  else if (AverageVoltage <= CompValues[5]) {
     CompressedVoltage = (CompValues[6] - ((CompValues[6] - AverageVoltage) * 2));
  }
  else if (AverageVoltage <= CompValues[6]) {
     CompressedVoltage = (CompValues[7] - ((CompValues[7] - AverageVoltage) * 2));
  }
  else if (AverageVoltage <= CompValues[7]) {
     CompressedVoltage = (CompValues[8] - ((CompValues[8] - AverageVoltage) * 2));
  }
  else {
    CompressedVoltage = AverageVoltage;
  }
}
  





/*
else if (voltage > 100 && voltage < 200) {
digitalWrite(Yellow,HIGH);
digitalWrite(Green,LOW);
digitalWrite(Red,LOW);
  }
else if (voltage < 30 && voltage > 0)
digitalWrite(Red,HIGH);
*/
