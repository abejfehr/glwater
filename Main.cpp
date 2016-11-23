/**
 * The main entry point for the GLWater Project.
 *
 * @author Abe Fehr
 */
#include "GLWater.h"

int main(int argc, char** argv) {
  // Make an instance of the app and start it
  GLWater app(argc, argv);
  app.start();

  return 0;
}
