#include "OBJLoader.h"
#include "vecSomething.h"

OBJLoader::OBJLoader(char *fname, char *mtlName, bool texturedata)
{
	char ch;

	char line[128];

	int a, b, c;

	int vtx, vty, vtz;

	int vnx, vny, vnz;

	int uselessVariable;

	vec3 verts;

	vec3 textureCo;

	vec3 vnormals;

	char mtlLine[256];
	vec3Float ambient;
	vec3Float diffuse;
	vec3Float specular;
	int t = 0;

	int matid = 0;

	bool lastLine = false;

	//bool useTexture;

	mtlFile = fopen(mtlName, "r");

	if (!mtlFile)
	{
		printf("can't open file %s\n", "scene_tes.mtl");
		exit(1);
	}

	while (fscanf(mtlFile, "%s", &mtlLine) != EOF)
	{
		if (strcmp(mtlLine, "Ka") == 0)
		{
			fscanf(mtlFile, "%f %f %f\n", &ambient.x, &ambient.y, &ambient.z);
			
			ambients.push_back(ambient);
		}
		else if (strcmp(mtlLine, "Kd") == 0)
		{
			fscanf(mtlFile, "%f %f %f\n", &diffuse.x, &diffuse.y, &diffuse.z);
			diffuses.push_back(diffuse);
		}
		else if (strcmp(mtlLine, "Ks") == 0)
		{
			fscanf(mtlFile, "%f %f %f\n", &specular.x, &specular.y, &specular.z);
			speculars.push_back(specular);
		}
		else if (strcmp(mtlLine, "texture") == 0)
		{
			int i = 0;
			fscanf(mtlFile, "%d %d\n", &i, &t);
			if (i == 1)
			{
				usingTextures.push_back(true);
			}
			else
			{
				usingTextures.push_back(false);
			}

			somethingsomething.push_back(t);
		}
	}

	for (int i = 0; i < ambients.size(); i++)
	{
		customMaterial material;

		//material.name = materialNames[i];
		material.ambient[0] = ambients[i].x;
		material.ambient[1] = ambients[i].y;
		material.ambient[2] = ambients[i].z;

		material.diffuse[0] = diffuses[i].x;
		material.diffuse[1] = diffuses[i].y;
		material.diffuse[2] = diffuses[i].z;

		material.specular[0] = speculars[i].x;
		material.specular[1] = speculars[i].y;
		material.specular[2] = speculars[i].z;

		material.useTexture = usingTextures[i];
		material.textureIDS = somethingsomething[i];

		materials.push_back(material);
	}

	fclose(mtlFile);

	file = fopen(fname, "r");

	if (!file)
	{
		printf("can't open file %s\n", fname);
		//exit(1);
	}

	while (fscanf(file, "%s", &line) != EOF)
	{
		if (strcmp(line, "v") == 0)
		{
			fscanf(file, "%f %f %f\n", &verts.x, &verts.y, &verts.z);
			verticies.push_back(verts);
			//counter++;
			//printf("Counter %d", counter);
			lastLine = false;
		}
		else if (strcmp(line, "vt") == 0)
		{
			fscanf(file, "%f %f %f\n", &textureCo.x, &textureCo.y, &textureCo.z);
			//printf("read the file at this point");
			textureCoordinates.push_back(textureCo);
			lastLine = false;
		}
		else if (strcmp(line, "vn") == 0)
		{
			fscanf(file, "%f %f %f\n", &vnormals.x, &vnormals.y, &vnormals.z);
			vertexNormals.push_back(vnormals);
			lastLine = false;
		}
		else if (strcmp(line, "usemtl") == 0)
		{
			fscanf(file, "%d\n", &matid);
			lastLine = true;
		}
		else if (strcmp(line, "f") == 0)
		{
			fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &a, &vtx, &vnx, &b, &vty, &vny, &c, &vtz, &vnz);
			faces.push_back(a);
			faces.push_back(b);
			faces.push_back(c);

			tCoords.push_back(vtx);
			tCoords.push_back(vty);
			tCoords.push_back(vtz);

			normals.push_back(vnz);
			normals.push_back(vny);
			normals.push_back(vnz);

			if (lastLine)
			{
				whenToSwitch.push_back(a);
				whenToSwitch.push_back(b);
				whenToSwitch.push_back(c);
				whenToSwitch.push_back(matid);
			}

			faceCounter++;
		//	printf("\nFace Counter %d", faceCounter);

			lastLine = false;

		}
		else if (strcmp(line, "t") == 0)
		{
			//fscanf(file, "%s\n", &textureFilePath);
		}
	}

	fclose(file);
}

OBJLoader OBJLoader::setColor(int r, int g, int b)
{
	this->r = r;
	this->b = b;
	this->g = g;

	return *this;
}

OBJLoader OBJLoader::translate(float x, float y, float z)
{
	this->tx = x;
	this->ty = y;
	this->tz = z;

	return *this;
}

OBJLoader OBJLoader::scale(float x, float y, float z)
{
	this->sx = x;
	this->sy = y;
	this->sz = z;

	return *this;
}

OBJLoader OBJLoader::rotate(float rotation, float x, float y, float z)
{
	this->rotationAngle = rotation;
	this->rx = x;
	this->ry = y;
	this->rz = z;

	return *this;
}

OBJLoader OBJLoader::faceCounterOutput()
{
	printf("Contains %d faces", faces.size());
	return *this;
}

void copyVertex(GLfloat ToZ[3], GLfloat FromZ[3])
{
	ToZ[0] = FromZ[0];
	ToZ[1] = FromZ[1];
	ToZ[2] = FromZ[2];
}

void displayNormal(GLfloat pointZ[3], GLfloat normalZ[3])
{
	GLfloat p[3];
	copyVertex(p, pointZ);
	p[0] = p[0] + normalZ[0];
	p[1] = p[1] + normalZ[1];
	p[2] = p[2] + normalZ[2];

	glColor3f(0.8, 0.8, 0);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glVertex3fv(pointZ);
	glVertex3fv(p);
	glEnd();
	enableLighting();
}

void displayNormal3(GLfloat point0[3], GLfloat point1[3], GLfloat point2[3], GLfloat normalZ[3])
{
	// displays the normal of a triangle
	GLfloat ave[3];
	ave[0] = (point0[0] + point1[0] + point2[0]) / 3;
	ave[1] = (point0[1] + point1[1] + point2[1]) / 3;
	ave[2] = (point0[2] + point1[2] + point2[2]) / 3;
	displayNormal(ave, normalZ);
}

void OBJLoader::renderObjectAllProperties(bool drawNormals, GLuint texHandle[])
{
	glPushMatrix();
	//Translate, rotate, scale

	glTranslatef(tx, ty, tz);
	glRotatef(rotationAngle, rx, ry, rz);
	glScalef(sx, sy, sz);

	glLineWidth(1);
	glColor3ub(r, g, b);
	GLfloat norm[3];
	//int t = 0;

	for (int i = 0; i < faces.size(); i += 3)
	{
		//int useTexture = false;

		bool useTexture;
		int tID;

			for (int j = 0; j < whenToSwitch.size(); j+= 4)
			{
				if (whenToSwitch[j] == faces[i] && whenToSwitch[j + 1] == faces[i + 1] && whenToSwitch[j + 2] == faces[i + 2])
				{
					customMaterial a = materials[whenToSwitch[j + 3]];
					//printf("Current Material %d", );
					glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, a.ambient);
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, a.diffuse);
					glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, a.specular);
					//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny1);
					//t += 1;
					//printf("Exectured\n");

					if (a.useTexture)
					{
						useTexture = true;
						tID = a.textureIDS;
					}
					else
						useTexture = false;
				}
			}

			
			if(useTexture) glEnable(GL_TEXTURE_2D);
			if (useTexture) glBindTexture(GL_TEXTURE_2D, texHandle[tID]);

			glBegin(GL_TRIANGLES);
			GLfloat point1[3] = { verticies[faces[i] - 1].x, verticies[faces[i] - 1].y, verticies[faces[i] - 1].z };
			GLfloat point2[3] = { verticies[faces[i + 1] - 1].x, verticies[faces[i + 1] - 1].y, verticies[faces[i + 1] - 1].z };
			GLfloat point3[3] = { verticies[faces[i + 2] - 1].x, verticies[faces[i + 2] - 1].y, verticies[faces[i + 2] - 1].z };

			GLv3PolygonNormal(point1, point2, point3, norm);

			glNormal3fv(norm);

			glTexCoord3f(textureCoordinates[tCoords[i] - 1].x, textureCoordinates[tCoords[i] - 1].y, textureCoordinates[tCoords[i] - 1].z);
			glVertex3f(verticies[faces[i] - 1].x, verticies[faces[i] - 1].y, verticies[faces[i] - 1].z);

			glTexCoord3f(textureCoordinates[tCoords[i + 1] - 1].x, textureCoordinates[tCoords[i + 1] - 1].y, textureCoordinates[tCoords[i + 1] - 1].z);
			glVertex3f(verticies[faces[i + 1] - 1].x, verticies[faces[i + 1] - 1].y, verticies[faces[i + 1] - 1].z);

			glTexCoord3f(textureCoordinates[tCoords[i + 2] - 1].x, textureCoordinates[tCoords[i + 2] - 1].y, textureCoordinates[tCoords[i + 2] - 1].z);
			glVertex3f(verticies[faces[i + 2] - 1].x, verticies[faces[i + 2] - 1].y, verticies[faces[i + 2] - 1].z);

			glEnd();

			if (useTexture) glDisable(GL_TEXTURE_2D);
			//useTexture = false;

		if(drawNormals) displayNormal3(point1, point2, point3, norm);
	}
	glPopMatrix();
}

void OBJLoader::renderObjectWithTexture()
{
	glPushMatrix();
	//Translate, rotate, scale

	glTranslatef(tx, ty, tz);
	glRotatef(rotationAngle, rx, ry, rz);
	glScalef(sx, sy, sz);

	glLineWidth(1);
	glColor3ub(r, g, b);
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < faces.size(); i++)
	{
		glTexCoord3f(textureCoordinates[tCoords[i] - 1].x, textureCoordinates[tCoords[i] - 1].y, textureCoordinates[tCoords[i] - 1].z);
		glVertex3f(verticies[faces[i] - 1].x, verticies[faces[i] - 1].y, verticies[faces[i] - 1].z);
	}

	glEnd();

	glPopMatrix();
}

void OBJLoader::renderObject()
{
	glPushMatrix();
	//Translate, rotate, scale

	glTranslatef(tx, ty, tz);
	glRotatef(rotationAngle, rx, ry, rz);
	glScalef(sx, sy, sz);
	
	glLineWidth(1);
	glColor3ub(r, g, b);
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < faces.size(); i++)
	{
		glVertex3f(verticies[faces[i] - 1].x, verticies[faces[i] - 1].y, verticies[faces[i] - 1].z);
	}

	glEnd();

	glPopMatrix();
}

void OBJLoader::renderObjectNoMatrix()
{
	glTranslatef(tx, ty, tz);
	glRotatef(rotationAngle, rx, ry, rz);
	glScalef(sx, sy, sz);

	glLineWidth(1);
	glColor3ub(r, g, b);
	glBegin(GL_TRIANGLES);

	//printf("face counter %d", faceCounter);

	//printf("face counter %d", faceCounter);

	for (int i = 0; i < faces.size(); i++)
	{
		glVertex3f(verticies[faces[i] - 1].x, verticies[faces[i] - 1].y, verticies[faces[i] - 1].z);
	}

	glEnd();
}