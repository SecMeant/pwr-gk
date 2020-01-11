constexpr int MODEL_POINTS = 1;
constexpr int MODEL_MESH = 2;
constexpr int MODEL_TRIANGLE = 3;
constexpr int MODEL_TRIANGLE_FULL = 4;
int model = MODEL_POINTS;

void RenderScene()
{
  // ...

  switch(model)
  {
    case MODEL_POINTS:
      _drawegg();
      break;
    case MODEL_MESH:
      _drawegg();
      break;
    case MODEL_TRIANGLE_FULL:
      _draweggtricolo();
    case MODEL_TRIANGLE:
    default:
      _draweggtri();
      break;
  }

  // ...
}

void keys(unsigned char key, int x, int y)
{
  if(key == 'p') model = MODEL_POINTS;
  if(key == 'm') model = MODEL_MESH;
  if(key == 't') model = MODEL_TRIANGLE;
  if(key == 'f') model = MODEL_TRIANGLE_FULL;

  RenderScene(); // przerysowanie obrazu sceny
}

int main (int argc, char **argv)
{
 // ...
  glutKeyboardFunc(keys);
 // ...
}
