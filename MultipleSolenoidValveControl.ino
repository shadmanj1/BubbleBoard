//MultipleSolenoidValveControl - April 1, 2019
//Lyle Antieau - leantiea@umich.edu - (734)624-1204

//Solenoid Valves are driven using a P-MOS high side driver.  
//A high signal to the gate of the MOSFET will keep the solenoid valve closed.
//A low signal to the gate of the MOSFET will open the solenoid valve.

const long openDuration = 10; // OpenDuration is milliseconds
int pins[] = {13,12,11};  //Output pin assignment, indexed from left to right. 
int i = 0;
//int rows = 21; //This doesn't seem to work
//int columns = 3;
unsigned long currentMillis = 0;
unsigned long openDurationStartTime = 0;
unsigned long intervalTime = 500;
int outputState = HIGH;
bool zigzag[21][3] = {{1,0,1}, //See if we can get the size to be variable
                   {0,1,0},
                   {1,0,1},
                   {0,1,0},
                   {1,0,1},
                   {0,1,0},
                   {1,0,1},
                   {0,1,0},
                   {1,0,1},
                   {0,1,0},
                   {1,0,1},
                   {0,1,0},
                   {1,0,1},
                   {0,1,0},
                   {1,0,1},
                   {0,1,0},
                   {1,0,1},
                   {0,1,0},
                   {1,0,1},
                   {0,1,0},
                   {1,0,1}
                   };
         
void setup(){
  delay(3000);
  for (i = 0; i < sizeof(pins)/sizeof(pins[0]) ; i++){
    pinMode(pins[i], OUTPUT);
  }
}

//Send row of bubbles: pulse solenoid valves for openDuration length
int printRow(bool row[]){
  outputState = LOW;
  currentMillis = millis();
  openDurationStartTime = currentMillis;
  while(outputState == LOW){
    currentMillis = millis();
    for(i = 0; i < sizeof(row)/sizeof(row[0]); i++){
      if(row[i] == 1){
        digitalWrite(pins[i], outputState);
      }
    }
    if(currentMillis - openDurationStartTime >= openDuration){
      outputState = HIGH;
    }
  }
  for(i = 0; i < sizeof(row)/sizeof(row[0]); i++){
    digitalWrite(pins[i], outputState);
  }
  return 0;
}

//Print an Image
int printImage(bool image[21][3]){ //See if we can get the size to be variables
  for(i = 0; i < sizeof(image)/sizeof(image[0]); i++){
    printRow(image[i]);
    delay(intervalTime);
  }
  return 0;
}

void loop(){
  //send ZigZag pattern if button if pressed, implement later
  printImage(zigzag);
}


