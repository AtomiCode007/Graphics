#include "CSCIx229.h"

void steeringwheel()
{

   float black[] = {0,0,0,1.0};
   float Diff[] ={0.4f, 0.2368f, 0.1036f, 1.0f};
   float Specular[]  = {0.0,.0,.0,1.0};
   glEnable(GL_COLOR_MATERIAL);
   glMaterialf(GL_FRONT,GL_SHININESS,1);
   glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,black);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,Diff);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Specular);

   glColor3f(.1,.1,.1);
   glPushMatrix();
   glutSolidTorus(0.01,0.06,20,20);
   glPopMatrix();

   glColor3f(1,1,1);
   glPushMatrix();
   glTranslatef(0,0.0585,0);
   glutSolidTorus(0.006,0.006,20,20);
   glPopMatrix();
}