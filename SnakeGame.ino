/*Incluimos la librería LedControl: */
#include "LedControl.h"

/*
  Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
  pin 12 is connected to the DataIn
  pin 11 is connected to the CLK
  pin 10 is connected to LOAD
  We have only a single MAX72XX.
*/
LedControl lc = LedControl(12, 11, 10, 1); /* (DataIN, CLK, CS, Número de matrices8x8)  */

/* Puertos analógicos para la conexion del joystick:  */
const short int joystick[2] = {0,1};

/* Largo máximo para la víbora: */
const short int snakeMaxLenght = 10;

/* Largo actual de la víbora: */
short int snakeLenght = 3;

/* Variable booleana para el gameover: */
boolean gameOver = false;

/* La víbora es una lista de coordenadas [x,y] a encenderse en la matriz led: */
short int snake[snakeMaxLenght][2] = {{0,0},{1,0},{2,0}};

/* El ratón es una coordenada */
short int mouseX;
short int mouseY;

/*Guarda la dirección hacia donde debe moverse la víbora: */
short int orientation = 0; /* 0 right, 1 left, 2 up, 3 down. */

/* we always wait a bit between updates of the display */
unsigned long delaytime = 300;

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

  randomSeed(analogRead(5)); /* Valor aleatorio para inicializar random */
  newMouse();                /* Creramos el primer ratón */
}

/* Función que crea un nuevo ratón en una posición aleatoria: */
void newMouse(){
  mouseX = random(7);
  mouseY = random(7);
}

/* Función que evalúa la posición del joystick y devuelve una orientación: */
short int control(const short int device[2]){
  int x = analogRead(device[0]);
  int right = x-512;
  int left = 512-x;
  int y = analogRead(device[1]);
  int up = y-512;
  int down = 512-y;

  if(orientation != 1 and right > 300 and right > left and right > up and right > down){
    return 0;
  }
  else if (orientation != 0 and left > 300 and left > right and left > up and left > down){
    return 1;
  }
  else if (orientation != 3 and up > 300 and up > right and up > left and up > down){
    return 2;
  }
  else if (orientation != 2 and down > 300 and down > right and down > left and down > up){
    return 3;
  }
  else{
    return orientation;
  }
}

/* Función que establece una nueva posición para la víbora: */
void go() {
  /* Se establecen nuevas posiciones para el cuerpo; */
  for (int i=snakeLenght-1;i>0;i--){ /* desde la cola hasta el cuello... */
    snake[i][0] = snake[i-1][0];     /* cada nodo ocupa la posición que ocupaba el siguiente */
    snake[i][1] = snake[i-1][1];
   }
   
  /* ...y una nueva posicion para la cabeza: */
  switch(orientation){
    case 0: {                /* Si estamos yendo a la derecha... */
      if (snake[0][0]==7){  /* ... y ya llegamos al límite de la pantalla... */
        snake[0][0] = 0;    /* ... reraparecemos del otro lado. */
      }
      else {                /* Si no hemos llegado al límite... */
        snake[0][0]++;      /* avanzamos una celda a la derecha */
      }
      break;
    }
    case 1: {                /* Si estamos yendo a la izquierda... */
      if (snake[0][0]==0){  /* ... y ya llegamos al límite de la pantalla... */
        snake[0][0] = 7;    /* ... reraparecemos del otro lado. */
      }
      else {                /* Si no hemos llegado al límite... */
        snake[0][0]--;      /* avanzamos una celda a la izquierda */
      }
      break;
    }
    case 2: {                /* Si estamos yendo arriba... */
      if (snake[0][1]==0){  /* ... y ya llegamos al límite de la pantalla... */
        snake[0][1] = 7;    /* ... reraparecemos del otro lado. */
      }
      else {                /* Si no hemos llegado al límite... */
        snake[0][1]--;      /* avanzamos una celda hacia arriba */
      }
      break;
    }
    case 3: {                /* Si estamos yendo hacia abajo... */
      if (snake[0][1]==7){  /* ... y ya llegamos al límite de la pantalla... */
        snake[0][1] = 0;    /* ... reraparecemos del otro lado. */
      }
      else {                /* Si no hemos llegado al límite... */
        snake[0][1]++;      /* avanzamos una celda hacia abajo */
      }
      break;
    }
  }
  /* Si la nueva posición de la cabeza coincide con alguna posición del cuerpo: */
  for(int i = 3; i < snakeLenght; i++){ /* revisamos desde la posición 3 hasta la cola...*/
    if(snake[i][0] == snake[0][0] and snake[i][1] == snake[0][1]){ /* si coincide con la cabeza...*/
      gameOver = true;  /* termina el juego */
    }
  }
  
  /* Si la nueva posicion de la cabeza coincide con la posición del ratón: */
  if(snake[0][0] == mouseX and snake[0][1] == mouseY){
    /* Aumenta en 1 el largo de la víbora: */
    snakeLenght++;
    /* Y se crea un nuevo ratón: */
    newMouse();
  }

  /* Si sobrepasamos el máximo largo de la víbora: */
  if(snakeLenght > snakeMaxLenght){
    /* Aumenta la velocidad del juego en 40ms por loop: */
    delaytime-= 40;
    /* Y devuelve la víbora al largo inicial: */
    snakeLenght = 3; 
  }
}

/* Función que muestra en la pantalla la víbora y el ratón: */
void show(){
  /*Muestra desde la cabeza a la cola todos los nodos de la víbora: */
  for (int i=0;i<snakeLenght;i++){
    lc.setLed(0,snake[i][0],snake[i][1], true);
  }
  /* Muestra el ratón: */
  lc.setLed(0, mouseX, mouseY, true);
}

/* Pantalla de game over: */
void gameOverScreen(){
  for(int i = 0; i < 8; i++){
    lc.setRow(0, i, B11111111);
    delay(100); 
  }
  for(int i = 0; i < 8; i++){
    lc.setRow(0, i, B00000000);
    delay(100); 
  }
}

void loop() {
  /*Si hemos perdido, muestra la pantala de game over: */
  if(gameOver){
    gameOverScreen();
  }
  else{  /* de lo contrario... */
    /* actualiza la orientación de la víbora: */
    orientation = control(joystick);
    /* actualiza la posición de la víbora: */
    go();
    /* limpia la pantalla: */
    lc.clearDisplay(0);
    /* muestra la vibora y el ratón: */
    show();
    /* espera antes de reiniciar el loop: */
    delay(delaytime);
  }
}
