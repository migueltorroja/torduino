//www.elegoo.com
//2016.12.9

//We always have to include the library
#include "LedControl.h"
#include "LiquidCrystal.h"


LiquidCrystal lcd(3, 4, 5, 6, 7, 8);

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to LOAD(CS)
 pin 10 is connected to the CLK 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(12,10,11,1);

#define ROW_ENEMY (0)
#define COL_ENEMY (1)
#define ROW_REV_ENEMY (2)
#define COL_REV_ENEMY (3)

struct init_enemy_t {
    int init_point;
    int type;
};

init_enemy_t seq[] = {
    {4, ROW_ENEMY},
    {3, COL_ENEMY},
    {2, COL_ENEMY},
    {3, COL_REV_ENEMY},
    {7, ROW_REV_ENEMY}
};


/* we always wait a bit between updates of the display */
unsigned long delaytime1=500;
unsigned long delaytime2=100;

const int SW_pin = 2;
const int X_pin = 0;
const int Y_pin = 1;


unsigned int n_lives = 3;
unsigned long exp_time;


void reset_timeout()
{
  exp_time = millis() + 120000UL;
}

void print_lives(unsigned int n)
{
  unsigned int i;
  lcd.setCursor(0,1);
  lcd.print("Vidas: ");
  for (i=0; i < n_lives;i++)
  {
    lcd.print("#");
  }
  lcd.print(" ");
}

void print_time_left(void)
{
  lcd.setCursor(0,0);
  lcd.print("Tiempo: ");
  lcd.print((exp_time - millis())/1000);
  lcd.print("    ");
}

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);
  reset_timeout();
  lcd.begin(16,2);
  print_time_left();
  print_lives(n_lives);
}

/*
 This method will display the characters for the
 word "Arduino" one after the other on the matrix. 
 (you need at least 5x7 leds to see the whole chars)
 */
void writeArduinoOnMatrix() {
  /* here is the data for the characters */
  byte a[5]={B01111110,B10001000,B10001000,B10001000,B01111110};
  byte r[5]={B00010000,B00100000,B00100000,B00010000,B00111110};
  byte d[5]={B11111110,B00010010,B00100010,B00100010,B00011100};
  byte u[5]={B00111110,B00000100,B00000010,B00000010,B00111100};
  byte i[5]={B00000000,B00000010,B10111110,B00100010,B00000000};
  byte n[5]={B00011110,B00100000,B00100000,B00010000,B00111110};
  byte o[5]={B00011100,B00100010,B00100010,B00100010,B00011100};

  /* now display them one by one with a small delay */
  lc.setRow(0,0,a[0]);
  lc.setRow(0,1,a[1]);
  lc.setRow(0,2,a[2]);
  lc.setRow(0,3,a[3]);
  lc.setRow(0,4,a[4]);
  delay(delaytime1);
  lc.setRow(0,0,r[0]);
  lc.setRow(0,1,r[1]);
  lc.setRow(0,2,r[2]);
  lc.setRow(0,3,r[3]);
  lc.setRow(0,4,r[4]);
  delay(delaytime1);
  lc.setRow(0,0,d[0]);
  lc.setRow(0,1,d[1]);
  lc.setRow(0,2,d[2]);
  lc.setRow(0,3,d[3]);
  lc.setRow(0,4,d[4]);
  delay(delaytime1);
  lc.setRow(0,0,u[0]);
  lc.setRow(0,1,u[1]);
  lc.setRow(0,2,u[2]);
  lc.setRow(0,3,u[3]);
  lc.setRow(0,4,u[4]);
  delay(delaytime1);
  lc.setRow(0,0,i[0]);
  lc.setRow(0,1,i[1]);
  lc.setRow(0,2,i[2]);
  lc.setRow(0,3,i[3]);
  lc.setRow(0,4,i[4]);
  delay(delaytime1);
  lc.setRow(0,0,n[0]);
  lc.setRow(0,1,n[1]);
  lc.setRow(0,2,n[2]);
  lc.setRow(0,3,n[3]);
  lc.setRow(0,4,n[4]);
  delay(delaytime1);
  lc.setRow(0,0,o[0]);
  lc.setRow(0,1,o[1]);
  lc.setRow(0,2,o[2]);
  lc.setRow(0,3,o[3]);
  lc.setRow(0,4,o[4]);
  delay(delaytime1);
  lc.setRow(0,0,0);
  lc.setRow(0,1,0);
  lc.setRow(0,2,0);
  lc.setRow(0,3,0);
  lc.setRow(0,4,0);
  delay(delaytime1);
}

/*
  This function lights up a some Leds in a row.
 The pattern will be repeated on every row.
 The pattern will blink along with the row-number.
 row number 4 (index==3) will blink 4 times etc.
 */
void rows() {
  for(int row=0;row<8;row++) {
    delay(delaytime2);
    lc.setRow(0,row,B10100000);
    delay(delaytime2);
    lc.setRow(0,row,(byte)0);
    for(int i=0;i<row;i++) {
      delay(delaytime2);
      lc.setRow(0,row,B10100000);
      delay(delaytime2);
      lc.setRow(0,row,(byte)0);
    }
  }
}

/*
  This function lights up a some Leds in a column.
 The pattern will be repeated on every column.
 The pattern will blink along with the column-number.
 column number 4 (index==3) will blink 4 times etc.
 */
void columns() {
  for(int col=0;col<8;col++) {
    delay(delaytime2);
    lc.setColumn(0,col,B10100000);
    delay(delaytime2);
    lc.setColumn(0,col,(byte)0);
    for(int i=0;i<col;i++) {
      delay(delaytime2);
      lc.setColumn(0,col,B10100000);
      delay(delaytime2);
      lc.setColumn(0,col,(byte)0);
    }
  }
}

/* 
 This function will light up every Led on the matrix.
 The led will blink along with the row-number.
 row number 4 (index==3) will blink 4 times etc.
 */
void single() {
}


const bool screen_inverted = false;

#define VCOL(x) (screen_inverted?(7-x):x)
#define VROW(y) (screen_inverted?(7-y):y)

void pointxy(int col, int row, bool on)
{
  lc.setLed(0,VROW(row),VCOL(col),on);
}

void boxxy(int col, int row, bool on)
{
  lc.setLed(0,VROW(row),VCOL(col),on);
  lc.setLed(0,VROW(row+1),VCOL(col),on);
  lc.setLed(0,VROW(row),VCOL(col+1),on);
  lc.setLed(0,VROW(row+1),VCOL(col+1),on);
}

void readXYJoystick(int *p_x, int *p_y)
{
	*p_x = ((analogRead(X_pin) + 128) >> 8) - 2;
	*p_y = ((analogRead(Y_pin) + 128) >> 8) - 2;
}



bool point_eq(int a_x,int a_y, int b_x, int b_y)
{
    if (a_x != b_x) return false;
    if (a_y != b_y) return false;
    return true;
}

bool check_collision(int sq_x, int sq_y, int dot_x, int dot_y)
{
    if (point_eq(sq_x,sq_y,dot_x,dot_y)) return true;
    if (point_eq(sq_x+1,sq_y,dot_x,dot_y)) return true;
    if (point_eq(sq_x+1,sq_y,dot_x+1,dot_y)) return true;
    if (point_eq(sq_x,sq_y,dot_x+1,dot_y)) return true;
    return false;
}


void draw_all(void)
{
  for (int i=0;i<8;i++) {
      lc.setRow(0,i,B11111111);
  }
}

void erase_all(void)
{
  for (int i=0;i<8;i++) {
      lc.setRow(0,i,B00000000);
  }
}


bool did_timeout(void)
{
  if (millis() > exp_time)
    return true;
  else
    return false;
}

void reset_game(void)
{
  reset_timeout();
  n_lives = 3;
  draw_all();
  delay(delaytime2);
  erase_all();
  delay(delaytime2);
  draw_all();
  delay(delaytime2);
  erase_all();
  delay(delaytime2);
  print_lives(n_lives);
}

void loop() { 
  int x, y;
  int col = 4;
  int row = 4;
  int nseq = 0;
  int enemy_col = 0;
  int enemy_row = 0;
  int delta_col = 0;
  int delta_row = 0;
  int type;
  int i;
  while (1) {
      type = seq[nseq].type;
      switch(type) {
          case ROW_ENEMY:
              enemy_row = 0;
              enemy_col = seq[nseq].init_point;
              delta_row = 1;
              delta_col = 0;
              break;
          case COL_ENEMY:
              enemy_row = seq[nseq].init_point;
              enemy_col = 0;
              delta_row = 0;
              delta_col = 1;
              break;
          case ROW_REV_ENEMY:
              enemy_row = 7;
              enemy_col = seq[nseq].init_point;
              delta_row = -1;
              delta_col = 0;
              break;
          case COL_REV_ENEMY:
              enemy_row = seq[nseq].init_point;
              enemy_col = 7;
              delta_row = 0;
              delta_col = -1;
              break;
      }
      for (i=0;i<8;i++) {
          if (did_timeout())
          {
            reset_game();
          }
          pointxy(enemy_col,enemy_row,true);
          boxxy(col,row,true);
          print_time_left();
          delay(2*delaytime2);
          if (check_collision(col,row,enemy_col,enemy_row)) {
              boxxy(col,row,false);
              delay(delaytime2);
              boxxy(col,row,true);
              delay(delaytime2);
              boxxy(col,row,false);
              delay(delaytime2);
              boxxy(col,row,true);
              delay(delaytime2);
              if (n_lives > 0)
              {
                n_lives --;
                print_lives(n_lives);
              }
              if (!n_lives)
              {
                reset_game();
              }

          }

          boxxy(col,row,false);
          pointxy(enemy_col,enemy_row,false);
          readXYJoystick(&x,&y);
          col -= x;
          row -= y;
          if (col < 0) col = 0;
          if (col > 6) col = 6;
          if (row < 0) row = 0;
          if (row > 6) row = 6;
          enemy_col += delta_col;
          enemy_row += delta_row;
      }
      nseq ++;
      if (nseq > 7) nseq = 0;
  }
}
