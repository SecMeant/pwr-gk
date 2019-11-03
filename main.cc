#include <GL/gl.h>
#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <fmt/format.h>

/*************************************************************************************/

struct Color
{
  GLfloat r,g,b;
};

struct Square
{
  GLfloat midx, midy, rad;
  Color color;
};

constexpr Color bgcolor{1.0f, 1.0f, 1.0f};
constexpr Color red{1.0f, 0.0f, 0.0f};
constexpr Color green{0.0f, 1.0f, 0.0f};
constexpr Color blue{0.0f, 0.0f, 1.0f};

constexpr GLfloat RAND_MAXf = (double) RAND_MAX;

Color RandomColor()
{
  return {rand() / RAND_MAXf, rand() / RAND_MAXf, rand() / RAND_MAXf};
}

void draw2Triangles()
{
  glColor3f(0.0f, 1.0f, 0.0f);

  glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 50.0f);
    glVertex2f(50.0f, 0.0f);
  glEnd();

  glColor3f(1.0f, 0.0f, 0.0f);

  glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 50.0f);
    glVertex2f(-50.0f, 0.0f);
  glEnd();
}

void draw3ColorTriangle()
{
  glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-50.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(0.0f, 50.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(50.0f, 0.0f);
  glEnd();
}

#define RANDOM_OFFSET ((static_cast<GLfloat>(rand() % 16) - 8.0f) / 32.0f)
void DrawSquare(Square sq)
{
  glColor3f(sq.color.r, sq.color.g, sq.color.b);
  glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(sq.midx - sq.rad + RANDOM_OFFSET, sq.midy - sq.rad + RANDOM_OFFSET);
    glVertex2f(sq.midx - sq.rad + RANDOM_OFFSET, sq.midy + sq.rad + RANDOM_OFFSET);
    glVertex2f(sq.midx + sq.rad + RANDOM_OFFSET, sq.midy - sq.rad + RANDOM_OFFSET);
    glVertex2f(sq.midx + sq.rad + RANDOM_OFFSET, sq.midy + sq.rad + RANDOM_OFFSET);
  glEnd();
}

#define RAINBOW_SWITCH \
{ \
  Color color = RandomColor(); \
  glColor3f(color.r, color.g, color.b); \
}

void DrawPrettySquare(Square sq)
{
  glBegin(GL_TRIANGLE_STRIP);
    RAINBOW_SWITCH
    glVertex2f(sq.midx - sq.rad + RANDOM_OFFSET, sq.midy - sq.rad + RANDOM_OFFSET);
    RAINBOW_SWITCH
    glVertex2f(sq.midx - sq.rad + RANDOM_OFFSET, sq.midy + sq.rad + RANDOM_OFFSET);
    RAINBOW_SWITCH
    glVertex2f(sq.midx + sq.rad + RANDOM_OFFSET, sq.midy - sq.rad + RANDOM_OFFSET);
    RAINBOW_SWITCH
    glVertex2f(sq.midx + sq.rad + RANDOM_OFFSET, sq.midy + sq.rad + RANDOM_OFFSET);
  glEnd();
}

void RemoveSquareMid(Square sq)
{
  sq.rad /= 3.0f;
  //sq.color = bgcolor;
  DrawSquare(sq);
}

void Sierpinski(Square sq, GLfloat limit = 5.0f)
{
  if (sq.rad < limit)
    return;

  RemoveSquareMid(sq);
  GLfloat radsmaller = sq.rad / 3;
  GLfloat radsmalleroff = radsmaller * 2;

  Square sq1 {sq.midx - radsmalleroff, sq.midy + radsmalleroff, radsmaller, RandomColor()};
  Square sq2 {sq.midx, sq.midy + radsmalleroff, radsmaller, RandomColor()};
  Square sq3 {sq.midx + radsmalleroff, sq.midy + radsmalleroff, radsmaller, RandomColor()};
  Square sq4 {sq.midx - radsmalleroff, sq.midy, radsmaller, RandomColor()};
  Square sq6 {sq.midx + radsmalleroff, sq.midy, radsmaller, RandomColor()};
  Square sq7 {sq.midx - radsmalleroff, sq.midy - radsmalleroff, radsmaller, RandomColor()};
  Square sq8 {sq.midx, sq.midy - radsmalleroff, radsmaller, RandomColor()};
  Square sq9 {sq.midx + radsmalleroff, sq.midy - radsmalleroff, radsmaller, RandomColor()};

  RemoveSquareMid(sq1);
  RemoveSquareMid(sq2);
  RemoveSquareMid(sq3);
  RemoveSquareMid(sq4);
  RemoveSquareMid(sq6);
  RemoveSquareMid(sq7);
  RemoveSquareMid(sq8);
  RemoveSquareMid(sq9);

  Sierpinski(sq1);
  Sierpinski(sq2);
  Sierpinski(sq3);
  Sierpinski(sq4);
  Sierpinski(sq6);
  Sierpinski(sq7);
  Sierpinski(sq8);
  Sierpinski(sq9);
}

using uint = unsigned int;

void SierpinskiIter(Square sqbase, uint level)
{
  Square sq = sqbase;
  GLfloat off;

  DrawPrettySquare(sqbase);

  for (uint i = 0; i < level; ++i)
  {
    sq.rad /= 3;
    off = sq.rad * 2;

    GLfloat midx_start = sq.midx - off;
    GLfloat midy_start = sq.midy + off;

    sq.midx = midx_start;
    sq.midy = midy_start;

    //fmt::print("Main X:{} Y:{} off:{}\n", sqbase.midx, sqbase.midy, off);
    //fmt::print("Small X:{} Y:{}\n", sq.midx, sq.midy);
    //fmt::print("Stop at X:{} Y:{}\n\n", sqbase.midx + sqbase.rad, sqbase.midy - sqbase.rad);

    while (sq.midy >= sqbase.midy - sqbase.rad)
    {
      while (sq.midx <= sqbase.midx + sqbase.rad)
      {
        RemoveSquareMid(sq);

        //fmt::print("Small X:{} Y:{}\n", sq.midx, sq.midy);
        sq.midx += off;
      }
      sq.midx = midx_start;
      sq.midy -= off;
    }
    sq.midy = midy_start;

    //fmt::print("\n\n=======\n\n");
  }
}

void RenderScene(void)

{
  glClear(GL_COLOR_BUFFER_BIT);
  // Czyszczenie okna aktualnym kolorem czyszczącym

  Square sq{0.0f, 0.0f, 80.0f, bgcolor};
  //DrawSquare(sq);
  //Sierpinski(sq, 0.01f);
  SierpinskiIter(sq, 5);

  glFlush();
  // Przekazanie poleceń rysujących do wykonania

}

/*************************************************************************************/

// Funkcja ustalająca stan renderowania



void MyInit(void)

{

  srand(time(0));
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  // Kolor okna wnętrza okna - ustawiono na szary

}

/*************************************************************************************/

// Funkcja służąca do kontroli zachowania proporcji rysowanych obiektów
// niezależnie od rozmiarów okna graficznego



void ChangeSize(GLsizei horizontal, GLsizei vertical)

  // Parametry horizontal i vertical (szerokość i wysokość okna) są
  // przekazywane do funkcji za każdym razem, gdy zmieni się rozmiar okna

{

  GLfloat AspectRatio;

  // Deklaracja zmiennej AspectRatio określającej proporcję wymiarów okna



  if(vertical==0)
    // Zabezpieczenie pzred dzieleniem przez 0

    vertical = 1;

  glViewport(0, 0, horizontal, vertical);
  // Ustawienie wielkościokna okna urządzenia (Viewport)
  // W tym przypadku od (0,0) do (horizontal, vertical)

  glMatrixMode(GL_PROJECTION); 
  // Określenie układu współrzędnych obserwatora

  glLoadIdentity();
  // Określenie przestrzeni ograniczającej

  AspectRatio = (GLfloat)horizontal/(GLfloat)vertical;
  // Wyznaczenie współczynnika proporcji okna

  // Gdy okno na ekranie nie jest kwadratem wymagane jest
  // określenie okna obserwatora.
  // Pozwala to zachować właściwe proporcje rysowanego obiektu
  // Do określenia okna obserwatora służy funkcja glOrtho(...)



  if(horizontal <= vertical)

    glOrtho(-100.0,100.0,-100.0/AspectRatio,100.0/AspectRatio,1.0,-1.0); 

  else

    glOrtho(-100.0*AspectRatio,100.0*AspectRatio,-100.0,100.0,1.0,-1.0); 

  glMatrixMode(GL_MODELVIEW);
  // Określenie układu współrzędnych     

  glLoadIdentity();

}

/*************************************************************************************/

// Główny punkt wejścia programu. Program działa w trybie konsoli



int main(int argc, char **argv)

{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
  glutInitWindowSize(1000, 1000);
  // Ustawienie trybu wyświetlania
  // GLUT_SINGLE - pojedynczy bufor wyświetlania
  // GLUT_RGBA - model kolorów RGB

  glutCreateWindow("Drugi program w OpenGL");
  // Utworzenie okna i określenie treści napisu w nagłówku okna

  glutDisplayFunc(RenderScene);
  // Określenie, że funkcja RenderScene będzie funkcją zwrotną (callback)
  // Biblioteka GLUT będzie wywoływała tą funkcję za każdym razem, gdy
  // trzeba będzie przerysować okno


  glutReshapeFunc(ChangeSize);
  // Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną za
  // zmiany rozmiaru okna

  MyInit();
  // Funkcja MyInit (zdefiniowana powyżej) wykonuje wszelkie 
  // inicjalizacje konieczneprzed przystąpieniem do renderowania


  glutMainLoop();
  // Funkcja uruchamia szkielet biblioteki GLUT

}

/*************************************************************************************/
