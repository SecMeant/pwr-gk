#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include <fmt/format.h>

typedef float point3[3];

static GLfloat viewer[] = { 10.0, 5.0, 10.0 };
static GLfloat thetax = 0.0;
static GLfloat thetay = 0.0;
static GLfloat pix2anglex;
static GLfloat pix2angley;

int figure = 0;

static GLint status = 0;

static int x_pos_old = 0;
static int delta_x = 0;

static int y_pos_old = 0;
static int delta_y = 0;

static GLfloat scale = 1.0;

static GLfloat offset_x = 0;
static GLfloat offset_y = 0;

static GLfloat R = 1.0;
static GLfloat alpha = M_PI / 2.0;
static GLfloat beta = 0;

static GLfloat lookpoint_x = 0;
static GLfloat lookpoint_y = 0;
static GLfloat lookpoint_z = 0;

constexpr float N = 512.0f;
constexpr size_t uN = static_cast<size_t>(N);

point3 eggpoints[uN][uN];
point3 eggnorms[uN][uN]{};
point3 eggcolo[uN][uN];

GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess = { 20.0 };

GLfloat light_position[] = { 2.5, 2.5, 10.0, 1.0 };
GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat att_constant = { 1.0 };
GLfloat att_linear = { 0.05 };
GLfloat att_quadratic = { 0.001 };

struct Color
{
  GLfloat r, g, b;
};

constexpr GLfloat RAND_MAXf = (double)RAND_MAX;

constexpr float
egg_x(float u, float v)
{
  return (std::pow(u, 5.0f) * (-90.0f) + std::pow(u, 4.0f) * 225 +
          std::pow(u, 3.0f) * (-270.0f) + std::pow(u, 2.0f) * 180.0f +
          u * (-45.0f)) *
         std::cos(M_PI * v);
}

constexpr float
egg_y(float u, [[maybe_unused]] float v)
{
  return std::pow(u, 4.0f) * 160 + std::pow(u, 3.0f) * (-320.0f) +
         std::pow(u, 2.0f) * 160.0f;
}

constexpr float
egg_z(float u, float v)
{
  return (std::pow(u, 5.0f) * (-90.0f) + std::pow(u, 4.0f) * 225.0f +
          std::pow(u, 3.0f) * (-270.0f) + std::pow(u, 2.0f) * 180 +
          u * (-45.0f)) *
         std::sin(M_PI * v);
}

constexpr float
egg_normal_xu(float u, float v)
{

  return (-450.0f * std::pow(u, 4.0f) + 900.0f * std::pow(u, 3.0f) -
          810.0f * std::pow(u, 2.0f) + 360.0f * u - 45) *
         std::cos(M_PI * v);
}

constexpr float
egg_normal_xv(float u, float v)
{
  return M_PI *
         (90.0f * std::pow(u, 5.0f) - 225.0f * std::pow(u, 4.0f) +
          270.0f * std::pow(u, 3.0f) - 180.0f * std::pow(u, 2.0f) + 45.0f * u) *
         std::sin(M_PI * v);
}

constexpr float
egg_normal_yu(float u, float v)
{
  return 640.0f * std::pow(u, 3.0f) - 960.0f * std::pow(u, 2.0f) + 320.0f * u;
}

constexpr float
egg_normal_yv([[maybe_unused]] float u, [[maybe_unused]] float v)
{
  return 0;
}

constexpr float
egg_normal_zu(float u, float v)
{
  return (-450.0f * std::pow(u, 4.0f) + 900.0f * std::pow(u, 3.0f) -
          810.0f * std::pow(u, 2.0f) + 360.0f * u - 45) *
         std::sin(M_PI * v);
}

constexpr float
egg_normal_zv(float u, float v)
{
  return -M_PI *
         (90.0f * std::pow(u, 5.0f) - 225.0f * std::pow(u, 4.0f) +
          270.0f * std::pow(u, 3.0f) - 180.0f * std::pow(u, 2.0f) + 45.0f * u) *
         std::cos(M_PI * v);
}

void
egg_normal(float norm[3], float u, float v)
{
  u /= (N);
  v /= (N);

  auto xu = egg_normal_xu(u, v);
  auto xv = egg_normal_xv(u, v);
  auto yu = egg_normal_yu(u, v);
  auto yv = egg_normal_yv(u, v);
  auto zu = egg_normal_zu(u, v);
  auto zv = egg_normal_zv(u, v);

  norm[0] = yu * zv - zu * yv;
  norm[1] = zu * xv - xu * zv;
  norm[2] = xu * yv - yu * xv;

  auto len =
    std::sqrt(norm[0] * norm[0] + norm[1] * norm[1] + norm[2] * norm[2]);

  if (u == 0 || u == N) {
    norm[0] = 0;
    norm[1] = -1;
    norm[2] = 0;
  } else if (u < 0.5f) {
    norm[0] = (yu * zv - zu * yv) / len;
    norm[1] = (zu * xv - xu * zv) / len;
    norm[2] = (xu * yv - yu * xv) / len;
  } else if (u > 0.5f) {
    norm[0] = -1 * (yu * zv - zu * yv) / len;
    norm[1] = -1 * (zu * xv - xu * zv) / len;
    norm[2] = -1 * (xu * yv - yu * xv) / len;
  } else {
    norm[0] = 0;
    norm[1] = 1;
    norm[2] = 0;
  }
}

void
init_eggpoints()
{
  for (size_t j = 0; j < N; ++j) {
    for (size_t i = 0; i < N; ++i) {
      eggpoints[j][i][0] =
        egg_x(static_cast<float>(j) / (N - 1), static_cast<float>(i) / (N - 1));

      eggpoints[j][i][1] = egg_y(static_cast<float>(j) / (N - 1),
                                 static_cast<float>(i) / (N - 1)) -
                           5.0f;
      eggpoints[j][i][2] =
        egg_z(static_cast<float>(j) / (N - 1), static_cast<float>(i) / (N - 1));
    }
  }

  for (size_t j = 0; j < N; ++j) {
    for (size_t i = 0; i < N; ++i) {
      Color c = { 0xd4, 0xaf, 0x37 };
      eggcolo[j][i][0] = c.r;
      eggcolo[j][i][1] = c.g;
      eggcolo[j][i][2] = c.b;
    }
  }

  for (size_t j = 0; j < N; ++j) {
    for (size_t i = 0; i < N; ++i) {
      egg_normal(eggnorms[j][i], j, i);
    }
  }
}

void
Axes(void)
{
  point3 x_min = { -5.0, 0.0, 0.0 };
  point3 x_max = { 5.0, 0.0, 0.0 };

  point3 y_min = { 0.0, -5.0, 0.0 };
  point3 y_max = { 0.0, 5.0, 0.0 };

  point3 z_min = { 0.0, 0.0, -5.0 };
  point3 z_max = { 0.0, 0.0, 5.0 };

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

void
draw_egg()
{
  glBegin(GL_TRIANGLES);
  for (size_t j = 0; j < N - 1; ++j) {
    for (size_t i = 0; i < N - 1; ++i) {
      glNormal3fv(eggnorms[j][i]);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3fv(eggpoints[j][i]);

      glNormal3fv(eggnorms[j][i + 1]);
      glTexCoord2f(0.75f, 0.0f);
      glVertex3fv(eggpoints[j][i + 1]);

      glNormal3fv(eggnorms[j + 1][i]);
      glTexCoord2f(0.50f, 1.0f);
      glVertex3fv(eggpoints[j + 1][i]);

      glNormal3fv(eggnorms[j + 1][i]);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3fv(eggpoints[j + 1][i]);

      glNormal3fv(eggnorms[j + 1][i + 1]);
      glTexCoord2f(0.75f, 0.0f);
      glVertex3fv(eggpoints[j + 1][i + 1]);

      glNormal3fv(eggnorms[j][i + 1]);
      glTexCoord2f(0.50f, 1.0f);
      glVertex3fv(eggpoints[j][i + 1]);
    }
  }
  glEnd();
}
struct point3d
{
  GLfloat x, y, z;
};

void
draw_tri(point3d p1, point3d p2, point3d p3)
{
  glBegin(GL_TRIANGLES);
  glNormal3f(1.0f, 0.0f, 0.0f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(p1.x, p1.y, p1.z);

  glNormal3f(1.0f, 0.0f, 0.0f);
  glTexCoord2f(0.75f, 0.0f);
  glVertex3f(p2.x, p2.y, p2.z);

  glNormal3f(1.0f, 0.0f, 0.0f);
  glTexCoord2f(0.50f, 1.0f);
  glVertex3f(p3.x, p3.y, p3.z);
  glEnd();
}

void
draw_tri()
{
  draw_tri({ 0.0f, 0.0f }, { 5.0f, 0.0f }, { 0.0f, 5.0f });
}

void
draw_pyra()
{
  draw_tri({ 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 2.5f, 4.0f, -2.5f });
  draw_tri({ 0.0f, 0.0f, -5.0f }, { 0.0f, 0.0f, 0.0f }, { 2.5f, 4.0f, -2.5f });
  draw_tri({ 5.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, -5.0f }, { 2.5f, 4.0f, -2.5f });
  draw_tri({ 5.0f, 0.0f, -5.0f }, { 0.0f, 0.0f, -5.0f }, { 2.5f, 4.0f, -2.5f });

  // base
  draw_tri({ 5.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -5.0f });
  draw_tri({ 5.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -5.0f }, { 5.0f, 0.0f, -5.0f });
}

void
RenderScene(void)
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  auto vec = 1.0;

  gluLookAt(viewer[0],
            viewer[1],
            viewer[2],
            lookpoint_x,
            lookpoint_y,
            lookpoint_z,
            0.0,
            vec,
            0.0);

  Axes();

  if (status == 1) {
    thetax += delta_x * pix2anglex;
    thetay += delta_y * pix2angley;
  }

  glRotatef(thetax, 0.0, 1.0, 0.0);
  glRotatef(thetay, 1.0, 0.0, 0.0);
  glScalef(scale * 1.0, scale * 1.0, scale * 1.0);
  glTranslatef(offset_x, offset_y, 0.0);

  glColor3f(0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glColor3f(1.0f, 1.0f, 1.0f);

  switch (figure) {
    case 0:
      draw_egg();
      break;
    case 1:
      draw_tri();
      break;
    case 2:
      draw_pyra();
      break;
  }

  glFlush();

  glutSwapBuffers();
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

struct texture
{
  GLint ImWidth, ImHeight, ImComponents;
  GLenum ImFormat;
  GLbyte* data;
} t1, t2;
int tex_active = 0;

void
set_texture(texture tex)
{
  glTexImage2D(GL_TEXTURE_2D,
               0,
               tex.ImComponents,
               tex.ImWidth,
               tex.ImHeight,
               0,
               tex.ImFormat,
               GL_UNSIGNED_BYTE,
               tex.data);
}

void
MyInit(void)
{
  GLint ImWidth, ImHeight, ImComponents;
  GLenum ImFormat;

  glEnable(GL_CULL_FACE);

  t1.data = LoadTGAImage("tekstury/D1_t.tga",
                         &t1.ImWidth,
                         &t1.ImHeight,
                         &t1.ImComponents,
                         &t1.ImFormat);

  if (t1.data == NULL)
    fmt::print("Reading texture file failed\n");

  t2.data = LoadTGAImage("tekstury/yui.tga",
                         &t2.ImWidth,
                         &t2.ImHeight,
                         &t2.ImComponents,
                         &t2.ImFormat);

  if (t2.data == NULL)
    fmt::print("Reading texture file failed\n");

  set_texture(t1);

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

  pix2anglex = 360.0 / (float)horizontal;
  pix2angley = 360.0 / (float)vertical;

  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();

  gluPerspective(70, 1.0, 1.0, 30.0);

  if (horizontal <= vertical)
    glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

  else
    glViewport((horizontal - vertical) / 2, 0, vertical, vertical);

  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();
}

void
Mouse(int btn, int state, int x, int y)
{
  if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    x_pos_old = x;
    y_pos_old = y;
    status = 1;
  } else {
    status = 0;
  }
}

void
Motion(GLsizei x, GLsizei y)
{
  delta_x = x - x_pos_old;
  x_pos_old = x;

  delta_y = y - y_pos_old;
  y_pos_old = y;

  glutPostRedisplay();
}

void
Keyboard(unsigned char key, int x, int y)
{
  constexpr double camera_speed = 0.1;
  constexpr size_t light_speed = 1; // ;]

  switch (key) {
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

    case 'f':
      figure = (figure + 1) % 3;
      break;

    case 't':
      tex_active ^= 1;
      tex_active &= 1;

      if (tex_active == 0) {
        set_texture(t1);
      } else {
        set_texture(t2);
      }

      break;

    default:
      fmt::print("Key {}\n", key);
      break;
  }

  RenderScene();
}

int
main(int argc, char** argv)
{

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(300, 300);
  glutCreateWindow("Lighting");

  glutDisplayFunc(RenderScene);

  glutReshapeFunc(ChangeSize);

  MyInit();

  glEnable(GL_DEPTH_TEST);

  init_eggpoints();

  glutMouseFunc(Mouse);
  glutMotionFunc(Motion);
  glutKeyboardFunc(Keyboard);
  glutMainLoop();
}

/*************************************************************************************/
