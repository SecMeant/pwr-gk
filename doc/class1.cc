struct Color
{
  GLfloat r,g,b;
};

struct Square
{
  // Rad is short for radius.
  // Yes, Im aware that its not radius.
  GLfloat midx, midy, rad;
  Color color;
};

constexpr Color bgcolor{1.0f, 1.0f, 1.0f};
constexpr Color red{1.0f, 0.0f, 0.0f};
constexpr Color green{0.0f, 1.0f, 0.0f};
constexpr Color blue{0.0f, 0.0f, 1.0f};

constexpr GLfloat RAND_MAXf = (double) RAND_MAX;

Color randomColor()
{
  return {rand() / RAND_MAXf, rand() / RAND_MAXf, rand() / RAND_MAXf};
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

// Set random color
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
  sq.color = bgcolor;
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

    while (sq.midy >= sqbase.midy - sqbase.rad)
    {

      while (sq.midx <= sqbase.midx + sqbase.rad)
      {
        RemoveSquareMid(sq);
        sq.midx += off;
      }

      sq.midx = midx_start;
      sq.midy -= off;
    }

    sq.midy = midy_start;
  }

}

void RenderScene(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  Square sq{0.0f, 0.0f, 80.0f, bgcolor};
  Sierpinski(sq, 0.01f);
  //SierpinskiIter(sq, 5);
  glFlush();
}

