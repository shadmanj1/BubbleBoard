//TenSolenoidValveControlWithCalibration - April 13, 2019
//Lyle Antieau - leantiea@umich.edu - (734)624-1204

//Solenoid Valves for current implementation are driven using an N-MOS low side driver.  
//A low signal to the gate of the MOSFET will keep the solenoid valve closed.
//A high signal to the gate of the MOSFET will open the solenoid valve.

//Beta Prototype will have an N-MOS low side driver

//consider using digitalWriteFast instead of diditalWrite if release times aren't consistent
//#include <digitalWriteFast.h>

const long openDuration = 5; // OpenDuration is milliseconds
const long intervalTime = 750;

int outputPins[] = {12,11,10,9,8,7,6,5,4,3};  //Output pin assignment, indexed from left to right. 
int pianoPins[] = {14,15,16,17,18,19,20,21,22,23};
int buttonPins[] = {24,25,26,27,28};
bool pianoRow[] = {0,0,0,0,0,0,0,0,0,0};


int solenoidExtensions[] =  {2,3,4,4,3,6,3,1,0,5};//Set for Checkerborar//{5,29,30,10,5,20,10,4,0,0};//
//for Squares, pins 1,3,5,7,9//{2,3,3,4,3,6,3,1,1,5};
//for zig, pins 1,3,5,7,9 {1,3,0,4,1,6,1,1,1,5};
bool test[1][10] = {{1,0,1,0,1,0,1,0,1,0}
                 };
long maxDelay = 0; // maxDelay is calculated from SolenoidExtensions Array
                    //Maximum delay must be less than intervalTime
                    
int mode = 0;
int i = 0;
int outputState = 0;

unsigned long currentMillis = 0;
unsigned long openDurationStartTime = 0;
unsigned long delayStartTime = 0;

bool smallD[4][10] = {
  {0,0,0,0,1,0,0,0,0,0},
  {0,0,1,0,0,0,1,0,0,0},
  {1,0,0,0,0,0,0,0,1,0},
  {0,0,1,0,0,0,1,0,0,0}
};

bool leftM[9][10] = {
  {0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,1,0},
  {1,0,1,0,0,0,1,0,1,0},
  {1,0,0,0,1,0,0,0,1,0},
  {1,0,0,0,0,0,0,0,1,0},
  {1,0,0,0,0,0,0,0,1,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0}
};
bool zig[8][10] = {
  {1,0,0,0,0,0,0,0,0,0},
  {0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,1,0,0,0,0,0},
  {0,0,0,0,0,0,1,0,0,0},
  {0,0,0,0,0,0,0,0,1,0},
  {0,0,0,0,0,0,1,0,0,0},
  {0,0,0,0,1,0,0,0,0,0},
  {0,0,1,0,0,0,0,0,0,0},
};
//mode 0
bool blockM[14][10] = {
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {1,1,1,0,0,0,0,1,1,1},
  {0,1,0,1,0,0,1,0,1,0},
  {0,1,0,0,1,1,0,0,1,0},
  {0,1,0,0,0,0,0,0,1,0},
  {0,1,0,0,0,0,0,0,1,0},
  {1,1,1,0,0,0,0,1,1,1},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0}
};

//mode 1
bool zigzag[22][10] = {
  {1,0,1,0,1,0,1,0,1,0}, //See if we can get the size to be variable
  {0,1,0,1,0,1,0,1,0,1},
  {1,0,1,0,1,0,1,0,1,0},
  {0,1,0,1,0,1,0,1,0,1},
  {1,0,1,0,1,0,1,0,1,0},
  {0,1,0,1,0,1,0,1,0,1},
  {1,0,1,0,1,0,1,0,1,0},
  {0,1,0,1,0,1,0,1,0,1},
  {1,0,1,0,1,0,1,0,1,0},
  {0,1,0,1,0,1,0,1,0,1},
  {1,0,1,0,1,0,1,0,1,0},
  {0,1,0,1,0,1,0,1,0,1},
  {1,0,1,0,1,0,1,0,1,0},
  {0,1,0,1,0,1,0,1,0,1},
  {1,0,1,0,1,0,1,0,1,0},
  {0,1,0,1,0,1,0,1,0,1},
  {1,0,1,0,1,0,1,0,1,0},
  {0,1,0,1,0,1,0,1,0,1},
  {1,0,1,0,1,0,1,0,1,0},
  {0,1,0,1,0,1,0,1,0,1},
  {1,0,1,0,1,0,1,0,1,0},
  {0,1,0,1,0,1,0,1,0,1}
};

//mode 2
bool alternate[16][10] = {
  {1,1,1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,0,0,0,0},          
};

//mode 3
bool diamond[11][10] = {//21 when full
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,1,0,0,1,0,0,0},
  {0,0,1,0,0,0,0,1,0,0},
  {0,1,0,0,0,0,0,0,1,0},
  {1,0,0,0,0,0,0,0,0,1},
  {0,1,0,0,0,0,0,0,1,0},
  {0,0,1,0,0,0,0,1,0,0},
  {0,0,0,1,0,0,1,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0}
  
//  {0,0,0,0,1,1,0,0,0,0},
//  {0,0,0,1,0,0,1,0,0,0},
//  {0,0,1,0,1,1,0,1,0,0},
//  {0,1,0,1,0,0,1,0,1,0},
//  {1,0,1,0,0,0,0,1,0,1},
//  {0,1,0,1,0,0,1,0,1,0},
//  {0,0,1,0,1,1,0,1,0,0},
//  {0,0,0,1,0,0,1,0,0,0},
//  {0,0,0,0,1,1,0,0,0,0},
//  {0,0,0,0,0,0,0,0,0,0},
};

//mode 4
bool invertedDiamond[21][10] = {
  {1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,0,0,1,1,1,1},
  {1,1,1,0,1,1,0,1,1,1},
  {1,1,0,1,1,1,1,0,1,1},
  {1,0,1,1,1,1,1,1,0,1},
  {0,1,1,1,1,1,1,1,1,0},
  {1,0,1,1,1,1,1,1,0,1},
  {1,1,0,1,1,1,1,0,1,1},
  {1,1,1,0,1,1,0,1,1,1},
  {1,1,1,1,0,0,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,0,0,1,1,1,1},
  {1,1,1,0,1,1,0,1,1,1},
  {1,1,0,1,0,0,1,0,1,1},
  {1,0,1,0,1,1,0,1,0,1},
  {0,1,0,1,1,1,1,0,1,0},
  {1,0,1,0,1,1,0,1,0,1},
  {1,1,0,1,0,0,1,0,1,1},
  {1,1,1,0,1,1,0,1,1,1},
  {1,1,1,1,0,0,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1},
  
};

void setup(){
  delay(1000);
  Serial.begin(9600);
  for (i = 0; i < 10; i++){
    pinMode(outputPins[i], OUTPUT);
  }
  for (i = 0; i < 10; i++){
    pinMode(pianoPins[i], INPUT);
  }
  for (i = 0; i < 10; i++){
    if(solenoidExtensions[i] > maxDelay){
      maxDelay = solenoidExtensions[i];
    }
  } 
}

//Send row of bubbles: pulse solenoid valves for openDuration length
int printRow(bool row[]){
  outputState = 1;
  currentMillis = millis();
  openDurationStartTime = currentMillis;
  while(outputState == 1){
    currentMillis = millis();
    //write output pin high if 1, keep low if 0
    for(i = 0; i < 10; i++){
      if(row[i] == 1){
        digitalWrite(outputPins[i], HIGH);
      }
    }
    if(currentMillis - openDurationStartTime >= openDuration){
      currentMillis = millis();
      delayStartTime = currentMillis;
      while(currentMillis - delayStartTime <= maxDelay){
        //Serial.print("delay");
        //Serial.print('\n');
        currentMillis = millis(); 
        for(i = 0; i < 11; i++){  
          if(currentMillis - delayStartTime <= solenoidExtensions[i]){
            if(row[i] == 1){
              digitalWrite(outputPins[i], HIGH);
              digitalWrite(13,HIGH);
              //Serial.print("pin:");
              //Serial.print(i);
              //Serial.print('\n');
              //Serial.print('\n');
              //Serial.print("Here");
            }
          }
          else{
            digitalWrite(outputPins[i], LOW);
          }
        }
      }
      outputState = 0;
    }
  }
  for(i = 0; i < 10; i++){
    digitalWrite(outputPins[i], LOW);
  }
  return 0;
}

//Print an Image
int printImage(bool image[][10], int rows){ //See if we can get the size to be variables
  for(int j = 0; j < rows; j++){
    printRow(image[j]);
    delay(intervalTime - maxDelay);
  }
  return 0;
}
//mode 5
int piano(){  
  for(i = 0; i < 10; i++){
    pianoRow[i] = digitalRead(outputPins[i]);
  }
  printRow(pianoRow);
return 0;
}

int cycleImages(){
  printImage(blockM, 22);
  delay(1000);
  printImage(zigzag, 22);
  delay(1000);
  printImage(alternate, 16);
  delay(1000);
  printImage(diamond, 21);
  delay(1000);
  printImage(invertedDiamond, 21);
  return 0;
}

void loop(){
  /*for(i = 0; i<5; i++){
    if(digitalRead(buttonPins[i])){
      mode = i;
    }
  }
  if (mode == 0){
    printImage(blockM, 12);
  }  
  else if (mode == 1){
    printImage(zigzag, 22);
  }
  else if (mode == 2){
    printImage(alternate, 22);
  }
  else if (mode == 3){
    printImage(diamond, 21);
  }
  else if (mode == 4){
    piano();
  }
  else if (mode == 5){
    cycleImages();
  }
  */

  printImage(test, 1);
}




