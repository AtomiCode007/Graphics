#include "CSCIx229.h"

void road(double a, double b, unsigned int* textures)
{
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,textures[2]);
   float road[] = {0.2,0.2,0.2,1.0};
   glColor3f(1,1,1);
   
      double new_b = b;
      double c = 0;
      double d = .25;
      double e = 0;
      double f = .33;

      double a_inc;
      double b_inc;
      
      if (a < 0)
      {
         if (b < 0)
         {
             b_inc = b*(-.6666667);
             a_inc = a*(-.5);
         }
         else
         {
            b_inc = b*(.6666667);
            a_inc = a*(-.5);
         }
      }
      else
      {
         if (b < 0)
         {
            b_inc = b*(-.6666667);
            a_inc = a*(.5);
         }
         else
         {
            b_inc = b*(.6666667);
            a_inc = a*(.5);
         }
      }


      for (int i = 0; i < 4; i++)
      {
         for (int j = 0; j < 3; j++)
         {
            // road was too shiny so i adjusted values
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1,1);
            glMaterialf(GL_FRONT,GL_SHININESS,0);
            glMaterialfv(GL_FRONT,GL_SPECULAR,road);
            glMaterialf(GL_FRONT,GL_AMBIENT,0.1);
            glNormal3f(0,1,0);
            glBegin(GL_QUADS);

            
            glTexCoord2f(e,c*10); glVertex3d(a,-0.40,new_b); //1
            glTexCoord2f(e,d*10); glVertex3d(a+a_inc,-0.40,new_b); //4
            glTexCoord2f(f,d*10); glVertex3d(a+a_inc,-0.40,new_b+b_inc); //3
            glTexCoord2f(f,c*10); glVertex3d(a,-0.40,new_b+b_inc);  //2
            
            glEnd();
            glDisable(GL_POLYGON_OFFSET_FILL);
            new_b += b_inc;
            e += .33;
            f += .33;
         }
         new_b = b;
         e = 0;
         f = .33;
         a += a_inc;
         c += .25;
         d += .25;
      }
   glDisable(GL_TEXTURE_2D);

}
