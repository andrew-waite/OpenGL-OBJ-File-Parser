#include "vecSomething.h"



// -----------  Vec3 print  ----------------------------------------
void v3Print(const vec3 u, char fformat[], char stringLabel[])
// Print vec3 something like "Displacement vec: [0.0400, 1.0000, 0.6800]" 
// according to given floating point format
{
  char s[200];
  sprintf(s, "%s:  [%%%s, %%%s, %%%s]\n", stringLabel, fformat, fformat, fformat);
  printf(s, u[0], u[1], u[2]);
}

// -----------  Vec3 creation  -------------------------------------

void v3FloatToVec(const float a, const float b, const float c, vec3 u)
// Build a vector u from 3 scalars...
{
  u[0]=a; 
  u[1]=b; 
  u[2]=c;
}

void v3Zero(vec3 u)    
// Effectively:  u = 0
{
  u[0]=0.0; 
  u[1]=0.0; 
  u[2]=0.0;
}

void v3Copy(const vec3 u, vec3 v)
// Effectively: v = u
{
  v[0] = u[0]; 
  v[1] = u[1]; 
  v[2] = u[2];
}


// -----------  Arithmetic Operations - In place  ------------------

void v3ChangeSign (vec3 u)
// Change sign of vec: effectively  u = -u
{
  u[0] = -u[0];
  u[1] = -u[1];
  u[2] = -u[2];
}

void v3Scale (const float a, vec3 u)
// In-place scalar mult: effectively  u = a*u
{
  u[0] *= a;
  u[1] *= a;
  u[2] *= a;
}

void v3AddToVec(const vec3 u, vec3 v)
// In-place addition: effectively  v = v + u;
{
  v[0] += u[0]; 
  v[1] += u[1]; 
  v[2] += u[2];
}

void v3SubtractFromVec (const vec3 u, vec3 v)
// In-place subtraction: effectively  v = v - u;
{
  v[0] -= u[0]; 
  v[1] -= u[1]; 
  v[2] -= u[2];
}

void v3Accumulate (const float a, const vec3 u, vec3 v)
// In-place accumulation of linear combination: effectively v = v + a*u
{
  v[0] += a*u[0];  
  v[1] += a*u[1];  
  v[2] += a*u[2];
}

void v3UpdateMin(const vec3 u, vec3 v)
// In-place vec3 min: effectively  v = min(u,v)
{
   if (u[0]<v[0])  v[0] = u[0];
   if (u[1]<v[1])  v[1] = u[1];
   if (u[2]<v[2])  v[2] = u[2];
}

void v3UpdateMax(const vec3 u, vec3 v)
// In-place vec3 max: effectively  v = max(u,v)
{
   if (u[0]>v[0])  v[0] = u[0];
   if (u[1]>v[1])  v[1] = u[1];
   if (u[2]>v[2])  v[2] = u[2];
}

// -----------  Arithmetic Operations - To new vec  ----------------

void v3ScalarMult(const vec3 u, const float a, vec3 v)
// Scalar multiplication: effectively  v = a*u
{
  v[0] = a*u[0];
  v[1] = a*u[1];
  v[2] = a*u[2];
}

void v3Plus(const vec3 u, const vec3 v, vec3 w)
// Vector addition: effectively  w = u + v
{
  w[0] = u[0] + v[0]; 
  w[1] = u[1] + v[1]; 
  w[2] = u[2] + v[2];
}

void v3Minus(const vec3 u, const vec3 v, vec3 w)
// Vector subtraction: effectively  w = u - v
{
  w[0] = u[0] - v[0]; 
  w[1] = u[1] - v[1]; 
  w[2] = u[2] - v[2];
}


void v3ComponentwiseMult(vec3 u, vec3 v, vec3 w)
// Multiply components of vectors u, v to give vector w
{
  w[0] = u[0] * v[0];  
  w[1] = u[1] * v[1];  
  w[2] = u[2] * v[2];
}

// -----------  Geometric Operations  -----------------------------

float v3Length(const vec3 u)
// Find length of a vector...
{
  return (float) sqrt(u[0]*u[0] + u[1]*u[1] + u[2]*u[2]);
}

void v3DisplacementVec(const vec3 P, const vec3 Q, vec3 u)
// Find displacement vector from point P to point Q.
// More or less the same as v3Minus, apart from order of args.
{
  u[0] = Q[0] - P[0]; 
  u[1] = Q[1] - P[1]; 
  u[2] = Q[2] - P[2];
}

float v3Distance(const vec3 P, const vec3 Q)
// Find distance between points P & Q.
// Note: symmetric in args -- order of P, Q doesn't matter.
{
  vec3 u;
  v3DisplacementVec(P, Q, u);
  return v3Length(u);
}

float v3DotProduct(const vec3 u, const vec3 v)
// Find dot product u.v of vectors u, v
{
  return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
}

void v3CrossProduct(const vec3 u, const vec3 v, vec3 w)
// Find cross product: effectively  w = u X v
{
  w[0] = u[1]*v[2] - u[2]*v[1];  
  w[1] = u[2]*v[0] - u[0]*v[2];  
  w[2] = u[0]*v[1] - u[1]*v[0];
}


void v3Normalise(vec3 u)
{
   // In-place normalisation of a 3-vector: effectively u = u / |u|.
   float len;
   len = v3Length(u);
   if (len != 0.0)
    v3Scale(1.0f / len, u);
}

void v3UnitVector(const vec3 u, vec3 v)
{
   // Create unit vector v in direction of u: effectively  v = u / |u|
   float len;
   len = v3Length(u);
   if (len == 0.0)         // Trap divide-by-zero & return zero vec.
     v3Zero(v);
   else
    v3ScalarMult(u, 1.0f / len, v);
}

void v3PolygonNormal(const vec3 P1, const vec3 P2, const vec3 P3, vec3 normalVec)
{
  // Calculates a unit normal to the plane containing points P1, P2, P3.
  // Points P1, P2, P3 specified in anti-clockwise order.
   vec3 u, v;
   v3DisplacementVec (P1, P2, u);
   v3DisplacementVec (P2, P3, v);
   v3CrossProduct (u, v, normalVec);
   v3Normalise(normalVec);
}

void GLv3PolygonNormal(const GLvec3 P1, const GLvec3 P2, const GLvec3 P3, GLvec3 normalVec)
{
	// Calculates a unit normal to the plane containing points P1, P2, P3.
	// Points P1, P2, P3 specified in anti-clockwise order.
	GLvec3 u, v;
	v3DisplacementVec(P1, P2, u);
	v3DisplacementVec(P2, P3, v);
	v3CrossProduct(u, v, normalVec);
	v3Normalise(normalVec);
}
