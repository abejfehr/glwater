class Light {
  public:
    Light();
    ~Light();
    setSpecularComponent(float, float, float, float);
    setDiffuseComponent(float, float, float, float);
    setAmbientComponent(float, float, float, float);
    setPosition(float, float, float);
  private:
    float specular[4];
    float diffuse[4];
    float ambient[4];
    float position[4];
}
