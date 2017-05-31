#include "main.h"
#include "Camera.h"
#include "OBJLoader.h"
#include "vec3.h"
#include "vec4.h"

#include <string>
#include <iostream>

using namespace std;

OBJLoader water = OBJLoader("OBJ_FILES\\scene_test.obj", "OBJ_Files\\scene_test.mtl", true);
OBJLoader boat = OBJLoader("OBJ_FILES\\boat.obj", "OBJ_Files\\boat.mtl", true);

Camera camera = Camera(); //Instantiate a new camera object

#define framesPerSecond 25
#define timerMSecs ((int)1000/framesPerSecond)  // compute time in miliseconds for a frame

// random integer value 0 to max (macro for speed)
#define rndToMax(max) (rand()%(int)max)

// random float value 0 to max (macro for speed)
#define rndToMaxf(max) ((float)(rand()%(int)max))

#define MAX_PSYSTEM 400

#define PI 3.14159265

struct particleStruct
{
	bool active;   // active Y or n
	int time;      // ticks of activity
	vec3 pos;      // position
	vec3 prevPos;  // prevPosition
	vec3 velosity; // how fast do i move
	vec4 colour;
	float pSize;   // size 
};

// 0=Points of initial size only
// 1=last turn lines
// 2=128x128 texture

struct PsystemStruct
{
	bool active;            // true if active
	int activeticks;       // num of ticks for the posys to be active -1 lasts forever
	bool particlesStillActive; // 
	int typeOfPsystem;      // 0,1,2 - see above
	GLuint texHandle;       // texture handle for type 2 systems
	bool billboard;         // true if billboard
	vec3 pos;               // location of start     
	GLfloat emissionRate;   // number of particles per tick (can be fractional)
	GLfloat emmisionRateRandom; // a max random number which we div by 100 and add to emision rate
	GLfloat residualEmmision; // how many to emit this turn - to allow for fractional emission rates
	int maxParticles;       // no more than this please
	int numParticles;       // the actual number 
	GLfloat particleSize;   // size in points 
	GLfloat particleSizeDelta;   // size in points change per tick 
	int max_ticks;          // after this many ticks die
	vec3 dieIfLess;      // kill the particl if its less than any of these values
	vec4 colour;     // particle color
	vec4 colourChange; // colour change + added each tick
	vec4 colourMax;  // 1,1,1 = white
	vec3 gravity;    // velosity change due to gravity
	vec3 wind;       // movement due to wind
	vec3 initialVelosity;
	vec3 initialVelosityRandom; // max random number which we dive by 100 and add to initial velosity 
	vec3 airResistance;    // how it slows always positive
	vec3 absTerminalVelosity; // max speed always positive

	vec4 maxColour;

	// particle data
	particleStruct p[MAX_PSYSTEM];
};

int psCurr;
PsystemStruct ps[1]; // 0 through 9 

bool wireFrame = false;
bool sinking = false;
static GLfloat	cubeAngle = 0;
GLfloat posx = -300, posz = -200, speed = 5;

string bmpFileNames[N_TEXTURES];
GLuint texHandles[N_TEXTURES];

bool drawTextures = false;

bool light1 = false;

float currentTime, timeBase = 0;
int frames;

int angle = 0;

bool drawNormals = false;

bool ambientLight = false;

bool timeForNextFrame = false;

bool eLight = false;
static bool eLight0 = false;
static GLfloat ambientIntensity01[] = { 0.3f, 0.3f ,0.3f ,1.0f }; // RGBA color for ambient
static GLfloat ambientIntensity0[] = { 0.0f, 0.0f, 0.0f ,1.0f }; // RGBA color for ambient

float	dif1[] = { 0.4f, 0.133f, 0.0f, 1.0f}; // Warf Planks 102, 34, 0
float	amb1[] = { 0.4f, 0.133, 0.0f, 1.0f };
float	spe1[] = { 0.0f, 0.0f, 0.0f, 1.0f};
float   shiny1 = 0.25 * 128; //0.25 * 128; //1.6

float	dif2[] = { 19.0f, 183.0f, 237.0f, 1.0f }; // Iceberg color
float	amb2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float	spe2[] = { 0.0f, 0.0f, 0.0f, 0.0f };
float   shiny2 = 0;

float	dif3[] = { 0.0, 0.0f, 1.0f, 1.0f }; // Water colour blue
float	amb3[] = { 0.0f, 0.0f, 1.0f, 1.0f };
float	spe3[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float   shiny3 = 0.01f * 128;

float	dif4[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // white
float	amb4[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float	spe4[] = { 0.0f, 0.0f, 0.0f, 0.0f };
float   shiny4 = 0;

float	dif5[] = { 1.0f, 0.0f, 0.0f, 1.0f }; // red
float	amb5[] = { 1.0f, 0.0f, 0.0f, 1.0f };
float	spe5[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float   shiny5 = 60;

float	dif6[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // black
float	amb6[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float	spe6[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float   shiny6 = 60;

float	dif7[] = { 0.545f, 0.270f, 0.074f, 1.0f }; // Warf Poles 139, 69, 19
float	amb7[] = { 0.545f, 0.270f, 0.074f, 1.0f };
float	spe7[] = { 0.02f, 0.02f, 0.02f, 1.0f };
float   shiny7 = 0;

int lpos = 0;
float	lpos1[] = { 0.0f,  3.5f, -20.0f, 1.0f }; // point source close 
float	lpos2[] = { 6.0f,  6.0f, 0.0f, 1.0f }; // point source 
float	lpos3[] = { -40.0f, 20.0f,  50.0f, 1.0f }; // distant light

float	ldif[] = { 0.6f, 0.6f, 0.6f, 1.0f };
float	lamb[] = { 0.1f, 0.1f, 0.1f, 1.0f };
float	lspe[] = { 0.6f, 0.6f, 0.6f, 1.0f }; //.6, .6, .6, 1

float	ldif1[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //0.6, 0.6, 0.6
float	lamb1[] = { 0.0f, 0.0f, 0.0f, 0.0f }; // 0.4, 0..4, 0.4
float	lspe1[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //.6, .6, .6, 1

GLfloat fogColour[4] = { 0.513, 0.513, 0.513, 1.0 };
bool doFog = false;

GLfloat billboardAngle = 0;
GLfloat	spinAngle;
bool spining = false;

void setMaterial(int m)
{
	if (m == 1)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe1);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny1);
	}
	if (m == 2)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe2);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny2);
	}
	if (m == 3)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb3);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif3);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe3);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny3);
	}
	if (m == 4)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb4);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif4);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe4);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny4);
	}

	if (m == 5)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb5);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif5);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe5);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny5);
	}

	if (m == 6)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb6);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif6);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe6);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny6);
	}
	if (m == 7)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb7);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif7);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe7);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny7);
	}
}

void DrawSkyboxAt(float x, float y, float z, float sz)
//
// skybox routine  
//
{
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);									//disables depth test
	glDepthMask(0);												//so skybox will never cover any geometry
	glBindTexture(GL_TEXTURE_2D, texHandles[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);									// front face
	glVertex3f(-sz + x, -sz + y, -sz + z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(sz + x, -sz + y, -sz + z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(sz + x, sz + y, -sz + z);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-sz + x, sz + y, -sz + z);
	glTexCoord2f(0.0f, 1.0f);									// left face
	glVertex3f(-sz + x, -sz + y, sz + z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-sz + x, -sz + y, -sz + z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-sz + x, sz + y, -sz + z);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-sz + x, sz + y, sz + z);
	glTexCoord2f(0.0f, 1.0f);									// right face
	glVertex3f(sz + x, -sz + y, -sz + z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(sz + x, -sz + y, sz + z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(sz + x, sz + y, sz + z);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(sz + x, sz + y, -sz + z);
	glTexCoord2f(0.0f, 1.0f);									// back face
	glVertex3f(sz + x, -sz + y, sz + z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-sz + x, -sz + y, sz + z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-sz + x, sz + y, sz + z);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(sz + x, sz + y, sz + z);
	glTexCoord2f(0.0f, 1.0f);									// upper face
	glVertex3f(-sz + x, sz + y, -sz + z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(sz + x, sz + y, -sz + z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(sz + x, sz + y, sz + z);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-sz + x, sz + y, sz + z);
	glTexCoord2f(0.0f, 1.0f);									// lower face
	glVertex3f(-sz + x, -sz + y, sz + z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(sz + x, -sz + y, sz + z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(sz + x, -sz + y, -sz + z);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-sz + x, -sz + y, -sz + z);
	glEnd();
	glDepthMask(1);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

}


void renderObjects()
{
	glPolygonMode(GL_FRONT_AND_BACK, wireFrame ? GL_LINE : GL_FILL);

	water.scale(1, 1, 1).renderObjectAllProperties(drawNormals, texHandles);

	if (doFog)
	{
		//printf("foggy\n");
		glEnable(GL_FOG);
		glFogi(GL_FOG_MODE, GL_LINEAR); // try GL_EXP
										//glFogi(GL_FOG_MODE, GL_EXP2); // try GL_EXP
		glFogfv(GL_FOG_COLOR, fogColour);
		glFogf(GL_FOG_DENSITY, 0.4);   // doesn't affect GL_LINEAR fog
		glFogf(GL_FOG_START, 20);    // doesn't affect GL_EXP fog
		glFogf(GL_FOG_END, 1200); 	   // doesn't affect GL_EXP fog
	}
	else
	{
		glDisable(GL_FOG);
	}

	if(sinking) boat.scale(2, 2, 2).rotate(45, 1, 0, 1).translate(posx, 1, posz).renderObjectAllProperties(drawNormals, texHandles);
	else boat.scale(2, 2, 2).rotate(cubeAngle, 0, 1, 0).translate(posx, 1, posz).renderObjectAllProperties(drawNormals, texHandles);
}

void initPsystemToDefault(PsystemStruct *ps)
{
	ps->active = false;
	ps->activeticks = -1;
	ps->typeOfPsystem = 0;       // future expansion
	ps->texHandle = 0;           // sane value
	ps->billboard = false;       // sane value
	v3FloatToVecM(ps->pos, psCurr % 4, 0, psCurr / 4);
	ps->emissionRate = 1;      // number of particles per tick (can be fractional)
	ps->emmisionRateRandom = 0;  // a max random number which we div by 100 and add to emision rate
	ps->residualEmmision = 0;
	ps->maxParticles = 100;      // no more than this please
	ps->numParticles = 0;        // the actual number 
	ps->particleSize = 3;        // size in points 
	ps->particleSizeDelta = 0;   // no change
	ps->max_ticks = 120;         // after this many ticks die
	v3FloatToVec(-10000, 0, -10000, ps->dieIfLess);      // kill the particl if its less than any of these values
	v4FloatToVecM(ps->colour, 0.1, 0.8, 0.1, 1); // particle color       
	v4FloatToVecM(ps->colourChange, 0.06, 0.06, 0.06, 0);   // colour drift + added each tick
	v4FloatToVecM(ps->colourMax, 1, 1, 1, 1);
	v3FloatToVec(0, -0.02, 0, ps->gravity);    // velosity change due to gravity
	v3FloatToVec(0, 0, 0, ps->wind);           // movement due to wind
	v3FloatToVec(-0.025, 0.3, -0.025, ps->initialVelosity);
	v3FloatToVec(5, 5, 5, ps->initialVelosityRandom); // max random number which we dive by 100 and add to initial velosity 
	v3FloatToVec(0, 0, 0, ps->airResistance);       // how it slows
	v3FloatToVec(5, 5, 5, ps->absTerminalVelosity);    // max speed

	for (int i = 0; i< MAX_PSYSTEM; i++)
	{
		ps->p[i].active = false;   // active Y or n
		ps->p[i].time = 0;      // ticks of activity
		v3FloatToVec(0, 0, 0, ps->p[i].pos);      // position
		v3FloatToVec(0, 0, 0, ps->p[i].velosity); // how fast do i move
	}
}

void initPsystems()
{
	for (psCurr = 0; psCurr<1; psCurr++) initPsystemToDefault(&(ps[psCurr]));
}
void initPsystem6()
{
	ps[0].typeOfPsystem = 2;
	ps[0].activeticks = 30000;
	ps[0].emissionRate = 0.2;
	ps[0].maxParticles = 30;
	ps[0].emmisionRateRandom = 10;
	ps[0].particleSize = 20;         // size in points 
	ps[0].particleSizeDelta = 0.2;   // size change
	ps[0].texHandle = 4;
	ps[0].billboard = false;
	ps[0].initialVelosity = vec3(0, 0.8, 0);
	ps[0].gravity = vec3(0, 0, 0);
	ps[0].wind = vec3(0.0, 0, -0.3);
}

void changeSize(int w, int h) 
{

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 20.0f, 1500.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void setUpLights()
{
	if (lpos == 1)
	{
		glPushMatrix();
		//glLoadIdentity();
		glTranslatef(posx, 20, posz - 25);
		glRotatef(cubeAngle + 180, 0, 1, 0);
		glLightfv(GL_LIGHT1, GL_POSITION, lpos1);
		GLfloat spotDirection[] = { 1.0, 0.0, 0.0, 1.0 };
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 15.0f);
		glPopMatrix();
	}
	if (lpos == 3)
	{
		glLightfv(GL_LIGHT0, GL_POSITION, lpos3);
	}
	glLightfv(GL_LIGHT0, GL_AMBIENT, lamb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ldif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lspe);


	glLightfv(GL_LIGHT1, GL_AMBIENT, lamb1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, ldif1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lspe1);

	//glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
	//glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
	//glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);

	//glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
//	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.3);
	//glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.3);

	glShadeModel(GL_SMOOTH);
	//glShadeModel(GL_FLAT);

	enableLighting();
}

double dist(double x1, double y1, double x2, double y2)
{
	/* calc the distance between 2 points */
	double retv;
	double d1, d2;
	double temp;

	d1 = x1 - x2;
	d2 = y1 - y2;
	temp = d1*d1 + d2*d2;
	retv = sqrt(temp);
	return(retv);
}

void direct(GLfloat xx1, GLfloat yy1, GLfloat xx2, GLfloat yy2, GLfloat *angle, GLfloat *distz)
/* calculate angle in degrees and distance to location */
{
	*distz = dist(xx1, yy1, xx2, yy2);
	GLfloat result = atan2(yy1 - yy2, xx1 - xx2) * 180 / PI;
	*angle = result;
}

void drawPsystemType2(PsystemStruct *ps)
{
	int i;
	GLfloat temp;
	if (!ps->active && !ps->particlesStillActive) return;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_TEXTURE_2D);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texHandles[4]);

	direct(0, 0, camera.getCAMX(), camera.getCAMZ(), &billboardAngle, &temp); // I look at 0,0

	for (i = 0; i< ps->maxParticles; i++)
	{
		if (ps->p[i].active)
		{
			//glColor4fv(ps->p[i].colour);
			glColor4f(1, 1, 1, 1);
			glPushMatrix();
			glTranslatef(ps->p[i].pos[X], ps->p[i].pos[Y], ps->p[i].pos[Z]);
			glScalef(ps->p[i].pSize, ps->p[i].pSize, ps->p[i].pSize);
			if (ps->billboard)
			{
				glRotatef(billboardAngle - spinAngle, 0.0, 1.0, 0.0);
			}
			glBegin(GL_QUADS);
			glTexCoord2f(0, 1);
			glVertex3f(-0.5, 0.5, 0.0);
			glTexCoord2f(1, 1);
			glVertex3f(0.0, 0.5, 0.0);
			glTexCoord2f(1, 0);
			glVertex3f(0.0, -0.5, 0.0);
			glTexCoord2f(0, 0);
			glVertex3f(-0.5, -0.5, 0.0);
			glEnd();
			glPopMatrix();
		}
	}
	glDisable(GL_TEXTURE_2D);
}

void drawPsystem(PsystemStruct *ps)
{
	if (ps->typeOfPsystem == 2) drawPsystemType2(ps); // 64 x 64 bmp with alpha
}

void updatePsystem(PsystemStruct *ps)
{
	int i;
	GLfloat f;

	if (!ps->active && !ps->particlesStillActive) return;
	if (ps->activeticks>0)
	{
		ps->activeticks--;
		if (ps->activeticks <= 0) { ps->active = false; ps->residualEmmision = 0; }
	}

	// process each particle slot for this time step...
	if (ps->active)
	{
		ps->residualEmmision = ps->residualEmmision + ps->emissionRate;
		if (ps->emmisionRateRandom != 0) ps->residualEmmision = ps->residualEmmision + rndToMaxf(ps->emmisionRateRandom) / 100;
	}

	ps->particlesStillActive = false;

	for (i = 0; i< ps->maxParticles; i++)
	{
		if ((!ps->p[i].active) && ps->residualEmmision > 1)
		{
			// create a new particle
			ps->residualEmmision = ps->residualEmmision - 1;
			ps->p[i].active = true;
			ps->numParticles++;
			ps->p[i].pSize = ps->particleSize;
			ps->p[i].time = 0;
			v3CopyM(ps->p[i].pos, ps->pos);
			v3CopyM(ps->p[i].prevPos, ps->pos);
			v3CopyM(ps->p[i].velosity, ps->initialVelosity);
			v4CopyM(ps->p[i].colour, ps->colour);
			//for(n=0;n<3;n++)
			// {
			if ((int)ps->initialVelosityRandom[X] != 0)
			{
				f = rndToMaxf(ps->initialVelosityRandom[X]) / 100;
				ps->p[i].velosity.x = ps->p[i].velosity.x + f;
			}
			if ((int)ps->initialVelosityRandom[Y] != 0)
			{
				f = rndToMaxf(ps->initialVelosityRandom[Y]) / 100;
				ps->p[i].velosity.y = ps->p[i].velosity.y + f;
			}
			if ((int)ps->initialVelosityRandom[Z] != 0)
			{
				f = rndToMaxf(ps->initialVelosityRandom[Z]) / 100;
				ps->p[i].velosity.z = ps->p[i].velosity.z + f;
			}
		}

		//... then move all the particles...
		if (ps->p[i].active)
		{
			ps->particlesStillActive = true;
			ps->p[i].time++;
			v3CopyM(ps->p[i].prevPos, ps->p[i].pos);
			if (ps->p[i].time>ps->max_ticks) { ps->p[i].active = false; ps->numParticles--; continue; }
			v3AddToVecM(ps->p[i].pos, ps->p[i].velosity); // my velosity
			v3AddToVecM(ps->p[i].pos, ps->wind); // wind
			if (v3isLess(ps->p[i].pos, ps->dieIfLess)) { ps->p[i].active = false; ps->numParticles--; continue; }

			v3AddToVecM(ps->p[i].velosity, ps->gravity); // gravity
			v3clipAbsM(ps->p[i].velosity, ps->absTerminalVelosity);
			v3dampM(ps->p[i].velosity, ps->airResistance); // air resistance
			v3clipAbsM(ps->p[i].velosity, ps->absTerminalVelosity); // terminal velosity

			v4AddToVecM(ps->p[i].colour, ps->colourChange);
			v4ClipMaxM(ps->p[i].colour, ps->colourMax);

			ps->p[i].pSize = ps->p[i].pSize + ps->particleSizeDelta;
			if (ps->p[i].pSize<0)ps->p[i].pSize = 0;
		}
	}
}


void renderScene() {

	if (camera.getDeltaMove())
		camera.computePos(camera.getDeltaMove());

	if (doFog) glClearColor(fogColour[0], fogColour[1], fogColour[2], 0.0);
	//if (light1) glClearColor(0.0, 0.0, 0.0, 0.0);
	else
	glClearColor(0, 129.0, 255.0, 1.0);
	//glClearColor(0, 0, 0, 1);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);                // Enables depth testing
	glDepthFunc(GL_NORMALIZE);                 // The type of depth testing to do
	//glDepthRange(0.1, 200.0);

	glMatrixMode(GL_MODELVIEW);

	setUpLights();
	// Reset transformations
	glLoadIdentity();


	camera.setLY(0.1 - camera.getMouseDirection());

	// Set the camera
	gluLookAt(camera.getCAMX(), camera.getCAMY(), camera.getCAMZ(),
		camera.getCAMX() + camera.getLX(), camera.getCAMY() + camera.getLY(), camera.getCAMZ() + camera.getLZ(),
		0.0f, 1.0f, 0.0f);



	//glBindTexture(GL_TEXTURE_2D, texHandles[0]);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //GL_NEAREST

	/*frames++;
	currentTime = glutGet(GLUT_ELAPSED_TIME);

	if (currentTime - timeBase > 1000)
	{
		timeBase = currentTime;
		printf("FPS: %d", frames);
		frames = 0;
	}*/

	if (timeForNextFrame)
	{
		angle += 1.0;
		if (angle > 360.0)
			angle -= 360.0;

		updatePsystem(&(ps[0]));
		if (spining)
		{
			spinAngle += 2.0;
			if (spinAngle > 360.0) spinAngle -= 360.0;
		}
		//glutPostRedisplay();
		timeForNextFrame = false;
	}

	//lPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//spinAngle = 0.0;


	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	if (!doFog) DrawSkyboxAt(camera.getCAMX(), camera.getCAMY(), camera.getCAMZ(), 200);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	renderObjects();


	glPushMatrix();
	glTranslatef(759, 160, -120);
	glRotatef(90, 0, 1, 0);
	drawPsystem(&(ps[0]));
	glPopMatrix();

	glutSwapBuffers();
}

void timer(int value)
{
	timeForNextFrame = true;
	glutTimerFunc(timerMSecs, timer, 0);
}

void enableLighting()
{
	if (eLight) glEnable(GL_LIGHTING); else glDisable(GL_LIGHTING);

	if (eLight0)
	{
		glEnable(GL_LIGHT0);
		//printf("LIGHT0 Enabled");
	}
	else
	{
		glDisable(GL_LIGHT0);
		//printf("LIGHT0 Disabled");
	}

	if (light1)
	{
		glEnable(GL_LIGHT1);
		//printf("LIGHT1 Enabled");
	}
	else
	{
		glDisable(GL_LIGHT1);
		//printf("LIGHT0 Disabled");
	}

	if (ambientLight)
	{
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &ambientIntensity01[0]);
		//err1 = glGetError();
	}
	else
	{
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &ambientIntensity0[0]);
		//err1 = glGetError();
	}
	//if (err1 != GL_NO_ERROR) printf("err1=%d\n", (int)err1);
}

void MovePlane2D(GLfloat oldx, GLfloat oldz, GLfloat angle, GLfloat speed, GLfloat *resultx, GLfloat *resultz)
{
	GLfloat tmpx, tmpz;
	GLfloat newx, newz;
	float yaw; // yaw is an angle in radians

	yaw = angle*3.1416 / 180;


	tmpx = 0;/* set to origin */
	tmpz = speed; /* move us forward by speed */

	newx = (tmpz * sin(yaw)) + (tmpx * cos(yaw));
	newz = (tmpz * cos(yaw)) - (tmpx * sin(yaw));
	tmpx = newx;
	tmpz = newz;

	newx = tmpx + oldx; // adjust because it was around the origin
	newz = tmpz + oldz;

	*resultx = newx;
	*resultz = newz;
}

bool collision(int pos1x, int pos1y, int pos1z, int pos2x, int pos2y, int pos2z, int size1x, int size1y, int size1z, int size2x, int size2y, int size2z)
{
	
	//check the X axis
	if (std::abs(pos1x - pos2x) < size1x + size2x)
	{
		//check the Y axis
		if (std::abs(pos1y - pos2y) < size1y + size2y)
		{
			//check the Z axis
			if (std::abs(pos1z - pos2z) < size1z + size2z)
			{
				return true;
			}
		}
	}
	return false;
}

void processNormalKeys(unsigned char key, int xx, int yy) 
{

	switch (key)
	{
		case 27: exit(0); break;
		case 119: camera.setDeltaMove(6.0f); break;
		case 115:  camera.setDeltaMove(-6.0f); break; 
		case 97: camera.crossProduct(true, 0.0f, 15.0f, 0.0f, camera.getLX(), camera.getLY(), camera.getLZ()); break;
		case 100: camera.crossProduct(false, 0.0f, 15.0f, 0.0f, camera.getLX(), camera.getLY(), camera.getLZ()); break;
		case 113: wireFrame = !wireFrame; break; //Q
		//case 108: speed = speed + 0.25; if (speed >2.1) speed = 0.25; break; //l
		case 111: speed = speed - 0.25; if (speed < 1) speed = 1; break; //o
		case 106: sinking = !sinking; break; //j

		case 108: eLight = !eLight; //Key L
		case 109: ambientLight = !ambientLight; break; //Key m for ambient light
		case 116: drawTextures = !drawTextures; break; //Key t enable/disable textures

		case 49: lpos = 1; break; //Key 1 - Source close 
		case 50: lpos = 2; break; //Key 2 - Source 
		case 51: lpos = 3; printf("pressed key 3"); break; //Key 3 - Distant
		case 110: eLight0 = !eLight0; break; //Key n - Turn on Light0
		case 98: light1 = !light1; break; //Key b, turn on light1

		case 118: drawNormals = !drawNormals; break;

		case 107: angle += 2; break;

		case 32: doFog = !doFog; break; //Space bar

		case 54:  ps[0].active = !ps[0].active; ps[0].activeticks = 5000; break; //Number 6
	}
}

void processNormalKeysUP(unsigned char key, int xx, int yy) {

	switch (key)
	{
		case 119:
		case 115: camera.setDeltaMove(0); break;
	}
}

void specialPressKey(int key, int xx, int yy) 
{
	switch (key)
	{
		case GLUT_KEY_UP: if(collision(posx, 1, posz, -190, 1, -90, 20, 38, 20, 30, 130, 30)) sinking = true;  MovePlane2D(posx, posz, cubeAngle + 90, -speed, &posx, &posz);  break;
		case GLUT_KEY_DOWN: if (collision(posx, 1, posz, -190, 1, -90, 20, 38, 20, 30, 130, 30)) sinking = true;  MovePlane2D(posx, posz, cubeAngle + 90, speed, &posx, &posz); break;
		case GLUT_KEY_LEFT: cubeAngle = cubeAngle - 10; if (cubeAngle<0) cubeAngle = 360;  break;
		case GLUT_KEY_RIGHT: cubeAngle = cubeAngle + 10; if (cubeAngle>360) cubeAngle = 0;  break;
	}
}

void specialReleaseKey(int key, int x, int y) {

}

void mouseMove(int x, int y) {

	// this will only be true when the left button is down
	if (camera.getXOrigin() >= 0) {

		camera.setMouseDirection((y - glutGet(GLUT_WINDOW_HEIGHT) / 2) * 0.002f);

		// update deltaAngle
		camera.setDeltaAngle((x - camera.getXOrigin()) * 0.001f);

		// update camera's direction

		camera.setLX(sin(camera.getAngle() + camera.getDeltaAngle()));
		camera.setLZ(-cos(camera.getAngle() + camera.getDeltaAngle()));
	}
}

void mouseButton(int button, int state, int x, int y)
{

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			camera.setAngle(camera.getAngle() + camera.getDeltaAngle());
			camera.setXOrigin(-1);
		}
		else {// state = GLUT_DOWN
			camera.setXOrigin(x);
		}
	}
}

void readTexture()
{
	// Read in a couple of BMPs and attach them to texture handles.
	bmpFileNames[0] = "woodwall2.bmp";
	bmpFileNames[1] = "download.bmp";
	bmpFileNames[2] = "bricks.bmp";
	bmpFileNames[3] = "old_wood.bmp";
	bmpFileNames[4] = "smoke.bmp";
	bmpFileNames[5] = "sky.bmp";
	glGenTextures(N_TEXTURES, texHandles); // Get some texture handles.
	for (int j = 0; j < N_TEXTURES; j++)
	{
		CBitmap bmpData(bmpFileNames[j].c_str());  // Construct to hold BMP file data
		int texWidth = bmpData.GetWidth();
		int texHeight = bmpData.GetHeight();
		if (texWidth == 0 || texHeight == 0 || bmpData.GetBits() == NULL)
		{
			char response;
			cerr << "Unable to load BMP file \"" << bmpFileNames[j] << "\"..." << endl;
			cerr << "Press 'q' <ENTER> to exit: ";
			cin >> response;
			exit(1);
		}
		// OK, looks like we got BMP data.
		cout << "Loaded BMP file  " << bmpFileNames[j] << endl;
		//showBMPInfo(bmpData);
		// The file read has arranged it to be RGBA
		// (whereas 24-bit BMPs are natively in BGR).
		// The A (transparency) byte is probably worthless, but who cares?
		// Now attach it to OpenGL texture...
		glBindTexture(GL_TEXTURE_2D, texHandles[j]);
		cout << "Attaching pixel data from " << bmpFileNames[j]
			<< " to texture handle " << texHandles[j] << endl;
		int returnCode = gluBuild2DMipmaps(
			GL_TEXTURE_2D,       // target type of texture
			GL_RGBA,              // format in which GL should store it
			texWidth, texHeight, // [obvious]
			GL_RGBA,             // format of data
			GL_UNSIGNED_BYTE,    // each R,G,B,A component is a byte
			bmpData.GetBits()    // pointer to location of pixel data
		);
		if (returnCode != 0)
			cout << gluErrorString(returnCode) << endl;
		// Alternative -- would NOT generate mipmaps.
		//glTexImage2D(
		//    GL_TEXTURE_2D,       // target type of texture
		//    0,                   // mipmap level
		//    GL_RGBA,             // format in which GL should store texels
		//    texWidth, texHeight, // [obvious]
		//    0,                   // border width
		//    GL_RGBA,             // format of data
		//    GL_UNSIGNED_BYTE,    // each R,G,B component is a byte
		//    bmpData.GetBits()    // pointer to pixel data
		//  );
	}  // End of loop for texture file reading.
	   // As the bmpData goes out of scope it will be destroyed.
	   // But the pixels have been copied into OpenGL's memory area,
	   // so that is okay.

	printf("Loaded once");
}

int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Lighthouse3D - GLUT Tutorial");

	//glutFullScreen();

	initPsystems();
	initPsystem6();

	// register callbacks

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutKeyboardUpFunc(processNormalKeysUP);
	glutSpecialFunc(specialPressKey);
	glutSpecialUpFunc(specialReleaseKey);

	// here are the two new functions
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	glutTimerFunc(timerMSecs, timer, 0);

	//Read in the texture files
	readTexture();

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}