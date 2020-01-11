GLfloat light_ambient[] = { 0.2, 0.0, 0.0, 1.0 };
GLfloat light_diffuse[] = { 3.4, 0.0, 0.0, 1.0 };
GLfloat light_specular[] = { 15.0, 0.0, 0.0, 1.0 };

GLfloat light_ambient2[] = { 0.0, 0.0, 0.2, 1.0 };
GLfloat light_diffuse2[] = { 0.0, 0.0, 2.8, 1.0 };
GLfloat light_specular2[] = { 0.0, 0.0, 15.0, 1.0 };

GLfloat att_constant = { 1.0 };
GLfloat att_linear = { 0.05 };
GLfloat att_quadratic = { 0.001 };

void
MyInit(void)
{
  // ...

  // Configuring first light source
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

  // Configuring second light source
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient2);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse2);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular2);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position2);

  glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant);
  glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear);
  glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic);

  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  // ...
}
