#ifdef _WIN32
#include <GL/glut.h>
#define FRAME_MS 16.666*2
#else
#include <GLUT/glut.h>
#define FRAME_MS 16.666
#endif

#include <iostream>

#include "GLWater.h"
#include "Water.h"
#include "Ground.h"
#include "Skybox.h"

/**
 * Simulates a water's surface using OpenGL. Loosely follows a few of the
 * principles in these places:
 *
 * Water:
 * http://stackoverflow.com/questions/30661266/water-rendering-in-opengl
 * http://zejiazheng.com/?p=87
 * http://matthias-mueller-fischer.ch/talks/GDC2008.pdf
 * http://www.darwin3d.com/gdm1999.htm
 * http://www.delphigroups.info/2/5/312304.html
 *
 * Rotating camera:
 * http://stackoverflow.com/questions/1426415/opengl-moving-camera-with-mouse
 *
 * Textures:
 * https://sidvind.com/wiki/Skybox_tutorial
 * https://learnopengl.com/#!Advanced-OpenGL/Cubemaps
 *
 * @author: Abe Fehr
 */

// The various things that are in the world
Water water;
Ground ground;
Skybox skybox;

// Camera things
double camera_angle_h = 15;
double camera_angle_v = 30;
int drag_x_origin;
int drag_y_origin;
int dragging = 0;

int lighting = 0;
int reflection = 0;

// Used to store random values
int i, j; // Counters; used in like, every loop

GLWater::GLWater(int argc, char** argv) {
  // Create a window
  initWindow(argc, argv);

  // Initialize OpenGL
  initOpenGL();

  // Create the things we'll need
  initWorld();

  // Create the light sources
  initLights();

  // Bind the various callback functions
  glutReshapeFunc(handleResize);
  glutDisplayFunc(handleDisplay);
  glutMouseFunc(handleMouseClick);
  glutMotionFunc(handleMouseMove);
  glutKeyboardFunc(handleKeyboard);
  glutTimerFunc(FRAME_MS, handleUpdate, 16);
}

GLWater::~GLWater() {
  std::cout << "Exiting";
}

/**
 * Starts the program
 */
void GLWater::start() {
  // Start the loop
  glutMainLoop();
}

/**
 * Initializes a GLUT window
 */
void GLWater::initWindow(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
  glutInitWindowSize(800, 400);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("GLWater");
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

/**
 * Initializes OpenGL
 */
void GLWater::initOpenGL() {
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

/**
 * Initializes all the objects in the world
 */
void GLWater::initWorld() {
  water.init();
  skybox.init();
}

/**
 * Initializes the lighting in the world
 */
void GLWater::initLights() {
  // Initialize the light colours
  GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
  GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_specular[] = { 1.0, 1.0, 1.0, 0.4 };
  GLfloat light_position[] = { 0.0, 1.0, 1.5, 1.0 };
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  // Enable attenuation
  // glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION , 0.2f );

  // Enable the light
  if (lighting % 2 == 0) {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
  }

}

/**
 * The callback function which resizes the canvas.
 */
void GLWater::handleResize(int width, int height) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(20, width / (float) height, 0.1, 15);
  glViewport(0, 0, width, height);
  glMatrixMode(GL_MODELVIEW);
  glutPostRedisplay();
}

/**
 * The callback function which updates the world.
 */
void GLWater::handleUpdate(int value) {
  // Update the physics of the water
  water.update();

  // Update the angle of the camera
  glLoadIdentity();
  glTranslatef(0, 0.0f, -4.0f);
  glRotated(camera_angle_v, 1.0, 0.0, 0.0);
  glRotated(camera_angle_h, 0.0, 1.0, 0.0);

  // Update the status of the lighting
  if (lighting % 2 == 0) {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
  }

  // Draw this frame and schedule the next
  glutPostRedisplay();
  glutTimerFunc(FRAME_MS, handleUpdate, 32);
}

void GLWater::handleKeyboard(unsigned char key, int x, int y) {
  water.handleKeyPressed(key);

  switch (key) {
    case '2':
      lighting++;
      break;
    case '3':
      reflection++;
      break;
    case 'q':
      exit(1);
  }

}

void GLWater::handleMouseClick(int button, int state, int x, int y) {
  if(button == GLUT_LEFT_BUTTON) {
    if(state == GLUT_DOWN) {
      dragging = 1;
      drag_x_origin = x;
      drag_y_origin = y;
    } else {
      dragging = 0;
    }
  }
}

void GLWater::handleMouseMove(int x, int y) {
  if(dragging) {
    camera_angle_v += (y - drag_y_origin)*0.3;
    camera_angle_h += (x - drag_x_origin)*0.3;
    drag_x_origin = x;
    drag_y_origin = y;
  }
}

/**
 * The callback function which draws simulation.
 */
void GLWater::handleDisplay() {
  // Clear the screen
  if (reflection % 2 == 0) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  } else {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  // Draw the ground
  // ground.render();

  if (reflection % 2 == 0) {
    // Don't update the colour or the depth
    glDisable(GL_DEPTH_TEST);
    glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);

    // Draw 1 into the stencil buffer
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xffffffff);

    // Now drawing the floor just tags the pixels as stencil value 1
    water.render(1);

    // Re-enable the update of the color and depth
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
    glEnable(GL_DEPTH_TEST);

    // Only render where the stencil is set to 1
    glStencilFunc(GL_EQUAL, 1, 0xffffffff);
    glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);

    // Draw the reflected sky piece, but only where the water is
    skybox.renderReflection();

    glDisable(GL_STENCIL_TEST);
  }

  // Draw the water
  water.render(0);

  // Draw the actual skybox
  skybox.render();

  glutSwapBuffers();
  glutPostRedisplay();
}
