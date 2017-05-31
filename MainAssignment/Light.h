#ifndef _LIGHT_H_
#define _LIGHT_H_


// Define parent class that lights & materials can inherit from...
class LightMatParent
{
public:
  float ambient[4];
  float diffuse[4];
  float specular[4];
  LightMatParent();
  void setAmbient (float ambR, float ambG, float ambB);
  void setDiffuse (float difR, float difG, float difB);
  void setSpecular (float specR, float specG, float specB);
};

// Lights additionally have position.
// Add other attributes as needed.
class Light : public LightMatParent
{
public:
  float position[4];
  Light();
  void setPosition(float posX, float posY, float posZ, float posW);
};

// Materials additionally have shininess.
// Add other attributes as needed.
class Material : public LightMatParent
{
public:
  float shininess;
  Material();
  void setShininess(float shineValue);
	void setAsCurrentMaterial();
};
       
                                                                                

#endif 
