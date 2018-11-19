//We always have to include the library
#include "LedControl.h"

/*
  Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
  pin 12 is connected to the DataIn
  pin 11 is connected to the CLK
  pin 10 is connected to LOAD
  We have only a single MAX72XX.
*/
LedControl lc = LedControl(12, 11, 10, 1);

int VX[] = {0, 1, 2};
int VY[] = {0, 0, 0};
int orientation = 1;// 0 up, 1 right, 2 down, 3 left.

/* we always wait a bit between updates of the display */
unsigned long delaytime = 100;

void setup() {
  /*
    The MAX72XX is in power-saving mode on startup,
    we have to do a wakeup call
  */
  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 8);
  /* and clear the display */
  lc.clearDisplay(0);

}



void go() {
  int lenght = sizeof(VX)/sizeof(VX[0]);
  for (int i=(lenght-1);i>0;i--){ 
    VX[i] = VX[(i-1)];
    VY[i] = VY[(i-1)];
   }
  if (orientation == 0){
   if (VY[0]==0){
    VY[0] = 7 ;
   }
   else {
    VY[0]--; 
   }
  }
  else if (orientation == 2){
   if (VY[0]==7){
    VY[0] = 0 ;
   }
   else {
    VY[0]++; 
   }
  }
  if (orientation == 1){
   if (VX[0]==7){
    VX[0] = 0 ;
   }
   else {
    VX[0]++; 
   }
  }
  else if (orientation == 3){
   if (VX[0]==0){
    VX[0] = 7 ;
   }
   else {
    VX[0]--; 
   }
  }

}
void show(){
  int lenght = sizeof(VX)/sizeof(VX[0]);

  for (int i=0;i<lenght;i++){
    lc.setLed(0,VX[i],VY[i], true);
  }
}

void loop() {

  lc.clearDisplay(0);
  show();
  go();
  
  delay(300);
}
