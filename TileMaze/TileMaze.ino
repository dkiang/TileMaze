/*
  TileMaze.pde
 
 Example file using the The Meggy Jr Simplified Library (MJSL)
  from the Meggy Jr RGB library for Arduino
   
 Adapted from Meggy_Rotate demo.
   
   
 
 Version 1.25 - 12/2/2008
 Copyright (c) 2008 Windell H. Oskay.  All right reserved.
 http://www.evilmadscientist.com/
 
 This library is free software: you can redistribute it and/or modify
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

#include <MeggyJrSimple.h>    // Required code, line 1 of 2.
// This holds a copy of the active quadrant.
int screen[4][4];

// This saves the entire game board.
int screenLarge[8][8];

/* This is the active quadrant that syncs with screen[][].
 *  
 *  0  1
 *  3  2
 *  
 */
int quadrant;
int counter;

// This is used for saving the state of the screen.
int screen[8][8];

int quad01[4][4] = {
{1,1,0,1},
{0,0,0,1},
{1,1,0,1},
{0,1,0,1}
};

int quad02[4][4] = {
{1,1,1,1},
{0,0,0,1},
{0,0,0,0},
{0,1,0,0}
};

int quad03[4][4] = {
{1,1,1,0},
{1,1,1,0},
{0,0,0,0},
{0,0,1,0}
};

int quad04[4][4] = {
{1,0,1,1},
{1,0,1,1},
{1,0,0,0},
{1,1,1,1}
};



void setup()                    // run once, when the sketch starts
{
  MeggyJrSimpleSetup();      // Required code, line 2 of 2.
  Serial.begin(9600);
}

void loop()                     // run over and over again
{
  if (counter < 20)
    counter++;
  else counter = 0;
  
  ClearSlate();
  CheckButtonsPress();

  if (Button_Up)
  {
    rotateQuad(0);
  }
    if (Button_Right)
  {
    rotateQuad(1);
  }
    if (Button_Down)
  {
    rotateQuad(3);
  }
    if (Button_Left)
  {
    rotateQuad(3);
  }
  
  if (Button_A)
  {
    SaveScreen();
    RotateScreen(90);
  }
  
  if (Button_B)
  {
    SaveScreen();
    RotateScreen(-90);
  }
  DisplayScreen();
  DisplaySlate();
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
void RotateQuad(int quadrant) 
{
  int startX, startY;
  int temp[4][4]; // create temp array to hold rotated coords
  for (int i = 0; i < 4; j++)
  {
     for (int j = 0; j < 4; j++)
     {
       if (quadrant == 0)
         temp[i][j] = quad0[i][j];
       else if (quadrant == 1)
         temp[i][j] = quad1[i][j];
       else if (quadrant == 2)
         temp[i][j] = quad2[i][j];
       else temp[i][j] = quad3[i][j];
     }
   }

   // Rotate the values 
  for (int i = startX; i < startX + 4; i++)
  {
    for (int j = startY; j < startY + 4; j++)
    {
      temp[j][4-i] = screen[i][j];
    }
  }
  // copy everything from temp array over to screen array
  for (int i = startX; i < startX + 4; i++)
  {
    for (int j = startY; j < startY + 4; j++)
    {
      screen[i][j] = temp[i][j];
    }
  }
  PrintScreen(screen);
}

void DisplayScreen()
{
  ClearSlate();
  for (int x = 0; x < 4; x++)
  {
    for (int y = 0; y < 4; y++)
    {
      DrawPx(x,y,screen01[x][y]);
    }
  }

  for (int x = 4; x < 8; x++)
  {
    for (int y = 0; y < 4; y++)
    {
      DrawPx(x,y,screen02[x][y]);
    }
  }

  for (int x = 0; x < 4; x++)
  {
    for (int y = 4; y < 8; y++)
    {
      DrawPx(x,y,screen03[x][y]);
    }
  }

  for (int x = 4; x < 8; x++)
  {
    for (int y = 4; y < 8; y++)
    {
      DrawPx(x,y,screen04[x][y]);
    }
  }
}

void PrintScreen(int[][4]) // Call this whenever you want to see the contents of the screen array.
{
  for (int x = 0; x < 4; x++)
  {
    for (int y = 0; y < 4; y++)
    {
      //Serial.print(screen[x][y]);
      Serial.print(" ");
    }
    Serial.println();
  }
}

