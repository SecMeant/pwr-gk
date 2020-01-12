void draw_egg()
{
  glBegin(GL_TRIANGLES);
  for (size_t j = 0; j < (N - 1) / 2; ++j) {
    for (size_t i = 0; i < N; i+=2) {
      glTexCoord2f(0.5f, 0.0f);
      glVertex3fv(eggpoints[j][i + 2]);

      glTexCoord2f(0.0f, 0.0f);
      glVertex3fv(eggpoints[j][i]);

      glTexCoord2f(0.0f, 1.0f);
      glVertex3fv(eggpoints[j + 1][i]);

      glTexCoord2f(0.0f, 1.0f);
      glVertex3fv(eggpoints[j + 1][i]);

      glTexCoord2f(0.5f, 1.0f);
      glVertex3fv(eggpoints[j + 1][i + 2]);

      glTexCoord2f(0.5f, 0.0f);
      glVertex3fv(eggpoints[j][i + 2]);
    }
  }

  for (size_t j = (N-1) / 2; j < N - 1; ++j) {
    for (size_t i = 0; i < N; i+=2) {
      glTexCoord2f(0.5f, 0.0f);
      glVertex3fv(eggpoints[j][i]);

      glTexCoord2f(1.0f, 0.0f);
      glVertex3fv(eggpoints[j][i + 2]);

      glTexCoord2f(0.5f, 1.0f);
      glVertex3fv(eggpoints[j + 1][i]);

      glTexCoord2f(1.0f, 1.0f);
      glVertex3fv(eggpoints[j + 1][i + 2]);

      glTexCoord2f(0.5f, 1.0f);
      glVertex3fv(eggpoints[j + 1][i]);

      glTexCoord2f(1.0f, 0.0f);
      glVertex3fv(eggpoints[j][i + 2]);
    }
  }
  glEnd();
}
