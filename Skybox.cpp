#ifdef _WIN32
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include <iostream>

#include "includes/SOIL.h"
#include "Skybox.h"

float HEIGHT = 0.1f;

Skybox::Skybox() {

}

Skybox::~Skybox() {

}

void Skybox::init() {
  for (int i = 0; i < 6; ++i) {
    textures[i] = 0;
    glGenTextures(1, &(textures[i]));
    std::string filename = "assets/skybox-";
    filename.push_back(i + '0');
    filename += ".jpg";
    const char *thingy = filename.c_str();
    std::cout << "Loading texture: " << thingy << "\n";
    textures[i] = SOIL_load_OGL_texture(
      thingy,
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
    if (textures[i] == 0) {
      std::cout << "Texture was not properly loaded.\n";
    }
    glBindTexture(GL_TEXTURE_2D, textures[i]);
  }
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

void Skybox::render() {
  // Store the current matrix
  glPushMatrix();

  // Enable/disable features
  glPushAttrib(GL_ENABLE_BIT);
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glDisable(GL_BLEND);

  // Always render the skybox in fill mode
  glPolygonMode(GL_FRONT, GL_FILL);

  // Set all vertices to blue, for funsies
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

  // Render the front quad
  glBindTexture(GL_TEXTURE_2D, textures[5]);
  glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(  5.0f, -5.0f + HEIGHT, -5.0f );
    glTexCoord2f(1, 0);
    glVertex3f( -5.0f, -5.0f + HEIGHT, -5.0f );
    glTexCoord2f(1, 1);
    glVertex3f( -5.0f,  5.0f + HEIGHT, -5.0f );
    glTexCoord2f(0, 1);
    glVertex3f(  5.0f,  5.0f + HEIGHT, -5.0f );
  glEnd();

  // Render the left quad
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glBegin(GL_QUADS);
      glTexCoord2f(0, 0);
      glVertex3f(  5.0f, -5.0f + HEIGHT,  5.0f );
      glTexCoord2f(1, 0);
      glVertex3f(  5.0f, -5.0f + HEIGHT, -5.0f );
      glTexCoord2f(1, 1);
      glVertex3f(  5.0f,  5.0f + HEIGHT, -5.0f );
      glTexCoord2f(0, 1);
      glVertex3f(  5.0f,  5.0f + HEIGHT,  5.0f );
  glEnd();

  // Render the back quad
  glBindTexture(GL_TEXTURE_2D, textures[4]);
  glBegin(GL_QUADS);
      glTexCoord2f(0, 0); glVertex3f( -5.0f, -5.0f + HEIGHT,  5.0f );
      glTexCoord2f(1, 0); glVertex3f(  5.0f, -5.0f + HEIGHT,  5.0f );
      glTexCoord2f(1, 1); glVertex3f(  5.0f,  5.0f + HEIGHT,  5.0f );
      glTexCoord2f(0, 1); glVertex3f( -5.0f,  5.0f + HEIGHT,  5.0f );

  glEnd();

  // Render the right quad
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glBegin(GL_QUADS);
      glTexCoord2f(0, 0); glVertex3f( -5.0f, -5.0f + HEIGHT, -5.0f );
      glTexCoord2f(1, 0); glVertex3f( -5.0f, -5.0f + HEIGHT,  5.0f );
      glTexCoord2f(1, 1); glVertex3f( -5.0f,  5.0f + HEIGHT,  5.0f );
      glTexCoord2f(0, 1); glVertex3f( -5.0f,  5.0f + HEIGHT, -5.0f );
  glEnd();

  // Render the top quad
  glBindTexture(GL_TEXTURE_2D, textures[2]);
  glBegin(GL_QUADS);
      glTexCoord2f(0, 1);
      glVertex3f( -5.0f,  5.0f + HEIGHT, -5.0f );
      glTexCoord2f(0, 0);
      glVertex3f( -5.0f,  5.0f + HEIGHT,  5.0f );
      glTexCoord2f(1, 0);
      glVertex3f(  5.0f,  5.0f + HEIGHT,  5.0f );
      glTexCoord2f(1, 1);
      glVertex3f(  5.0f,  5.0f + HEIGHT, -5.0f );
  glEnd();

  // Render the bottom quad
  glBindTexture(GL_TEXTURE_2D, textures[3]);
  glBegin(GL_QUADS);
      glTexCoord2f(0, 0); glVertex3f( -5.0f, -5.0f + HEIGHT, -5.0f );
      glTexCoord2f(0, 1); glVertex3f( -5.0f, -5.0f + HEIGHT,  5.0f );
      glTexCoord2f(1, 1); glVertex3f(  5.0f, -5.0f + HEIGHT,  5.0f );
      glTexCoord2f(1, 0); glVertex3f(  5.0f, -5.0f + HEIGHT, -5.0f );
  glEnd();

  // Restore enable bits and matrix
  glPopAttrib();
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);

}

void Skybox::renderReflection() {
  // Store the current matrix
  glPushMatrix();

  glScalef(1.0f, -1.0f, 1.0f);

  // Enable/disable features
  glPushAttrib(GL_ENABLE_BIT);
  glEnable(GL_TEXTURE_2D);
  // glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glDisable(GL_BLEND);

  // Always render the skybox in fill mode
  glPolygonMode(GL_FRONT, GL_FILL);

  // Set all vertices to white
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

  // Render the front quad
  glBindTexture(GL_TEXTURE_2D, textures[5]);
  glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(  5.0f, -5.0f + HEIGHT, -5.0f );
    glTexCoord2f(1, 0);
    glVertex3f( -5.0f, -5.0f + HEIGHT, -5.0f );
    glTexCoord2f(1, 1);
    glVertex3f( -5.0f,  5.0f + HEIGHT, -5.0f );
    glTexCoord2f(0, 1);
    glVertex3f(  5.0f,  5.0f + HEIGHT, -5.0f );
  glEnd();

  // Render the left quad
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glBegin(GL_QUADS);
      glTexCoord2f(0, 0);
      glVertex3f(  5.0f, -5.0f + HEIGHT,  5.0f );
      glTexCoord2f(1, 0);
      glVertex3f(  5.0f, -5.0f + HEIGHT, -5.0f );
      glTexCoord2f(1, 1);
      glVertex3f(  5.0f,  5.0f + HEIGHT, -5.0f );
      glTexCoord2f(0, 1);
      glVertex3f(  5.0f,  5.0f + HEIGHT,  5.0f );
  glEnd();

  // Render the back quad
  glBindTexture(GL_TEXTURE_2D, textures[4]);
  glBegin(GL_QUADS);
      glTexCoord2f(0, 0); glVertex3f( -5.0f, -5.0f + HEIGHT,  5.0f );
      glTexCoord2f(1, 0); glVertex3f(  5.0f, -5.0f + HEIGHT,  5.0f );
      glTexCoord2f(1, 1); glVertex3f(  5.0f,  5.0f + HEIGHT,  5.0f );
      glTexCoord2f(0, 1); glVertex3f( -5.0f,  5.0f + HEIGHT,  5.0f );

  glEnd();

  // Render the right quad
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glBegin(GL_QUADS);
      glTexCoord2f(0, 0); glVertex3f( -5.0f, -5.0f + HEIGHT, -5.0f );
      glTexCoord2f(1, 0); glVertex3f( -5.0f, -5.0f + HEIGHT,  5.0f );
      glTexCoord2f(1, 1); glVertex3f( -5.0f,  5.0f + HEIGHT,  5.0f );
      glTexCoord2f(0, 1); glVertex3f( -5.0f,  5.0f + HEIGHT, -5.0f );
  glEnd();

  // Render the top quad
  glBindTexture(GL_TEXTURE_2D, textures[2]);
  glBegin(GL_QUADS);
      glTexCoord2f(0, 1);
      glVertex3f( -5.0f,  5.0f + HEIGHT, -5.0f );
      glTexCoord2f(0, 0);
      glVertex3f( -5.0f,  5.0f + HEIGHT,  5.0f );
      glTexCoord2f(1, 0);
      glVertex3f(  5.0f,  5.0f + HEIGHT,  5.0f );
      glTexCoord2f(1, 1);
      glVertex3f(  5.0f,  5.0f + HEIGHT, -5.0f );
  glEnd();

  // Render the bottom quad
  glBindTexture(GL_TEXTURE_2D, textures[3]);
  glBegin(GL_QUADS);
      glTexCoord2f(0, 0); glVertex3f( -5.0f, -5.0f + HEIGHT, -5.0f );
      glTexCoord2f(0, 1); glVertex3f( -5.0f, -5.0f + HEIGHT,  5.0f );
      glTexCoord2f(1, 1); glVertex3f(  5.0f, -5.0f + HEIGHT,  5.0f );
      glTexCoord2f(1, 0); glVertex3f(  5.0f, -5.0f + HEIGHT, -5.0f );
  glEnd();

  // Restore enable bits and matrix
  glPopAttrib();
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);

}
