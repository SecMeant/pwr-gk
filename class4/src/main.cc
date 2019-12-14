#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include <fmt/format.h>

typedef float point3[3];

static GLfloat viewer[] = { 0.0, 0.0, 12.0 };
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

constexpr float N = 512.0f;
constexpr size_t uN = static_cast<size_t>(N);

point3 eggpoints[uN][uN];
point3 eggnorms[uN][uN] {};
point3 eggcolo[uN][uN];

GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
GLfloat light_position2[] = { 10.0, 0.0, 5.0, 1.0 };

GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat mat_shininess = { 20.0 };

GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat light_ambient2[] = { 0.1, 0.1, 0.1, 1.0 };

GLfloat light_diffuse[] = { 1.0, 0.0, 1.0, 1.0 };
GLfloat light_diffuse2[] = { 1.0, 1.0, 0.0, 1.0 };

GLfloat light_specular[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat light_specular2[] = { 0.0, 0.0, 1.0, 1.0 };

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

  auto len = std::sqrt(norm[0] * norm[0] + norm[1] * norm[1] + norm[2] * norm[2]);

  if (u == 0 || u == N)
  {
    norm[0] = 0;
    norm[1] = -1;
    norm[2] = 0;
  }
  else if (u < 0.5f)
  {
    norm[0] = (yu*zv - zu*yv) / len;
    norm[1] = (zu*xv - xu*zv) / len;
    norm[2] = (xu*yv - yu*xv) / len;
  }
  else if (u > 0.5f)
  {
    norm[0] = -1 * (yu*zv - zu*yv) / len;
    norm[1] = -1 * (zu*xv - xu*zv) / len;
    norm[2] = -1 * (xu*yv - yu*xv) / len;
  }
  else
  {
    norm[0] = 0;
    norm[1] = 1;
    norm[2] = 0;
  }

  fmt::print("Norm: {} {} {}\n", norm[0], norm[1], norm[2]);
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
  glColor3f(1.0f, 1.0f, 1.0f);
  for (size_t j = 0; j < N - 1; ++j) {
    for (size_t i = 0; i < N - 1; ++i) {
      glNormal3fv(eggnorms[j][i]);
      glVertex3fv(eggpoints[j][i]);

      glNormal3fv(eggnorms[j][i + 1]);
      glVertex3fv(eggpoints[j][i + 1]);

      glNormal3fv(eggnorms[j + 1][i]);
      glVertex3fv(eggpoints[j + 1][i]);

      glNormal3fv(eggnorms[j + 1][i]);
      glVertex3fv(eggpoints[j + 1][i]);

      glNormal3fv(eggnorms[j + 1][i + 1]);
      glVertex3fv(eggpoints[j + 1][i + 1]);

      glNormal3fv(eggnorms[j][i + 1]);
      glVertex3fv(eggpoints[j][i + 1]);
    }
  }
  glEnd();
}

void
RenderScene(void)
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  auto vec = 1.0;

  viewer[0] = R * cos(alpha) * cos(beta) + lookpoint_x;
  viewer[1] = R * sin(beta) + lookpoint_y;
  viewer[2] = R * sin(alpha) * cos(beta) + lookpoint_z;

  if (beta >= (M_PI / 2.0) && beta <= (M_PI * 1.5))
    vec = -1.0;

  if (beta <= -(M_PI / 2.0) && beta >= -(M_PI * 1.5))
    vec = -1.0;


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

  glColor3f(1.0f, 1.0f, 1.0f);

  draw_egg();

  glFlush();

  glutSwapBuffers();
}

void
MyInit(void)
{

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient2);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse2);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular2);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position2);

  glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant);
  glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear);
  glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic);

  glShadeModel(GL_SMOOTH); // właczenie łagodnego cieniowania
  glEnable(GL_LIGHTING);   // właczenie systemu oświetlenia sceny
  glEnable(GL_LIGHT0);     // włączenie źródła o numerze 0
  //glEnable(GL_LIGHT1);     // włączenie źródła o numerze 0
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

    case 'z':
      light_position[0] -= light_speed;
      glLightfv(GL_LIGHT0, GL_POSITION, light_position);
      break;

    case 'Z':
      light_position[0] += light_speed;
      glLightfv(GL_LIGHT0, GL_POSITION, light_position);
      break;

    case 'x':
      light_position[1] -= light_speed;
      glLightfv(GL_LIGHT0, GL_POSITION, light_position);
      break;

    case 'X':
      light_position[1] += light_speed;
      glLightfv(GL_LIGHT0, GL_POSITION, light_position);
      break;

    case 'c':
      light_position[2] -= light_speed;
      glLightfv(GL_LIGHT0, GL_POSITION, light_position);
      break;

    case 'C':
      light_position[2] += light_speed;
      glLightfv(GL_LIGHT0, GL_POSITION, light_position);
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
