//We always have to include the library
#include "LedControl.h"

const int DIN = 12;
const int CLK = 11;
const int CS = 10;
LedControl lc = LedControl(DIN, CLK, CS, 1);

const int Xsensor=A0;
const int Ysensor=A1;

const int maxSnakeLenght = 8;
int SX[maxSnakeLenght] = {0, 1, 2};
int SY[maxSnakeLenght] = {0, 0, 0};
int lenght = 3;

int orientation = 3;// 0 up, 1 right, 2 down, 3 left.

int MX;
int MY;

/* we always wait a bit between updates of the display */
unsigned long slowness = 200;
unsigned int velocityIncrease = 30;

boolean gameOver = false;

void go() {
  //Each snake node in the place of the next:
  for (int i=lenght-1;i>0;i--){ 
    SX[i] = SX[(i-1)];
    SY[i] = SY[(i-1)];
  }
  //Draws the head in a new place:
  if (orientation == 0){
   if (SY[0]==0){
    SY[0] = 7 ;
   }
   else {
    SY[0]--; 
   }
  }
  else if (orientation == 2){
   if (SY[0]==7){
    SY[0] = 0 ;
   }
   else {
    SY[0]++; 
   }
  }
  if (orientation == 1){
   if (SX[0]==7){
    SX[0] = 0 ;
   }
   else {
    SX[0]++; 
   }
  }
  else if (orientation == 3){
   if (SX[0]==0){
    SX[0] = 7 ;
   }
   else {
    SX[0]--; 
   }
  }
  
  //If new head position coincides with a body node position:
  for(int i=lenght-1; i>2; i--){
    if(SX[0]==SX[i]and SY[0]==SY[i]){
      gameOver=true;
    }
  }
  
  //If new head position coincides with mouse position:
  if(SX[0]==MX and SY[0]==MY){
    //increase the lenght...
    lenght++;
    //on reaching maximum lenght, next level:
    if(lenght == maxSnakeLenght){
      lenght = 3;
      slowness-=velocityIncrease; 
    }
    //and make a new mouse:
    makeAMouse();
  }

}
void showSnake(){
  for (int i=0;i<lenght;i++){
    lc.setLed(0,SX[i],SY[i], true);
  }
}

void control(){
  int Xvalue = analogRead(Xsensor);
  int Yvalue = analogRead(Ysensor);
  if(Yvalue>723){ if(orientation != 3){orientation = 1;}}
  if(Yvalue<300){ if(orientation != 1){orientation = 3;}}
  if(Xvalue>723){ if(orientation != 0){orientation = 2;}}
  if(Xvalue<300){ if(orientation != 2){orientation = 0;}}
}

void makeAMouse(){
  MX=random(8);
  MY=random(8);
}

void showMouse(){
  lc.setLed(0,MX,MY,true);
}

void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  delay(100);
  randomSeed(analogRead(5));
  makeAMouse();
}

void loop(){
  if(gameOver==true){
    gameOverScreen();
  }
  else{
    control();
    lc.clearDisplay(0);
    showMouse();
    showSnake();
    go();
    delay(slowness);
  }
}
