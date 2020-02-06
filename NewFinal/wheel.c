#include "CSCIx229.h"

void wheel(unsigned int* textures)
{
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,textures[7]);
   glColor3f(0.1,0.1,0.1);
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(2,2);

   glPushMatrix();
   glTranslatef(0,0,0);
   glNormal3f(0,0,1);
   glBegin(GL_TRIANGLE_FAN);
   for(int i = 0; i <= 360; i+=15)
   {
      glTexCoord2f(0.5*Cos(i)+0.5,0.5*Sin(i)+0.5);
      glVertex3d(0.1*Cos(i),0.1*Sin(i),0);
   }
   glEnd();
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0,0,.1);
   glNormal3f(0,0,1);
   glBegin(GL_TRIANGLE_FAN);
   for(int i = 0; i <= 360; i+=15)
   {
      glTexCoord2f(0.5*Cos(i)+0.5,0.5*Sin(i)+0.5);
      glVertex3d(0.1*Cos(i),0.1*Sin(i),0);
   }
   glEnd();
   glPopMatrix();
   
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,textures[1]);
   glPushMatrix();
   glBegin(GL_QUAD_STRIP);
   for (int a = 0; a <= 360; a+=15) 
   {

      glNormal3d(Cos(a),Sin(a),0);
      glTexCoord2f(0,0.5*a); glVertex3d(.1*Cos(a), .1*Sin(a), .1);
      glTexCoord2f(1,0.5*a); glVertex3d(.1*Cos(a), .1*Sin(a), 0); 
   }
   glEnd();
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}