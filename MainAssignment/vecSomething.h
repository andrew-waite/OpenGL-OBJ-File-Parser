#ifndef _VECSOMETHING_
#define _VECSOMETHING_

#include <stdio.h>
#include <math.h>
#include "glut.h"

#define pi 3.14159265359
#define degrees pi/180.0
#define radians 180.0/pi

/* Type definitions...  */
typedef float vec3[3];

typedef GLfloat GLvec3[3];

// -----------  Vec3 print  ----------------------------------------
// Print a vec3.
// If u is the vec3 {0.0400, 1.0000, 0.6800} , then the call
//   v3Print(u, "6.4f", "Displacement vec" 
// should print something like 
//   Displacement vec:  [0.0400, 1.0000, 0.6800]
void v3Print(const vec3 u, char fformat[], char stringLabel[]);


// -----------  Vec3 creation  -------------------------------------
// Build a vector u from 3 scalars...
void v3FloatToVec(const float a, const float b, const float c, vec3 u);

// Set u to zero vector...
void v3Zero(vec3 u);

// Copy a vector: effectively  v = u...
void v3Copy(const vec3 u, vec3 v);


// -----------  Arithmetic Operations - In place  ------------------

// Change sign of vec: effectively  u = -u...
void v3ChangeSign (vec3 u);

// In-place scalar mult: effectively  u = a*u...
void v3Scale (const float a, vec3 u);

// In-place addition: effectively  v = v + u...
void v3AddToVec(const vec3 u, vec3 v);

// In-place subtraction: effectively  v = v - u...
void v3SubtractFromVec (const vec3 u, vec3 v);

// In-place accumulation of linear combination: effectively v = v + a*u...
void v3Accumulate (const float a, const vec3 u, vec3 v);

// In-place vec3 min: effectively  v = min(u,v)...
void v3UpdateMin(const vec3 u, vec3 v);

// In-place vec3 max: effectively  v = max(u,v)...
void v3UpdateMax(const vec3 u, vec3 v);


// -----------  Arithmetic Operations - To new vec  ----------------

// Scalar multiplication: effectively  v = a*u...
void v3ScalarMult(const vec3 u, const float a, vec3 v);

// Vector addition: effectively  w = u + v...
void v3Plus(const vec3 u, const vec3 v, vec3 w);

// Vector subtraction: effectively  w = u - v...
void v3Minus(const vec3 u, const vec3 v, vec3 w);

// Multiply components of vectors u, v to give vector w...
void v3ComponentwiseMult(vec3 u, vec3 v, vec3 w);


// -----------  Geometric Operations  -----------------------------

// Find length of a vector...
float v3Length(const vec3 u);

// Find displacement vector from point P to point Q...
void v3DisplacementVec(const vec3 P, const vec3 Q, vec3 u);

// Find distance between points P & Q...
float v3Distance(const vec3 P, const vec3 Q);

// Find dot product u.v of vectors u, v...
float v3DotProduct(const vec3 u, const vec3 v);

// Find cross product: effectively  w = u X v...
void v3CrossProduct(const vec3 u, const vec3 v, vec3 w);

// Normalise vector u: effectively  u = u / |u|...
void v3Normalise(vec3 u);

// Create unit vector v in direction of u: effectively  v = u / |u|...
void v3UnitVector(const vec3 u, vec3 v);

// Calculates unit normal to the plane containing points P1, P2, P3...
void v3PolygonNormal(const vec3 P1, const vec3 P2, const vec3 P3, vec3 normalVec);

void GLv3PolygonNormal(const GLvec3 P1, const GLvec3 P2, const GLvec3 P3, GLvec3 normalVec);

#endif  // _VEC3_
