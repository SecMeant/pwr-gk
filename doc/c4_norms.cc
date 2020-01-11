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
