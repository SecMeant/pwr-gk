#include <GL/gl.h>
#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <fmt/format.h>
#include <cmath>

constexpr float N = 32.0f;
constexpr size_t uN = static_cast<size_t>(N);
typedef float point3[3];

point3 asdftab[uN][uN];
point3 asdftabcolo[uN][uN];

static GLfloat theta[] = {0.0, 0.0, 0.0};

constexpr int MODEL_POINTS = 1;
constexpr int MODEL_MESH = 2;
constexpr int MODEL_TRIANGLE = 3;
constexpr int MODEL_TRIANGLE_FULL = 4;
int model = MODEL_POINTS;

struct Color
{
  GLfloat r,g,b;
};

constexpr GLfloat RAND_MAXf = (double) RAND_MAX;

Color RandomColor()
{
  return {rand() / RAND_MAXf, rand() / RAND_MAXf, rand() / RAND_MAXf};
}

constexpr float x(float u, float v)
{
  return   (std::pow(u, 5.0f) * (-90.0f) + std::pow(u, 4.0f) * 225 + std::pow(u, 3.0f) * (-270.0f)
         + std::pow(u, 2.0f) * 180.0f + u * (-45.0f)) * std::cos(M_PI*v);
}

constexpr float y(float u, [[maybe_unused]] float v)
{
  return std::pow(u, 4.0f) * 160 + std::pow(u, 3.0f) * (-320.0f) + std::pow(u, 2.0f) * 160.0f;
}

constexpr float z(float u, float v)
{
  return   (std::pow(u, 5.0f) * (-90.0f) + std::pow(u, 4.0f) * 225.0f + std::pow(u, 3.0f) * (-270.0f)
         + std::pow(u, 2.0f) * 180 + u * (-45.0f)) * std::sin(M_PI*v);
}

void initasdftab()
{
  for (size_t j = 0; j < N; ++j) {
    for (size_t i = 0; i < N; ++i) {
      asdftab[j][i][0] = x(static_cast<float>(j) / (N - 1), static_cast<float>(i) / (N-1));
      asdftab[j][i][1] = y(static_cast<float>(j) / (N - 1), static_cast<float>(i) / (N-1)) - 5.0f;
      asdftab[j][i][2] = z(static_cast<float>(j) / (N - 1), static_cast<float>(i) / (N-1));
    }
  }

  for (size_t j = 0; j < N; ++j) {
    for (size_t i = 0; i < N; ++i) {
      Color c = RandomColor();
      asdftabcolo[j][i][0] = c.r;
      asdftabcolo[j][i][1] = c.g;
      asdftabcolo[j][i][2] = c.b;
    }
  }
}

void RenderScene();

void keys(unsigned char key, int x, int y)
{
  if(key == 'p') model = MODEL_POINTS;
  if(key == 'm') model = MODEL_MESH;
  if(key == 't') model = MODEL_TRIANGLE;
  if(key == 'f') model = MODEL_TRIANGLE_FULL;

  RenderScene(); // przerysowanie obrazu sceny
}

/*************************************************************************************/

// Funkcja rysująca osie układu współrzędnych

void Axes(void) {

  point3 x_min = {-5.0, 0.0, 0.0};
  point3 x_max = {5.0, 0.0, 0.0};
  // początek i koniec obrazu osi x

  point3 y_min = {0.0, -5.0, 0.0};
  point3 y_max = {0.0, 5.0, 0.0};
  // początek i koniec obrazu osi y

  point3 z_min = {0.0, 0.0, -5.0};
  point3 z_max = {0.0, 0.0, 5.0};
  //  początek i koniec obrazu osi y

  glColor3f(1.0f, 0.0f, 0.0f); // kolor rysowania osi - czerwony
  glBegin(GL_LINES);           // rysowanie osi x

  glVertex3fv(x_min);
  glVertex3fv(x_max);

  glEnd();

  glColor3f(0.0f, 1.0f, 0.0f); // kolor rysowania - zielony
  glBegin(GL_LINES);           // rysowanie osi y

  glVertex3fv(y_min);
  glVertex3fv(y_max);

  glEnd();

  glColor3f(0.0f, 0.0f, 1.0f); // kolor rysowania - niebieski
  glBegin(GL_LINES);           // rysowanie osi z

  glVertex3fv(z_min);
  glVertex3fv(z_max);

  glEnd();
}

/*************************************************************************************/

// Funkcja określająca co ma być rysowane (zawsze wywoływana gdy trzeba
// przerysować scenę)

void _drawegg()
{
  for (size_t j = 0; j < N - 1; ++j) {
    for (size_t i = 0; i < N - 1; ++i) {
      glVertex3fv(asdftab[j][i]);
      glVertex3fv(asdftab[j][i+1]);

      glVertex3fv(asdftab[j][i]);
      glVertex3fv(asdftab[j+1][i+1]);
    }
  }
}

void _draweggtri()
{
  for (size_t j = 0; j < N - 1; ++j) {
    for (size_t i = 0; i < N - 1; ++i) {
      glVertex3fv(asdftab[j][i]);
      glVertex3fv(asdftab[j][i+1]);

      glVertex3fv(asdftab[j][i+1]);
      glVertex3fv(asdftab[j+1][i]);

      glVertex3fv(asdftab[j][i]);
      glVertex3fv(asdftab[j+1][i]);
    }
  }
}

void drawWithColor(size_t j, size_t i)
{
  glColor3f(asdftabcolo[j][i][0], asdftabcolo[j][i][1], asdftabcolo[j][i][2]);
  glVertex3fv(asdftab[j][i]);
}

void _draweggtricolo()
{
  for (size_t j = 0; j < N - 1; ++j) {
    for (size_t i = 0; i < N - 1; ++i) {
      drawWithColor(j,i);
      drawWithColor(j,i+1);
      drawWithColor(j+1,i);
      drawWithColor(j+1, i+1);
    }
  }
}

void spinEgg()
{
  theta[0] -= 0.5;
  if( theta[0] > 360.0 ) theta[0] -= 360.0;

  theta[1] -= 0.5;
  if( theta[1] > 360.0 ) theta[1] -= 360.0;

  theta[2] -= 0.5;
  if( theta[2] > 360.0 ) theta[2] -= 360.0;

  glutPostRedisplay(); //odświeżenie zawartości aktualnego okna
}

void RenderScene(void) {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Czyszczenie okna aktualnym kolorem czyszczącym

  glLoadIdentity();
  // Czyszczenie macierzy bieżącej

  Axes();
  glColor3f(1.0f, 1.0f, 1.0f);
  //glutWireTeapot(3.0);
  
  glRotatef(theta[0], 1.0, 0.0, 0.0);
  glRotatef(theta[1], 0.0, 1.0, 0.0);
  glRotatef(theta[2], 0.0, 0.0, 1.0); 

  switch(model)
  {
    case MODEL_POINTS:
      glBegin(GL_POINTS);
      _drawegg();
      break;
    case MODEL_MESH:
      glBegin(GL_LINES);
      _drawegg();
      break;
    case MODEL_TRIANGLE_FULL:
      glBegin(GL_TRIANGLE_STRIP);
      _draweggtricolo();
    case MODEL_TRIANGLE:
    default:
      glBegin(GL_LINES);
      _draweggtri();
      break;
  }

  glEnd();

  // Narysowanie osi przy pomocy funkcji zdefiniowanej wyżej

  glFlush();
  // Przekazanie poleceń rysujących do wykonania

  glutSwapBuffers();
  //
}

/*************************************************************************************/

// Funkcja ustalająca stan renderowania

void MyInit(void) {

  srand(time(0));
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  // Kolor czyszcący (wypełnienia okna) ustawiono na czarny
}

/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie stałych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokość i szerokość okna) są
// przekazywane do funkcji za każdym razem gdy zmieni się rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical) {

  GLfloat AspectRatio;
  // Deklaracja zmiennej AspectRatio  określającej proporcję
  // wymiarów okna

  if (vertical == 0) // Zabezpieczenie przed dzieleniem przez 0

    vertical = 1;

  glViewport(0, 0, horizontal, vertical);
  // Ustawienie wielkościokna okna widoku (viewport)
  // W tym przypadku od (0,0) do (horizontal, vertical)

  glMatrixMode(GL_PROJECTION);
  // Przełączenie macierzy bieżącej na macierz projekcji

  glLoadIdentity();
  // Czyszcznie macierzy bieżącej

  AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
  // Wyznaczenie współczynnika  proporcji okna
  // Gdy okno nie jest kwadratem wymagane jest określenie tak zwanej
  // przestrzeni ograniczającej pozwalającej zachować właściwe
  // proporcje rysowanego obiektu.
  // Do okreslenia przestrzeni ograniczjącej służy funkcja
  // glOrtho(...)

  if (horizontal <= vertical)

    glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);

  else

    glOrtho(-7.5 * AspectRatio, 7.5 * AspectRatio, -7.5, 7.5, 10.0, -10.0);

  glMatrixMode(GL_MODELVIEW);
  // Przełączenie macierzy bieżącej na macierz widoku modelu

  glLoadIdentity();
  // Czyszcenie macierzy bieżącej
}

/*************************************************************************************/

// Główny punkt wejścia programu. Program działa w trybie konsoli

int main(int argc, char **argv) {

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500, 500);

  glutCreateWindow("Drugi program w OpenGL");
  glutDisplayFunc(RenderScene);
  // Określenie, że funkcja RenderScene będzie funkcją zwrotną
  // (callback function).  Bedzie ona wywoływana za każdym razem
  // gdy zajdzie potrzba przeryswania okna

  glutReshapeFunc(ChangeSize);
  // Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną
  // zazmiany rozmiaru okna

  MyInit();
  // Funkcja MyInit() (zdefiniowana powyżej) wykonuje wszelkie
  // inicjalizacje konieczne  przed przystąpieniem do renderowania

  glEnable(GL_DEPTH_TEST);
  // Włączenie mechanizmu usuwania powierzchni niewidocznych

  initasdftab();

  glutKeyboardFunc(keys);
  glutIdleFunc(spinEgg);

  glutMainLoop();
  // Funkcja uruchamia szkielet biblioteki GLUT
}

/*************************************************************************************/
