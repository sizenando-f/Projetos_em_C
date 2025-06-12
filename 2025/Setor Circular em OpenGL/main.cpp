/**
 * @file main.cpp
 * @brief Desenho de figuras geométricas com OpenGL (Com setores circulares)
 * @author Sizenando S. França - 50575
 * @date 10-05-2025
*/

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

using namespace std;

// Variáveis globais
static GLsizei width, height; // Tamanho da janela do OpenGL
static float pointSize = 3.0; // Tamanho do ponto
static int primitive = INACTIVE; // Primitiva selecionada
static int pointCount = 0; // Número de pontos especifificados
static int tempX, tempY; // Coordenadas do ponto clicado
static int temp2X, temp2Y; // Coordenadas adicionais para o setor
static int isGrid = 1; // Exibir grade?
static int gridSize = 10; //Tamanho do grid (10 divisões por padrão)
static float currentRed = 0.0, currentGreen = 0.0, currentBlue = 0.0; // Cor de desenho atual
static int isFilled = 0; // A primitiva atual é preenchida?
static int isStippled = 0; // A linha atual é pontilhada?
static char inputText[100] = ""; // Texto para ser desenhado
static int textSize = 12; // Tamanho do texto
static int currentX = 0, currentY = 0; // Coordenadas do mouse


/**
 * @brief Classe que representa um ponto no espaço 2D.
 * 
 * Esta classe é usada para armazenar as coordenadas de um ponto, bem como sua cor.
 * Ela também possui um método para desenhar o ponto na tela.
*/
class Point
{
public:
   //xVal = coordenada x, yVal = coordenada y, r = vermelho, g = verde, b = azul
   Point(int xVal, int yVal, float r = 0.0, float g = 0.0, float b = 0.0)
   {
      x = xVal; y = yVal;
      red = r; green = g; blue = b;
   }
   void drawPoint(void); // Função para desenhar o ponto
public:
   static void setSize(float newSize) { size = newSize; } // Setter para o tamanho do ponto
   static float size; // Tamanho do ponto

private:
   int x, y; // Coordenadas do ponto
   float red, green, blue; // Cor do ponto

};

float Point::size = pointSize; // Seta o tamanho do ponto

/**
 * @brief Desenha o ponto na tela.
*/
void Point::drawPoint()
{  
   glPointSize(size);
   glColor3f(red, green, blue);
   glBegin(GL_POINTS);
      glVertex3f(x, y, 0.0);
   glEnd();   
}

// Vetor para os pontos
vector<Point> points;

// Iterador para percorrer o vetor de pontos
vector<Point>::iterator pointsIterator; 

/**
 * @brief Desenha todos os pontos armazenados no vetor de pontos.
 * 
 * Esta função itera sobre o vetor de pontos e chama o método drawPoint() de cada ponto.
*/
void drawPoints(void)
{
   // Loop através de cada ponto do vetor e desenha o ponto.
   pointsIterator = points.begin();
   while(pointsIterator != points.end() )
   {
      pointsIterator->drawPoint();
      pointsIterator++;
   }
}

/**
 * @brief Classe que representa uma linha no espaço 2D.
*/
class Line
{
public:
   // x1Val = coordenada x1, y1Val = coordenada y1, x2Val = coordenada x2, y2Val = coordenada y2, r = vermelho, g = verde, b = azul, stip = pontilhada?
   Line(int x1Val, int y1Val, int x2Val, int y2Val, 
        float r = 0.0, float g = 0.0, float b = 0.0, int stip = 0)
   {
      x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
      red = r; green = g; blue = b;
      stippled = stip;
   }
   void drawLine();
private:
   int x1, y1, x2, y2; // Coordenadas começo e fim da linha
   float red, green, blue; // Cor da linha
   int stippled; // A linha é pontilhada?
};

/**
 * @brief Desenha a linha na tela.
*/
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

// Vetor de linhas
vector<Line> lines;

// Iterador para percorrer o vetor de linhas
vector<Line>::iterator linesIterator;

/**
 * @brief Desenha todas as linhas armazenadas no vetor de linhas.
*/
void drawLines(void)
{
   // Loop através de cada linha do vetor e desenha a linha.
   linesIterator = lines.begin();
   while(linesIterator != lines.end() )
   {
      linesIterator->drawLine();
      linesIterator++;
   }
}

/**
 * @brief Classe que representa um retângulo no espaço 2D.
 * 
 * Esta classe é usada para armazenar as coordenadas de um retângulo, bem como sua cor e se ele é preenchido ou não.
 * Ela também possui um método para desenhar o retângulo na tela.
*/
class Rectangle
{
public:
   // x1Val = coordenada x1, y1Val = coordenada y1, x2Val = coordenada x2, y2Val = coordenada y2, r = vermelho, g = verde, b = azul, fill = preenchido?
   Rectangle(int x1Val, int y1Val, int x2Val, int y2Val, 
             float r = 0.0, float g = 0.0, float b = 0.0, int fill = 0)
   {
      x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
      red = r; green = g; blue = b;
      filled = fill;
   }
   void drawRectangle();
private:
   int x1, y1, x2, y2; // Coordenadas x e y diagonalmente opostas ao vértice
   float red, green, blue; // Cor do retângulo
   int filled; // O retângulo é preenchido?
};

/**
 * @brief Desenha o retângulo na tela.
*/
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

// Vetor de retângulos
vector<Rectangle> rectangles;

// Iterador para percorrer o vetor de retângulos
vector<Rectangle>::iterator rectanglesIterator;

/**
 * @brief Desenha todos os retângulos armazenados no vetor de retângulos.
*/
void drawRectangles(void)
{
   // Loop através de cada retângulo do vetor e desenha o retângulo.
   rectanglesIterator = rectangles.begin();
   while(rectanglesIterator != rectangles.end() )
   {
      rectanglesIterator->drawRectangle();
      rectanglesIterator++;
   }
}

/**
 * @brief Classe que representa um setor circular no espaço 2D.
 * 
 * Esta classe é usada para armazenar as coordenadas do centro, os pontos do arco, o raio, os ângulos de início e fim, a cor e se o setor é preenchido ou não.
 * Ela também possui um método para desenhar o setor na tela.
*/
class Sector
{
public:
   // cxVal = coordenada x do centro, cyVal = coordenada y do centro, p1xVal = coordenada x do primeiro ponto, p1yVal = coordenada y do primeiro ponto, p2xVal = coordenada x do segundo ponto, p2yVal = coordenada y do segundo ponto, r = vermelho, g = verde, b = azul, fill = preenchido?
   Sector(int cxVal, int cyVal, int p1xVal, int p1yVal, int p2xVal, int p2yVal, 
          float r = 0.0, float g = 0.0, float b = 0.0, int fill = 0)
   {
      cx = cxVal; cy = cyVal; // Centro
      p1x = p1xVal; p1y = p1yVal; // Primeiro ponto no arco
      p2x = p2xVal; p2y = p2yVal; // Segundo ponto no arco
      red = r; green = g; blue = b; // Cor
      filled = fill; // Preenchido?
      
      // Calcula o raio como a distância do centro ao primeiro ponto
      radius = sqrt(pow(p1x - cx, 2) + pow(p1y - cy, 2));
      
      // Calcula os ângulos de início e fim
      startAngle = atan2(p1y - cy, p1x - cx);
      endAngle = atan2(p2y - cy, p2x - cx);
      
      // Garante a direção correta do arco
      if (endAngle < startAngle) endAngle += 2 * PI;
   }
   void drawSector();
private:
   int cx, cy; // Centro das coordenadas
   int p1x, p1y, p2x, p2y; // Pontos no arco
   float radius; // Raio
   float startAngle, endAngle; // Angulos de início e fim
   float red, green, blue; // Cor
   int filled; // É preenchido?
};

/**
 * @brief Desenha o setor circular na tela.
 * 
 * Esta função usa a biblioteca OpenGL para desenhar o setor circular com base nas coordenadas, ângulos e cor especificados.
*/
void Sector::drawSector()
{
   glColor3f(red, green, blue);
   
   if (filled)
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   else
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   
   // Desenha o setor
   const int numSegments = 100;
   float angleIncrement = (endAngle - startAngle) / numSegments;
   
   if (filled)
   {
      glBegin(GL_TRIANGLE_FAN);
      // Adiciona o ponto central
      glVertex2f(cx, cy);
      
      // Adiciona os pontos ao longo do arco
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
      // Desenha o arco
      glBegin(GL_LINE_STRIP);
      for (int i = 0; i <= numSegments; i++)
      {
         float angle = startAngle + i * angleIncrement;
         float x = cx + radius * cos(angle);
         float y = cy + radius * sin(angle);
         glVertex2f(x, y);
      }
      glEnd();
      
      // Desenha os raios
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

// Iterador para percorrer o vetor de setores
vector<Sector>::iterator sectorsIterator;

/**
 * @brief Desenha todos os setores armazenados no vetor de setores.
*/
void drawSectors(void)
{
   // Loop através de cada setor do vetor e desenha o setor.
   sectorsIterator = sectors.begin();
   while(sectorsIterator != sectors.end())
   {
      sectorsIterator->drawSector();
      sectorsIterator++;
   }
}

/**
 * @brief Classe que representa um texto na tela.
*/
class Text
{
public:
   // xVal = coordenada x, yVal = coordenada y, str = string do texto, r = vermelho, g = verde, b = azul, s = tamanho do texto
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
   int x, y; // Posição do texto
   string text; // Texto
   float red, green, blue; // Cor
   int size; // Tamanho do texto
};

/**
 * @brief Desenha o texto na tela.
 * 
 * Esta função usa a biblioteca OpenGL para desenhar o texto com base nas coordenadas, cor e tamanho especificados.
*/
void Text::drawText()
{
   glColor3f(red, green, blue);
   glRasterPos2f(x, y);
   
   // Escolhe o tamanho da fonte
   void* font;
   if (size <= 10)
      font = GLUT_BITMAP_HELVETICA_10;
   else if (size <= 12)
      font = GLUT_BITMAP_HELVETICA_12;
   else
      font = GLUT_BITMAP_HELVETICA_18;
   
   // Desenha cada caractere do texto
   for (unsigned int i = 0; i < text.length(); i++)
   {
      glutBitmapCharacter(font, text[i]);
   }
}

//Vetor de objetos de texto
vector<Text> texts;

// Iterador para percorrer o vetor de textos
vector<Text>::iterator textsIterator;

/**
 * @brief Desenha todos os textos armazenados no vetor de textos.
*/
void drawTexts(void)
{
   textsIterator = texts.begin();
   while(textsIterator != texts.end())
   {
      textsIterator->drawText();
      textsIterator++;
   }
}

/**
 * @brief Desenha a caixa de seleção de ponto na área de seleção à esquerda.
*/
void drawPointSelectionBox(void)
{  
   if (primitive == POINT) glColor3f(1.0, 1.0, 1.0); // Destaque.
   else glColor3f(0.8, 0.8, 0.8); // Sem destaque.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.9*height, 0.1*width, height);

   // Desenha a borda preta.
   glColor3f(0.0, 0.0, 0.0); 
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.9*height, 0.1*width, height); 

   // Desenha o ícone do ponto.
   glPointSize(pointSize);
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_POINTS);
      glVertex3f(0.05*width, 0.95*height, 0.0);
   glEnd();  
}

/**
 * @brief Desenha a caixa de seleção de linha na área de seleção à esquerda.
*/
void drawLineSelectionBox(void)
{  
   if (primitive == LINE) glColor3f(1.0, 1.0, 1.0); // Destaque.
   else glColor3f(0.8, 0.8, 0.8); // Sem destaque.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

   // Desenha a borda preta.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

   // Desenha o ícone da linha.
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_LINES);
      glVertex3f(0.025*width, 0.825*height, 0.0);
      glVertex3f(0.075*width, 0.875*height, 0.0);
   glEnd();  
}

/**
 * @brief Desenha a caixa de seleção de retângulo na área de seleção à esquerda.
*/
void drawRectangleSelectionBox(void)
{  
   if (primitive == RECTANGLE) glColor3f(1.0, 1.0, 1.0); // Destaque.
   else glColor3f(0.8, 0.8, 0.8); // Sem destaque.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

   // Desenha a borda preta.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

   // Desenha o ícone do retângulo.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.025*width, 0.735*height, 0.075*width, 0.765*height);
   glEnd();  
}

/**
 * @brief Desenha a caixa de seleção de setor na área de seleção à esquerda.
*/
void drawSectorSelectionBox(void)
{  
   if (primitive == SECTOR) glColor3f(1.0, 1.0, 1.0); // Destaque
   else glColor3f(0.8, 0.8, 0.8); // Sem destaque
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

   // Desenha a borda preta.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

   // Desenha o ícone do setor.
   glColor3f(0.0, 0.0, 0.0);
   
   // Desenha um formato meio triangular
   float centerX = 0.05 * width;
   float centerY = 0.65 * height;
   float radius = 0.025 * width;
   
   glBegin(GL_LINE_LOOP);
      // Ponto central
      glVertex2f(centerX, centerY);
      
      // Pontos do arco (cerca de 120 graus)
      for (int i = 0; i <= 8; i++)
      {
         float angle = -0.4 * PI + i * (0.8 * PI) / 8;
         float x = centerX + radius * cos(angle);
         float y = centerY + radius * sin(angle);
         glVertex2f(x, y);
      }
   glEnd();
}

/**
 * @brief Desenha a caixa de seleção de texto na área de seleção à esquerda.
*/
void drawTextSelectionBox(void)
{  
   if (primitive == TEXT) glColor3f(1.0, 1.0, 1.0); // Destaque.
   else glColor3f(0.8, 0.8, 0.8); // Sem destaque.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.5*height, 0.1*width, 0.6*height);

   // Desenha a borda preta.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.5*height, 0.1*width, 0.6*height);

   // Desenha o ícone (letra T)
   glColor3f(0.0, 0.0, 0.0);
   glLineWidth(2.0);
   
   // Desenha a letra T
   glBegin(GL_LINES);
      // Barra horizontal
      glVertex2f(0.03*width, 0.57*height);
      glVertex2f(0.07*width, 0.57*height);
      
      // Desenha a linha vertical
      glVertex2f(0.05*width, 0.57*height);
      glVertex2f(0.05*width, 0.53*height);
   glEnd();
   
   glLineWidth(1.0);
}

/**
 * @brief Desenha a área não utilizada da caixa de seleção à esquerda.
*/
void drawInactiveArea(void)
{
   glColor3f(0.6, 0.6, 0.6);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);

   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);
}

/**
 * @brief Desenha o ponto temporário na tela.
*/
void drawTempPoint(void)
{
   glColor3f(1.0, 0.0, 0.0);
   glPointSize(pointSize);
   glBegin(GL_POINTS);
      glVertex3f(tempX, tempY, 0.0);
   glEnd(); 
}

/**
 * @brief Desenha o segundo ponto temporário na tela. (Para o setor
*/
void drawTemp2Point(void)
{
   glColor3f(0.0, 1.0, 0.0);
   glPointSize(pointSize);
   glBegin(GL_POINTS);
      glVertex3f(temp2X, temp2Y, 0.0);
   glEnd(); 
}

/**
 * @brief Desenha a grade na tela.
*/
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

/**
 * @brief Processa a entrada de texto do usuário.
 * 
 * Esta função adiciona o caractere digitado ao buffer de texto, ou apaga o último caractere se backspace for pressionado.
 * Se enter for pressionado, o texto é adicionado ao canvas.
 * 
 * @param key O caractere digitado pelo usuário.
*/
void processTextInput(unsigned char key)
{
   // Se a tecla for backspace e houver texto para apagar
   if (key == 8 && strlen(inputText) > 0)
   {
      inputText[strlen(inputText) - 1] = '\0';
   }
   // Se a tecla for enter, adiciona o texto ao canvas
   else if (key == 13)
   {
      // Adiciona o texto ao canvas, vetor de textos
      texts.push_back(Text(tempX, tempY, inputText, 
                         currentRed, currentGreen, currentBlue, textSize));
      
      // Reseta o texto de entrada
      inputText[0] = '\0';
      primitive = INACTIVE;
      pointCount = 0;
   }
   // Entrada de caractere regular
   else if (key >= 32 && key <= 126) // Caracteres ASCII imprimíveis
   {
      int len = strlen(inputText);
      if (len < 99) // Previne o overflow do buffer
      {
         inputText[len] = key;
         inputText[len + 1] = '\0';
      }
   }
}

/**
 * @brief Função para limpar o anterior
*/
void clearPreview()
{
   currentX = currentY = 0;
   glutPostRedisplay();
}

/**
 * @brief Desenha a pré-visualização da primitiva selecionada. (Auxilar da drawScene)
 *
 * Esta função desenha uma linha, retângulo ou setor na tela com base nas coordenadas do mouse e no número de pontos especificados.
*/
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

/**
 * @brief Função de callback para desenhar a cena.
*/
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
      // Mostra o texto atual na posição do mouse
      glColor3f(currentRed, currentGreen, currentBlue);
      glRasterPos2f(tempX, tempY);
      
      void* font = GLUT_BITMAP_HELVETICA_12;
      for (unsigned int i = 0; i < strlen(inputText); i++)
      {
         glutBitmapCharacter(font, inputText[i]);
      }
      
      // Mostra o cursor piscando
      if ((glutGet(GLUT_ELAPSED_TIME) / 500) % 2) // Pisca o cursor a cada 500ms
      {
         glutBitmapCharacter(font, '_');
      }
   }
   
   if (isGrid) drawGrid();

   glutSwapBuffers();
}

/**
 * @brief Seleciona a primitiva com base na posição do mouse.
 *
 * Esta função verifica a posição do mouse e define a primitiva selecionada com base na área clicada.
 *
 * @param y A coordenada y do mouse.
*/
void pickPrimitive(int y)
{
   if (y < (1 - NUMBERPRIMITIVES*0.1)*height) primitive = INACTIVE;
   else if (y < (1 - 4*0.1)*height) primitive = TEXT;
   else if (y < (1 - 3*0.1)*height) primitive = SECTOR;
   else if (y < (1 - 2*0.1)*height) primitive = RECTANGLE;
   else if (y < (1 - 1*0.1)*height) primitive = LINE;
   else primitive = POINT;
   
   pointCount = 0;
   inputText[0] = '\0'; // Limpa o texto de entrada
}

/**
 * @brief Função de callback para o controle do mouse.
 *
 * Esta função processa os eventos de clique do mouse e atualiza a primitiva selecionada ou adiciona pontos, linhas, retângulos, setores ou textos ao canvas.
 *
 * @param button O botão do mouse que foi pressionado.
 * @param state O estado do botão (pressionado ou solto).
 * @param x A coordenada x do mouse.
 * @param y A coordenada y do mouse.
*/
void mouseControl(int button, int state, int x, int y)
{
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
   {
      y = height - y; // Corrige a coordenada y para o sistema de coordenadas do OpenGL

      // Clicar fora do canvas - não faz nada.
      if (x < 0 || x > width || y < 0 || y > height) ;

      // Clica na área de seeleção à esquerda
      else if (x < 0.1*width) 
      {
         pickPrimitive(y);
         currentX = currentY = 0; // Resetar pré-visualização
      }

      // Clica no canvas
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
               tempX = x; tempY = y; // Centro
               pointCount++;
            }
            else if (pointCount == 1) {
               temp2X = x; temp2Y = y; // Primeiro ponto
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

/**
 * @brief Função de callback para o teclado.
 *
 * Esta função processa a entrada do teclado e executa ações com base na tecla pressionada.
 *
 * @param key A tecla pressionada.
 * @param x A coordenada x do mouse.
 * @param y A coordenada y do mouse.
*/
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
   {
      case 27: // Tecla ESC
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

/**
 * @brief Limpa todos os objetos desenhados e reseta o estado do canvas.
*/
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

/**
 * @brief Função de callback do submenu de cores.
 *
 * Esta função altera a cor atual com base na opção selecionada no menu.
 *
 * @param id O ID da opção selecionada no menu.
*/
void color_menu(int id)
{
   switch(id)
   {
      case 5: // Preto
         currentRed = 0.0; currentGreen = 0.0; currentBlue = 0.0;
         break;
      case 6: // Vermelho
         currentRed = 1.0; currentGreen = 0.0; currentBlue = 0.0;
         break;
      case 7: // Verde
         currentRed = 0.0; currentGreen = 1.0; currentBlue = 0.0;
         break;
      case 8: // Azul
         currentRed = 0.0; currentGreen = 0.0; currentBlue = 1.0;
         break;
      case 9: // Amarelo
         currentRed = 1.0; currentGreen = 1.0; currentBlue = 0.0;
         break;
      case 10: // Roxo
         currentRed = 1.0; currentGreen = 0.0; currentBlue = 1.0;
         break;
   }
   glutPostRedisplay();
}

/**
 * @brief Função de callback do submenu de preenchimento.
 *
 * Esta função altera o estilo de preenchimento (preenchido ou contorno) com base na opção selecionada no menu.
 *
 * @param id O ID da opção selecionada no menu.
*/
void fill_menu(int id)
{
   switch(id)
   {
      case 11: // Preenchido
         isFilled = 1;
         break;
      case 12: // Contorno
         isFilled = 0;
         break;
   }
   glutPostRedisplay();
}

/**
 * @brief Função de callback do submenu de estilo de linha.
 *
 * Esta função altera o estilo da linha (sólido ou tracejado) com base na opção selecionada no menu.
 *
 * @param id O ID da opção selecionada no menu.
*/
void line_menu(int id)
{
   switch(id)
   {
      case 13: // Solido
         isStippled = 0;
         break;
      case 14: // Tracejado
         isStippled = 1;
         break;
   }
   glutPostRedisplay();
}

/**
 * @brief Função de callback do submenu de tamanho da grade.
 *
 * Esta função altera o tamanho da grade com base na opção selecionada no menu.
 *
 * @param id O ID da opção selecionada no menu.
*/
void grid_size_menu(int id)
{
   switch(id)
   {
      case 15: // 5 divisões
         gridSize = 5;
         break;
      case 16: // 10 divisões
         gridSize = 10;
         break;
      case 17: // 20 divisões
         gridSize = 20;
         break;
      case 18: // 50 divisões
         gridSize = 50;
         break;
   }
   glutPostRedisplay();
}

/**
 * @brief Função de callback do submenu de tamanho do texto.
 *
 * Esta função altera o tamanho do texto com base na opção selecionada no menu.
 *
 * @param id O ID da opção selecionada no menu.
*/
void text_size_menu(int id)
{
   switch(id)
   {
      case 19: // Pequeno
         textSize = 10;
         break;
         case 20: // Médio
         textSize = 12;
         break;
      case 21: // Grande
         textSize = 18;
         break;
   }
   glutPostRedisplay();
}

/**
 * @brief Função de callback do submenu de alternância da grade.
 *
 * Esta função alterna a exibição da grade com base na opção selecionada no menu.
 *
 * @param id O ID da opção selecionada no menu.
*/
void grid_toggle(int id)
{
   switch(id)
   {
      case 22: // Mostra o grid
         isGrid = 1;
         break;
      case 23: // Esconde o grid
         isGrid = 0;
         break;
   }
   glutPostRedisplay();
}

/**
 * @brief Função de callback do submenu de tamanho do ponto.
 *
 * Esta função altera o tamanho do ponto com base na opção selecionada no menu.
 *
 * @param id O ID da opção selecionada no menu.
*/
void point_size_menu(int id)
{
   switch(id)
   {
      case 24: // Pequeno
         pointSize = 3.0;
         Point::size = pointSize;
         break;
      case 25: // Médio
         pointSize = 6.0;
         Point::size = pointSize;
         break;
      case 26: // Grande
         pointSize = 9.0;
         Point::size = pointSize;
         break;
   }
   glutPostRedisplay();
}

/**
   * @brief Função de callback do menu principal.
   *
   * Esta função processa as opções selecionadas no menu principal.
   *
   * @param id O ID da opção selecionada no menu.
*/
void main_menu(int id)
{
   switch(id)
   {
      case 1: // Limpa o canvas
         clearAll();
         break;
      case 2: // Sair
         exit(0);
         break;
   }
   glutPostRedisplay();
}

/**
   * @brief Cria o menu de opções.
   *
   * Esta função cria os submenus e o menu principal, associando cada opção a uma função de callback.
*/
void makeMenu(void)
{
   // Cria os submenus
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
   
   // Cria o menu principal
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

/**
   * @brief Função de configuração inicial.
   *
   * Esta função configura a cor de fundo da tela e cria o menu de opções.
*/
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   makeMenu(); // Cria o menu
}

/**
   * @brief Função de redimensionamento da janela OpenGL.
   *
   * Esta função ajusta a caixa de visualização e as variáveis globais de largura e altura.
   *
   * @param w A nova largura da janela.
   * @param h A nova altura da janela.
*/
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // Seta a caixa de visualização para o tamanho da janela.
   glOrtho(0.0, w, 0.0, h, -1.0, 1.0);

   // Passa o tamanho da janela OpenGL para as variáveis globais.
   width = w; 
   height = h; 

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

/**
   * @brief Função de callback para teclas especiais.
   *
   * Esta função processa as teclas especiais pressionadas e ajusta a posição do cursor de texto.
   *
   * @param key A tecla especial pressionada.
   * @param x A coordenada x do mouse.
   * @param y A coordenada y do mouse.
*/
void specialKeyInput(int key, int x, int y)
{
   if (primitive == TEXT && pointCount == 1)
   {
      if (key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT || 
          key == GLUT_KEY_UP || key == GLUT_KEY_DOWN)
      {
         // Ajusta a posição do cursor
         int moveAmount = 5; // Pixels para mover
         if (key == GLUT_KEY_LEFT) tempX -= moveAmount;
         if (key == GLUT_KEY_RIGHT) tempX += moveAmount;
         if (key == GLUT_KEY_UP) tempY += moveAmount;
         if (key == GLUT_KEY_DOWN) tempY -= moveAmount;
         
         // Mantém dentro dos limites da janela
         tempX = max(tempX, (int)(0.1 * width));
         tempX = min(tempX, width);
         tempY = max(tempY, 0);
         tempY = min(tempY, height);
      }
   }
   glutPostRedisplay();
}


/**
   * @brief Função de callback para o movimento do mouse.
   *
   * Esta função processa o movimento do mouse e atualiza a posição atual do mouse.
   *
   * @param x A coordenada x do mouse.
   * @param y A coordenada y do mouse.
*/
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

/**
   * @brief Função principal do programa.
   *
   * Esta função inicializa o GLUT, cria a janela e define as funções de callback.
   *
   * @param argc Número de argumentos da linha de comando.
   * @param argv Argumentos da linha de comando.
   * @return 0 se o programa for executado com sucesso.
*/
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