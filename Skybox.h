#ifdef _WIN32
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include <iostream>

#include "includes/SOIL.h"

class Skybox {
  public:
    Skybox();
    ~Skybox();
    void init();
    void render();
    void renderReflection();
  private:
    GLuint textures[6];
};
