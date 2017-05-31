#ifndef OBJLoader_H
#define OBJLoader_H

#include "main.h"

class OBJLoader
{
	private:

		FILE *file;
		FILE *mtlFile;

		int counter = 0;
		int faceCounter = 0;
		float rotationAngle = 0.0f;
		int r = 0, g = 0, b = 0;
		float tx = 0, ty = 0, tz = 0;
		float sx = 1, sy = 1, sz = 1;
		float rx = 0, ry = 0, rz = 0;
		int mouseX, mouseY;
		int currentMaterial = 0;

		struct vec3
		{
			GLfloat x;
			GLfloat y;
			GLfloat z;
		};

		struct vec3Float
		{
			float x;
			float y;
			float z;
		};

		struct customMaterial
		{
			//std::string name;
			float ambient[3];
			float diffuse[3];
			float specular[3];
			bool useTexture;
			int textureIDS;
		};

		std::vector<vec3> verticies;

		std::vector<vec3> textureCoordinates;

		std::vector<int> faces;
		
		std::vector<int> tCoords;

		std::string textureFilePath;

		std::vector<vec3> vertexNormals;

		std::vector<int> normals;

		std::vector<customMaterial> materials;

		std::vector<vec3Float> ambients;
		std::vector<vec3Float> diffuses;
		std::vector<vec3Float> speculars;
		std::vector<bool> usingTextures;
		std::vector<int> somethingsomething;

		std::vector<int> whenToSwitch;

	public:
		OBJLoader(char *file, char *mtlName, bool textureData);
		void renderObject();
		void renderObjectNoMatrix();
		void renderObjectWithTexture();
		void renderObjectAllProperties(bool drawNormals, GLuint texHandle[]);
		OBJLoader setColor(int r, int g, int b);
		OBJLoader translate(float x, float y, float z);
		OBJLoader scale(float x, float y, float z);
		OBJLoader rotate(float rotation, float x, float y, float z);
		OBJLoader faceCounterOutput();
};

#endif
