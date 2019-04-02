//SolenoidValveControl - February 19, 2018

const long openDuration = 10;    //Input openDuration in milliseconds
long closedDuration = 1000;        //Input closedDuration in milliseconds
int i = 0;     
int noOfPins = 3;    
unsigned long previousMillis = 0; 
boolean outputState[3] = {1,1,1};
int pins[3] = {13,12,11};
//boolean outputMatrix[4] = {outputState, {HIGH, LOW, HIGH}, {LOW, HIGH, LOW}, outputState};
 

void setup() {
  // set the digital pin as output:
  delay(3000);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
}

void loop() {
while(1){   
  unsigned long currentMillis = millis();
 
  //Open solenoid for openDuration, keep closed for closedDuration
  for(i=0; i<noOfPins+1; i++){
    if(outputState[1] == LOW){
      if (currentMillis - previousMillis >= openDuration) {
          outputState[1] = HIGH;
          previousMillis = currentMillis;
      }
    }
    else if (outputState[1] == HIGH) {
      if (currentMillis - previousMillis >= closedDuration){
          outputState[1] = LOW;
          previousMillis = currentMillis;
      }
    }
  }         
  digitalWrite(13, outputState[1]);
  digitalWrite(12, outputState[1]);
  digitalWrite(11, outputState[1]);
}
digitalWrite(13, LOW); 
digitalWrite(12, LOW);  
digitalWrite(11, LOW);   
}
