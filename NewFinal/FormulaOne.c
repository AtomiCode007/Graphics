#include "CSCIx229.h"

void FormulaOne(double x,double y,double z,
                       double dx,double dy,double dz,
                       double ux,double uy, double uz, 
                       double a1, double a2, double a3, 
                       double b1, double b2, double b3, 
                       double scale, int displayTime, 
                       double rotation[], unsigned int* textures)
{
   /* eventuall build a rotation array and pass it in */
   
   /* arrays and values for specular shininess and emissions colors */  
   float black[] = {0,0,0,1.0};
   float Amb[] = {0.25,0.25,0.25,1.0};
   float Diff[] ={0.4f, 0.2368f, 0.1036f, 1.0f};
   //float High[] = {0.7,0.7,0.7,1.0};
   float Specular[]  = {0.774597f,.774597f,.774597f,1.0};
   glMaterialf(GL_FRONT,GL_SHININESS,1);
   glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Amb);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,Diff);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Specular);
   //  Unit vector in direction of the nose of the car
   double D0 = sqrt(dx*dx+dy*dy+dz*dz);
   double X0 = dx/D0;
   double Y0 = dy/D0;
   double Z0 = dz/D0;
   //  Unit vector in "up" direction
   double D1 = sqrt(ux*ux+uy*uy+uz*uz);
   double X1 = ux/D1;
   double Y1 = uy/D1;
   double Z1 = uz/D1;
   //  Cross product gives the third vector
   double X2 = Y0*Z1-Y1*Z0;
   double Y2 = Z0*X1-Z1*X0;
   double Z2 = X0*Y1-X1*Y0;
   //  Rotation matrix
   double mat[16];
   mat[0] = X0;   mat[4] = X1;   mat[ 8] = X2;   mat[12] = 0;
   mat[1] = Y0;   mat[5] = Y1;   mat[ 9] = Y2;   mat[13] = 0;
   mat[2] = Z0;   mat[6] = Z1;   mat[10] = Z2;   mat[14] = 0;
   mat[3] =  0;   mat[7] =  0;   mat[11] =  0;   mat[15] = 1;
   
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glMultMatrixd(mat);

   /*
   triangles for outside of nose.
   */ 
   glColor3f(a1,a2,a3);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,textures[8]);
   glBegin(GL_TRIANGLES);
   
   glNormal3f(0,0,1);
   glTexCoord2f(0,0); glVertex3d(0.0, 0.05, 0.15);
   glTexCoord2f(1,0); glVertex3d(0.0, 0.25, 0.15);
   glTexCoord2f(0,1); glVertex3d(0.4, 0.05, 0.15);

   glNormal3f(0,0,-1);
   glTexCoord2f(0,0); glVertex3d(0.0, 0.05, -0.15);
   glTexCoord2f(1,1); glVertex3d(0.0, 0.25, -0.15);
   glTexCoord2f(0,1); glVertex3d(0.4, 0.05, -0.15);
   glEnd();

   /*
   polygons for top and bottom of nose
   */
   
   // top
   glBegin(GL_POLYGON);
   glNormal3f(.4187762688,1,0); // had to find angles of the triangle and use the ratio between the two to determine these values
   glTexCoord2f(1,0); glVertex3d(0.0, 0.25, 0.15);
   glTexCoord2f(1,1); glVertex3d(0.0, 0.25, -0.15);
   glTexCoord2f(0,1); glVertex3d(0.4, 0.05, -0.15);
   glTexCoord2f(0,0); glVertex3d(0.4, 0.05, 0.15);
   glEnd();

   // bottom
   glBegin(GL_POLYGON);
   glNormal3f(0,-1,0);
   glTexCoord2f(0,1); glVertex3d(0.0, 0.05, 0.15);
   glTexCoord2f(0,0); glVertex3d(0.0, 0.05, -0.15);
   glTexCoord2f(1,0); glVertex3d(0.4, 0.05, -0.15);
   glTexCoord2f(1,1); glVertex3d(0.4, 0.05, 0.15);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   
   //cubes for bodywork (middle x,y,z - extenting from middle x,y,z - color - texture - texture array)

   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(1,1);
   glColor3f(1,1,1);
   cube(-0.05,0.15,0 , 0.05,0.1,0.15 , 0 , b1,b2,b3, 5, textures); // Front Panel

   cube(-0.225,0.1,0 , 0.175,0.05,0.15, 0 , 0.1,0.1,0.1, 5, textures); // Seat Area
   glColor3f(1,1,1);
   cube(-0.5,0.15,0 , 0.1,0.1,0.15, 0 , b1,b2,b3, 5, textures); // Rear Panel
   glDisable(GL_POLYGON_OFFSET_FILL);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,textures[10]);
   glBegin(GL_POLYGON);
   glTexCoord2f(0,0); glVertex3d(-.1,0.05,-0.1);
   glTexCoord2f(0,2); glVertex3d(-.1,0.25,-0.1);
   glTexCoord2f(1,2); glVertex3d(-.1,0.25,0.1);
   glTexCoord2f(1,0); glVertex3d(-.1,0.05,0.1);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   glColor3f(0,0,0);
   glBegin(GL_POLYGON);
   glVertex3d(-.1,0.05,-0.15);
   glVertex3d(-.1,0.25,-0.15);
   glVertex3d(-.1,0.25,-0.1);
   glVertex3d(-.1,0.05,-0.1);

   glEnd();
   glBegin(GL_POLYGON);
   glVertex3d(-.1,0.05,0.1);
   glVertex3d(-.1,0.25,0.1);
   glVertex3d(-.1,0.25,0.15);
   glVertex3d(-.1,0.05,0.15);
   glEnd();

   /*
   sidepods
   */
   glColor3f(a1,a2,a3);
   cube(-0.3,0.125,0.2 , 0.3,0.075,0.05 , 0 , a1,a2,a3, 8, textures);
   cube(-0.3,0.125,-0.2 , 0.3,0.075,0.05 , 0 , a1,a2,a3, 8, textures);

   /*
   rear wing pylons
   */
   glColor3f(b1,b2,b3);
   cube(-0.55,0.25,0.125 , .01,.05,.01 , 0 , b1,b2,b3, 8, textures);
   cube(-0.55,0.25,-0.125 , .01,.05,.01 , 0 , b1,b2,b3, 8, textures);
   

   /*
   tapered tail section
   */
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,textures[8]);
   glColor3f(a1,a2,a3);
   /* side facing positive z */
   glNormal3f(-0.7930276717,0,1);
   glBegin(GL_POLYGON);
   glTexCoord2f(0,0); glVertex3d(-0.6,0.05,0.15);
   glTexCoord2f(1,0); glVertex3d(-0.75,0.125,0.025);
   glTexCoord2f(1,1); glVertex3d(-0.75,0.175,0.025);
   glTexCoord2f(0,1); glVertex3d(-0.6,0.25,0.15);
   glEnd();
   /* side facing negative z */
   glNormal3f(-0.7930276717,0,-1);
   glBegin(GL_POLYGON);
   glTexCoord2f(0,0); glVertex3d(-0.6,0.05,-0.15);
   glTexCoord2f(1,0); glVertex3d(-0.75,0.125,-0.025);
   glTexCoord2f(1,1); glVertex3d(-0.75,0.175,-0.025);
   glTexCoord2f(0,1); glVertex3d(-0.6,0.25,-0.15);
   glEnd();
   /* top */
   glBegin(GL_POLYGON);
   glNormal3f(-0.4187762688,1,0); // these triangles actually have the same ratio as the top of the nose cone. lucky but unintentional 
   glTexCoord2f(0,0); glVertex3d(-0.6,0.25,-0.15);
   glTexCoord2f(0,1); glVertex3d(-0.6,0.25,0.15);
   glTexCoord2f(1,1); glVertex3d(-0.75,0.175,0.025);
   glTexCoord2f(1,0); glVertex3d(-0.75,0.175,-0.025);
   glEnd();
   /* bottom */
   glBegin(GL_POLYGON);
   glNormal3f(-0.4187762688,-1,0);
   glTexCoord2f(0,0); glVertex3d(-0.6,0.05,-0.15);
   glTexCoord2f(0,1); glVertex3d(-0.6,0.05,0.15);
   glTexCoord2f(1,1); glVertex3d(-0.75,0.125,0.025);
   glTexCoord2f(1,0); glVertex3d(-0.75,0.125,-0.025);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   /*
   back of tail
   */
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,textures[9]);
   glColor3f(1,1,1);
   glBegin(GL_POLYGON);
   glNormal3f(-1,0,0);
   glTexCoord2f(0,0); glVertex3d(-0.75,0.175,-0.025);
   glTexCoord2f(1,0); glVertex3d(-0.75,0.175,0.025);
   glTexCoord2f(1,1); glVertex3d(-0.75,0.125,0.025);
   glTexCoord2f(0,1); glVertex3d(-0.75,0.125,-0.025);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   // Left Front
   glPushMatrix();
   glTranslatef(0.1,0.1,-0.3);
   glRotated(rotation[displayTime],0,1,0); // give the appearence of the wheels turning
   glRotated(-displayTime*10,0,0,1); // give the appearance of the wheels/tires spinning
   wheel(textures);
   glPopMatrix();

   // Right Front
   glPushMatrix();
   glTranslatef(0.1,0.1,0.2);
   glRotated(rotation[displayTime],0,1,0);
   glRotated(-displayTime*10,0,0,1);
   wheel(textures);
   glPopMatrix();

   // Left Rear
   glPushMatrix();
   glTranslatef(-0.71,0.1,-0.3);
   glRotated(-displayTime*10,0,0,1);
   wheel(textures);
   glPopMatrix();

   //Right Rear
   glPushMatrix();
   glTranslatef(-0.71,0.1,0.2);
   glRotated(-displayTime*10,0,0,1);
   wheel(textures);
   glPopMatrix();

   // Rear Axels
   glPushMatrix();
   glRotated(-20,1,0,0);
   tube(-0.71,0.165,-0.25, 1,1,1, 0.005, 0.3, 4, textures);
   glPopMatrix();
   glPushMatrix();
   glRotated(-6,1,0,0);
   tube(-0.71,0.12,-0.25, 1,1,1, 0.005, 0.3, 4, textures);
   glPopMatrix();
   
   glPushMatrix();
   glRotated(20,1,0,0);
   tube(-0.71,0.165,0.0, 1,1,1, 0.005, 0.2, 4, textures);
   glPopMatrix();
   glPushMatrix();
   glRotated(6,1,0,0);
   tube(-0.71,0.12,0.0, 1,1,1, 0.005, 0.2, 4, textures);
   glPopMatrix();

   // Front Axle
   tube(0.1,0.1,-0.25, 1,1,1, 0.01, 0.5, 4, textures);

   // Steering Wheel
   glPushMatrix();
   glTranslatef(-0.15,0.22,0);
   glRotated(-rotation[displayTime]*3,1,0,0);
   glRotated(90,0,1,0); 
   steeringwheel();
   glPopMatrix();


   /*
   front wing
   */
   glPushMatrix();
   glTranslated(.35,.06,0);
   glScaled(0.6,0.8,0.8);
   wing(textures,a1,a2,a3,b1,b2,b3);
   glPopMatrix();

   /*
   rear wing
   */
   glPushMatrix();
   glTranslated(-.45,.3,0);
   wing(textures,a1,a2,a3,b1,b2,b3);
   glPopMatrix();
   
   

   // Windshield -- DRAWN LAST --
   glUseProgram(0);
   glPushMatrix();
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glBegin(GL_QUADS);
   //  Front
   glColor4f(0,0.5,0.5,0.5);
   glNormal3f(1, 0, 0);
   glVertex3f(0,0.25,-0.15); // bottom left 0,0.25,-0.15
   glVertex3f(0,0.25,0.15); // bottom right
   glVertex3f(-0.05,0.32,0.12); // top right
   glVertex3f(-0.05,0.32,-0.12); // top left

   // PUSHBACK AND DUPLICATE (FOR ALL 3) SO LIGHTING IS CORRECT

   glVertex3f(-0.1,0.25,0.15); // bottom left 0,0.25,-0.15
   glVertex3f(0.0,0.25,0.15); // bottom right
   glVertex3f(-0.05,0.32,0.12); // top right
   glVertex3f(-0.1,0.32,0.12); // top left

   glVertex3f(-0.1,0.25,-0.15); // bottom left 0,0.25,-0.15
   glVertex3f(0.0,0.25,-0.15); // bottom right
   glVertex3f(-0.05,0.32,-0.12); // top right
   glVertex3f(-0.1,0.32,-0.12); // top left

   glEnd();
   glDisable(GL_BLEND);
   glPopMatrix();



   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   /*
   glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
    glEnable(GL_TEXTURE_GEN_T);
    glBindTexture(GL_TEXTURE_2D, theTexture[2]);
    glutSolidCube(2);
    glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
    glDisable(GL_TEXTURE_GEN_T);
   */

}