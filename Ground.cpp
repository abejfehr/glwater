#ifdef _WIN32
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include <iostream>

#include "Ground.h"

void Ground::render() {
  glPolygonMode(GL_FRONT, GL_FILL);
  GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glMaterialfv(GL_FRONT, GL_SPECULAR, black);
  glMaterialfv(GL_FRONT, GL_EMISSION, black);
  glMaterialf(GL_FRONT, GL_SHININESS, 128.0f);

  glBegin(GL_TRIANGLE_FAN);
  glColor3f(1.0, 0.8, 0.7);
  glVertex3f(-1, 0, -1);
  glVertex3f(-1, 0,  1);
  glVertex3f( 1, 0,  1);
  glVertex3f( 1, 0, -1);
  glEnd();
}
