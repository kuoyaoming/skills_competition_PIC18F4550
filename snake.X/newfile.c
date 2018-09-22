#include <p18cxxx.h>

#pragma config FOSC=HS, WDT=OFF, BOR = ON_ACTIVE, BORV = 1, LVP=OFF, PBADEN=OFF, XINST = OFF, MCLRE = ON

// Snake on a plane.. ehm.. dotmatrix

int latchPin = 12;
int clockPin = 10;
int dataPin = 9;
int button1 = 2;
int button2 = 3;
int numrow = 8;
int numcol = 8;
int mask8 = 0B11111111;
int row = 0;
int col = 0;
int pixel = 0;
int pointer = 0;
long rowoutput;
long coloutput;
long redoutput;
long greenoutput;
long redoutputrow;
long greenoutputrow;

/*
Colours:
 0 = none
 1 = red
 2 = green
 3 = orange
*/
int empty = 0;
int red = 1;
int green = 2;
int orange = 3;

/*
Directions
 0 = up
 1 = right
 2 = down
 3 = left
*/
int direction = 1;

int matrix[64];
int snake[64];
int newsnake[64];
int startingpoint = 24; // row 4 col 0
int snakehead = 0;
int oldsnakehead = 0;
int snaketail = 0;
int applecaught = 0;

int apple = 0;

int moveSpeed = 200;

unsigned long lastMillis = 0;

Bounce bouncer1 = Bounce(button1, 10); 
Bounce bouncer2 = Bounce(button2, 10); 
int button1value = 1;
int button2value = 1;

void setup() {
 
 randomSeed(analogRead(5));
 
 // set pins to output 
 pinMode(latchPin, OUTPUT);
 pinMode(clockPin, OUTPUT);
 pinMode(dataPin, OUTPUT);
 // buttons to input
 pinMode(button1, INPUT);
 pinMode(button2, INPUT);
 
 // Enable internal pullup resistors
 digitalWrite(button1, HIGH);
 digitalWrite(button2, HIGH);
 
 //Serial.begin(9600);
 
 resetSnake();
 
 Timer1.initialize(2000);
 Timer1.attachInterrupt(outputDisplay);
 
}

void loop() {
 bouncer1.update();
 bouncer2.update();
 if (bouncer1.read() == LOW) {
   if (button1value == HIGH) {
     prevDirection();
   }
 }
 button1value = bouncer1.read();
 
 if (bouncer2.read() == LOW) {
   if (button2value == HIGH) {
     nextDirection();
   }
 }
 button2value = bouncer2.read();
 
 moveSnake();
}

void resetSnake() {
 // Create empty snake
 for (int i=0; i<64; i++)  {
   snake[i] = -1;
   newsnake[i] = -1;
 }
   
 clearMatrix();
 
 // Add starting dot
 matrix[startingpoint] = orange;
 snakehead = startingpoint;
 snake[0] = snakehead;
 
 generateApple();
 matrix[apple] = green;
 
 direction = 1;
}

void clearMatrix() {
 // Clear matrix
 for (int i=0; i<64; i++)  {
   matrix[i] = 0;
 }
}

void outputDisplay() {
 greenoutputrow = 0;
 redoutputrow = 0;
 
 for (int c = 0; c < numcol; c++) {
   pointer = (row * numcol) + c;
   pixel = matrix[pointer];

   switch(pixel) {
     case 1:
      redoutput = 1 << c;
      greenoutput = 0;
      break;
     case 2:
      redoutput = 0;
      greenoutput = 1 << c;
      break;
     case 3:
      redoutput = 1 << c;
      greenoutput = 1 << c;
      break;
     default:
      redoutput = 0;
      greenoutput = 0;
      break;
   }
   redoutputrow += redoutput;
   greenoutputrow += greenoutput;  
 }
 
 rowoutput = (1 << row) ^ mask8; // 0 = enable row, 1 = disable row, so invert it.
 
 digitalWrite(latchPin, LOW);
 
 shiftOut(dataPin, clockPin, MSBFIRST, greenoutputrow);
 shiftOut(dataPin, clockPin, MSBFIRST, redoutputrow);
 shiftOut(dataPin, clockPin, MSBFIRST, rowoutput);

 digitalWrite(latchPin, HIGH);
   
 nextRow();
}

void moveSnake() {
 
 if (millis() - lastMillis >= moveSpeed) {
   oldsnakehead = snakehead;
   switch(direction) {
     case 0:
      // up
      if ((snakehead - numcol) < 0) {
        snakehead += ((numrow - 1) * numcol);
      }
      else {
        snakehead -= numcol; 
      }
      break;
     case 1:
      // right
      if ((snakehead - (snakehead % numcol)) == (((snakehead+1) - (snakehead+1) % numcol))) {
        snakehead += 1;
      }
      else {
        snakehead = snakehead - (numcol - 1); // to start of row 
      }
      break;
     case 2:
      // down
      if ((snakehead + numcol) > ((numrow * numcol - 1))) {
        snakehead = snakehead % numcol;
      }
      else {
        snakehead += numcol;
      }
      break;
     case 3:
      // left
      if (snakehead == 0) {
        // exception for topleft, pixel 0
        snakehead += (numcol - 1);
      }
      else if ((snakehead - (snakehead % numcol)) == (((snakehead-1) - (snakehead-1) % numcol))) {
        snakehead -= 1;
      }
      else {
        snakehead += (numcol - 1); // end of row 
      }
      break;
     default:
      // wtf?
      break;
   }
   
   // Check if we have the apple
   if (snakehead == apple) {
     // Got it!
     generateApple();
     applecaught = 1;
   }
   
   for (int i = 1; i < 64; i++) {
     if (snake[i] == snakehead) {
       endGame(); 
     }
   }
   
   newsnake[0] = snakehead;
   for(int n = 0; n < 63; n++) {
     if (snake[n] == -1) {
       snaketail = snake[n-1];
       if (applecaught == 0) {
         newsnake[n] = -1; // undo last, we are moving on
       }
       break;
     }
     newsnake[n+1] = snake[n];
   }
   for(int m = 0; m < 64; m++) {
     snake[m] = newsnake[m]; 
   }
   
   if (applecaught == 1) {
     generateApple();
     applecaught = 0; 
   }
   
   updateMatrix();
   
   lastMillis = millis(); 
 }
}

void updateMatrix() {
 clearMatrix();
 for (int i = 1; i < 64; i++) {
   if (snake[i] == -1) {
     break; 
   }
   matrix[snake[i]] = red; 
 }
 matrix[snakehead] = orange;
 matrix[apple] = green;
}

void generateApple() {
 apple = random(64);
 for (int n = 1; n < 64; n++) {
   if (snake[n] == apple) {
     generateApple(); 
   }
 }
}

void nextRow() {
row++;
if (row >= numrow) {
  row = 0;
} 
}

void nextDirection() {
 direction++;
 if (direction > 3) {
   direction = 0;
 } 
}

void prevDirection() {
 direction--;
 if (direction < 0) {
   direction = 3;
 }
}

void endGame() {
 for (int i = 0; i < numrow; i++) {
   for (int n = 0; n < numcol; n++) {
     matrix[n + (i * numcol)] = green;
   }
   delay(200);
 }
 delay(4000);
 resetSnake();
}
