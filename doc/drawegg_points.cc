void _drawegg()
{
  glBegin(GL_POINTS);

  // If want to draw mesh, use this:
  // glBegin(GL_LINES);

  for (size_t j = 0; j < N - 1; ++j) {
    for (size_t i = 0; i < N - 1; ++i) {

      // Points got calculated and cached in the eggpoints array
      glVertex3fv(eggpoints[j][i]);
      glVertex3fv(eggpoints[j][i+1]);

      glVertex3fv(eggpoints[j][i]);
      glVertex3fv(eggpoints[j+1][i+1]);
    }
  }
  glEnd();
}

