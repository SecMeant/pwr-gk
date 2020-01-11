void cache_colors()
{
  // Initialization of egg colors arrray
  for (size_t j = 0; j < N; ++j) {
    for (size_t i = 0; i < N; ++i) {
      Color c = RandomColor();
      eggpoints_colo[j][i][0] = c.r;
      eggpoints_colo[j][i][1] = c.g;
      eggpoints_colo[j][i][2] = c.b;
    }
  }
}

void drawWithColor(size_t j, size_t i)
{
  glColor3f(eggpoints_colo[j][i][0], eggpoints_colo[j][i][1], eggpoints_colo[j][i][2]);
  glVertex3fv(eggpoints[j][i]);
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
