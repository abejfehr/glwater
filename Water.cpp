#include <GL/glut.h>
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
  // Initialize the textures
  texture = SOIL_load_OGL_texture(
    "ocean.jpg",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
  );
  if (texture == 0) {
    std::cout << "Texture was not properly loaded.\n";
  }

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Initialize the normals
  normals = new double*[HEIGHT];
  velocities = new double*[HEIGHT];
  for(i = 0; i < HEIGHT; ++i) {
    normals[i] = new double[WIDTH];
    velocities[i] = new double[WIDTH];
    for (j = 0; j < WIDTH; ++j) {
      normals[i][j] = 0;
      velocities[i][j] = 0;
      k = rand() % 10 + 1;
      m = rand() % 10 + 1;
      if ((i * WIDTH + j) % 10 == k) {
        if (i != 0 && i != WIDTH - 1 && j != 0 && j != HEIGHT - 1) {
          normals[i][j] = 0.05;
        }
      }
      if ((i * WIDTH + j) % 10 == m) {
        if (i != 0 && i != WIDTH - 1 && j != 0 && j != HEIGHT - 1) {
          normals[i][j] = -0.05;
        }
      }
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
      velocities[i][j] += (n+e+s+w+ne+nw+se+sw)/12 - normals[i][j];
      velocities[i][j] = velocities[i][j] * damping;

      if (normals[i][j] > 0.1) {
        normals[i][j] = 0.1;
      } else if (normals[i][j] < -0.1) {
        normals[i][j] = -0.1;
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

void Water::render() {
  GLfloat white[] = { 0.1f, 0.1f, 0.7f, 0.3f };
  GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };

  A = 2; // ACTUAL WIDTH
  B = 2; // ACTUAL HEIGHT
  if (fillMode % 2 == 1) {
    glPolygonMode(GL_FRONT, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT, GL_FILL);
  }

  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glMaterialfv(GL_FRONT, GL_SPECULAR, white);
  glMaterialfv(GL_FRONT, GL_EMISSION, black);
  glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);

  glEnable(GL_TEXTURE_2D);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glColor4f(0.4, 0.4, 0.7, 0.6);
  for (i = 0; i < HEIGHT - 1; ++i) {
    glBegin(GL_TRIANGLE_STRIP);
    for (j = 0; j < WIDTH; ++j) {
      glTexCoord2f(
        1 * float(j) / float(WIDTH - 1),
        1 * float(i) / float(HEIGHT - 1)
      );
      glNormal3f(
        -float(A / 2) + j * (A / (WIDTH - 1)),
        SURFACE_HEIGHT + normals[i][j],
        -float(B / 2) + (i + 0) * (B / (HEIGHT - 1))
      );
      glVertex3f(
        -float(A / 2) + j * (A / (WIDTH - 1)),
        SURFACE_HEIGHT + normals[i][j],
        -float(B / 2) + (i + 0) * (B / (HEIGHT - 1))
      );
      glTexCoord2f(
        1 * float(j) / float(WIDTH - 1),
        1 * float(i + 1) / float(HEIGHT - 1)
      );
      glNormal3f(
        -float(A / 2) + j * (A / (WIDTH - 1)),
        SURFACE_HEIGHT + normals[i + 1][j],
        -float(B / 2) + (i + 1) * (B / (HEIGHT - 1))
      );
      glVertex3f(
        -float(A / 2) + j * (A / (WIDTH - 1)),
        SURFACE_HEIGHT + normals[i + 1][j],
        -float(B / 2) + (i + 1) * (B / (HEIGHT - 1))
      );
    }
    glEnd();
  }
  glDisable(GL_TEXTURE_2D); // Comment out later
}

void Water::handleKeyPressed(unsigned char key) {
  switch (key) {
    case ' ':
      // Make a wave in the center of the water
      normals[HEIGHT / 2][WIDTH / 2] += 0.15;
      break;
    case '1':
      ++fillMode;
      break;
    default:
      exit(0);
      break;
  }
}
