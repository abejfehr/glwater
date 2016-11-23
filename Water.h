class Water {
  public:
    Water();
    ~Water();
    void init();
    void update();
    void render();
    void handleKeyPressed(unsigned char);
  private:
    int WIDTH = 80;
    int HEIGHT = 80;
    double SURFACE_HEIGHT = 0.2;
    double** normals;
    double** velocities;
    int i, j, m, k; // Counters; used in like, every loop
    GLuint texture;
    double damping = 0.995;
    double ne,nw,se,sw,n,e,s,w;
    double A,B;
    int fillMode; // Whether to show a line or a filled thingy
};
