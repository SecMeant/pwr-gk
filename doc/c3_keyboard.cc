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

int main (int argc, char **argv)
{
  // ...

  glutMouseFunc(Mouse);
  glutMotionFunc(Motion);

  // ...
}
