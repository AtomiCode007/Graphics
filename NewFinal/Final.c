/*
*     KEY BINDINGS
*     m/M     change view mode
*     w       move forward (first person)
*     s       move backward (first person)
*     a       turn left (first person)
*     d       turn right (first person)
*     q       strafe left (first person)
*     e       strafe right (first person)
*     arrows  change view angle
*     ]/[     increase/decrease size of the cars
*     /       toggle axis
*     0       reset view angle 
*     +/-     increase/decrease FOV
*     l/L     toggle lightsource on/off
*     </>     rasise/lower light source
*     k/K     pause lightsource
*     ,/.     move lightsource left/right (when paused)
*     Y/y     increase/decrease ambient values
*     U/u     increase/decrease diffuse values
*     I/i     increase/decrease specular values
*     O/o     increase/decrease emission values
*     P/p     increase/decrease shininess values
*     F1      Toggle smooth/flat shading
*     F2      Toggle local viewer mode
*     F3      Toggle light distance (1/5)
*     F8      Change ball increment
*     F9      Invert bottom normal
*     ESC     exit  
*/

#include "CSCIx229.h"

int th=30;        //  Azimuth of view angle
int ph=20;        //  Elevation of view angle
int move = 1;    //  Move light 
int axes=0;       //  Display axes
int mode=1;       //  Projection mode
double carSize=1; //  Size of the cars
double asp = 1;   //  Aspect ratio
int fov=55;       //  Field of view (for perspective)
int light = 1;    //  lighting
double dim=5.0;   //  Size of world
double increment = 0.01;//  increment the forward/backward movement

/* Eye coordinates stored globally */
double Ex = 0;
double Ey = 0;
double Ez = 5;    // Because of world size

/* Camera perspective coordinates stored globally - no looking up as of yet */
double Cx = 0;
double Cz = 0;

int turn = 0; // rotation in degrees. needs to be an int to use modulo

/* Light values */
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  = 0.3;  // Elevation of light
unsigned int textures[10];


static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}



/*
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, double c1, double c2, double c3,
                 int tex)
{
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_COMBINE);
   glBindTexture(GL_TEXTURE_2D,textures[tex]);
   // arrays and values for specular shininess and emissions colors 
   float black[] = {0,0,0,1.0};
   float white[] = {1.0,1.0,1.0,1.0};
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
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
*/



static void tube(double x, double y, double z, 
                  double dx, double dy, double dz,
                  double radius, double width, int tex)
{
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,textures[tex]);
   float black[] = {0,0,0,1.0};
   float white[] = {1.0,1.0,1.0,1.0};
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
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
 /* 
 //need to figure out how to do normals
static void disk(double x, double y, double z, double radius)
{
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(2,2);

   glPushMatrix();
   glColor3f(1,1,1);
   glTranslatef(x,y,z);
   glNormal3f(0,0,1);
   glBegin(GL_TRIANGLE_FAN);
   for(int i = 0; i <= 360; i+=15)
   {
      glVertex3d(0.1*Cos(i),0.1*Sin(i),0);
   }
   glEnd();
   glPopMatrix();
}
*/


/* display function was getting crowded.moved the function to
*  draw the grid. can be updated later to do other road segments.
*  added more polygons to make road smoother.
*/
static void grid()
{
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,textures[2]);
   float road[] = {0.2,0.2,0.2,1.0};
   glColor3f(1,1,1);
   
      double a = -10.0;
      double b = -1.5;
      double c = 0;
      double d = .25;
      double e = 0;
      double f = .33;
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
            glTexCoord2f(e,c); glVertex3d(a,-0.40,b); //1
            glTexCoord2f(e,d); glVertex3d(a+5,-0.40,b); //4
            glTexCoord2f(f,d); glVertex3d(a+5,-0.40,b+1); //3
            glTexCoord2f(f,c); glVertex3d(a,-0.40,b+1);  //2
            
            
            glEnd();
            glDisable(GL_POLYGON_OFFSET_FILL);
            b += 1;
            e += .33;
            f += .33;
         }
         b = -1.5;
         e = 0;
         f = .33;
         a += 5.0;
         c += .25;
         d += .25;
      }
   glDisable(GL_TEXTURE_2D);

}

static void ground()
{
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,textures[0]);
   glColor3f(1,1,1);
   
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(2,2);
   glBegin(GL_QUADS);
   glNormal3f(0,1,0);
   glTexCoord2f(0,0); glVertex3d(-10.0,-0.40,-10); 
   glTexCoord2f(1,0); glVertex3d(10,-0.40,-10); 
   glTexCoord2f(1,1); glVertex3d(10,-0.40,10); 
   glTexCoord2f(0,1); glVertex3d(-10,-0.40,10); 
   glEnd();
   glDisable(GL_POLYGON_OFFSET_FILL);

   glDisable(GL_TEXTURE_2D);
}

/*
 *  Draw Formula One car!
 *    at (x,y,z)
 *    nose towards (dx,dy,dz)
 *    up towards (ux,uy,uz)
 *    two tone paint job passed in
 *    as six doubles (a1,a2,a3,b1,b2,b3)
 *    scale makes the car larger or smaller
 */

/*
static void FormulaOne(double x,double y,double z,
                       double dx,double dy,double dz,
                       double ux,double uy, double uz, 
                       double a1, double a2, double a3, 
                       double b1, double b2, double b3, 
                       double scale)
{
   // arrays and values for specular shininess and emissions colors
   float black[] = {0,0,0,1.0};
   float white[] = {1.0,1.0,1.0,1.0};
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT,GL_EMISSION,black);
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
   // cylinders for tires 
   //GLUquadricObj *tyler = gluNewQuadric();
   //gluQuadricDrawStyle(tyler, GLU_FILL);
   //  Save current transforms
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glMultMatrixd(mat);
   
   //triangles for outside of nose.
    
   glColor3f(a1,a2,a3);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,textures[8]);
   glBegin(GL_TRIANGLES);
   
   glNormal3f(0,0,1);
   glTexCoord2f(0,0); glVertex3d(0.0, 0.05*scale, 0.15*scale);
   glTexCoord2f(1,0); glVertex3d(0.0, 0.25*scale, 0.15*scale);
   glTexCoord2f(0,1); glVertex3d(0.4*scale, 0.05*scale, 0.15*scale);

   glNormal3f(0,0,-1);
   glTexCoord2f(0,0); glVertex3d(0.0, 0.05*scale, -0.15*scale);
   glTexCoord2f(1,1); glVertex3d(0.0, 0.25*scale, -0.15*scale);
   glTexCoord2f(0,1); glVertex3d(0.4*scale, 0.05*scale, -0.15*scale);
   glEnd();

   
   //polygons for top and bottom of nose
   
   
   // top
   glBegin(GL_POLYGON);
   glNormal3f(.4187762688,1,0); // had to find angles of the triangle and use the ratio between the two to determine these values
   glTexCoord2f(1,0); glVertex3d(0.0, 0.25*scale, 0.15*scale);
   glTexCoord2f(1,1); glVertex3d(0.0, 0.25*scale, -0.15*scale);
   glTexCoord2f(0,1); glVertex3d(0.4*scale, 0.05*scale, -0.15*scale);
   glTexCoord2f(0,0); glVertex3d(0.4*scale, 0.05*scale, 0.15*scale);
   glEnd();

   // bottom
   glBegin(GL_POLYGON);
   glTexCoord2f(0,1); glVertex3d(0.0, 0.05*scale, 0.15*scale);
   glTexCoord2f(0,0); glVertex3d(0.0, 0.05*scale, -0.15*scale);
   glTexCoord2f(1,0); glVertex3d(0.4*scale, 0.05*scale, -0.15*scale);
   glTexCoord2f(1,1); glVertex3d(0.4*scale, 0.05*scale, 0.15*scale);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   
   //front wing
   
   cube(0.265*scale,0.07*scale,0.0, 0.065*scale,0.01*scale,0.3*scale, 0, 1,1,1, 4);

   
   //cubes for bodywork
   
   cube(-0.1*scale,0.15*scale,0 , 0.1*scale,0.1*scale,0.15*scale , 0 , b1,b2,b3, 5);
   cube(-0.3*scale,0.1*scale,0 , 0.1*scale,0.05*scale,0.15*scale, 0 , 0.1,0.1,0.1, 5);
   cube(-0.5*scale,0.15*scale,0 , 0.1*scale,0.1*scale,0.15*scale, 0 , b1,b2,b3, 5);

   
   //sidepods
   
   cube(-0.3*scale,0.125*scale,0.2*scale , 0.3*scale,0.075*scale,0.05*scale , 0 , a1,a2,a3, 8);
   cube(-0.3*scale,0.125*scale,-0.2*scale , 0.3*scale,0.075*scale,0.05*scale , 0 , a1,a2,a3, 8);

   
   //rear wing pylons
   
   cube(-0.55*scale,0.25*scale,0.125*scale , .01*scale,.05*scale,.01*scale , 0 , b1,b2,b3, 8);
   cube(-0.55*scale,0.25*scale,-0.125*scale , .01*scale,.05*scale,.01*scale , 0 , b1,b2,b3, 8);

   
   //rear wing
   
   cube(-0.55*scale,.3*scale,0 , 0.1*scale,0.02*scale,0.3*scale , 0 , 1,1,1, 4);

   
   //tapered tail section
   
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,textures[8]);
   glColor3f(a1,a2,a3);
   // side facing positive z
   glNormal3f(-0.7930276717,0,1);
   glBegin(GL_POLYGON);
   glTexCoord2f(0,0); glVertex3d(-0.6*scale,0.05*scale,0.15*scale);
   glTexCoord2f(1,0); glVertex3d(-0.75*scale,0.125*scale,0.025*scale);
   glTexCoord2f(1,1); glVertex3d(-0.75*scale,0.175*scale,0.025*scale);
   glTexCoord2f(0,1); glVertex3d(-0.6*scale,0.25*scale,0.15*scale);
   glEnd();
   // side facing negative z 
   glNormal3f(-0.7930276717,0,-1);
   glBegin(GL_POLYGON);
   glTexCoord2f(0,0); glVertex3d(-0.6*scale,0.05*scale,-0.15*scale);
   glTexCoord2f(1,0); glVertex3d(-0.75*scale,0.125*scale,-0.025*scale);
   glTexCoord2f(1,1); glVertex3d(-0.75*scale,0.175*scale,-0.025*scale);
   glTexCoord2f(0,1); glVertex3d(-0.6*scale,0.25*scale,-0.15*scale);
   glEnd();
   // top 
   glBegin(GL_POLYGON);
   glNormal3f(-0.4187762688,1,0); // these triangles actually have the same ratio as the top of the nose cone. lucky but unintentional 
   glTexCoord2f(0,0); glVertex3d(-0.6*scale,0.25*scale,-0.15*scale);
   glTexCoord2f(0,1); glVertex3d(-0.6*scale,0.25*scale,0.15*scale);
   glTexCoord2f(1,1); glVertex3d(-0.75*scale,0.175*scale,0.025*scale);
   glTexCoord2f(1,0); glVertex3d(-0.75*scale,0.175*scale,-0.025*scale);
   glEnd();
   // bottom 
   glBegin(GL_POLYGON);
   glNormal3f(-0.4187762688,-1,0);
   glTexCoord2f(0,0); glVertex3d(-0.6*scale,0.05*scale,-0.15*scale);
   glTexCoord2f(0,1); glVertex3d(-0.6*scale,0.05*scale,0.15*scale);
   glTexCoord2f(1,1); glVertex3d(-0.75*scale,0.125*scale,0.025*scale);
   glTexCoord2f(1,0); glVertex3d(-0.75*scale,0.125*scale,-0.025*scale);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   
   //back of tail
   
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,textures[9]);
   glColor3f(1,1,1);
   glBegin(GL_POLYGON);
   glNormal3f(-1,0,0);
   glTexCoord2f(0,0); glVertex3d(-0.75*scale,0.175*scale,-0.025*scale);
   glTexCoord2f(1,0); glVertex3d(-0.75*scale,0.175*scale,0.025*scale);
   glTexCoord2f(1,1); glVertex3d(-0.75*scale,0.125*scale,0.025*scale);
   glTexCoord2f(0,1); glVertex3d(-0.75*scale,0.125*scale,-0.025*scale);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   
   //outside tire disks
   
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,textures[7]);
   glColor3f(0.1,0.1,0.1);
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(2,2);

   glPushMatrix();
   glTranslatef(-0.71*scale,0.1*scale,0.3*scale);
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
   glTranslatef(-0.71*scale,0.1*scale,-0.3*scale);
   glNormal3f(0,0,-1);
   glBegin(GL_TRIANGLE_FAN);
   for(int i = 0; i <= 360; i+=15)
   {
      glTexCoord2f(0.5*Cos(i)+0.5,0.5*Sin(i)+0.5);
      glVertex3f(0.1*Cos(i),0.1*Sin(i),0);
   }
   glEnd();
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.1*scale,0.1*scale,0.3*scale);
   glNormal3f(0,0,1);
   glBegin(GL_TRIANGLE_FAN);
   for(int i = 0; i <= 360; i+=15)
   {
      glTexCoord2f(0.5*Cos(i)+0.5,0.5*Sin(i)+0.5);
      glVertex3f(0.1*Cos(i),0.1*Sin(i),0);
   }
   glEnd();
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.1*scale,0.1*scale,-0.3*scale);
   glNormal3f(0,0,-1);
   glBegin(GL_TRIANGLE_FAN);
   for(int i = 0; i <= 360; i+=15)
   {
      glTexCoord2f(0.5*Cos(i)+0.5,0.5*Sin(i)+0.5);
      glVertex3f(0.1*Cos(i),0.1*Sin(i),0);
   }
   glEnd();
   glPopMatrix();

   glPushMatrix();
   glTranslatef(-0.71*scale,0.1*scale,0.2*scale);
   glNormal3f(0,0,-1);
   glBegin(GL_TRIANGLE_FAN);
   for(int i = 0; i <= 360; i+=15)
   {
      glTexCoord2f(0.5*Cos(i)+0.5,0.5*Sin(i)+0.5);
      glVertex3f(0.1*Cos(i),0.1*Sin(i),0);
   }
   glEnd();
   glPopMatrix();

   glPushMatrix();
   glTranslatef(-0.71*scale,0.1*scale,-0.2*scale);
   glNormal3f(0,0,1);
   glBegin(GL_TRIANGLE_FAN);
   for(int i = 0; i <= 360; i+=15)
   {
      glTexCoord2f(0.5*Cos(i)+0.5,0.5*Sin(i)+0.5);
      glVertex3f(0.1*Cos(i),0.1*Sin(i),0);
   }
   glEnd();
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.1*scale,0.1*scale,0.2*scale);
   glNormal3f(0,0,-1);
   glBegin(GL_TRIANGLE_FAN);
   for(int i = 0; i <= 360; i+=15)
   {
      glTexCoord2f(0.5*Cos(i)+0.5,0.5*Sin(i)+0.5);
      glVertex3f(0.1*Cos(i),0.1*Sin(i),0);
   }
   glEnd();
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.1*scale,0.1*scale,-0.2*scale);
   glNormal3f(0,0,1);
   glBegin(GL_TRIANGLE_FAN);
   for(int i = 0; i <= 360; i+=15)
   {
      glTexCoord2f(0.5*Cos(i)+0.5,0.5*Sin(i)+0.5);
      glVertex3f(0.1*Cos(i),0.1*Sin(i),0);
   }
   glEnd();
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

   
   //tires
   

   tube(-0.71*scale, 0.1*scale, 0.2*scale, scale, scale, scale, 0.1, 0.1, 1);
   tube(-0.71*scale, 0.1*scale, -0.3*scale, scale, scale, scale, 0.1, 0.1, 1);
   tube(0.1*scale, 0.1*scale, 0.2*scale, scale, scale, scale, 0.1, 0.1, 1);
   tube(0.1*scale, 0.1*scale, -0.3*scale, scale, scale, scale, 0.1, 0.1, 1);

   //axels
   tube(-0.71*scale,0.1*scale,-0.25*scale, scale,scale,scale, 0.01, 0.5, 4);
   tube(0.1*scale,0.1*scale,-0.25*scale, scale,scale,scale, 0.01, 0.5, 4);

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}
*/

void display()
{
   const double len=1.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();

   /* first person mode */
   if (mode == 2)
   {
      th = 0;
      ph = 0;
      Cx = +2*dim*Sin(turn); // changing perspective based on the rotation
      Cz = -2*dim*Cos(turn);
      gluLookAt(Ex,Ey,Ez, Cx+Ex,Ey,Cz+Ez, 0,1,0); // cos(ph) will always be 1
   }

   /* perspective mode
   *  using independent Ex, Ey, Ez so that their values have not been 
   *  influenced by and will not influence first person mode */
   if (mode == 1)
   {
      double ExP = -2*dim*Sin(th)*Cos(ph);
      double EyP = +2*dim*Sin(ph);
      double EzP = +2*dim*Cos(th)*Cos(ph);
      //gluLookAt(Ex,Ey,Ez, 0,0,0, 0,Cos(ph),0);
      gluLookAt(ExP,EyP,EzP , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light position
      float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 0.1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  Location of viewer for specular calculations
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
      glDisable(GL_LIGHTING);

   /* at, nose towards, up towards */
   FormulaOne(1,-0.4,1 , 1,0,0 , 0,1,0 , 0.8,0,0 , 1,1,1, carSize);
   FormulaOne(-1,-0.4,1 , 1,0,0 , 0,1,0 , 0.8,0.8,1.0 , 1,0.5,0.0, carSize);
   FormulaOne(1,-0.4,-1 , 1,0,0 , 0,1,0 , 0.8,0.8,0.0 , 0.2,1,0.2, carSize);
   FormulaOne(-1,-0.4,-1 , 1,0,0 , 0,1,0 , 0.8,0.6,1.0 , 0.5,0.1,0.3, carSize);
   
   grid();
   ground();
   
   

   
   if (axes)
   {
      glBegin(GL_LINES);
      glColor3f(1.0,1.0,1.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Five pixels from the lower left corner of the window
   glColor3f(1,1,1);
   glWindowPos2i(5,40);
   //  Print the text string
   Print("Angle=%d,%d",th,ph);
   Print("  Car Scale=%f", carSize);
   if (mode == 0)
      Print("  Orthogonal Mode");
   else if (mode == 1)
      Print("  Perspective Mode");
   else 
      Print("  First Person Mode");
   Print("  FOV=%d", fov);
   glWindowPos2i(5,20);
   Print("Ambient = %d, Diffuse = %d, Specular = %d, Emission = %d, Shininess = %f, zh = %d", ambient,diffuse,specular,emission,shiny,zh);
   //  Render the scene
   glFlush();
   //  Make the rendered scene visible
   glutSwapBuffers();
}

void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

void special(int key,int x,int y)
{
   if (mode != 2)
   {
      //  Right arrow key - increase angle by 5 degrees
      if (key == GLUT_KEY_RIGHT)
        th += 5;
      //  Left arrow key - decrease angle by 5 degrees
      else if (key == GLUT_KEY_LEFT)
        th -= 5;
      //  Up arrow key - increase elevation by 5 degrees capped at 180
      else if (key == GLUT_KEY_UP)
      {
        ph += 5;
        /* keeps us from going underground */
         if (ph > 180)
            ph = 180;
      }
      //  Down arrow key - decrease elevation by 5 degrees capped at 0
      else if (key == GLUT_KEY_DOWN)
      {
        ph -= 5;
        /* keeps us from going underground */
         if (ph < 0)
            ph = 0;
      }
      //  Keep angles to +/-360 degrees
      th %= 360;
      ph %= 360;
   }

   //  Smooth color model
   if (key == GLUT_KEY_F1)
      smooth = 1-smooth;
   //  Local Viewer
   else if (key == GLUT_KEY_F2)
      local = 1-local;
   else if (key == GLUT_KEY_F3)
      distance = (distance==1) ? 5 : 1;
   //  Toggle ball increment
   else if (key == GLUT_KEY_F8)
      inc = (inc==10)?3:10;
   //  Flip sign
   else if (key == GLUT_KEY_F9)
      one = -one;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(mode?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
   {
      th = 0;
      ph = 0;
      fov = 55;
      Ex = 0;
      Ey = 0;
      Ez = 5;
      turn = 0;
   }
   //  Toggle axes
   else if (ch == '/')
      axes = 1-axes;
   // Resize the cars
   else if (ch == ']')
      carSize = carSize + 0.05;
   else if (ch == '[')
   {
      carSize = carSize - 0.05;
      /* check to make sure scale of car never reaches or goes below 0 */
      if (carSize < 0.05)
         carSize = 0.05;
   }
   else if (ch == 'm' || ch == 'M')
   {
      mode = mode+1;
      if (mode > 2)
         mode = 0;
   }
   /* lighting */
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Light elevation
   else if (ch=='>')
      ylight -= 0.1;
   else if (ch=='<')
      ylight += 0.1;
   else if (ch == 'k' || ch == 'K')
      move = 1-move;
   //  Move light
   else if (ch == ',')
      zh += 1;
   else if (ch == '.')
      zh -= 1;
   //  Ambient level
   else if (ch=='y' && ambient>0)
      ambient -= 5;
   else if (ch=='Y' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='u' && diffuse>0)
      diffuse -= 5;
   else if (ch=='U' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='i' && specular>0)
      specular -= 5;
   else if (ch=='I' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='o' && emission>0)
      emission -= 5;
   else if (ch=='O' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='p' && shininess>-1)
      shininess -= 1;
   else if (ch=='P' && shininess<7)
      shininess += 1;
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   /* only allow wasdqe movement in first person mode */
   if (mode == 2) 
   {
      if (ch == 'w')
      {
         /* represents the eye relative to the camera in the 'forward' direction */
         Ex += Cx*increment;
         Ez += Cz*increment;
      }
      else if (ch == 'a')
      {
         turn -= 3; // 'turn' the camera to the left
      }
      else if (ch == 's')
      {
         /* represents the eye relative to the camera in the 'backward' direction */
         Ex -= Cx*increment;
         Ez -= Cz*increment;
      }
      else if (ch == 'd')
      {
         turn += 3; // 'turn' the camera to the right
      }
      /* strafe left */
      else if (ch == 'q')
      {
         Ex += Cz*increment;
         Ez -= Cx*increment;
      }
      /* strafe right */
      else if (ch == 'e')
      {
         Ex -= Cz*increment;
         Ez += Cx*increment;
      }

      turn %= 360; // will keep angles between -360 and 360 degrees
   }
   // reproject
   Project(mode?fov:0,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(mode?fov:0,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(1200,1000);
   glutCreateWindow("Lighting");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);

   textures[0] = LoadTexBMP("concrete.bmp");
   textures[1] = LoadTexBMP("tiretread.bmp");
   textures[2] = LoadTexBMP("road.bmp");
   textures[3] = LoadTexBMP("CF.bmp");
   textures[4] = LoadTexBMP("Carbon.bmp");
   textures[5] = LoadTexBMP("metFlo.bmp");
   textures[6] = LoadTexBMP("Aluminum.bmp");
   textures[7] = LoadTexBMP("S71B.bmp");
   textures[8] = LoadTexBMP("metalPlate.bmp");
   textures[9] = LoadTexBMP("taillight.bmp");

   glutIdleFunc(idle);
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}