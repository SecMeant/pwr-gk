struct point3d
{
  GLfloat x, y, z;
};

void
draw_tri(point3d p1, point3d p2, point3d p3)
{
  glBegin(GL_TRIANGLES);
  glNormal3f(1.0f, 0.0f, 0.0f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(p1.x, p1.y, p1.z);

  glNormal3f(1.0f, 0.0f, 0.0f);
  glTexCoord2f(0.5f, 0.0f);
  glVertex3f(p2.x, p2.y, p2.z);

  glNormal3f(1.0f, 0.0f, 0.0f);
  glTexCoord2f(0.5f, 1.0f);
  glVertex3f(p3.x, p3.y, p3.z);
  glEnd();
}

void
draw_tri()
{
  draw_tri({ 0.0f, 0.0f }, { 5.0f, 0.0f }, { 0.0f, 5.0f });
}

void
draw_pyra()
{
  draw_tri({ 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 2.5f, 4.0f, -2.5f });
  draw_tri({ 0.0f, 0.0f, -5.0f }, { 0.0f, 0.0f, 0.0f }, { 2.5f, 4.0f, -2.5f });
  draw_tri({ 5.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, -5.0f }, { 2.5f, 4.0f, -2.5f });
  draw_tri({ 5.0f, 0.0f, -5.0f }, { 0.0f, 0.0f, -5.0f }, { 2.5f, 4.0f, -2.5f });

  // base
  draw_tri({ 5.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -5.0f });
  draw_tri({ 5.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -5.0f }, { 5.0f, 0.0f, -5.0f });
}
