/*
  TileMaze.pde
 
 Example file using the The Meggy Jr Simplified Library (MJSL)
  from the Meggy Jr RGB library for Arduino
   
 Adapted from Meggy_Rotate demo.
   
   
 
 Version 1.25 - 12/2/2008
 Copyright (c) 2008 Windell H. Oskay.  All right reserved.
 http://www.evilmadscientist.com/
 
 This library is free software: you can mazeColoristribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this library.  If not, see <http://www.gnu.org/licenses/>.
     
 */

 
 
 
 

/*
 * Adapted from "Blink,"  The basic Arduino example.  
 * http://www.arduino.cc/en/Tutorial/Blink
 */

#include <MeggyJrSimple.h>    // RequimazeColor code, line 1 of 2.

/* The quadrant layout is numbemazeColor as below:
 *  
 *  1  2
 *  4  3
 *  
 */

// This is used for saving the state of the screen.
int screen[8][8];

// This toggles the state of the player.
boolean mazeActive = true;
int playerX, playerY;
int playerColor = 1;
int mazeColor = 6;
int counter;
int appleX, appleY;
boolean appleEaten = true;

int quad01[4][4] = {
{4,4,4,4},
{0,0,0,4},
{4,4,0,4},
{0,4,0,4}
};

int quad02[4][4] = {
{4,4,4,4},
{0,0,0,4},
{4,4,0,4},
{0,4,0,4}
};

int quad03[4][4] = {
{4,4,4,4},
{0,0,0,4},
{4,4,0,4},
{0,4,0,4}
};

int quad04[4][4] = {
{4,4,4,4},
{0,0,0,4},
{4,4,0,4},
{0,4,0,4}
};



void setup()                    // run once, when the sketch starts
{
  MeggyJrSimpleSetup();      // RequimazeColor code, line 2 of 2.
  Serial.begin(9600);
  counter = 0;
  playerX = 1;
  playerY = 1;
}

void loop()                     // run over and over again
{ 
  if (counter < 20)
    counter++;
  else counter = 0;
  
  DisplaySlate();
  ClearSlate();
  DrawScreen();
  DrawApple();
  DrawPx(appleX,appleY,Red);
  CheckButtonsPress();
  if (mazeActive)
  {
    if (Button_Up)
      RotateQuad(1);
    if (Button_Right)
      RotateQuad(2);
    if (Button_Down)
      RotateQuad(3);
    if (Button_Left)
      RotateQuad(4);
    DrawScreen();
  }
  else
  {
    if (Button_Up)
    {
      if (playerY < 7 && ReadPx(playerX, playerY + 1) == 0)
        playerY++;
    }
    if (Button_Right)
    {
      if (playerX < 7 && ReadPx(playerX + 1, playerY) == 0)
        playerX++;
    }
    if (Button_Down)
    {
      if (playerY > 0 && ReadPx(playerX, playerY - 1) == 0)
        playerY--;
    }
    if (Button_Left)
    {
      if (playerX > 0  && ReadPx(playerX - 1, playerY) == 0)
        playerX--;
    }
  }

  DrawScreen();
  DrawPlayer();
  
  if (Button_A)
  {
    mazeActive = !mazeActive;
  }
  
  if (Button_B)
  {
    appleEaten = true;
  }
}

void SaveScreen() // Takes whatever is on the screen and copies it into screen[][] which is an 8x8 2D array.
{
  for (int x = 0; x < 8; x++)
  {
    for (int y = 0; y < 8; y++)
    {
      screen[x][y] = ReadPx(x,y); // The color of each pixel is saved in its corresponding location in screen[][].
    }
  }
}

// Proceeds through quad01[][] and copies everything into a temp array to rotate it.
// Having to use a quadrant variable is stupid inefficient and a better way would
// be to pass in a reference to the appropriate array, but Version 2.
void RotateQuad(int quadrant) 
{
  Serial.println("IN");
  Serial.print(playerX);
  Serial.print(" ");
  Serial.println(playerY);
  int startX, startY;
  int temp[4][4]; // create temp array to hold rotated coords
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if (quadrant == 1)
        temp[i][j] = quad01[j][3-i];
      else if (quadrant == 2)
        temp[i][j] = quad02[j][3-i];
      else if (quadrant == 3)
        temp[i][j] = quad03[j][3-i];
      else temp[i][j] = quad04[j][3-i];
    }
  }

  // The position of the player must be rotated if it exists on the same quadrant
  // Same for apple. Pass in 0 to update coordinates for the player, and 1 for the apple
  if (locateQuadrant(playerX,playerY) == quadrant)
    rotatePoints(quadrant,0);
  if (locateQuadrant(appleX,appleY) == quadrant)
    rotatePoints(quadrant,1);

  // copy everything from temp array over to screen array
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if (quadrant == 1)
         quad01[i][j] = temp[i][j];
       else if (quadrant == 2)
         quad02[i][j] = temp[i][j];
       else if (quadrant == 3)
         quad03[i][j] = temp[i][j];
       else quad04[i][j] = temp[i][j];
    }
  }
  Serial.println("OUT");
  Serial.print(playerX);
  Serial.print(" ");
  Serial.println(playerY);
}

void DrawScreen()
{
  // Draw Quad01
  int col = 0;
  for (int x = 0; x < 4; x++)
  {
    int row = 0;
    for (int y = 7; y > 3; y--)
    {
      DrawPx(x,y,quad01[row][col]);
      col++;
    }
    row++;
  }

  // Draw Quad02
  col = 0;
  for (int x = 4; x < 8; x++)
  {
    int row = 0;
    for (int y = 7; y > 3; y--)
    {
      DrawPx(x,y,quad02[row][col]);
      col++;
    }
    row++;
  }

  // Draw Quad03
  col = 0;
  for (int x = 4; x < 8; x++)
  {
    int row = 0;
    for (int y = 3; y >= 0; y--)
    {
      DrawPx(x,y,quad03[row][col]);
      col++;
    }
    row++;
  }

  // Draw Quad04
  col = 0;
  for (int x = 0; x < 4; x++)
  {
    int row = 0;
    for (int y = 3; y >= 0; y--)
    {
      DrawPx(x,y,quad04[row][col]);
      col++;
    }
    row++;
  }
}

void RotateScreen(int angle) // Proceeds through screen[][] and copies everything into a temp array to rotate it.
{
  int temp[8][8]; // create temp array to hold rotated coords
  
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (angle == 90)
        temp[j][7-i] = screen[i][j];
      else
        temp[i][j] = screen[j][7-i];
    }
  }
  // copy everything from temp array over to screens array
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      screen[i][j] = temp[i][j];
    }
  }
  PrintScreen();
}

void PrintScreen() // Call this whenever you want to see the contents of the screen array.
{
  for (int x = 0; x < 8; x++)
  {
    for (int y = 0; y < 8; y++)
    {
      Serial.print(screen[x][y]);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void DrawPlayer()
{
  if (!mazeActive)
    DrawPx(playerX, playerY, playerColor);
  else if (counter % 4 == 0)
    DrawPx(playerX, playerY, playerColor);
  else
    DrawPx(playerX, playerY, 0);
}

// Returns the quadrant the player is in.
// Modified to allow any coordinates to be passed in
int locateQuadrant(int x , int y)
{
  if (playerY > 3 && playerX < 4)
    return 1;
  if (playerY > 3 && playerX > 3)
    return 2;
  if (playerY < 4 && playerX > 3)
    return 3;
  else return 4;
}

// Target is 0 for player, 1 for apple
void rotatePoints(int quadrant,int target)
{
  // Because the player could be located in a different quadrant, we have to
  // do the rotation on a 4x4 grid and then add in the offset.
  int offsetX, offsetY;
  int x, y;

  // Copying values from target
  if (target == 0)
  {
    x = playerX;
    y = playerY;
  }

  else if (target == 1)
  {
    x = appleX;
    y = appleY;
  }
  
  switch (quadrant)
  {
    case 1:
    {
      offsetX = 0;
      offsetY = 4;
      break;
    }
    case 2:
    {
      offsetX = 4;
      offsetY = 4;
      break;
    }
    case 3:
    {
      offsetX = 4;
      offsetY = 0;
      break;
    }
    default:
    {
      offsetX = 0;
      offsetY = 0;
    }
  }

  x -= offsetX;
  x -= offsetY;

  // First case: one diagonal where x is the same as y
  if (x == y)
  {
    y = 3 - x;
  }
  // Second case: other diagonal where sum is 3
  else if (x + y == 3)
  {
    x = y;
  }

  // All other cases follow same pattern
  else
  {
    int t = x;
    x = y;
    y = 3 - t;
  }

  // Adding the offset back
  x += offsetX;
  y += offsetY;

  // Copying back to target
  if (target == 0)
  {
    playerX = x;
    playerY = y;
  }

  else if (target == 1)
  {
    appleX = x;
    appleY = y;
  }
}

void DrawApple()
{
  if (appleEaten == true)
  {
    do
    {
      appleX = random(8);
      appleY = random(8);
    }
    while (ReadPx(appleX,appleY) != 0);
    appleEaten = false;
  }
  else DrawPx(appleX,appleY,Red);
}

