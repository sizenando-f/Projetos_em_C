////////////////////////////////////////////////////////////////////////////////////        
// canvas.cpp
//
// This program allows the user to draw simple shapes on a canvas.
//
// Interaction:
// Left click on a box on the left to select a primitive.
// Then left click on the drawing area: once for point, twice for line or rectangle,
// three times for sector.
// Right click for menu options.
//
//  Updated version with additional features.
//////////////////////////////////////////////////////////////////////////////////// 

#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <cmath>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

#define INACTIVE 0
#define POINT 1
#define LINE 2
#define RECTANGLE 3
#define SECTOR 19
#define TEXT 5
#define NUMBERPRIMITIVES 5

#define PI 3.14159265358979323846

// Use the STL extension of C++.
using namespace std;

// Globals.
static GLsizei width, height; // OpenGL window size.
static float pointSize = 3.0; // Size of point
static int primitive = INACTIVE; // Current drawing primitive.
static int pointCount = 0; // Number of specified points.
static int tempX, tempY; // Co-ordinates of clicked point.
static int temp2X, temp2Y; // Additional co-ordinates for sector.
static int isGrid = 1; // Is there grid?
static int gridSize = 10; // Grid size (10 divisions by default)
static float currentRed = 0.0, currentGreen = 0.0, currentBlue = 0.0; // Current drawing color
static int isFilled = 0; // Is current shape filled?
static int isStippled = 0; // Is current line stippled?
static char inputText[100] = ""; // Text to be drawn
static int textSize = 12; // Text size
static int currentX = 0, currentY = 0;


// Point class.
class Point
{
public:
   Point(int xVal, int yVal, float r = 0.0, float g = 0.0, float b = 0.0)
   {
      x = xVal; y = yVal;
      red = r; green = g; blue = b;
   }
   void drawPoint(void); // Function to draw a point.
public:
   static void setSize(float newSize) { size = newSize; } // Setter for size.
   static float size; // Size of point.

private:
   int x, y; // x and y co-ordinates of point.
   float red, green, blue; // Color of point

};

float Point::size = pointSize; // Set point size.

// Function to draw a point.
void Point::drawPoint()
{  
   glPointSize(size);
   glColor3f(red, green, blue);
   glBegin(GL_POINTS);
      glVertex3f(x, y, 0.0);
   glEnd();   
}

// Vector of points.
vector<Point> points;

// Iterator to traverse a Point array.
vector<Point>::iterator pointsIterator; 

// Function to draw all points in the points array.
void drawPoints(void)
{
   // Loop through the points array drawing each point.
   pointsIterator = points.begin();
   while(pointsIterator != points.end() )
   {
      pointsIterator->drawPoint();
      pointsIterator++;
   }
}

// Line class.
class Line
{
public:
   Line(int x1Val, int y1Val, int x2Val, int y2Val, 
        float r = 0.0, float g = 0.0, float b = 0.0, int stip = 0)
   {
      x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
      red = r; green = g; blue = b;
      stippled = stip;
   }
   void drawLine();
private:
   int x1, y1, x2, y2; // x and y co-ordinates of endpoints.
   float red, green, blue; // Color of line
   int stippled; // Is line stippled
};

// Function to draw a line.
void Line::drawLine()
{
   glColor3f(red, green, blue);
   
   if (stippled)
   {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0x5555);
   }
   
   glBegin(GL_LINES);
      glVertex3f(x1, y1, 0.0);
      glVertex3f(x2, y2, 0.0);
   glEnd();
   
   if (stippled)
   {
      glDisable(GL_LINE_STIPPLE);
   }
}

// Vector of lines.
vector<Line> lines;

// Iterator to traverse a Line array.
vector<Line>::iterator linesIterator;

// Function to draw all lines in the lines array.
void drawLines(void)
{
   // Loop through the lines array drawing each line.
   linesIterator = lines.begin();
   while(linesIterator != lines.end() )
   {
      linesIterator->drawLine();
      linesIterator++;
   }
}

// Rectangle class.
class Rectangle
{
public:
   Rectangle(int x1Val, int y1Val, int x2Val, int y2Val, 
             float r = 0.0, float g = 0.0, float b = 0.0, int fill = 0)
   {
      x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
      red = r; green = g; blue = b;
      filled = fill;
   }
   void drawRectangle();
private:
   int x1, y1, x2, y2; // x and y co-ordinates of diagonally opposite vertices.
   float red, green, blue; // Color of rectangle
   int filled; // Is rectangle filled
};

// Function to draw a rectangle.
void Rectangle::drawRectangle()
{
   glColor3f(red, green, blue);
   
   if (filled)
   {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glRectf(x1, y1, x2, y2);
   }
   else
   {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glRectf(x1, y1, x2, y2);
   }
}

// Vector of rectangles.
vector<Rectangle> rectangles;

// Iterator to traverse a Rectangle array.
vector<Rectangle>::iterator rectanglesIterator;

// Function to draw all rectangles in the rectangles array.
void drawRectangles(void)
{
   // Loop through the rectangles array drawing each rectangle.
   rectanglesIterator = rectangles.begin();
   while(rectanglesIterator != rectangles.end() )
   {
      rectanglesIterator->drawRectangle();
      rectanglesIterator++;
   }
}

// Sector class (for circular sectors)
class Sector
{
public:
   Sector(int cxVal, int cyVal, int p1xVal, int p1yVal, int p2xVal, int p2yVal, 
          float r = 0.0, float g = 0.0, float b = 0.0, int fill = 0)
   {
      cx = cxVal; cy = cyVal; // Center
      p1x = p1xVal; p1y = p1yVal; // First point on arc
      p2x = p2xVal; p2y = p2yVal; // Second point on arc
      red = r; green = g; blue = b;
      filled = fill;
      
      // Calculate radius as distance from center to first point
      radius = sqrt(pow(p1x - cx, 2) + pow(p1y - cy, 2));
      
      // Calculate start and end angles
      startAngle = atan2(p1y - cy, p1x - cx);
      endAngle = atan2(p2y - cy, p2x - cx);
      
      // Ensure proper arc direction
      if (endAngle < startAngle) endAngle += 2 * PI;
   }
   void drawSector();
private:
   int cx, cy; // Center coordinates
   int p1x, p1y, p2x, p2y; // Points on arc
   float radius; // Radius
   float startAngle, endAngle; // Start and end angles
   float red, green, blue; // Color
   int filled; // Is sector filled
};

// Function to draw a sector.
void Sector::drawSector()
{
   glColor3f(red, green, blue);
   
   if (filled)
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   else
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   
   // Draw the sector
   const int numSegments = 100;
   float angleIncrement = (endAngle - startAngle) / numSegments;
   
   if (filled)
   {
      glBegin(GL_TRIANGLE_FAN);
      // Add center point
      glVertex2f(cx, cy);
      
      // Add points along the arc
      for (int i = 0; i <= numSegments; i++)
      {
         float angle = startAngle + i * angleIncrement;
         float x = cx + radius * cos(angle);
         float y = cy + radius * sin(angle);
         glVertex2f(x, y);
      }
      glEnd();
   }
   else
   {
      // Draw the arc
      glBegin(GL_LINE_STRIP);
      for (int i = 0; i <= numSegments; i++)
      {
         float angle = startAngle + i * angleIncrement;
         float x = cx + radius * cos(angle);
         float y = cy + radius * sin(angle);
         glVertex2f(x, y);
      }
      glEnd();
      
      // Draw the radii
      glBegin(GL_LINES);
      glVertex2f(cx, cy);
      glVertex2f(cx + radius * cos(startAngle), cy + radius * sin(startAngle));
      
      glVertex2f(cx, cy);
      glVertex2f(cx + radius * cos(endAngle), cy + radius * sin(endAngle));
      glEnd();
   }
}

// Vector of sectors.
vector<Sector> sectors;

// Iterator to traverse a Sector array.
vector<Sector>::iterator sectorsIterator;

// Function to draw all sectors in the sectors array.
void drawSectors(void)
{
   // Loop through the sectors array drawing each sector.
   sectorsIterator = sectors.begin();
   while(sectorsIterator != sectors.end())
   {
      sectorsIterator->drawSector();
      sectorsIterator++;
   }
}

// Text class
class Text
{
public:
   Text(int xVal, int yVal, const char* str, 
        float r = 0.0, float g = 0.0, float b = 0.0, int s = 12)
   {
      x = xVal; y = yVal;
      text = str;
      red = r; green = g; blue = b;
      size = s;
   }
   void drawText();
private:
   int x, y; // Position
   string text; // Text string
   float red, green, blue; // Color
   int size; // Text size
};

// Function to draw text
void Text::drawText()
{
   glColor3f(red, green, blue);
   glRasterPos2f(x, y);
   
   // Choose font size
   void* font;
   if (size <= 10)
      font = GLUT_BITMAP_HELVETICA_10;
   else if (size <= 12)
      font = GLUT_BITMAP_HELVETICA_12;
   else
      font = GLUT_BITMAP_HELVETICA_18;
   
   // Draw each character
   for (unsigned int i = 0; i < text.length(); i++)
   {
      glutBitmapCharacter(font, text[i]);
   }
}

// Vector of text objects
vector<Text> texts;

// Iterator to traverse a Text array
vector<Text>::iterator textsIterator;

// Function to draw all texts in the texts array
void drawTexts(void)
{
   textsIterator = texts.begin();
   while(textsIterator != texts.end())
   {
      textsIterator->drawText();
      textsIterator++;
   }
}

// Function to draw point selection box in left selection area.
void drawPointSelectionBox(void)
{  
   if (primitive == POINT) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.9*height, 0.1*width, height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0); 
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.9*height, 0.1*width, height); 

   // Draw point icon.
   glPointSize(pointSize);
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_POINTS);
      glVertex3f(0.05*width, 0.95*height, 0.0);
   glEnd();  
}

// Function to draw line selection box in left selection area.
void drawLineSelectionBox(void)
{  
   if (primitive == LINE) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

   // Draw line icon.
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_LINES);
      glVertex3f(0.025*width, 0.825*height, 0.0);
      glVertex3f(0.075*width, 0.875*height, 0.0);
   glEnd();  
}

// Function to draw rectangle selection box in left selection area.
void drawRectangleSelectionBox(void)
{  
   if (primitive == RECTANGLE) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

   // Draw rectangle icon.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.025*width, 0.735*height, 0.075*width, 0.765*height);
   glEnd();  
}

// Function to draw sector selection box in left selection area.
void drawSectorSelectionBox(void)
{  
   if (primitive == SECTOR) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

   // Draw sector icon
   glColor3f(0.0, 0.0, 0.0);
   
   // Draw a wedge-like shape
   float centerX = 0.05 * width;
   float centerY = 0.65 * height;
   float radius = 0.025 * width;
   
   glBegin(GL_LINE_LOOP);
      // Center point
      glVertex2f(centerX, centerY);
      
      // Arc points (about 120 degrees)
      for (int i = 0; i <= 8; i++)
      {
         float angle = -0.4 * PI + i * (0.8 * PI) / 8;
         float x = centerX + radius * cos(angle);
         float y = centerY + radius * sin(angle);
         glVertex2f(x, y);
      }
   glEnd();
}

// Function to draw text selection box in left selection area.
void drawTextSelectionBox(void)
{  
   if (primitive == TEXT) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.5*height, 0.1*width, 0.6*height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.5*height, 0.1*width, 0.6*height);

   // Draw text icon (T letter)
   glColor3f(0.0, 0.0, 0.0);
   glLineWidth(2.0);
   
   // Draw T
   glBegin(GL_LINES);
      // Horizontal bar
      glVertex2f(0.03*width, 0.57*height);
      glVertex2f(0.07*width, 0.57*height);
      
      // Vertical stem
      glVertex2f(0.05*width, 0.57*height);
      glVertex2f(0.05*width, 0.53*height);
   glEnd();
   
   glLineWidth(1.0);
}

// Function to draw unused part of left selection area.
void drawInactiveArea(void)
{
   glColor3f(0.6, 0.6, 0.6);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);

   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);
}

// Function to draw temporary point.
void drawTempPoint(void)
{
   glColor3f(1.0, 0.0, 0.0);
   glPointSize(pointSize);
   glBegin(GL_POINTS);
      glVertex3f(tempX, tempY, 0.0);
   glEnd(); 
}

// Function to draw second temporary point (for sector)
void drawTemp2Point(void)
{
   glColor3f(0.0, 1.0, 0.0);
   glPointSize(pointSize);
   glBegin(GL_POINTS);
      glVertex3f(temp2X, temp2Y, 0.0);
   glEnd(); 
}

// Function to draw a grid.
void drawGrid(void)
{
   int i;
   
   glEnable(GL_LINE_STIPPLE);
   glLineStipple(1, 0x5555); 
   glColor3f(0.75, 0.75, 0.75);

   glBegin(GL_LINES);
      for (i = 1; i < gridSize; i++)
      {
         float pos = (float)i/gridSize;
         glVertex3f((0.1 + pos * 0.9) * width, 0.0, 0.0);
         glVertex3f((0.1 + pos * 0.9) * width, height, 0.0);
         
         glVertex3f(0.1 * width, pos * height, 0.0);
         glVertex3f(width, pos * height, 0.0);
      }
   glEnd();
   glDisable(GL_LINE_STIPPLE);
}

// Function to handle text input
void processTextInput(unsigned char key)
{
   // If backspace and there's text to delete
   if (key == 8 && strlen(inputText) > 0)
   {
      inputText[strlen(inputText) - 1] = '\0';
   }
   // If enter key
   else if (key == 13)
   {
      // Add text to the canvas
      texts.push_back(Text(tempX, tempY, inputText, 
                         currentRed, currentGreen, currentBlue, textSize));
      
      // Reset text input
      inputText[0] = '\0';
      primitive = INACTIVE;
      pointCount = 0;
   }
   // Regular character input
   else if (key >= 32 && key <= 126) // Printable ASCII characters
   {
      int len = strlen(inputText);
      if (len < 99) // Prevent buffer overflow
      {
         inputText[len] = key;
         inputText[len + 1] = '\0';
      }
   }
}

void clearPreview()
{
   currentX = currentY = 0;
   glutPostRedisplay();
}

// Adicione esta função auxiliar antes de drawScene()
void drawPreview()
{
   if (primitive == LINE && pointCount == 1)
   {
      glColor3f(currentRed, currentGreen, currentBlue);
      if (isStippled)
      {
         glEnable(GL_LINE_STIPPLE);
         glLineStipple(1, 0x5555);
      }
      
      glBegin(GL_LINES);
         glVertex2f(tempX, tempY);
         glVertex2f(currentX, currentY);
      glEnd();
      
      if (isStippled) glDisable(GL_LINE_STIPPLE);
   }
   else if (primitive == RECTANGLE && pointCount == 1)
   {
      glColor3f(currentRed, currentGreen, currentBlue);
      if (isFilled) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      
      glBegin(GL_POLYGON);
         glVertex2f(tempX, tempY);
         glVertex2f(currentX, tempY);
         glVertex2f(currentX, currentY);
         glVertex2f(tempX, currentY);
      glEnd();
   }
   else if (primitive == SECTOR)
   {
      if (pointCount == 1)
      {
         // Mostrar linha do centro até o mouse
         glColor3f(currentRed, currentGreen, currentBlue);
         glBegin(GL_LINES);
            glVertex2f(tempX, tempY);
            glVertex2f(currentX, currentY);
         glEnd();
      }
      else if (pointCount == 2)
      {
         // Mostrar setor pré-visualizado
         glColor3f(currentRed, currentGreen, currentBlue);
         if (isFilled) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         
         float radius1 = sqrt(pow(temp2X - tempX, 2) + pow(temp2Y - tempY, 2));
         float radius2 = sqrt(pow(currentX - tempX, 2) + pow(currentY - tempY, 2));
         float startAngle = atan2(temp2Y - tempY, temp2X - tempX);
         float endAngle = atan2(currentY - tempY, currentX - tempX);
         
         if (endAngle < startAngle) endAngle += 2 * PI;
         
         const int numSegments = 100;
         float angleIncrement = (endAngle - startAngle) / numSegments;
         
         if (isFilled)
         {
            glBegin(GL_TRIANGLE_FAN);
               glVertex2f(tempX, tempY);
               for (int i = 0; i <= numSegments; i++)
               {
                  float angle = startAngle + i * angleIncrement;
                  float x = tempX + radius1 * cos(angle);
                  float y = tempY + radius1 * sin(angle);
                  glVertex2f(x, y);
               }
            glEnd();
         }
         else
         {
            glBegin(GL_LINE_STRIP);
               for (int i = 0; i <= numSegments; i++)
               {
                  float angle = startAngle + i * angleIncrement;
                  float x = tempX + radius1 * cos(angle);
                  float y = tempY + radius1 * sin(angle);
                  glVertex2f(x, y);
               }
            glEnd();
            
            glBegin(GL_LINES);
               glVertex2f(tempX, tempY);
               glVertex2f(tempX + radius1 * cos(startAngle), tempY + radius1 * sin(startAngle));
               
               glVertex2f(tempX, tempY);
               glVertex2f(tempX + radius1 * cos(endAngle), tempY + radius1 * sin(endAngle));
            glEnd();
         }
      }
   }
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0); 

   drawPointSelectionBox();
   drawLineSelectionBox();
   drawRectangleSelectionBox();
   drawSectorSelectionBox();
   drawTextSelectionBox();
   drawInactiveArea();

   drawPoints();
   drawLines();
   drawRectangles();
   drawSectors();
   drawTexts();

   drawPreview();
   
   if ((primitive == LINE || primitive == RECTANGLE) && pointCount == 1)
      drawTempPoint();
   
   if (primitive == SECTOR)
   {
      if (pointCount >= 1) drawTempPoint();
      if (pointCount >= 2) drawTemp2Point();
   }
   
   if (primitive == TEXT && pointCount == 1)
   {
      // Display current text input at mouse position
      glColor3f(currentRed, currentGreen, currentBlue);
      glRasterPos2f(tempX, tempY);
      
      void* font = GLUT_BITMAP_HELVETICA_12;
      for (unsigned int i = 0; i < strlen(inputText); i++)
      {
         glutBitmapCharacter(font, inputText[i]);
      }
      
      // Display cursor
      if ((glutGet(GLUT_ELAPSED_TIME) / 500) % 2) // Blink cursor every 500ms
      {
         glutBitmapCharacter(font, '_');
      }
   }
   
   if (isGrid) drawGrid();

   glutSwapBuffers();
}

// Function to pick primitive if click is in left selection area.
void pickPrimitive(int y)
{
   if (y < (1 - NUMBERPRIMITIVES*0.1)*height) primitive = INACTIVE;
   else if (y < (1 - 4*0.1)*height) primitive = TEXT;
   else if (y < (1 - 3*0.1)*height) primitive = SECTOR;
   else if (y < (1 - 2*0.1)*height) primitive = RECTANGLE;
   else if (y < (1 - 1*0.1)*height) primitive = LINE;
   else primitive = POINT;
   
   pointCount = 0;
   inputText[0] = '\0'; // Clear text input
}

// The mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
   {
      y = height - y; // Correct from mouse to OpenGL co-ordinates.

      // Click outside canvas - do nothing.
      if (x < 0 || x > width || y < 0 || y > height) ;

      // Click in left selection area.
      else if (x < 0.1*width) 
      {
         pickPrimitive(y);
         currentX = currentY = 0; // Resetar pré-visualizaçã
      }

      // Click in canvas.
      else
      {
         if (primitive == POINT) {
            points.push_back(Point(x, y, currentRed, currentGreen, currentBlue));
         }
         else if (primitive == LINE) { 
            if (pointCount == 0) {
               tempX = x; tempY = y;
               pointCount++;
            }
            else {
               lines.push_back(Line(tempX, tempY, x, y, 
                                   currentRed, currentGreen, currentBlue, isStippled));
               pointCount = 0;
            }
         }
         else if (primitive == RECTANGLE) { 
            if (pointCount == 0) {
               tempX = x; tempY = y;
               pointCount++;
            }
            else {
               rectangles.push_back(Rectangle(tempX, tempY, x, y, 
                                            currentRed, currentGreen, currentBlue, isFilled));
               pointCount = 0;
            }
         }
         else if (primitive == SECTOR) {
            if (pointCount == 0) {
               tempX = x; tempY = y; // Center
               pointCount++;
            }
            else if (pointCount == 1) {
               temp2X = x; temp2Y = y; // First point
               pointCount++;
            }
            else {
               sectors.push_back(Sector(tempX, tempY, temp2X, temp2Y, x, y, 
                                      currentRed, currentGreen, currentBlue, isFilled));
               pointCount = 0;
            }
         }
         else if (primitive == TEXT) {
            if (pointCount == 0) {
               tempX = x; tempY = y;
               pointCount++;
            }
         }
      }
   }
   glutPostRedisplay();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
   {
      case 27: // Escape key
         exit(0);
         break;
      default:
         if (primitive == TEXT && pointCount == 1)
         {
            processTextInput(key);
         }
         break;
   }
   glutPostRedisplay();
}

// Clear the canvas and reset for fresh drawing.
void clearAll(void)
{
   points.clear();
   lines.clear();
   rectangles.clear();
   sectors.clear();
   texts.clear();
   primitive = INACTIVE;
   pointCount = 0;
   inputText[0] = '\0';
}

// Color submenu callback
void color_menu(int id)
{
   switch(id)
   {
      case 5: // Black
         currentRed = 0.0; currentGreen = 0.0; currentBlue = 0.0;
         break;
      case 6: // Red
         currentRed = 1.0; currentGreen = 0.0; currentBlue = 0.0;
         break;
      case 7: // Green
         currentRed = 0.0; currentGreen = 1.0; currentBlue = 0.0;
         break;
      case 8: // Blue
         currentRed = 0.0; currentGreen = 0.0; currentBlue = 1.0;
         break;
      case 9: // Yellow
         currentRed = 1.0; currentGreen = 1.0; currentBlue = 0.0;
         break;
      case 10: // Purple
         currentRed = 1.0; currentGreen = 0.0; currentBlue = 1.0;
         break;
   }
   glutPostRedisplay();
}

// Fill submenu callback
void fill_menu(int id)
{
   switch(id)
   {
      case 11: // Filled
         isFilled = 1;
         break;
      case 12: // Outline
         isFilled = 0;
         break;
   }
   glutPostRedisplay();
}

// Line style submenu callback
void line_menu(int id)
{
   switch(id)
   {
      case 13: // Solid
         isStippled = 0;
         break;
      case 14: // Dashed
         isStippled = 1;
         break;
   }
   glutPostRedisplay();
}

// Grid size submenu callback
void grid_size_menu(int id)
{
   switch(id)
   {
      case 15: // 5 divisions
         gridSize = 5;
         break;
      case 16: // 10 divisions
         gridSize = 10;
         break;
      case 17: // 20 divisions
         gridSize = 20;
         break;
      case 18: // 50 divisions
         gridSize = 50;
         break;
   }
   glutPostRedisplay();
}

// Text size submenu callback
void text_size_menu(int id)
{
   switch(id)
   {
      case 19: // Small
         textSize = 10;
         break;
         case 20: // Medium
         textSize = 12;
         break;
      case 21: // Large
         textSize = 18;
         break;
   }
   glutPostRedisplay();
}

// Grid toggle callback
void grid_toggle(int id)
{
   switch(id)
   {
      case 22: // Show grid
         isGrid = 1;
         break;
      case 23: // Hide grid
         isGrid = 0;
         break;
   }
   glutPostRedisplay();
}

// Point size submenu callback
void point_size_menu(int id)
{
   switch(id)
   {
      case 24: // Small
         pointSize = 3.0;
         Point::size = pointSize;
         break;
      case 25: // Medium
         pointSize = 6.0;
         Point::size = pointSize;
         break;
      case 26: // Large
         pointSize = 9.0;
         Point::size = pointSize;
         break;
   }
   glutPostRedisplay();
}

// Main menu callback
void main_menu(int id)
{
   switch(id)
   {
      case 1: // Clear canvas
         clearAll();
         break;
      case 2: // Exit
         exit(0);
         break;
   }
   glutPostRedisplay();
}

// Routine to create menu.
void makeMenu(void)
{
   // Create submenus
   int colorSubMenu = glutCreateMenu(color_menu);
   glutAddMenuEntry("Black", 5);
   glutAddMenuEntry("Red", 6);
   glutAddMenuEntry("Green", 7);
   glutAddMenuEntry("Blue", 8);
   glutAddMenuEntry("Yellow", 9);
   glutAddMenuEntry("Purple", 10);
   
   int fillSubMenu = glutCreateMenu(fill_menu);
   glutAddMenuEntry("Filled", 11);
   glutAddMenuEntry("Outline", 12);
   
   int lineSubMenu = glutCreateMenu(line_menu);
   glutAddMenuEntry("Solid", 13);
   glutAddMenuEntry("Dashed", 14);
   
   int gridSizeSubMenu = glutCreateMenu(grid_size_menu);
   glutAddMenuEntry("5 divisions", 15);
   glutAddMenuEntry("10 divisions", 16);
   glutAddMenuEntry("20 divisions", 17);
   glutAddMenuEntry("50 divisions", 18);
   
   int gridToggleSubMenu = glutCreateMenu(grid_toggle);
   glutAddMenuEntry("Show grid", 22);
   glutAddMenuEntry("Hide grid", 23);
   
   int textSizeSubMenu = glutCreateMenu(text_size_menu);
   glutAddMenuEntry("Small (10pt)", 19);
   glutAddMenuEntry("Medium (12pt)", 20);
   glutAddMenuEntry("Large (18pt)", 21);
   
   int pointSizeSubMenu = glutCreateMenu(point_size_menu);
   glutAddMenuEntry("Small (3px)", 24);
   glutAddMenuEntry("Medium (6px)", 25);
   glutAddMenuEntry("Large (9px)", 26);
   
   // Create main menu
   glutCreateMenu(main_menu);
   glutAddSubMenu("Color", colorSubMenu);
   glutAddSubMenu("Fill Style", fillSubMenu);
   glutAddSubMenu("Line Style", lineSubMenu);
   glutAddSubMenu("Grid Size", gridSizeSubMenu);
   glutAddSubMenu("Grid Toggle", gridToggleSubMenu);
   glutAddSubMenu("Text Size", textSizeSubMenu);
   glutAddSubMenu("Point Size", pointSizeSubMenu);
   glutAddMenuEntry("Clear Canvas", 1);
   glutAddMenuEntry("Exit", 2);
   
   glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   makeMenu(); // Create menu.
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // Set viewing box dimensions equal to window dimensions.
   glOrtho(0.0, w, 0.0, h, -1.0, 1.0);

   // Pass the size of the OpenGL window to globals.
   width = w; 
   height = h; 

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine for special keys.
void specialKeyInput(int key, int x, int y)
{
   if (primitive == TEXT && pointCount == 1)
   {
      if (key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT || 
          key == GLUT_KEY_UP || key == GLUT_KEY_DOWN)
      {
         // Move cursor position
         int moveAmount = 5; // pixels to move
         if (key == GLUT_KEY_LEFT) tempX -= moveAmount;
         if (key == GLUT_KEY_RIGHT) tempX += moveAmount;
         if (key == GLUT_KEY_UP) tempY += moveAmount;
         if (key == GLUT_KEY_DOWN) tempY -= moveAmount;
         
         // Keep within window bounds
         tempX = max(tempX, (int)(0.1 * width));
         tempX = min(tempX, width);
         tempY = max(tempY, 0);
         tempY = min(tempY, height);
      }
   }
   glutPostRedisplay();
}


// Função de movimento do mouse
void mouseMotion(int x, int y)
{
   y = height - y; // Ajustar coordenada Y
   
   // Só processar se estiver na área de desenho e com uma primitiva ativa
   if (x > 0.1*width && primitive != INACTIVE && primitive != POINT && primitive != TEXT)
   {
      currentX = x;
      currentY = y;
      glutPostRedisplay(); // Forçar redesenho
   }
}

// Main routine.
int main(int argc, char **argv) 
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
   glutInitWindowSize(800, 600);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("Enhanced Canvas");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutMouseFunc(mouseControl);
   glutMotionFunc(mouseMotion);
   glutPassiveMotionFunc(mouseMotion);

   glutMainLoop(); 

   return 0;  
}