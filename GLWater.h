#ifndef GL_WATER
#define GL_WATER

class GLWater {

  public:
    GLWater(int, char**);
    ~GLWater();

    void initOpenGL();
    void initWindow(int, char**);
    void initWorld();
    void initLights();

    static void handleDisplay();
    static void handleUpdate(int);
    static void handleResize(int, int);
    static void handleMouseClick(int, int, int ,int);
    static void handleMouseMove(int, int);
    static void handleKeyboard(unsigned char, int, int);

    void start();

};

#endif
