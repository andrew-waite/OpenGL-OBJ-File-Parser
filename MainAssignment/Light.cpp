#include "Light.h"
#include "glut.h"


LightMatParent::LightMatParent()
{
  // Parent constructor just puts stuff in for amb, diff, spec
  ambient[0] = 0.5;
  ambient[1] = 0.5;
  ambient[2] = 0.5;
  ambient[3] = 1.0;
  diffuse[0] = 0.5;
  diffuse[1] = 0.5;
  diffuse[2] = 0.5;
  diffuse[3] = 1.0;
  specular[0] = 0.5;
  specular[1] = 0.5;
  specular[2] = 0.5;
  specular[3] = 1.0;
}

void LightMatParent::setAmbient(float ambR, float ambG, float ambB)
{
  ambient[0] = ambR;
  ambient[1] = ambG;
  ambient[2] = ambB;
  ambient[3] = 1.0;
}

void LightMatParent::setDiffuse(float difR, float difG, float difB)
{
  diffuse[0] = difR;
  diffuse[1] = difG;
  diffuse[2] = difB;
  diffuse[3] = 1.0;
}

void LightMatParent::setSpecular(float specR, float specG, float specB)
{
  specular[0] = specR;
  specular[1] = specG;
  specular[2] = specB;
  specular[3] = 1.0;
}


Light::Light() : LightMatParent()
{
  // Constructor puts light up x-axis...
  position[0] = 1.0f;
  position[1] = 0.0f;
  position[2] = 0.0f;
  position[3] = 0.0f;
  // ... and sets a bright white light as default...
  ambient[0] = 0.2f;
  ambient[1] = 0.2f;
  ambient[2] = 0.2f;
  ambient[3] = 1.0f;
  diffuse[0] = 0.7f;
  diffuse[1] = 0.7f;
  diffuse[2] = 0.7f;
  diffuse[3] = 1.0f;
  specular[0] = 0.3f;
  specular[1] = 0.3f;
  specular[2] = 0.3f;
  specular[3] = 1.0f;
}

void Light::setPosition(float posX, float posY, float posZ, float posW)
{
  position[0] = posX;
  position[1] = posY;
  position[2] = posZ;
  position[3] = posW;
}


Material::Material() : LightMatParent()
{
  // Constructor sets white material as default...
  ambient[0] = 0.8f;
  ambient[1] = 0.8f;
  ambient[2] = 0.8f;
  ambient[3] = 1.0f;
  diffuse[0] = 0.8f;
  diffuse[1] = 0.8f;
  diffuse[2] = 0.8f;
  diffuse[3] = 1.0f;
  specular[0] = 0.6f;
  specular[1] = 0.6f;
  specular[2] = 0.6f;
  specular[3] = 1.0f;
	shininess = 32;
}

void Material::setShininess(float shineValue)
{
  shininess = shineValue;
}

void Material::setAsCurrentMaterial()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}