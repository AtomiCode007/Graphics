/*
 //  Make track
*/
#include "CSCIx229.h"

void RaceTrack(unsigned int* textures)
{
    //Begin Airstrip
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,textures[0]);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1,1);
    glBegin(GL_QUADS);

    glColor3f(1,1,1);
    glNormal3b(0,1,0);
    glTexCoord2f(15,0);     glVertex3d(40, -.35, -40); 
    glTexCoord2f(0,0);      glVertex3d(-10,-.35, -40);
    glTexCoord2f(0,15);     glVertex3d(-10, -.35, 40);
    glTexCoord2f(15,15);    glVertex3d(40, -.35, 40);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glDisable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1,1);

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,textures[1]);

    glBegin(GL_QUADS);

    glColor3f(1,1,1);
    glNormal3b(0,1,0);
    glTexCoord2f(0.12,1);   glVertex3d(30, -.35, -4);  
    glTexCoord2f(0.12,0.1);   glVertex3d(-2,-.35, -4);   
    glTexCoord2f(.88,0.1);   glVertex3d(-2, -.35, 4);   
    glTexCoord2f(.88,1);   glVertex3d(30, -.35, 4);   

    glEnd();
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
    //End Airstrip
}

