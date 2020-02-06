#include "CSCIx229.h"

void tube(double x, double y, double z, 
                  double dx, double dy, double dz, 
                  double radius, double width, int tex,
                  unsigned int* textures)
{
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,textures[tex]);
   float black[] = {0,0,0,1.0};
   float white[] = {1.0,1.0,1.0,1.0};
   glMaterialf(GL_FRONT,GL_SHININESS,1);
   glMaterialfv(GL_FRONT,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z); // REMOVE THIS!
   glScaled(dx,dy,dz);
   int a;
   glBegin(GL_QUAD_STRIP);
   for (a = 0; a <= 360; a+=15) 
   {

      glNormal3d(Cos(a),Sin(a),0);
      glTexCoord2f(0,0.5*a); glVertex3d(radius*Cos(a), radius*Sin(a), width);
      glTexCoord2f(1,0.5*a); glVertex3d(radius*Cos(a), radius*Sin(a), 0); 
   }
   glEnd();
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}