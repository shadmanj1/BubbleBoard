//BetaPrototypeSolenoids - April 19, 2019
//Lyle Antieau - leantiea@umich.edu - (734)624-1204

//This code has a solenoidExtensions array that allows for tweaking of openDuration

//Solenoid Valves for current implementation are driven using an N-MOS low side driver.  
//A low signal to the gate of the MOSFET will keep the solenoid valve closed.
//A high signal to the gate of the MOSFET will open the solenoid valve.

//

//consider using digitalWriteFast instead of diditalWrite if release times aren't consistent
//#include <digitalWriteFast.h>

long openDuration = 2; // OpenDuration is milliseconds
long intervalTime = 600;

int outputPins[] = {12,11,10,9,8,7,6,5,4,3};  //Output pin assignment, indexed from left to right. 
int pianoPins[] = {44,46,48,50,52};
int buttonPins[] = {44,46,48,50,52};
bool pianoRow[] = {0,0,0,0,0}; //Plan was to use the five buttons on the Electrical Housing as a 5 key piano

int solenoidExtensions[] =  {2,5,7,6,7,4,7,6,4,9};

//Theses are some solenoidExtensions that seemed to work for these specific patterns. 
//There appears to be a maximum extensions before code starts to generate solenoid misfires
  //Set for Checkerboard {5,29,30,10,5,20,10,4,0,0};
  //for Squares, pins 1,3,5,7,9{2,3,3,4,3,6,3,1,1,5};
  //for zig, pins 1,3,5,7,9 {1,3,0,4,1,6,1,1,1,5};
  //Front Row lines (Test){2,5,4,6,4,4,4,1,0,9};
bool test[1][10] = {{1,0,1,0,1,0,1,0,1,0}};
bool test2[1][10] = {{0,1,0,1,0,1,0,1,0,1}};

long maxDelay = 0; // maxDelay is calculated from SolenoidExtensions Array
                    //Maximum delay must be less than intervalTime   

int mode = 0;
int i = 0;
int outputState = 0;

unsigned long currentMillis = 0;
unsigned long openDurationStartTime = 0;
unsigned long delayStartTime = 0;

bool jellyfish[7][10] = {
  {1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,0,0,0,0,0},
  {0,0,1,0,0,0,0,0,0,1},
  {0,0,0,0,1,0,0,0,0,0},
  {0,0,1,0,0,0,0,0,1,0},
  {0,0,0,0,1,0,0,0,0,0},
  {0,1,0,0,0,0,0,0,0,0}
};

bool goBlue[35][10] = { //Delay to 
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,1,0,1,0,1,0,1,0},
  {0,0,1,0,0,0,0,0,0,0},
  {0,0,1,0,0,0,1,0,1,0},
  {0,0,1,0,0,0,0,0,1,0},
  {0,0,1,0,1,0,1,0,1,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,1,0,1,0,1,0,1,0},
  {0,0,1,0,0,0,0,0,1,0},
  {0,0,1,0,0,0,0,0,1,0},
  {0,0,1,0,1,0,1,0,1,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,1,0,1,0,1,0,0,0},
  {0,0,1,0,0,0,0,0,1,0},
  {0,0,1,0,1,0,1,0,0,0},
  {0,0,1,0,0,0,0,0,1,0},
  {0,0,1,0,1,0,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,1,0,0,0,0,0,0,0},
  {0,0,1,0,0,0,0,0,0,0},
  {0,0,1,0,0,0,0,0,0,0},
  {0,0,1,0,1,0,1,0,1,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,1,0,0,0,0,0,1,0},
  {0,0,1,0,0,0,0,0,1,0},
  {0,0,1,0,0,0,0,0,1,0},
  {0,0,1,0,1,0,1,0,1,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,1,0,1,0,1,0,1,0},
  {0,0,1,0,0,0,0,0,0,0},
  {0,0,1,0,1,0,1,0,1,0},
  {0,0,1,0,0,0,0,0,0,0},
  {0,0,1,0,1,0,1,0,1,0},
  {0,0,0,0,0,0,0,0,0,0}
};

bool smallD[17][10] = {
  {0,0,0,0,1,0,0,0,0,0},
  {0,0,1,0,0,0,1,0,0,0},
  {1,0,0,0,0,0,0,0,1,0},
  {0,0,1,0,0,0,1,0,0,0},
  {0,0,0,0,1,0,0,0,0,0},
  {0,0,1,0,0,0,1,0,0,0},
  {1,0,0,0,0,0,0,0,1,0},
  {0,0,1,0,0,0,1,0,0,0},
  {0,0,0,0,1,0,0,0,0,0},
  {0,0,1,0,0,0,1,0,0,0},
  {1,0,0,0,0,0,0,0,1,0},
  {0,0,1,0,0,0,1,0,0,0},
  {0,0,0,0,1,0,0,0,0,0},
  {0,0,1,0,0,0,1,0,0,0},
  {1,0,0,0,0,0,0,0,1,0},
  {0,0,1,0,0,0,1,0,0,0},
  {0,0,0,0,1,0,0,0,0,0}
};

bool smiley[7][10] = {
 {0,0,0,0,0,0,0,0,0,0},
 {0,0,1,0,0,0,1,0,0,0},
 {0,0,1,0,0,0,1,0,0,0},
 {1,0,0,0,0,0,0,0,1,0},
 {0,0,1,0,1,0,1,0,0,0},
 {0,0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0,0},
};

bool leftM[8][10] = {
  {0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,1,0},
  {1,0,1,0,0,0,1,0,1,0},
  {1,0,0,0,1,0,0,0,1,0},
  {1,0,0,0,0,0,0,0,1,0},
  {1,0,0,0,0,0,0,0,1,0},
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

bool blockM[10][10] = {
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
};

bool zigzag[2][10] = {
  {1,0,1,0,1,0,1,0,1,0}, //See if we can get the size to be variable
  {0,1,0,1,0,1,0,1,0,1}
};

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

bool diamond[10][10] = {
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,1,0,0,1,0,0,0},
  {0,0,1,0,0,0,0,1,0,0},
  {0,1,0,0,0,0,0,0,1,0},
  {1,0,0,0,0,0,0,0,0,1},
  {0,1,0,0,0,0,0,0,1,0},
  {0,0,1,0,0,0,0,1,0,0},
  {0,0,0,1,0,0,1,0,0,0},
  {0,0,0,0,1,1,0,0,0,0}
};


bool invertedDiamond[10][10] = {
  {1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,0,0,1,1,1,1},
  {1,1,1,0,1,1,0,1,1,1},
  {1,1,0,1,1,1,1,0,1,1},
  {1,0,1,1,1,1,1,1,0,1},
  {0,1,1,1,1,1,1,1,1,0},
  {1,0,1,1,1,1,1,1,0,1},
  {1,1,0,1,1,1,1,0,1,1},
  {1,1,1,0,1,1,0,1,1,1},
  {1,1,1,1,0,0,1,1,1,1}
};

void setup(){
  delay(1000);
  Serial.begin(9600);
  for (i = 0; i < 10; i++){
    pinMode(outputPins[i], OUTPUT);
  }
  for (i = 0; i < 5; i++){
    pinMode(pianoPins[i], INPUT);
  }
  for (i = 0; i < 10; i++){
    if(solenoidExtensions[i] > maxDelay){
      maxDelay = solenoidExtensions[i];
    }
  } 
  pinMode(13, OUTPUT);
}

int jelly(){
  openDuration = 150;
  printImage(jellyfish,7);
  openDuration = 3;
}

//Send row of bubbles: pulse solenoid valves for openDuration length
int printRow(bool row[]){
  outputState = 1;
  currentMillis = millis();
  openDurationStartTime = currentMillis;
  while(outputState == 1){
    currentMillis = millis();
    for(i = 0; i < 5; i++){
      //Piano implentation does not work at this time
      /*if(digitalRead(pianoPins[i]) == HIGH){
        piano();
      }
      */
    }
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
        currentMillis = millis(); 
        for(i = 0; i < 11; i++){  
          if(currentMillis - delayStartTime <= solenoidExtensions[i]){
            if(row[i] == 1){
              digitalWrite(outputPins[i], HIGH);
              digitalWrite(13,HIGH);
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
    //Consider getting rid of this delay and implment
    delay(intervalTime - maxDelay);
  }
  return 0;
}
//mode 5
int piano(){  
  for(i = 0; i < 10; i++){
    pianoRow[i] = 0;
  }
  for(i = 0; i < 5; i++){
    pianoRow[2*i] = digitalRead(pianoPins[i]);
  }
  printRow(pianoRow);
  return 0;
}

bool pyramid[3][10] = {
 {0,0,0,0,1,0,0,0,0,0},
 {0,0,1,0,1,0,1,0,0,0},
 {1,0,1,0,1,0,1,0,1,0}
};

int cycleImages(){
  printImage(leftM, 8);
  printImage(goBlue, 35);
  printImage(smallD, 16);
  printImage(smallD, 17);
  printImage(zig, 8);
  printImage(zig, 8);
  printImage(zig, 8);
  intervalTime = 1000;
  printImage(test, 1);
  printImage(test, 1);
  printImage(test, 1);
  printImage(test, 1);
  printImage(test, 1);
  intervalTime = 600;
  printImage(smiley, 7);
  return 0;
}

void loop(){
  cycleImages();
  //printImage(zigzag,2);
}




