#include <GL/gl.h>
#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <fmt/format.h>

struct Color
{
  GLfloat r, g, b;
};

struct Square
{
  GLfloat midx, midy, rad;
  Color color;
};

constexpr Color bgcolor{ 1.0f, 1.0f, 1.0f };
constexpr Color red{ 1.0f, 0.0f, 0.0f };
constexpr Color green{ 0.0f, 1.0f, 0.0f };
constexpr Color blue{ 0.0f, 0.0f, 1.0f };

constexpr GLfloat RAND_MAXf = (double)RAND_MAX;

GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess = { 20.0 };
GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat light_diffuse[] = { 1.0, 0.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat att_constant = { 1.0 };
GLfloat att_linear = { 0.05 };
GLfloat att_quadratic = { 0.001 };

Color
RandomColor()
{
  return { rand() / RAND_MAXf, rand() / RAND_MAXf, rand() / RAND_MAXf };
}

using uint = unsigned int;

void
RenderScene(void)
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBegin(GL_TRIANGLES);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.25f, 0.25f);
    glVertex2f(0.0f, 0.0f);

    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.75f, 0.25f);
    glVertex2f(50.0f, 0.0f);

    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.5f, 0.75f);
    glVertex2f(0.0f, 50.0f);
  glEnd();

  glFlush();
}

GLbyte*
LoadTGAImage(const char* FileName,
             GLint* ImWidth,
             GLint* ImHeight,
             GLint* ImComponents,
             GLenum* ImFormat)
{
#pragma pack(1)
  typedef struct
  {
    GLbyte idlength;
    GLbyte colormaptype;
    GLbyte datatypecode;
    unsigned short colormapstart;
    unsigned short colormaplength;
    unsigned char colormapdepth;
    unsigned short x_orgin;
    unsigned short y_orgin;
    unsigned short width;
    unsigned short height;
    GLbyte bitsperpixel;
    GLbyte descriptor;
  } TGAHEADER;
#pragma pack(8)

  FILE* pFile;
  TGAHEADER tgaHeader;
  unsigned long lImageSize;
  short sDepth;
  GLbyte* pbitsperpixel = NULL;

  *ImWidth = 0;
  *ImHeight = 0;
  *ImFormat = GL_BGR_EXT;
  *ImComponents = GL_RGB8;

  pFile = fopen(FileName, "rb");
  if (pFile == NULL)
    return NULL;

  fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

  *ImWidth = tgaHeader.width;
  *ImHeight = tgaHeader.height;
  sDepth = tgaHeader.bitsperpixel / 8;

  if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 &&
      tgaHeader.bitsperpixel != 32)
    return NULL;

  lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

  pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));

  if (pbitsperpixel == NULL)
    return NULL;

  if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1) {
    free(pbitsperpixel);
    return NULL;
  }

  switch (sDepth) {
    case 3:
      *ImFormat = GL_BGR_EXT;
      *ImComponents = GL_RGB8;
      break;

    case 4:
      *ImFormat = GL_BGRA_EXT;
      *ImComponents = GL_RGBA8;
      break;

    case 1:
      *ImFormat = GL_LUMINANCE;
      *ImComponents = GL_LUMINANCE8;
      break;
  };

  fclose(pFile);
  return pbitsperpixel;
}

void
MyInit(void)

{
  srand(time(0));

  GLbyte* pBytes;
  GLint ImWidth, ImHeight, ImComponents;
  GLenum ImFormat;

  glEnable(GL_CULL_FACE);

  pBytes =
    LoadTGAImage("tekstury/D1_t.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);

  if (pBytes == NULL)
    fmt::print("Reading texture file failed\n");

  glTexImage2D(GL_TEXTURE_2D,
               0,
               ImComponents,
               ImWidth,
               ImHeight,
               0,
               ImFormat,
               GL_UNSIGNED_BYTE,
               pBytes);

  free(pBytes);

  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

  glShadeModel(GL_SMOOTH); // właczenie łagodnego cieniowania
  glEnable(GL_LIGHTING);   // właczenie systemu oświetlenia sceny
  glEnable(GL_LIGHT0);     // włączenie źródła o numerze 0
  glEnable(GL_DEPTH_TEST); // włączenie mechanizmu z-bufora
}

void
ChangeSize(GLsizei horizontal, GLsizei vertical)
{

  GLfloat AspectRatio;

  if (vertical == 0)
    // Zabezpieczenie pzred dzieleniem przez 0

    vertical = 1;

  glViewport(0, 0, horizontal, vertical);
  // Ustawienie wielkościokna okna urządzenia (Viewport)
  // W tym przypadku od (0,0) do (horizontal, vertical)

  glMatrixMode(GL_PROJECTION);
  // Określenie układu współrzędnych obserwatora

  glLoadIdentity();
  // Określenie przestrzeni ograniczającej

  AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
  // Wyznaczenie współczynnika proporcji okna

  // Gdy okno na ekranie nie jest kwadratem wymagane jest
  // określenie okna obserwatora.
  // Pozwala to zachować właściwe proporcje rysowanego obiektu
  // Do określenia okna obserwatora służy funkcja glOrtho(...)

  if (horizontal <= vertical)

    glOrtho(
      -100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);

  else

    glOrtho(
      -100.0 * AspectRatio, 100.0 * AspectRatio, -100.0, 100.0, 1.0, -1.0);

  glMatrixMode(GL_MODELVIEW);
  // Określenie układu współrzędnych

  glLoadIdentity();
}

/*************************************************************************************/

// Główny punkt wejścia programu. Program działa w trybie konsoli

int
main(int argc, char** argv)

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
