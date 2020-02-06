#include "CSCIx229.h"

void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, double c1, double c2, double c3,
                 int tex, unsigned int* textures)
{
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,textures[tex]);
   /* arrays and values for specular shininess and emissions colors */
   float black[] = {0,0,0,1.0};
   float white[] = {1.0,1.0,1.0,1.0};
   glMaterialf(GL_FRONT,GL_SHININESS,1);
   glMaterialfv(GL_FRONT,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(c1,c2,c3);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(.5,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(.5,.5); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,.5); glVertex3f(-1,+1, 1);
   //  Back
   glNormal3f( 0, 0,-1);
   glTexCoord2f(.5,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(0,.5); glVertex3f(-1,+1,-1);
   glTexCoord2f(.5,.5); glVertex3f(+1,+1,-1);
   //  Right
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,.5); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(.5,0); glVertex3f(+1,+1,-1);
   glTexCoord2f(.5,.5); glVertex3f(+1,+1,+1);
   //  Left
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(0,.5); glVertex3f(-1,-1,+1);
   glTexCoord2f(.5,.5); glVertex3f(-1,+1,+1);
   glTexCoord2f(.5,0); glVertex3f(-1,+1,-1);
   //  Top
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,.5); glVertex3f(-1,+1,+1);
   glTexCoord2f(.5,.5); glVertex3f(+1,+1,+1);
   glTexCoord2f(.5,0); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,0); glVertex3f(-1,+1,-1);
   //  Bottom
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(.5,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(.5,.5); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,.5); glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}
