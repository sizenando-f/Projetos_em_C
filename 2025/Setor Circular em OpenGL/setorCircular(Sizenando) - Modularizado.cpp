// Módulo de Desenhar Setor na Aplicação do Canvas (questão 18)- Sizenando
// Todos os pontos de referência usados pra ajudar na implementação foram baseados no código original que ela passou. Vou passar pra vocês apenas a parte única que no meu caso é o desenho do setor, o resto que é do 21 ao 26 e da implementação do monitoramento do mouse todo mundo fez então não compensa enviar para evitar redundância e facilitar o trampo de vocês, ela falou pra enviar exatamente tudo mas creio que seja exagero.
// Ignore os erros do arquivo pq tá sem as biblioteca geneŕicas, tá em CPP pra ajudar a visualização.


// Biblioteca para adicionar
#include <cmath>

// Defines necessários
#define PI 3.14159265358979323846
#define SECTOR 19  // Adiciona a primitiva, (N questão (18) + 1)
// Não esquecer de adicionar +1 no #define NUMBERPRIMITIVES

// Na parte de variáveis globais, adiciona:
static int temp2X, temp2Y; // São coordenadas adicionais para o setor
static float currentRed = 0.0, currentGreen = 0.0, currentBlue = 0.0; // Variáveis que vão ser usadas para armazenar a cor atual
static int isFilled = 0; // O estado de preenchimento dos setores, 0 = não preenchido, 1 = preenchido.

// Classe do setor (para setores circulares)
class Sector
{
public:
   Sector(int cxVal, int cyVal, int p1xVal, int p1yVal, int p2xVal, int p2yVal, 
          float r = 0.0, float g = 0.0, float b = 0.0, int fill = 0)
   {
      cx = cxVal; cy = cyVal;
      p1x = p1xVal; p1y = p1yVal; 
      p2x = p2xVal; p2y = p2yVal; 
      red = r; green = g; blue = b;
      filled = fill;
      
      radius = sqrt(pow(p1x - cx, 2) + pow(p1y - cy, 2));
      
      startAngle = atan2(p1y - cy, p1x - cx);
      endAngle = atan2(p2y - cy, p2x - cx);
      
      if (endAngle < startAngle) endAngle += 2 * PI;
   }
   void drawSector();
private:
   int cx, cy; 
   int p1x, p1y, p2x, p2y; 
   float radius; 
   float startAngle, endAngle; 
   float red, green, blue; 
   int filled; 
};

// Função para desenhar o setor
void Sector::drawSector()
{
   glColor3f(red, green, blue);
   
   if (filled)
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   else
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   
   const int numSegments = 100;
   float angleIncrement = (endAngle - startAngle) / numSegments;
   
   if (filled)
   {
      glBegin(GL_TRIANGLE_FAN);
      
      glVertex2f(cx, cy);
      
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
      glBegin(GL_LINE_STRIP);
      for (int i = 0; i <= numSegments; i++)
      {
         float angle = startAngle + i * angleIncrement;
         float x = cx + radius * cos(angle);
         float y = cy + radius * sin(angle);
         glVertex2f(x, y);
      }
      glEnd();
      
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

// A partir daqui são implementações dentro das funções já existentes

// Adicione na função drawScene antes da chamada "drawInactiveArea();"
/*
    drawSectorSelectionBox();
*/

// Adicione isso na função drawScene antes da chamada "glutSwapBuffers();":
/*
   drawSectors();
   
   if (primitive == SECTOR)
   {
      if (pointCount >= 1) drawTempPoint();
      if (pointCount >= 2) drawTemp2Point();
   }
*/

// Adicione isso na função pickPrimitive acima de "else if ( y < (1 - 2*0.1)*height ) primitive = RECTANGLE;":
/*
   else if (y < (1 - 3*0.1)*height) primitive = SECTOR;
*/

// Atualiza a função mouseControl adicionando isso após o último 'else if' que está dentro do 'else' com o comentário "Click in canvas":
/*
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
*/

// Adiciona isso na função clearAll:
/*
   sectors.clear();
*/
