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
    for (j = 0; j < WIDTH; ++j) {\
	  // Do the show that is calculating normals
	  int up, down, left, right;
	  up = i + 1 > HEIGHT - 1 ? 0 : i + 1;
	  down = i - 1 < 0 ? HEIGHT - 1 : i - 1;
	  right = j + 1 > HEIGHT - 1 ? 0 : j + 1;
	  left = j - 1 < 0 ? WIDTH - 1 : j - 1;

	  // Compute the normals by averaging out the normals between each vertex and its 4 neighbours
	  Vector3f n1 = Vector3f((-float(B / 2) + (i - 1) * (B / (HEIGHT - 1))) - (-float(B / 2) + i * (B / (HEIGHT - 1))), normals[down][j] * AMPLIFICATION - normals[i][j] * AMPLIFICATION, (-float(A / 2) + (j + 0) * (A / (WIDTH - 1))) - (-float(A / 2) + j * (A / (WIDTH - 1))));
	  Vector3f e1 = Vector3f((-float(B / 2) + (i - 0) * (B / (HEIGHT - 1))) - (-float(B / 2) + i * (B / (HEIGHT - 1))), normals[i][right] * AMPLIFICATION - normals[i][j] * AMPLIFICATION, (-float(A / 2) + (j + 1) * (A / (WIDTH - 1))) - (-float(A / 2) + j * (A / (WIDTH - 1))));
	  Vector3f s1 = Vector3f((-float(B / 2) + (i + 1) * (B / (HEIGHT - 1))) - (-float(B / 2) + i * (B / (HEIGHT - 1))), normals[up][j] * AMPLIFICATION - normals[i][j] * AMPLIFICATION, (-float(A / 2) + (j + 0) * (A / (WIDTH - 1))) - (-float(A / 2) + j * (A / (WIDTH - 1))));
	  Vector3f w1 = Vector3f((-float(B / 2) + (i - 0) * (B / (HEIGHT - 1))) - (-float(B / 2) + i * (B / (HEIGHT - 1))), normals[i][left] * AMPLIFICATION - normals[i][j] * AMPLIFICATION, (-float(A / 2) + (j - 1) * (A / (WIDTH - 1))) - (-float(A / 2) + j * (A / (WIDTH - 1))));

	  Vector3f normal1_ne = Vector3f::cross(n1, e1);
	  Vector3f normal1_nw = Vector3f::cross(n1, w1);
	  Vector3f normal1_se = Vector3f::cross(s1, e1);
	  Vector3f normal1_sw = Vector3f::cross(s1, w1);

	  Vector3f normal1 = Vector3f::normalize(normal1_ne + normal1_nw + normal1_se + normal1_sw);

	  up = i + 2 > HEIGHT - 1 ? (i + 1 > HEIGHT - 1 ? 1 : 0) : i + 1;
	  down = i;

	  Vector3f n2 = Vector3f((-float(B / 2) + (i - 0) * (B / (HEIGHT - 1))) - (-float(B / 2) + i * (B / (HEIGHT - 1))), normals[down][j] * AMPLIFICATION - normals[i][j] * AMPLIFICATION, (-float(A / 2) + (j + 0) * (A / (WIDTH - 1))) - (-float(A / 2) + j * (A / (WIDTH - 1))));
	  Vector3f e2 = Vector3f((-float(B / 2) + (i + 1) * (B / (HEIGHT - 1))) - (-float(B / 2) + i * (B / (HEIGHT - 1))), normals[i][right] * AMPLIFICATION - normals[i][j] * AMPLIFICATION, (-float(A / 2) + (j + 1) * (A / (WIDTH - 1))) - (-float(A / 2) + j * (A / (WIDTH - 1))));
	  Vector3f s2 = Vector3f((-float(B / 2) + (i + 2) * (B / (HEIGHT - 1))) - (-float(B / 2) + i * (B / (HEIGHT - 1))), normals[up][j] * AMPLIFICATION - normals[i][j] * AMPLIFICATION, (-float(A / 2) + (j + 0) * (A / (WIDTH - 1))) - (-float(A / 2) + j * (A / (WIDTH - 1))));
	  Vector3f w2 = Vector3f((-float(B / 2) + (i + 1) * (B / (HEIGHT - 1))) - (-float(B / 2) + i * (B / (HEIGHT - 1))), normals[i][left] * AMPLIFICATION - normals[i][j] * AMPLIFICATION, (-float(A / 2) + (j - 1) * (A / (WIDTH - 1))) - (-float(A / 2) + j * (A / (WIDTH - 1))));

	  Vector3f normal2_ne = Vector3f::cross(n2, e2);
	  Vector3f normal2_nw = Vector3f::cross(n2, w2);
	  Vector3f normal2_se = Vector3f::cross(s2, e2);
	  Vector3f normal2_sw = Vector3f::cross(s2, w2);

	  Vector3f normal2 = Vector3f::normalize(normal2_ne + normal2_nw + normal2_se + normal2_sw);

	  glNormal3f(
		normal1.x,
		normal1.y,
		normal1.z
      );
      glVertex3f(
        -float(A / 2) + j * (A / (WIDTH - 1)),
        SURFACE_HEIGHT + normals[i][j] * AMPLIFICATION,
        -float(B / 2) + (i + 0) * (B / (HEIGHT - 1))
      );
	  glNormal3f(
		normal2.x,
		normal2.y,
		normal2.z
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
