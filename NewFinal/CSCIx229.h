#ifndef CSCIx229
#define CSCIx229

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

#ifdef __cplusplus
extern "C" {
#endif

void FormulaOne(double x,double y,double z,
                double dx,double dy,double dz,
                double ux,double uy, double uz, 
                double a1, double a2, double a3, 
                double b1, double b2, double b3, 
                double scale, int displayTime, 
                double rotation[], unsigned int* textures);
void cube(double x,double y,double z,
                double dx,double dy,double dz,
                double th, double c1, double c2, double c3,
                int tex, unsigned int* textures);
void tube(double x, double y, double z, 
                double dx, double dy, double dz,
                double radius, double width, int tex,
                unsigned int* textures);
void wheel(unsigned int* textures);
void steeringwheel();
void wing(unsigned int* textures, double c10, double c11,
        double c12, double c20, double c21, double c22);
void road(double a, double b, unsigned int* textures);
void RaceTrack(unsigned int* textures);
void Print(const char* format , ...);
void Fatal(const char* format , ...);
unsigned int LoadTexBMP(const char* file);
void Project(double fov,double asp,double dim);
void ErrCheck(const char* where);
int  LoadOBJ(const char* file);


#ifdef __cplusplus
}
#endif

#endif
