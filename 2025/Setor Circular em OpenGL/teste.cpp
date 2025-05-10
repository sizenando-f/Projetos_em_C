////////////////////////////////////////////////////////////////////////////////////        
// canvas.cpp
//
// This program allows the user to draw simple shapes on a canvas.
//
// Interaction:
// Left click on a box on the left to select a primitive.
// Then left click on the drawing area: once for point, twice for line or rectangle.
// Right click for menu options.
//
//  Sumanta Guha.
//////////////////////////////////////////////////////////////////////////////////// 

#include <cstdlib>
#include <vector>
#include <iostream>
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
#define SECTOR 19  // Adiciona mais uma primitiva
#define NUMBERPRIMITIVES 4
#define PI 3.14159265358979323846

// Use the STL extension of C++.
using namespace std;

// Globals.
static GLsizei width, height; // OpenGL window size.
static float pointSize = 3.0; // Size of point
static int primitive = INACTIVE; // Current drawing primitive.
static int pointCount = 0; // Number of  specified points.
static int tempX, tempY; // Co-ordinates of clicked point.
static int isGrid = 1; // Is there grid?
static int temp2X, temp2Y; // São coordenadas adicionais para o setor
static float currentRed = 0.0, currentGreen = 0.0, currentBlue = 0.0; // Variáveis que vão ser usadas para armazenar a cor atual
static int isFilled = 0; // O estado de preenchimento dos setores, 0 = não preenchido, 1 = preenchido.

// Point class.
class Point
{
public:
   Point(int xVal, int yVal)
   {
	  x = xVal; y = yVal;
   }
   void drawPoint(void); // Function to draw a point.
private:
   int x, y; // x and y co-ordinates of point.
   static float size; // Size of point.
};

float Point::size = pointSize; // Set point size.

// Function to draw a point.
void Point::drawPoint()
{  
   glPointSize(size);
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
   Line(int x1Val, int y1Val, int x2Val, int y2Val)
   {
	  x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
   }
   void drawLine();
private:
   int x1, y1, x2, y2; // x and y co-ordinates of endpoints.
};


// Function to draw a line.
void Line::drawLine()
{
   glBegin(GL_LINES);
      glVertex3f(x1, y1, 0.0);
      glVertex3f(x2, y2, 0.0);
   glEnd();
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
   Rectangle(int x1Val, int y1Val, int x2Val, int y2Val)
   {
	  x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
   }
   void drawRectangle();
private:
   int x1, y1, x2, y2; // x and y co-ordinates of diagonally opposite vertices.
};

// Function to draw a rectangle.
void Rectangle::drawRectangle()
{
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(x1, y1, x2, y2);
}

// Vector of rectangles.
vector<Rectangle> rectangles;

// Iterator to traverse a Rectangle array.
vector<Rectangle>::iterator rectanglesIterator;

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

// Função para desenhar o setor
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

// Vetor de setores
vector<Sector> sectors;

// Iterador para percorrer o vetor
vector<Sector>::iterator sectorsIterator;

// Função para desenhar todos os setores no vetor de setores
void drawSectors(void)
{

   sectorsIterator = sectors.begin();
   while(sectorsIterator != sectors.end())
   {
      sectorsIterator->drawSector();
      sectorsIterator++;
   }
}

// Função para desenhar a opção de setor no menu esquerdo
void drawSectorSelectionBox(void)
{  
   if (primitive == SECTOR) glColor3f(1.0, 1.0, 1.0);
   else glColor3f(0.8, 0.8, 0.8);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

   glColor3f(0.0, 0.0, 0.0);
   
   float centerX = 0.05 * width;
   float centerY = 0.65 * height;
   float radius = 0.025 * width;
   
   glBegin(GL_LINE_LOOP);
      glVertex2f(centerX, centerY);
      
      for (int i = 0; i <= 8; i++)
      {
         float angle = -0.4 * PI + i * (0.8 * PI) / 8;
         float x = centerX + radius * cos(angle);
         float y = centerY + radius * sin(angle);
         glVertex2f(x, y);
      }
   glEnd();
}

// Função para desenhar o segundo ponto secundário (para o setor)
void drawTemp2Point(void)
{
   glColor3f(0.0, 1.0, 0.0);
   glPointSize(pointSize);
   glBegin(GL_POINTS);
      glVertex3f(temp2X, temp2Y, 0.0);
   glEnd(); 
}

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

// Function to draw a grid.
void drawGrid(void)
{
   int i;
   
   glEnable(GL_LINE_STIPPLE);
   glLineStipple(1, 0x5555); 
   glColor3f(0.75, 0.75, 0.75);

   glBegin(GL_LINES);
	  for (i=2; i <=9; i++)
	  {
         glVertex3f(i*0.1*width, 0.0, 0.0);
         glVertex3f(i*0.1*width, height, 0.0);
	  }
	  for (i=1; i <=9; i++)
	  {
         glVertex3f(0.1*width, i*0.1*height, 0.0);
         glVertex3f(width, i*0.1*height, 0.0);
	  }
   glEnd();
   glDisable(GL_LINE_STIPPLE);
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
   drawInactiveArea();

   drawPoints();
   drawLines();
   drawRectangles();
   if ( ((primitive == LINE) || (primitive == RECTANGLE)) &&
	   (pointCount == 1) ) drawTempPoint();
   if (isGrid) drawGrid();


   drawSectors();
   
   if (primitive == SECTOR)
   {
      if (pointCount >= 1) drawTempPoint();
      if (pointCount >= 2) drawTemp2Point();
   }

   glutSwapBuffers();
}

// Function to pick primitive if click is in left selection area.
void pickPrimitive(int y)
{
   if ( y < (1- NUMBERPRIMITIVES*0.1)*height ) primitive = INACTIVE;
   else if (y < (1 - 3*0.1)*height) primitive = SECTOR;
   else if ( y < (1 - 2*0.1)*height ) primitive = RECTANGLE;
   else if ( y < (1 - 1*0.1)*height ) primitive = LINE;
   else primitive = POINT;
}

// The mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
   {
      y = height - y; // Correct from mouse to OpenGL co-ordinates.

	  // Click outside canvas - do nothing.
      if ( x < 0 || x > width || y < 0 || y > height ) ;

	  // Click in left selection area.
      else if ( x < 0.1*width ) 
	  {
	     pickPrimitive(y);
		 pointCount = 0;
	  }

	  // Click in canvas.
      else
	  {
	     if (primitive == POINT) points.push_back( Point(x,y) );
         else if (primitive == LINE) 
		 { 
	        if (pointCount == 0)
			{
               tempX = x; tempY = y;
		       pointCount++;
			}
	        else 
			{
               lines.push_back( Line(tempX, tempY, x, y) );
		       pointCount = 0;
			}
		 }
         else if (primitive == RECTANGLE) 
		 { 
	        if (pointCount == 0)
			{
               tempX = x; tempY = y;
		       pointCount++;
			}
	        else 
			{
               rectangles.push_back( Rectangle(tempX, tempY, x, y) );
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
	  }
   }
   glutPostRedisplay();
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // Set viewing box dimensions equal to window dimensions.
   glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);
   
   // Pass the size of the OpenGL window to globals.
   width = w; 
   height = h;  

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
   {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Clear the canvas and reset for fresh drawing.
void clearAll(void)
{
   points.clear();
   lines.clear();
   rectangles.clear();
   sectors.clear();
   primitive = INACTIVE;
   pointCount = 0;
}

// The right button menu callback function.
void rightMenu(int id)
{
   if (id==1) 
   {
	  clearAll();
	  glutPostRedisplay();
   }
   if (id==2) exit(0);
}

// The sub-menu callback function.
void grid_menu(int id)
{
   if (id==3) isGrid = 1;
   if (id==4) isGrid = 0;
   glutPostRedisplay();
}

// Function to create menu.
void makeMenu(void)
{
   int sub_menu;
   sub_menu = glutCreateMenu(grid_menu);
   glutAddMenuEntry("On", 3);
   glutAddMenuEntry("Off",4);

   glutCreateMenu(rightMenu);
   glutAddSubMenu("Grid", sub_menu);
   glutAddMenuEntry("Clear",1);
   glutAddMenuEntry("Quit",2);
   glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Left click on a box on the left to select a primitive." << endl
        << "Then left click on the drawing area: once for point, twice for line or rectangle." << endl
        << "Right click for menu options." <<  endl; 
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_DOUBLE); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("canvas.cpp");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutMouseFunc(mouseControl); 

   makeMenu(); // Create menu.

   glutMainLoop(); 

   return 0;  
}