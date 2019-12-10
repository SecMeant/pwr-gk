/*************************************************************************************/

//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacją osi
//  układu współrzędnych dla rzutowania perspektywicznego

/*************************************************************************************/

#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include <fmt/format.h>

typedef float point3[3];

static GLfloat viewer[] = {0.0, 0.0, 12.0};
static GLfloat thetax = 0.0;
static GLfloat thetay = 0.0;
static GLfloat pix2anglex;
static GLfloat pix2angley;

static GLint status = 0;

static int x_pos_old = 0;
static int delta_x = 0;

static int y_pos_old = 0;
static int delta_y = 0;

static GLfloat scale = 1;

static GLfloat offset_x = 0;
static GLfloat offset_y = 0;

static GLfloat R = 10.0;
static GLfloat alpha = 0;
static GLfloat beta = 0;

static GLfloat lookpoint_x = 0;
static GLfloat lookpoint_y = 0;
static GLfloat lookpoint_z = 0;
// inicjalizacja położenia obserwatora

/*************************************************************************************/

// Funkcja rysująca osie układu wspó?rz?dnych

void Axes(void) {

  point3 x_min = {-5.0, 0.0, 0.0};
  point3 x_max = {5.0, 0.0, 0.0};
  // pocz?tek i koniec obrazu osi x

  point3 y_min = {0.0, -5.0, 0.0};
  point3 y_max = {0.0, 5.0, 0.0};
  // pocz?tek i koniec obrazu osi y

  point3 z_min = {0.0, 0.0, -5.0};
  point3 z_max = {0.0, 0.0, 5.0};
  //  pocz?tek i koniec obrazu osi y

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

// Funkcja określająca co ma być rysowane (zawsze wywoływana, gdy trzeba
// przerysować scenę)

void RenderScene(void) {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Czyszczenie okna aktualnym kolorem czyszczącym

  glLoadIdentity();
  // Czyszczenie macierzy bie??cej

  auto vec = 1.0;

  viewer[0] = R * cos(alpha) * cos(beta) + lookpoint_x;
  viewer[1] = R * sin(beta) + lookpoint_y;
  viewer[2] = R * sin(alpha) * cos(beta) + lookpoint_z;

  if (beta >= (M_PI / 2.0) && beta <= (M_PI * 1.5))
    vec = -1.0;

  if (beta <= -(M_PI / 2.0) && beta >= -(M_PI * 1.5))
    vec = -1.0;

  fmt::print("{} {} {} {} | {} {}\n", viewer[0], viewer[1], viewer[2], vec, alpha, beta);

  gluLookAt(viewer[0], viewer[1], viewer[2], lookpoint_x, lookpoint_y, lookpoint_z, 0.0, vec, 0.0);
  // Zdefiniowanie położenia obserwatora

  Axes();
  // Narysowanie osi przy pomocy funkcji zdefiniowanej powyżej

  if (status == 1)
  {
    thetax += delta_x * pix2anglex;
    thetay += delta_y * pix2angley;
  }

  glRotatef(thetax, 0.0, 1.0, 0.0);
  glRotatef(thetay, 1.0, 0.0, 0.0);
  glScalef(scale * 1.0, scale * 1.0, scale * 1.0);
  glTranslatef(offset_x, offset_y, 0.0);

  glColor3f(1.0f, 1.0f, 1.0f);
  // Ustawienie koloru rysowania na biały

  glutSolidTeapot(3.0);
  // Narysowanie czajnika

  glFlush();
  // Przekazanie poleceń rysujących do wykonania

  glutSwapBuffers();
}
/*************************************************************************************/

// Funkcja ustalająca stan renderowania

void MyInit(void) {

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  /*************************************************************************************/

  //  Definicja materiału z jakiego zrobiony jest czajnik
  //  i definicja źródła światła

  /*************************************************************************************/


  /*************************************************************************************/
  // Definicja materiału z jakiego zrobiony jest czajnik
  GLfloat mat_ambient[]  = {1.0, 1.0, 1.0, 1.0};
  // współczynniki ka =[kar,kag,kab] dla światła otoczenia

  GLfloat mat_diffuse[]  = {1.0, 1.0, 1.0, 1.0};
  // współczynniki kd =[kdr,kdg,kdb] światła rozproszonego

  GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
  // współczynniki ks =[ksr,ksg,ksb] dla światła odbitego

  GLfloat mat_shininess  = {20.0};
  // współczynnik n opisujący połysk powierzchni

  /*************************************************************************************/
  // Definicja źródła światła

  GLfloat light_position[] = {0.0, 0.0, 10.0, 1.0};
  // położenie źródła


  GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1.0};
  // składowe intensywności świecenia źródła światła otoczenia
  // Ia = [Iar,Iag,Iab]

  GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
  // składowe intensywności świecenia źródła światła powodującego
  // odbicie dyfuzyjne Id = [Idr,Idg,Idb]

  GLfloat light_specular[]= {1.0, 1.0, 1.0, 1.0};
  // składowe intensywności świecenia źródła światła powodującego
  // odbicie kierunkowe Is = [Isr,Isg,Isb]

  GLfloat att_constant  = {1.0};
  // składowa stała ds dla modelu zmian oświetlenia w funkcji
  // odległości od źródła

  GLfloat att_linear    = {0.05};
  // składowa liniowa dl dla modelu zmian oświetlenia w funkcji
  // odległości od źródła

  GLfloat att_quadratic  = {0.001};
  // składowa kwadratowa dq dla modelu zmian oświetlenia w funkcji
  // odległości od źródła

  /*************************************************************************************/
  // Ustawienie parametrów materiału i źródła światła

  /*************************************************************************************/
  // Ustawienie patrametrów materiału


  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

  /*************************************************************************************/
  // Ustawienie parametrów źródła

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);


  /*************************************************************************************/
  // Ustawienie opcji systemu oświetlania sceny

  glShadeModel(GL_SMOOTH); // właczenie łagodnego cieniowania
  glEnable(GL_LIGHTING);   // właczenie systemu oświetlenia sceny
  glEnable(GL_LIGHT0);     // włączenie źródła o numerze 0
  glEnable(GL_DEPTH_TEST); // włączenie mechanizmu z-bufora

  /*************************************************************************************/
}

/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie stałych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokość i szerokość okna) są
// przekazywane do funkcji za każdym razem gdy zmieni się rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical) {

  pix2anglex = 360.0/(float)horizontal;
  pix2angley = 360.0/(float)vertical;

  glMatrixMode(GL_PROJECTION);
  // Przełączenie macierzy bieżącej na macierz projekcji

  glLoadIdentity();
  // Czyszcznie macierzy bieżącej

  gluPerspective(70, 1.0, 1.0, 30.0);
  // Ustawienie parametrów dla rzutu perspektywicznego

  if (horizontal <= vertical)
    glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

  else
    glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
  // Ustawienie wielkości okna okna widoku (viewport) w zależności
  // relacji pomiędzy wysokością i szerokością okna

  glMatrixMode(GL_MODELVIEW);
  // Przełączenie macierzy bieżącej na macierz widoku modelu

  glLoadIdentity();
  // Czyszczenie macierzy bieżącej
}

void Mouse (int btn, int state, int x, int y)
{
  if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    x_pos_old = x;
    y_pos_old = y;
    status = 1;
  }
  else
  {
    status = 0;
  }
}

void Motion (GLsizei x, GLsizei y)
{
  delta_x = x - x_pos_old;
  x_pos_old = x;

  delta_y = y - y_pos_old;
  y_pos_old = y;

  glutPostRedisplay();
}

void Keyboard (unsigned char key, int x, int y)
{
  constexpr double camera_speed = 0.1;

  switch (key)
  {
    case 'i':
      scale += 0.1;
      break;

    case 'o':
      scale -= 0.1;
      break;

    case 'l':
      offset_x += 0.1;
      break;

    case 'h':
      offset_x -= 0.1;
      break;

    case 'k':
      offset_y += 0.1;
      break;

    case 'j':
      offset_y -= 0.1;
      break;

    case 'a':
      alpha += camera_speed;
      break;

    case 'f':
      alpha -= camera_speed;
      break;

    case 's':
      beta += camera_speed;
      if (beta >= 2.0 * M_PI)
        beta = 0.0;
      break;

    case 'd':
      beta -= camera_speed;
      if (beta <= -2.0 * M_PI)
        beta = 0;
      break;

    case 'R':
      R += 0.5;
      break;

    case 'r':
      R -= 0.5;
      break;

    case 'X':
      lookpoint_x += 0.5;
      break;

    case 'x':
      lookpoint_x -= 0.5;
      break;

    case 'Y':
      lookpoint_y += 0.5;
      break;

    case 'y':
      lookpoint_y -= 0.5;
      break;

    case 'Z':
      lookpoint_z += 0.5;
      break;

    case 'z':
      lookpoint_z -= 0.5;
      break;

    default:
      break;
  }

  RenderScene();
}

/*************************************************************************************/

// Główny punkt wejścia programu. Program działa w trybie konsoli

int main(int argc, char **argv) {

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(300, 300);
  glutCreateWindow("Rzutowanie perspektywiczne");

  glutDisplayFunc(RenderScene);
  // Określenie, że funkcja RenderScene będzie funkcją zwrotną
  // (callback function).  Będzie ona wywoływana za każdym razem
  // gdy zajdzie potrzeba przerysowania okna

  glutReshapeFunc(ChangeSize);
  // Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną
  // za zmiany rozmiaru okna

  MyInit();
  // Funkcja MyInit() (zdefiniowana powyżej) wykonuje wszelkie
  // inicjalizacje konieczne  przed przystąpieniem do renderowania

  glEnable(GL_DEPTH_TEST);
  // Włączenie mechanizmu usuwania niewidocznych elementów sceny

  glutMouseFunc(Mouse);
  glutMotionFunc(Motion);
  glutKeyboardFunc(Keyboard);
  glutMainLoop();
  // Funkcja uruchamia szkielet biblioteki GLUT
}

/*************************************************************************************/
