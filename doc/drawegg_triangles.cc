void _draweggtri()
{
  glBegin(GL_TRIANGLE_STRIP);

  for (size_t j = 0; j < N - 1; ++j) {
    for (size_t i = 0; i < N - 1; ++i) {
      glVertex3fv(eggpoints[j][i]);

      \\ ith point with the very next to it on X axis
      glVertex3fv(eggpoints[j][i+1]);

      \\ the very next (to the first one) but on Y axis
      glVertex3fv(eggpoints[j+1][i]);

      \\ beggining of the next iteration
      glVertex3fv(eggpoints[j+1][i]);
    }
  }

  glEnd();
}
