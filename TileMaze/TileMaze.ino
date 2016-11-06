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

/* The quadrant layout is numbered as below:
 *  
 *  1  2
 *  4  3
 *  
 */

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
  DisplaySlate();
  ClearSlate();
  CheckButtonsPress();
  if (Button_Up)
  {
    RotateQuad(1);
  }
    if (Button_Right)
  {
    RotateQuad(2);
  }
    if (Button_Down)
  {
    RotateQuad(3);
  }
    if (Button_Left)
  {
    RotateQuad(4);
  }

  DisplayScreen();
  
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
//  PrintScreen();
}

void DisplayScreen()
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

