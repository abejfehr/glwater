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
