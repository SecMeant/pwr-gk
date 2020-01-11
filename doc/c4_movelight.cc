void
Keyboard(unsigned char key, int x, int y)
{
  constexpr float light_speed = 0.1; // ;]

  switch (key) {
    case 'a':
      alphal1 += light_speed;
      break;

    case 'A':
      alphal2 += light_speed;
      break;

    case 'f':
      alphal1 -= light_speed;
      break;

    case 'F':
      alphal2 -= light_speed;
      break;

    case 's':
      betal1 -= light_speed;
      if (betal1 <= -2.0 * M_PI) betal1 = 0;
      break;

    case 'S':
      betal2 -= light_speed;
      if (betal2 <= -2.0 * M_PI) betal2 = 0;
      break;

    case 'd':
      betal1 += light_speed;
      if (betal1 >= 2.0 * M_PI) betal1 = 0.0;
      break;

    case 'D':
      betal2 += light_speed;
      if (betal2 >= 2.0 * M_PI) betal2 = 0.0;
      break;
  }
  RenderScene();
}
