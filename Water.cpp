#ifdef _WIN32
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include <iostream>

#include "includes/SOIL.h"
#include "Water.h"

Water::Water() {
}

Water::~Water() {
  if (normals) {
    delete [] normals;
  }
  if (velocities) {
    delete [] velocities;
  }
}

void Water::init() {
  // Initialize the normals
  normals = new double*[HEIGHT];
  velocities = new double*[HEIGHT];
  for(i = 0; i < HEIGHT; ++i) {
    normals[i] = new double[WIDTH];
    velocities[i] = new double[WIDTH];
    for (j = 0; j < WIDTH; ++j) {
      normals[i][j] = 0;
      velocities[i][j] = 0;
    }
  }

}

void Water::update() {
  // Go through and update the normals based on everyone's neighbour
  for (i = 0; i < HEIGHT; ++i) {
    for (j = 0; j < WIDTH; ++j) {
      int up, down, left, right;
      up = i + 1 > HEIGHT - 1 ? 0 : i + 1;
      down = i - 1 < 0 ? HEIGHT - 1 : i - 1;
      right = j + 1 > HEIGHT - 1 ? 0 : j + 1;
      left = j - 1 < 0 ? WIDTH - 1 : j - 1;

      ne = normals[up][right];
      nw = normals[up][left];
      se = normals[down][right];
      sw = normals[down][left];
      n = normals[up][j];
      e = normals[i][right];
      s = normals[down][j];
      w = normals[i][left];

      // Calculate the new velocity of the water at that position
      velocities[i][j] += ((n+e+s+w+ne+nw+se+sw) / 8 - normals[i][j]) * 2.0f;
      velocities[i][j] = velocities[i][j] * damping;

      if (normals[i][j] > 0.2) {
        normals[i][j] = 0.2;
      } else if (normals[i][j] < -0.2) {
        normals[i][j] = -0.2;
      }
    }
  }

  // Set the new normals using the velocities
  for (i = 0; i < HEIGHT; ++i) {
    for (j = 0; j < WIDTH; ++j) {
      normals[i][j] += velocities[i][j];
    }
  }
}

void Water::render(int reflection, int color) {
  GLfloat white[] = { 0.1f, 0.1f, 0.7f, 0.3f };
  GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };

  A = 2; // ACTUAL WIDTH
  B = 2; // ACTUAL HEIGHT
  if (fillMode % 2 == 1 && ! reflection) {
    glPolygonMode(GL_FRONT, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT, GL_FILL);
  }

  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glMaterialfv(GL_FRONT, GL_SPECULAR, white);
  glMaterialfv(GL_FRONT, GL_EMISSION, black);
  glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);

  if (! reflection) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.2f, 0.2f, 0.7f, color % 2 == 0 ? 0.1f : 0.8f);
  } else {
    glColor4f(0.2f, 0.2f, 0.7f, 1.0f);
  }

  for (i = 0; i < HEIGHT - 1; ++i) {
    glBegin(GL_TRIANGLE_STRIP);
    for (j = 0; j < WIDTH; ++j) {
      glNormal3f(
        -float(A / 2) + j * (A / (WIDTH - 1)),
        SURFACE_HEIGHT + normals[i][j] * AMPLIFICATION,
        -float(B / 2) + (i + 0) * (B / (HEIGHT - 1))
      );
      glVertex3f(
        -float(A / 2) + j * (A / (WIDTH - 1)),
        SURFACE_HEIGHT + normals[i][j] * AMPLIFICATION,
        -float(B / 2) + (i + 0) * (B / (HEIGHT - 1))
      );
      glNormal3f(
        -float(A / 2) + j * (A / (WIDTH - 1)),
        SURFACE_HEIGHT + normals[i + 1][j] * AMPLIFICATION,
        -float(B / 2) + (i + 1) * (B / (HEIGHT - 1))
      );
      glVertex3f(
        -float(A / 2) + j * (A / (WIDTH - 1)),
        SURFACE_HEIGHT + normals[i + 1][j] * AMPLIFICATION,
        -float(B / 2) + (i + 1) * (B / (HEIGHT - 1))
      );
    }
    glEnd();
  }
  if (! reflection) {
    glDisable(GL_BLEND);
  }
}

void Water::handleKeyPressed(unsigned char key) {
  switch (key) {
    case ' ':
      // Make a wave in the center of the water
      normals[HEIGHT / 2][WIDTH / 2] += 0.1;
      break;
    case '1':
      ++fillMode;
      break;
  }
}
