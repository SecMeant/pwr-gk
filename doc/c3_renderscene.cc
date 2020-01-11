void
RenderScene(void)
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  auto vec = 1.0;

  viewer[0] = R * cos(alpha) * cos(beta) + lookpoint_x;
  viewer[1] = R * sin(beta) + lookpoint_y;
  viewer[2] = R * sin(alpha) * cos(beta) + lookpoint_z;

  // Flip vector sign if between 180 and 279 degrees
  // to prevent flipping of rendered image.
  if (beta >= (M_PI / 2.0) && beta <= (M_PI * 1.5))
    vec = -1.0;

  else if (beta <= -(M_PI / 2.0) && beta >= -(M_PI * 1.5))
    vec = -1.0;

  gluLookAt(viewer[0],
            viewer[1],
            viewer[2],
            lookpoint_x,
            lookpoint_y,
            lookpoint_z,
            0.0,
            vec,
            0.0);

  Axes();

  if (status == 1) {
    thetax += delta_x * pix2anglex;
    thetay += delta_y * pix2angley;
  }

  glRotatef(thetax, 0.0, 1.0, 0.0);
  glRotatef(thetay, 1.0, 0.0, 0.0);
  glScalef(scale * 1.0, scale * 1.0, scale * 1.0);
  glTranslatef(offset_x, offset_y, 0.0);

  glColor3f(1.0f, 1.0f, 1.0f);

  glutWireTeapot(3.0);

  glFlush();

  glutSwapBuffers();
}
