/******************************************************************************
 * MicroOLED_Demo.ino
 * SFE_MicroOLED Library Demo
 * Jim Lindblom @ SparkFun Electronics
 * Original Creation Date: October 27, 2014
 * 
 * This sketch uses the MicroOLED library to draw a 3-D projected
 * cube, and rotate it along all three axes.
 * 
 * Development environment specifics:
 *  Arduino 1.0.5
 *  Arduino Pro 3.3V
 *  Micro OLED Breakout v1.0
 * 
 * This code is beerware; if you see me (or any other SparkFun employee) at the
 * local, and you've found our code helpful, please buy us a round!
 * 
 * Distributed as-is; no warranty is given.
 ******************************************************************************/
#include "SFE_MicroOLED.h"  // Include the SFE_MicroOLED library
#include "math.h"

//////////////////////////
// MicroOLED Definition //
//////////////////////////
#define PIN_RESET D4  // Connect RST to pin 9
#define PIN_DC    D5  // Connect DC to pin 8
#define PIN_CS    A2 // Connect CS to pin 10
#define DC_JUMPER 0

float PI = 3.14159265359;
//////////////////////////////////
// MicroOLED Object Declaration //
//////////////////////////////////
MicroOLED oled(PIN_RESET, PIN_DC, PIN_CS); // SPI declaration
//MicroOLED oled(PIN_RESET, DC_JUMPER);    // I2C declaration

void setup()
{
  oled.begin();    // Initialize the OLED
  oled.clear(ALL); // Clear the display's internal memory
  oled.display();  // Display what's in the buffer (splashscreen)
  delay(1000);     // Delay 1000 ms
  oled.clear(PAGE); // Clear the buffer.
  
  randomSeed(analogRead(A0) + analogRead(A1));
}

void pixelExample()
{
  printTitle("Pixels", 1);
  
  for (int i=0; i<512; i++)
  {
    oled.pixel(random(oled.getLCDWidth()), random(oled.getLCDHeight()));
    oled.display();
  }
}

void lineExample()
{
  int middleX = oled.getLCDWidth() / 2;
  int middleY = oled.getLCDHeight() / 2;
  int xEnd, yEnd;
  int lineWidth = min(middleX, middleY);
  
  printTitle("Lines!", 1);
  
  for (int i=0; i<3; i++)
  {
    for (int deg=0; deg<360; deg+=15)
    {
      xEnd = lineWidth * cos(deg * PI / 180.0);
      yEnd = lineWidth * sin(deg * PI / 180.0);
      
      oled.line(middleX, middleY, middleX + xEnd, middleY + yEnd);
      oled.display();
      delay(10);
    }
    for (int deg=0; deg<360; deg+=15)
    {
      xEnd = lineWidth * cos(deg * PI / 180.0);
      yEnd = lineWidth * sin(deg * PI / 180.0);
      
      oled.line(middleX, middleY, middleX + xEnd, middleY + yEnd, BLACK, NORM);
      oled.display();
      delay(10);
    }
  }
}

void shapeExample()
{
  printTitle("Shapes!", 0);
  
  // Silly pong demo. It takes a lot of work to fake pong...
  int paddleW = 3;  // Paddle width
  int paddleH = 15;  // Paddle height
  // Paddle 0 (left) position coordinates
  int paddle0_Y = (oled.getLCDHeight() / 2) - (paddleH / 2);
  int paddle0_X = 2;
  // Paddle 1 (right) position coordinates
  int paddle1_Y = (oled.getLCDHeight() / 2) - (paddleH / 2);
  int paddle1_X = oled.getLCDWidth() - 3 - paddleW;
  int ball_rad = 2;  // Ball radius
  // Ball position coordinates
  int ball_X = paddle0_X + paddleW + ball_rad;
  int ball_Y = random(1 + ball_rad, oled.getLCDHeight() - ball_rad);//paddle0_Y + ball_rad;
  int ballVelocityX = 1;  // Ball left/right velocity
  int ballVelocityY = 1;  // Ball up/down velocity
  int paddle0Velocity = -1;  // Paddle 0 velocity
  int paddle1Velocity = 1;  // Paddle 1 velocity
    
  //while(ball_X >= paddle0_X + paddleW - 1)
  while ((ball_X - ball_rad > 1) && 
         (ball_X + ball_rad < oled.getLCDWidth() - 2))
  {
    // Increment ball's position
    ball_X+=ballVelocityX;
    ball_Y+=ballVelocityY;
    // Check if the ball is colliding with the left paddle
    if (ball_X - ball_rad < paddle0_X + paddleW)
    {
      // Check if ball is within paddle's height
      if ((ball_Y > paddle0_Y) && (ball_Y < paddle0_Y + paddleH))
      {
        ball_X++;  // Move ball over one to the right
        ballVelocityX = -ballVelocityX; // Change velocity
      }
    }
    // Check if the ball hit the right paddle
    if (ball_X + ball_rad > paddle1_X)
    {
      // Check if ball is within paddle's height
      if ((ball_Y > paddle1_Y) && (ball_Y < paddle1_Y + paddleH))
      {
        ball_X--;  // Move ball over one to the left
        ballVelocityX = -ballVelocityX; // change velocity
      }
    }
    // Check if the ball hit the top or bottom
    if ((ball_Y <= ball_rad) || (ball_Y >= (oled.getLCDHeight() - ball_rad - 1)))
    {
      // Change up/down velocity direction
      ballVelocityY = -ballVelocityY;
    }
    // Move the paddles up and down
    paddle0_Y += paddle0Velocity;
    paddle1_Y += paddle1Velocity;
    // Change paddle 0's direction if it hit top/bottom
    if ((paddle0_Y <= 1) || (paddle0_Y > oled.getLCDHeight() - 2 - paddleH))
    {
      paddle0Velocity = -paddle0Velocity;
    }
    // Change paddle 1's direction if it hit top/bottom
    if ((paddle1_Y <= 1) || (paddle1_Y > oled.getLCDHeight() - 2 - paddleH))
    {
      paddle1Velocity = -paddle1Velocity;
    }
    
    // Draw the Pong Field
    oled.clear(PAGE);  // Clear the page
    // Draw an outline of the screen:
    oled.rect(0, 0, oled.getLCDWidth() - 1, oled.getLCDHeight());
    // Draw the center line
    oled.rectFill(oled.getLCDWidth()/2 - 1, 0, 2, oled.getLCDHeight());
    // Draw the Paddles:
    oled.rectFill(paddle0_X, paddle0_Y, paddleW, paddleH);
    oled.rectFill(paddle1_X, paddle1_Y, paddleW, paddleH);
    // Draw the ball:
    oled.circle(ball_X, ball_Y, ball_rad);
    // Actually draw everything on the screen:
    oled.display();
    delay(25);  // Delay for visibility
  }
  delay(1000);
}

void textExamples()
{
  printTitle("Text!", 1);
  
  // Demonstrate font 0. 5x8 font
  oled.clear(PAGE);     // Clear the screen
  oled.setFontType(0);  // Set font to type 0
  oled.setCursor(0, 0); // Set cursor to top-left
  // There are 255 possible characters in the font 0 type.
  // Lets run through all of them and print them out!
  for (int i=0; i<=255; i++)
  {
    // You can write byte values and they'll be mapped to
    // their ASCII equivalent character.
    oled.write(i);  // Write a byte out as a character
    oled.display(); // Draw on the screen
    delay(10);      // Wait 10ms
    // We can only display 60 font 0 characters at a time.
    // Every 60 characters, pause for a moment. Then clear
    // the page and start over.
    if ((i%60 == 0) && (i != 0))
    {
      delay(500);           // Delay 500 ms
      oled.clear(PAGE);     // Clear the page
      oled.setCursor(0, 0); // Set cursor to top-left
    }
  }
  delay(500);  // Wait 500ms before next example
  
  // Demonstrate font 1. 8x16. Let's use the print function
  // to display every character defined in this font.
  oled.setFontType(1);  // Set font to type 1
  oled.clear(PAGE);     // Clear the page
  oled.setCursor(0, 0); // Set cursor to top-left
  // Print can be used to print a string to the screen:
  oled.print(" !\"#$%&'()*+,-./01234");
  oled.display();       // Refresh the display
  delay(1000);          // Delay a second and repeat
  oled.clear(PAGE);
  oled.setCursor(0, 0);
  oled.print("56789:;<=>?@ABCDEFGHI");
  oled.display();
  delay(1000);
  oled.clear(PAGE);
  oled.setCursor(0, 0);
  oled.print("JKLMNOPQRSTUVWXYZ[\\]^");
  oled.display();
  delay(1000);
  oled.clear(PAGE);
  oled.setCursor(0, 0);
  oled.print("_`abcdefghijklmnopqrs");
  oled.display();
  delay(1000);
  oled.clear(PAGE);
  oled.setCursor(0, 0);
  oled.print("tuvwxyz{|}~");
  oled.display();
  delay(1000);
  
  // Demonstrate font 2. 10x16. Only numbers and '.' are defined. 
  // This font looks like 7-segment displays.
  // Lets use this big-ish font to display readings from the
  // analog pins.
  for (int i=0; i<25; i++)
  {
    oled.clear(PAGE);            // Clear the display
    oled.setCursor(0, 0);        // Set cursor to top-left
    oled.setFontType(0);         // Smallest font
    oled.print("A0: ");          // Print "A0"
    oled.setFontType(2);         // 7-segment font
    oled.print(analogRead(A0));  // Print a0 reading
    oled.setCursor(0, 16);       // Set cursor to top-middle-left
    oled.setFontType(0);         // Repeat
    oled.print("A1: ");
    oled.setFontType(2);
    oled.print(analogRead(A1));
    oled.setCursor(0, 32);
    oled.setFontType(0);
    oled.print("A2: ");
    oled.setFontType(2);
    oled.print(analogRead(A2));
    oled.display();
    delay(100);
  }
  
  // Demonstrate font 3. 12x48. Stopwatch demo.
  oled.setFontType(3);  // Use the biggest font
  int ms = 0;
  int s = 0;
  while (s <= 5)
  {
    oled.clear(PAGE);     // Clear the display
    oled.setCursor(0, 0); // Set cursor to top-left
    if (s < 10)
      oled.print("00");   // Print "00" if s is 1 digit
    else if (s < 100)     
      oled.print("0");    // Print "0" if s is 2 digits
    oled.print(s);        // Print s's value
    oled.print(":");      // Print ":"
    oled.print(ms);       // Print ms value
    oled.display();       // Draw on the screen
    ms++;         // Increment ms
    if (ms >= 10) // If ms is >= 10
    {
      ms = 0;     // Set ms back to 0
      s++;        // and increment s
    }
  }
}

void loop()
{
  pixelExample();  // Run the pixel example function
  lineExample();   // Then the line example function
  shapeExample();  // Then the shape example
  textExamples();  // Finally the text example
}

// Center and print a small title
// This function is quick and dirty. Only works for titles one
// line long.
void printTitle(String title, int font)
{
  int middleX = oled.getLCDWidth() / 2;
  int middleY = oled.getLCDHeight() / 2;
  
  oled.clear(PAGE);
  oled.setFontType(font);
  // Try to set the cursor in the middle of the screen
  oled.setCursor(middleX - (oled.getFontWidth() * (title.length()/2)),
                 middleY - (oled.getFontWidth() / 2));
  // Print the title:
  oled.print(title);
  oled.display();
  delay(1500);
  oled.clear(PAGE);
}

/*


#include "SFE_MicroOLED.h"
//#include "7segment.h"
//#include "font5x7.h"
//#include "font8x16.h"
//#include "fontlargenumber.h"



#include "math.h"

//////////////////////////
// MicroOLED Definition //
//////////////////////////
#define PIN_RESET D4  // Connect RST to pin 9 (SPI & I2C)
#define PIN_DC    D5  // Connect DC to pin 8 (SPI only)
#define PIN_CS    A2 // Connect CS to pin 10 (SPI only)


MicroOLED oled(PIN_RESET, PIN_DC, PIN_CS);  // SPI Example

// Use these variables to set the initial time
int hours = 11;
int minutes = 50;
int seconds = 30;

// How fast do you want the clock to spin? Set this to 1 for fun.
// Set this to 1000 to get _about_ 1 second timing.
const int CLOCK_SPEED = 1000;

// Global variables to help draw the clock face:
const int MIDDLE_Y = oled.getLCDHeight() / 2;
const int MIDDLE_X = oled.getLCDWidth() / 2;

int CLOCK_RADIUS;
int POS_12_X, POS_12_Y;
int POS_3_X, POS_3_Y;
int POS_6_X, POS_6_Y;
int POS_9_X, POS_9_Y;
int S_LENGTH;
int M_LENGTH;
int H_LENGTH;

float PI = 3.14159265359;


unsigned long lastDraw = 0;

void initClockVariables()
{
  // Calculate constants for clock face component positions:
  oled.setFontType(0);
  CLOCK_RADIUS = min(MIDDLE_X, MIDDLE_Y) - 1;
  POS_12_X = MIDDLE_X - oled.getFontWidth();
  POS_12_Y = MIDDLE_Y - CLOCK_RADIUS + 2;
  POS_3_X  = MIDDLE_X + CLOCK_RADIUS - oled.getFontWidth() - 1;
  POS_3_Y  = MIDDLE_Y - oled.getFontHeight()/2;
  POS_6_X  = MIDDLE_X - oled.getFontWidth()/2;
  POS_6_Y  = MIDDLE_Y + CLOCK_RADIUS - oled.getFontHeight() - 1;
  POS_9_X  = MIDDLE_X - CLOCK_RADIUS + oled.getFontWidth() - 2;
  POS_9_Y  = MIDDLE_Y - oled.getFontHeight()/2;

  // Calculate clock arm lengths
  S_LENGTH = CLOCK_RADIUS - 2;
  M_LENGTH = S_LENGTH * 0.7;
  H_LENGTH = S_LENGTH * 0.5;
}

void setup()
{
  oled.begin();     // Initialize the OLED
  oled.clear(PAGE); // Clear the display's internal memory
  oled.clear(ALL);  // Clear the library's display buffer
  oled.display();   // Display what's in the buffer (splashscreen)

  initClockVariables();

  oled.clear(ALL);
  drawFace();
  drawArms(hours, minutes, seconds);
  oled.display(); // display the memory buffer drawn
}

void loop()
{

  // Check if we need to update seconds, minutes, hours:
  if (lastDraw + CLOCK_SPEED < millis())
  {
    lastDraw = millis();
    // Add a second, update minutes/hours if necessary:
    updateTime();

    // Draw the clock:
    oled.clear(PAGE);  // Clear the buffer
    drawFace();  // Draw the face to the buffer
    drawArms(hours, minutes, seconds);  // Draw arms to the buffer
    oled.display(); // Draw the memory buffer
  }
}

// Simple function to increment seconds and then increment minutes
// and hours if necessary.
void updateTime()
{
  seconds++;  // Increment seconds
  if (seconds >= 60)  // If seconds overflows (>=60)
  {
    seconds = 0;  // Set seconds back to 0
    minutes++;    // Increment minutes
    if (minutes >= 60)  // If minutes overflows (>=60)
    {
      minutes = 0;  // Set minutes back to 0
      hours++;      // Increment hours
      if (hours >= 12)  // If hours overflows (>=12)
      {
        hours = 0;  // Set hours back to 0
      }
    }
  }
}

// Draw the clock's three arms: seconds, minutes, hours.
void drawArms(int h, int m, int s)
{
  double midHours;  // this will be used to slightly adjust the hour hand
  static int hx, hy, mx, my, sx, sy;

  // Adjust time to shift display 90 degrees ccw
  // this will turn the clock the same direction as text:
  h -= 3;
  m -= 15;
  s -= 15;
  if (h <= 0)
    h += 12;
  if (m < 0)
    m += 60;
  if (s < 0)
    s += 60;

  // Calculate and draw new lines:
  s = map(s, 0, 60, 0, 360);  // map the 0-60, to "360 degrees"
  sx = S_LENGTH * cos(PI * ((float)s) / 180);  // woo trig!
  sy = S_LENGTH * sin(PI * ((float)s) / 180);  // woo trig!
  // draw the second hand:
  oled.line(MIDDLE_X, MIDDLE_Y, MIDDLE_X + sx, MIDDLE_Y + sy);

  m = map(m, 0, 60, 0, 360);  // map the 0-60, to "360 degrees"
  mx = M_LENGTH * cos(PI * ((float)m) / 180);  // woo trig!
  my = M_LENGTH * sin(PI * ((float)m) / 180);  // woo trig!
  // draw the minute hand
  oled.line(MIDDLE_X, MIDDLE_Y, MIDDLE_X + mx, MIDDLE_Y + my);

  midHours = minutes/12;  // midHours is used to set the hours hand to middling levels between whole hours
  h *= 5;  // Get hours and midhours to the same scale
  h += midHours;  // add hours and midhours
  h = map(h, 0, 60, 0, 360);  // map the 0-60, to "360 degrees"
  hx = H_LENGTH * cos(PI * ((float)h) / 180);  // woo trig!
  hy = H_LENGTH * sin(PI * ((float)h) / 180);  // woo trig!
  // draw the hour hand:
  oled.line(MIDDLE_X, MIDDLE_Y, MIDDLE_X + hx, MIDDLE_Y + hy);
}

// Draw an analog clock face
void drawFace()
{
  // Draw the clock border
  oled.circle(MIDDLE_X, MIDDLE_Y, CLOCK_RADIUS);

  // Draw the clock numbers
  oled.setFontType(0); // set font type 0, please see declaration in SFE_MicroOLED.cpp
  oled.setCursor(POS_12_X, POS_12_Y); // points cursor to x=27 y=0
  oled.print(12);
  oled.setCursor(POS_6_X, POS_6_Y);
  oled.print(6);
  oled.setCursor(POS_9_X, POS_9_Y);
  oled.print(9);
  oled.setCursor(POS_3_X, POS_3_Y);
  oled.print(3);
}
*/