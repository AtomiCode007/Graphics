/*
 *
 *  
 *
 *  Key bindings:
 *  
      m/n 	     Change mode(Projection, perspective, First Person)
      =/-        Increase and decrease the scale of the planes.
      ./,        Change the scenes 0-4
      f/v        Increase and decrease the field of view
      g          Toggle axes
      arrows     Change view angle
      R          Reset view angle
      ESC        Exit
      w,a,s,d.   Navigate in First person mode
      q/e        Rotate the FPP camera left and right
 */
#include "CSCIx229.h"

int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int mode = 1;     //  Change between perspective and orthogonal projections 
double moveplane = 0;
//double zh=0;      //  Pitch, Yaw of the plane / movement
int axes=1;       //  Display axes
double scale = 1; //scale changer
int fov=55;       //  Field of view (for perspective)
double asp= 1;     //  Aspect ratio
double dim= 5.0;   //  Size of world
int num = 0;      //  Change the scene, different planes in motion and or position
double Ex = 0;    //
double Ey = 0;    // Start position of the FPP
double Ez = 5.0;  // 
double Perx = 0;  //
double Pery = 0;  // Look angle of the FPP camera
double Perz = 0;  //
double Strafe = 0;
double stepSize = .02;
unsigned int texture[5];  //  Texture names
//
//Light values
int side = 1; //Two sided lighing on always.
int move      =   1;  //  Move light
int one       =   1;  // Unit value
int distance  =   7;  // Light distance
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
float ylight  =   6;  // Elevation of light
int light = 1; //Light switch
//GLUquadric* qobj;

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Draw vertex in polar coordinates with normal
 */
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

/*
 *  Draw solid airplane
 *    at (x,y,z)
 *    nose towards (dx,dy,dz)
 *    up towards (ux,uy,uz)
 */
static void SolidPlane(double x,double y,double z,double dx,double dy,double dz,
                       double ux,double uy, double uz, double scale)
{  
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   // Dimensions used to size airplane
   const double wid= .099 * scale;
   const double nose=+ 0.98 * scale;
   const double cone= .60 * scale;
   const double wing= .40 * scale;
   const double vertPilf = .35 * scale;
   const double vertPilr = .05 * scale;
   const double strk=- .35 * scale;
   const double tail=- .60 * scale;
   const double braceDiag = .025 * scale;
   const double vertwid = .018 * scale;
   const double tailFin = .35 * scale;
   const double wingspan = 1 * scale;
   //const double engine = .159 * scale;
   //const double prop = .221 * scale;
   const double wingDepth = .02 * scale;
   int d=15;
   int index = 0;
   int b;
   //  Unit vector in direction of flght
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
   //GLU object in order to make cylinders
   //GLUquadricObj *ptr = gluNewQuadric();
   //gluQuadricDrawStyle(ptr,GLU_FILL);
   
   //  Save current transforms
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glMultMatrixd(mat);

   //Rotate each instance of a cylinder as needed
   /*glPushMatrix();
   glTranslatef(cone,0,0);
   glRotatef(90,0,1,0);
   glColor3f(0.8,.8,.8);
   gluCylinder(ptr,wid,0.0,engine,100,100);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(nose,0,0);
   glRotatef(90,0,1,0);
   glColor3f(0.95,.95,.95);
   gluCylinder(ptr,prop,prop,.01,100,100);
   glPopMatrix();
   */
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[2]);

   glColor3f(0.88,0.88,.88);
   glBegin(GL_TRIANGLES);

   //Left Triangle(looking in front of plane)
   glNormal3f(.291, 0,1);
   glTexCoord2f(0.15,1);      glVertex3d(nose, 0.0, 0.0);
   glTexCoord2f(0.55,0);      glVertex3d(cone, wid, wid);
   glTexCoord2f(0.15,0);      glVertex3d(cone,-wid, wid);

   //Right Triangle
   glNormal3f(.291, 0,-1);

   glTexCoord2f(0.15,1);      glVertex3d(nose, 0.0, 0.0);
   glTexCoord2f(0.55,0);      glVertex3d(cone,-wid,-wid);
   glTexCoord2f(0.15,0);      glVertex3d(cone, wid,-wid);

   //Top Triangle
   glNormal3f(.291, 1,0);
   glTexCoord2f(0.15,1);      glVertex3d(nose, 0.0, 0.0);
   glTexCoord2f(0.15,0);      glVertex3d(cone, wid,-wid);
   glTexCoord2f(0.55,0);      glVertex3d(cone, wid, wid);

   //Bottom Triangle
   glNormal3f(.291, -1,0);
   glTexCoord2f(0.15,1);      glVertex3d(nose, 0.0, 0.0);
   glTexCoord2f(0.55,0);      glVertex3d(cone,-wid, wid);
   glTexCoord2f(0.15,0);      glVertex3d(cone,-wid,-wid);
   
   glEnd();
   glDisable(GL_TEXTURE_2D);

   //Propeller blades   
   glBegin(GL_TRIANGLE_FAN);

   for (index=0;index<=360;index+=d)
   {   
      glColor3f(0.95,0.95,0.95);
      glNormal3f(-1, 0,0);
      glVertex3d(nose, Sin(index)*Cos(d-92)*scale , Cos(index)*Cos(d-92)*scale);   
   }
   glEnd();

   //Propeller blades   
   glBegin(GL_TRIANGLE_FAN);

   for (index=0;index<=360;index+=d)
   {   
      glColor3f(0.95,0.95,0.95);
      glNormal3f(-1, 0,0);
      glVertex3d(nose+.0001, Sin(index)*Cos(d-92)*scale , Cos(index)*Cos(d-92)*scale);   
   }
   glEnd();

   glPushMatrix();
   glTranslatef(nose,0,0);
   glRotatef(90,0,1,0);
   glBegin(GL_QUAD_STRIP);
   for(b = 0; b < 360; b+=15)
   {
      glColor3f(1,0.25,1);
      glNormal3f(Cos(b),Sin(b),0);
      glVertex3d(Cos(d-92)*Cos(b)*scale,Cos(d-92)*Sin(b)*scale,.001);
      glVertex3d(Cos(d-92)*Cos(b)*scale,Cos(d-92)*Sin(b)*scale,0.0);
   }
   glEnd();
   glPopMatrix();
   
   //  Fuselage (square tube)
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[3]);

   glBegin(GL_QUADS);
   glColor3f(0,0,1);
   
   //Front plate
   glNormal3f( 1, 0,0);
   glVertex3d(cone, wid, wid);
   glVertex3d(cone,-wid, wid);
   glVertex3d(cone, -wid, -wid);
   glVertex3d(cone, wid, -wid);

   //Walls

   glColor3f(0.1,0.1,1);
   glNormal3f( 0, 0,1);
   glTexCoord2f(0,1);      glVertex3d(cone, wid, wid);
   glTexCoord2f(0,0);      glVertex3d(tail, wid, wid);
   glTexCoord2f(1,0);      glVertex3d(tail,-wid, wid);
   glTexCoord2f(1,1);      glVertex3d(cone,-wid, wid);
   
   //glColor3f(0,0,1);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0);      glVertex3d(cone, wid,-wid);
   glTexCoord2f(1,0);      glVertex3d(cone,-wid,-wid);
   glTexCoord2f(1,1);      glVertex3d(tail,-wid,-wid);
   glTexCoord2f(0,1);      glVertex3d(tail, wid,-wid);

   //Top of fuselage
   glNormal3f( 0, 1,0);
   glTexCoord2f(1,1);      glVertex3d(cone, wid, wid);
   glTexCoord2f(1,0);      glVertex3d(cone, wid,-wid);
   glTexCoord2f(0,0);      glVertex3d(tail, wid,-wid);
   glTexCoord2f(0,1);      glVertex3d(tail, wid, wid);

   //Bottom of fuselage
   glNormal3f( 0, -1,0);
   glTexCoord2f(1,0);      glVertex3d(cone,-wid, wid);
   glTexCoord2f(0,1);      glVertex3d(tail,-wid, wid);
   glTexCoord2f(0,0);      glVertex3d(tail,-wid,-wid);
   glTexCoord2f(1,1);      glVertex3d(cone,-wid,-wid);

   //Rear plate
   glNormal3f( -1, 0,0);
   glTexCoord2f(1,0);      glVertex3d(tail,-wid, wid);
   glTexCoord2f(0,0);      glVertex3d(tail, wid, wid);
   glTexCoord2f(0,1);      glVertex3d(tail, wid,-wid);
   glTexCoord2f(1,1);      glVertex3d(tail,-wid,-wid);
   
   glEnd();

   glDisable(GL_TEXTURE_2D);
   //  Wings (plane squares)
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[2]);

   glColor3f(1,0,0);
   glBegin(GL_QUADS);

   //Bottom wing (top)
   glNormal3f( 0, 1,0);
   glTexCoord2f(0.15,0.15);      glVertex3d(0.0, 0.0, -wingspan);
   glTexCoord2f(1,0.15);         glVertex3d(0.0, 0.0, wingspan);
   glTexCoord2f(1,1);            glVertex3d(wing, 0.0, wingspan);
   glTexCoord2f(.15,1);          glVertex3d(wing, 0.0, -wingspan);

   //Bottom wing(lower)
   glColor3f(1,0,0);
   glNormal3f( 0, -1,0);
   glTexCoord2f(0.15,0.15);      glVertex3d(0.0, -wingDepth, -wingspan);
   glTexCoord2f(1,0.15);         glVertex3d(wing, -wingDepth, -wingspan); 
   glTexCoord2f(1,1);            glVertex3d(wing, -wingDepth, wingspan);
   glTexCoord2f(0.15,1);         glVertex3d(0.0, -wingDepth, wingspan);
  
   //Bottom wing front trim
   glColor3f(1,1,0);
   glNormal3f( 1, 0,0);
   glTexCoord2f(.25,1);          glVertex3d(wing, 0.0, -wingspan);
   glTexCoord2f(.25,.15);        glVertex3d(wing, 0.0, wingspan);
   glTexCoord2f(0.15,.15);       glVertex3d(wing, -wingDepth, wingspan);
   glTexCoord2f(0.15,1);         glVertex3d(wing, -wingDepth, -wingspan);

   //Bottom wing rear trim
   glColor3f(0,1,0);
   glNormal3f( -1, 0,0);
   glTexCoord2f(.25,.15);     glVertex3d(0, 0.0, -wingspan);
   glTexCoord2f(.15,.15);     glVertex3d(0, -wingDepth, -wingspan);
   glTexCoord2f(.15,1);       glVertex3d(0, -wingDepth, wingspan);
   glTexCoord2f(.25,1);       glVertex3d(0, 0.0, wingspan);

   //Bottom wing outer trim (right)
   glColor3f(0,1,1);
   glNormal3f( 0, 0,1);
   glTexCoord2f(.25,1);          glVertex3d(wing, 0.0, wingspan);
   glTexCoord2f(.25,.15);        glVertex3d(0, 0.0, wingspan);
   glTexCoord2f(.15,.15);        glVertex3d(0, -wingDepth, wingspan);
   glTexCoord2f(.15,1);          glVertex3d(wing, -wingDepth, wingspan);
   
   //Bottom wing outer trim (left)
   glColor3f(1,0,1);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(.25,.15);           glVertex3d(wing, 0.0, -wingspan);
   glTexCoord2f(.15,.15);           glVertex3d(wing, -wingDepth, -wingspan);
   glTexCoord2f(.15,1);             glVertex3d(0, -wingDepth, -wingspan);
   glTexCoord2f(.25,1);             glVertex3d(0, 0.0, -wingspan);
   
   //Top wing
   glColor3f(1,0,0);
   glNormal3f( 0, 1,0);
   glTexCoord2f(.15,1);         glVertex3d(0.0, wing, -wingspan);
   glTexCoord2f(.15,.15);       glVertex3d(0.0, wing, wingspan);
   glTexCoord2f(1,.15);         glVertex3d(wing, wing, wingspan);
   glTexCoord2f(1,1);           glVertex3d(wing, wing, -wingspan);

   //Top wing(upper)
   
   glColor3f(1,0,0);
   glNormal3f( 0, 1,0);
   glTexCoord2f(0.15,0.15);   glVertex3d(0.0, wing+wingDepth, -wingspan);
   glTexCoord2f(1,0.15);      glVertex3d(0.0, wing+wingDepth, wingspan);
   glTexCoord2f(1,1);         glVertex3d(wing, wing + wingDepth, wingspan);
   glTexCoord2f(0.15,1);      glVertex3d(wing, wing + wingDepth, -wingspan);

   //Top wing front trim
   glColor3f(1,1,0);
   glNormal3f( 1, 0,0);
   glTexCoord2f(0.15,1);      glVertex3d(wing, wing, -wingspan);
   glTexCoord2f(0.25,1);      glVertex3d(wing, wing + wingDepth, -wingspan);
   glTexCoord2f(0.25,0.15);   glVertex3d(wing, wing + wingDepth, wingspan);
   glTexCoord2f(0.15,0.15);   glVertex3d(wing, wing, wingspan);
   
   //Top wing rear trim
   glColor3f(0,1,0);
   glNormal3f(-1,0,0);
   glTexCoord2f(0.15,0.15);   glVertex3d(0, wing, -wingspan);
   glTexCoord2f(0.15,1);      glVertex3d(0, wing, wingspan);
   glTexCoord2f(0.25,1);      glVertex3d(0, wing + wingDepth, wingspan);
   glTexCoord2f(0.25,0.15);   glVertex3d(0, wing + wingDepth, -wingspan);

   //Top wing outer trim (right)
   glColor3f(0,1,1);
   glNormal3f( 0, 0,1);
   glTexCoord2f(0.25,1);      glVertex3d(wing, wing + wingDepth, wingspan);
   glTexCoord2f(0.25,0.15);   glVertex3d(0, wing + wingDepth, wingspan);
   glTexCoord2f(0.15,0.15);   glVertex3d(0, wing, wingspan);
   glTexCoord2f(0.15,1);      glVertex3d(wing, wing, wingspan);
   
   //Top wing outer trim (left)
   glColor3f(1,0,1);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0.25,0.15);   glVertex3d(wing, wing + wingDepth, -wingspan);
   glTexCoord2f(0.15,0.15);   glVertex3d(wing, wing, -wingspan);
   glTexCoord2f(0.15,1);      glVertex3d(0, wing, -wingspan);
   glTexCoord2f(0.25,1);      glVertex3d(0, wing + wingDepth, -wingspan);

   glDisable(GL_TEXTURE_2D);
   glEnd();

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[4]);
   glBegin(GL_QUADS);
   //Support Braces
   glColor3f(1,1,0);
   //Diag Back

   //Top left to bottom right
   glNormal3f(0, -1,-.1524);
   glTexCoord2f(1,1);      glVertex3d(0.0, wing, -wingspan);
   glTexCoord2f(0,1);      glVertex3d(braceDiag, wing, -wingspan);
   glTexCoord2f(0,0);      glVertex3d(braceDiag, 0, wingspan - vertwid);
   glTexCoord2f(1,0);      glVertex3d(0.0, 0, wingspan - vertwid);

   //glColor3f(1,1,0);
   glNormal3f(0, 1,.1524);
   glTexCoord2f(0,1);      glVertex3d(0.0, wing, -wingspan + vertwid);
   glTexCoord2f(0,0);      glVertex3d(0.0, 0, wingspan);
   glTexCoord2f(1,0);      glVertex3d(braceDiag, 0, wingspan);
   glTexCoord2f(1,1);      glVertex3d(braceDiag, wing, -wingspan + vertwid);

   //glColor3f(1,1,0);
   glNormal3f(1, 0,0);
   glTexCoord2f(1,1);      glVertex3d(braceDiag, wing, -wingspan);
   glTexCoord2f(0,1);      glVertex3d(braceDiag, wing, -wingspan + vertwid);
   glTexCoord2f(0,0);      glVertex3d(braceDiag, 0, wingspan);
   glTexCoord2f(0,1);      glVertex3d(braceDiag, 0, wingspan - vertwid);

   //glColor3f(1,1,0);
   glNormal3f(-1, 0,0);
   glTexCoord2f(0,0);      glVertex3d(0, wing, -wingspan);
   glTexCoord2f(1,0);      glVertex3d(0, 0, wingspan - vertwid);
   glTexCoord2f(1,1);      glVertex3d(0, 0, wingspan);
   glTexCoord2f(0,1);      glVertex3d(0, wing, -wingspan + vertwid);
   
   //Top right to bottom left          glTexCoord2f(X,Y);      

   //glColor3f(1,1,0);
   glNormal3f(0, -1,.1524);
   glTexCoord2f(0,1);      glVertex3d(0.0, wing, wingspan);
   glTexCoord2f(0,0);      glVertex3d(0.0, 0, -wingspan + vertwid);
   glTexCoord2f(1,0);      glVertex3d(braceDiag, 0, -wingspan + vertwid);
   glTexCoord2f(1,1);      glVertex3d(braceDiag, wing, wingspan);
   
   //glColor3f(1,1,0);
   glNormal3f(0, 1,-.1524);
   glTexCoord2f(1,1);      glVertex3d(0.0, wing, wingspan - vertwid);
   glTexCoord2f(0,1);      glVertex3d(braceDiag, wing, wingspan - vertwid);
   glTexCoord2f(0,0);      glVertex3d(braceDiag, 0, -wingspan);
   glTexCoord2f(1,0);      glVertex3d(0.0, 0, -wingspan);

   //glColor3f(1,1,0);
   glNormal3f(1, 0,0);
   glTexCoord2f(0,1);      glVertex3d(braceDiag, wing, wingspan -vertwid);
   glTexCoord2f(1,1);      glVertex3d(braceDiag, wing, wingspan);
   glTexCoord2f(1,0);      glVertex3d(braceDiag, 0, -wingspan + vertwid);
   glTexCoord2f(0,0);      glVertex3d(braceDiag, 0, -wingspan);

   //glColor3f(1,1,0);
   glNormal3f(-1, 0,0);
   glTexCoord2f(0,1);      glVertex3d(0, wing, wingspan -vertwid);
   glTexCoord2f(0,0);      glVertex3d(0, 0, -wingspan);
   glTexCoord2f(1,0);      glVertex3d(0, 0, -wingspan + vertwid);
   glTexCoord2f(1,1);      glVertex3d(0, wing, wingspan);

   //Diag Front
   //Top left to bottom right
   //glColor3f(1,1,0);
   glNormal3f(0, -1,-.1524);
   glTexCoord2f(0,1);      glVertex3d(wing, wing, -wingspan);
   glTexCoord2f(0,0);      glVertex3d(wing, 0, wingspan - vertwid);
   glTexCoord2f(1,0);      glVertex3d(wing - braceDiag, 0, wingspan - vertwid);
   glTexCoord2f(1,1);      glVertex3d(wing - braceDiag, wing, -wingspan);
   
   //glColor3f(1,1,0);
   glNormal3f(0, 1,.1524);
   glTexCoord2f(1,1);      glVertex3d(wing, wing, -wingspan + vertwid);
   glTexCoord2f(0,1);      glVertex3d(wing - braceDiag, wing, -wingspan + vertwid);
   glTexCoord2f(0,0);      glVertex3d(wing - braceDiag, 0, wingspan);
   glTexCoord2f(1,0);      glVertex3d(wing, 0, wingspan);

   //glColor3f(1,1,0);
   glNormal3f(1, 0,0);
   glTexCoord2f(1,1);      glVertex3d(wing, wing, -wingspan);
   glTexCoord2f(0,1);      glVertex3d(wing, wing, -wingspan + vertwid);
   glTexCoord2f(0,0);      glVertex3d(wing, 0, wingspan);
   glTexCoord2f(1,0);      glVertex3d(wing, 0, wingspan - vertwid);

   //glColor3f(1,1,0);
   glNormal3f(-1, 0,0);
   glTexCoord2f(0,1);      glVertex3d(wing - braceDiag, wing, -wingspan);
   glTexCoord2f(0,0);      glVertex3d(wing - braceDiag, 0, wingspan - vertwid);
   glTexCoord2f(1,0);      glVertex3d(wing - braceDiag, 0, wingspan);
   glTexCoord2f(1,1);      glVertex3d(wing - braceDiag, wing, -wingspan + vertwid);

   //Top right to bottom left
   //glColor3f(1,1,0);
   glNormal3f(0,1,-.1524);
   glTexCoord2f(0,1);      glVertex3d(wing, wing, wingspan - vertwid);
   glTexCoord2f(0,0);      glVertex3d(wing, 0, -wingspan);
   glTexCoord2f(1,0);      glVertex3d(wing - braceDiag, 0, -wingspan);
   glTexCoord2f(1,1);      glVertex3d(wing - braceDiag, wing, wingspan - vertwid);

   //glColor3f(1,1,0);
   glNormal3f(0,-1,.1524);
   glTexCoord2f(1,1);      glVertex3d(wing, wing, wingspan);
   glTexCoord2f(0,1);      glVertex3d(wing - braceDiag, wing, wingspan);
   glTexCoord2f(0,0);      glVertex3d(wing - braceDiag, 0, -wingspan + vertwid);
   glTexCoord2f(1,0);      glVertex3d(wing, 0, -wingspan + vertwid);
   
   //glColor3f(1,1,0);
   glNormal3f(1,0,0);
   glTexCoord2f(0,1);      glVertex3d(wing, wing, wingspan);
   glTexCoord2f(0,0);      glVertex3d(wing, 0, -wingspan + vertwid);
   glTexCoord2f(1,0);      glVertex3d(wing, 0, -wingspan);
   glTexCoord2f(1,1);      glVertex3d(wing, wing, wingspan - vertwid);

   //glColor3f(1,1,0);
   glNormal3f(-1,0,0);
   glTexCoord2f(0,1);      glVertex3d(wing - braceDiag, wing, wingspan);
   glTexCoord2f(0,0);      glVertex3d(wing - braceDiag, wing, wingspan - vertwid);
   glTexCoord2f(1,0);      glVertex3d(wing - braceDiag, 0, -wingspan);
   glTexCoord2f(1,1);      glVertex3d(wing - braceDiag, 0, -wingspan + vertwid);
   
   //Vertical Beams

   //Front Right (pilot view)
   //glColor3f(1,1,0);
   glNormal3f(-1, 0,0);
   glTexCoord2f(1,1);      glVertex3d(vertPilf, wing, wid + vertwid);
   glTexCoord2f(0,1);      glVertex3d(vertPilf, wing, wid);
   glTexCoord2f(0,0);      glVertex3d(vertPilf, 0, wid);
   glTexCoord2f(1,0);      glVertex3d(vertPilf, 0, wid + vertwid);
   
   //glColor3f(1,1,0);
   glNormal3f( 0, 0,1);
   glTexCoord2f(0,1);      glVertex3d(vertPilf, wing, wid + vertwid);
   glTexCoord2f(0,0);      glVertex3d(vertPilf, 0, wid + vertwid);
   glTexCoord2f(1,0);      glVertex3d(vertPilf + wingDepth, 0, wid + vertwid);
   glTexCoord2f(1,1);      glVertex3d(vertPilf + wingDepth, wing, wid + vertwid);
   
   //glColor3f(1,1,0);
   glNormal3f( 1, 0,0);
   glTexCoord2f(0,1);      glVertex3d(vertPilf + wingDepth, wing, wid + vertwid);
   glTexCoord2f(0,0);      glVertex3d(vertPilf + wingDepth, 0, wid + vertwid);
   glTexCoord2f(1,0);      glVertex3d(vertPilf + wingDepth, 0, wid);
   glTexCoord2f(1,1);      glVertex3d(vertPilf + wingDepth, wing, wid);

   //glColor3f(1,1,0);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,1);      glVertex3d(vertPilf + wingDepth, wing, wid);
   glTexCoord2f(0,0);      glVertex3d(vertPilf + wingDepth, wid, wid);
   glTexCoord2f(1,0);      glVertex3d(vertPilf, wid, wid);
   glTexCoord2f(1,1);      glVertex3d(vertPilf, wing, wid);
   
   //Front left (pilot view)
   glNormal3f(-1, 0,0);
   glTexCoord2f(0,1);      glVertex3d(vertPilf, wing, -wid - vertwid);
   glTexCoord2f(0,0);      glVertex3d(vertPilf, 0, -wid - vertwid);
   glTexCoord2f(1,0);      glVertex3d(vertPilf, 0, -wid);
   glTexCoord2f(1,1);      glVertex3d(vertPilf, wing, -wid);
   
   //glColor3f(1,1,0);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(1,1);      glVertex3d(vertPilf, wing, -wid - vertwid);
   glTexCoord2f(0,1);      glVertex3d(vertPilf + wingDepth, wing, -wid - vertwid);
   glTexCoord2f(0,0);      glVertex3d(vertPilf + wingDepth, 0, -wid - vertwid);
   glTexCoord2f(1,0);      glVertex3d(vertPilf, 0, -wid - vertwid);
   
   //glColor3f(1,1,0);
   glNormal3f( 1, 0,0);
   glTexCoord2f(1,1);      glVertex3d(vertPilf + wingDepth, wing, -wid - vertwid);
   glTexCoord2f(0,1);      glVertex3d(vertPilf + wingDepth, wing, -wid);
   glTexCoord2f(0,0);      glVertex3d(vertPilf + wingDepth, 0, -wid);
   glTexCoord2f(1,0);      glVertex3d(vertPilf + wingDepth, 0, -wid - vertwid);

   //glColor3f(1,1,0);
   glNormal3f( 0, 0,1);
   glTexCoord2f(1,1);      glVertex3d(vertPilf + wingDepth, wing, -wid);
   glTexCoord2f(0,1);      glVertex3d(vertPilf, wing, -wid);
   glTexCoord2f(0,0);      glVertex3d(vertPilf, wid, -wid);
   glTexCoord2f(1,0);      glVertex3d(vertPilf + wingDepth, wid, -wid);
   
   //Rear Left (pilot view)
   glNormal3f(-1,0,0);
   glTexCoord2f(0,1);      glVertex3d(vertPilr,wing,-wid - vertwid);
   glTexCoord2f(0,0);      glVertex3d(vertPilr,0,-wid - vertwid);
   glTexCoord2f(1,0);      glVertex3d(vertPilr,0,-wid);
   glTexCoord2f(1,1);      glVertex3d(vertPilr,wing,-wid);

   //glColor3f(1,0,0);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,1);      glVertex3d(vertPilr, wing, -wid - vertwid);
   glTexCoord2f(0,0);      glVertex3d(vertPilr, 0, -wid - vertwid);
   glTexCoord2f(1,0);      glVertex3d(vertPilr - wingDepth, 0, -wid - vertwid);
   glTexCoord2f(1,1);      glVertex3d(vertPilr - wingDepth, wing, -wid - vertwid);
   
   //glColor3f(1,0,0);
   glNormal3f( 1, 0,0);
   glTexCoord2f(1,1);      glVertex3d(vertPilr - wingDepth, wing, -wid - vertwid);
   glTexCoord2f(0,1);      glVertex3d(vertPilr - wingDepth, wing, -wid);
   glTexCoord2f(0,0);      glVertex3d(vertPilr - wingDepth, 0, -wid);
   glTexCoord2f(1,0);      glVertex3d(vertPilr - wingDepth, 0, -wid - vertwid);

   //glColor3f(1,1,0);
   glNormal3f( 0, 0,1);
   glTexCoord2f(1,1);      glVertex3d(vertPilr - wingDepth, wing, -wid);
   glTexCoord2f(1,0);      glVertex3d(vertPilr - wingDepth, wid, -wid);
   glTexCoord2f(0,0);      glVertex3d(vertPilr, wid, -wid);
   glTexCoord2f(0,1);      glVertex3d(vertPilr, wing, -wid);

   //Rear Right (pilot view)
   glNormal3f(1,0,0);
   //glColor3f(1,1,0);
   glTexCoord2f(1,1);      glVertex3d(vertPilr, wing, wid + vertwid);
   glTexCoord2f(1,0);      glVertex3d(vertPilr, 0, wid + vertwid);
   glTexCoord2f(0,0);      glVertex3d(vertPilr, 0, wid);
   glTexCoord2f(0,1);      glVertex3d(vertPilr, wing, wid);
   
   //glColor3f(1,0,1);
   glNormal3f( 0, 0,1);
   glTexCoord2f(0,1);      glVertex3d(vertPilr, wing, wid + vertwid);
   glTexCoord2f(1,1);      glVertex3d(vertPilr - wingDepth, wing, wid + vertwid);
   glTexCoord2f(1,0);      glVertex3d(vertPilr - wingDepth, 0, wid + vertwid);
   glTexCoord2f(0,0);      glVertex3d(vertPilr, 0, wid + vertwid);
 
   //glColor3f(1,1,0);
   glNormal3f(-1, 0,0);
   glTexCoord2f(1,1);      glVertex3d(vertPilr - wingDepth, wing, wid + vertwid);
   glTexCoord2f(0,1);      glVertex3d(vertPilr - wingDepth, wing, wid);
   glTexCoord2f(0,0);      glVertex3d(vertPilr - wingDepth, 0, wid);
   glTexCoord2f(1,0);      glVertex3d(vertPilr - wingDepth, 0, wid + vertwid);
  
   //glColor3f(0,1,0);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(1,1);      glVertex3d(vertPilr - wingDepth, wing, wid);
   glTexCoord2f(0,1);      glVertex3d(vertPilr, wing, wid);
   glTexCoord2f(0,0);      glVertex3d(vertPilr, wid, wid);
   glTexCoord2f(1,0);      glVertex3d(vertPilr - wingDepth, wid, wid);

   glEnd();
   glDisable(GL_TEXTURE_2D);

   //Vertical tail sides 
   
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);

   glColor3f(1,0,0);
   glNormal3f(0,0,1);

   glTexCoord2f(1,1);      glVertex3d(strk - wid, tailFin, wingDepth/2);
   glTexCoord2f(0,1);      glVertex3d(tail, tailFin, wingDepth/2);
   glTexCoord2f(0,0);      glVertex3d(tail, 0.0, wingDepth/2);
   glTexCoord2f(1,0);      glVertex3d(strk + wid, 0.0, wingDepth/2);

   glColor3f(1,1,0);
   glNormal3f(0,0,-1);
   glTexCoord2f(1,1);      glVertex3d(strk - wid, tailFin, -wingDepth/2);
   glTexCoord2f(1,0);      glVertex3d(strk + wid, 0.0, -wingDepth/2);  
   glTexCoord2f(0,0);      glVertex3d(tail, 0.0, -wingDepth/2);
   glTexCoord2f(0,1);      glVertex3d(tail, tailFin, -wingDepth/2);

   //Rearfin back
   glColor3f(1,0,1);
   glNormal3f(-1,0,0);
   glTexCoord2f(0,1);      glVertex3d(tail, tailFin, -wingDepth/2);
   glTexCoord2f(0,0);      glVertex3d(tail, wid, -wingDepth/2);
   glTexCoord2f(1,0);      glVertex3d(tail, wid, wingDepth/2);
   glTexCoord2f(1,1);      glVertex3d(tail, tailFin, wingDepth/2);
   
   //Front of fin
   glColor3f(1,0.487,0);
   glNormal3f(1,0.487,0);
   glTexCoord2f(1,0);      glVertex3d(strk+wid, 0, -wingDepth/2);
   glTexCoord2f(1,1);      glVertex3d(strk - wid, tailFin, -wingDepth/2);
   glTexCoord2f(0,1);      glVertex3d(strk - wid, tailFin, wingDepth/2);
   glTexCoord2f(0,0);      glVertex3d(strk+wid, 0, wingDepth/2); 

   //Top of fin
   glColor3f(.5,0,1);
   glNormal3f( 0, 1,0);
   glTexCoord2f(1,1);      glVertex3d(tail, tailFin, -wingDepth/2);
   glTexCoord2f(0,1);      glVertex3d(tail, tailFin, wingDepth/2); 
   glTexCoord2f(0,0);      glVertex3d(strk - wid, tailFin, wingDepth/2);
   glTexCoord2f(1,0);      glVertex3d(strk - wid, tailFin, -wingDepth/2);
   
   //Right Horizontal tailfin
   glColor3f(1,1,0);
   glNormal3f( 0, 1,0);
   glTexCoord2f(1,1);      glVertex3d(tail, 0.0, tailFin);
   glTexCoord2f(0,1);      glVertex3d(strk - wid, 0.0, tailFin); 
   glTexCoord2f(0,0);      glVertex3d(strk, 0.0, 0.0);
   glTexCoord2f(1,0);      glVertex3d(tail, 0.0, 0.0);

   //Lower right 
   glColor3f(1,1,0);
   glNormal3f(0,-1,0);
   glTexCoord2f(1,1);      glVertex3d(strk - wid, -wingDepth, tailFin);
   glTexCoord2f(0,1);      glVertex3d(tail, -wingDepth, tailFin);
   glTexCoord2f(1,0);      glVertex3d(tail, -wingDepth, 0.0);
   glTexCoord2f(0,0);      glVertex3d(strk, -wingDepth, 0.0);

   //Right horiz backplate
   glColor3f(1,0,1);
   glNormal3f(-1,0,0);
   glTexCoord2f(1,1);      glVertex3d(tail, 0, tailFin);
   glTexCoord2f(1,0);      glVertex3d(tail, 0, wid);
   glTexCoord2f(0,0);      glVertex3d(tail, -wingDepth, wid);
   glTexCoord2f(0,1);      glVertex3d(tail, -wingDepth, tailFin);
   
   //Right horiz sideplate
   glColor3f(0,.4,1);
   glNormal3f(0,0,1);
   glTexCoord2f(1,1);      glVertex3d(strk-wid, 0, tailFin);
   glTexCoord2f(0,1);      glVertex3d(tail, 0, tailFin);
   glTexCoord2f(0,0);      glVertex3d(tail, -wingDepth, tailFin);
   glTexCoord2f(1,0);      glVertex3d(strk-wid, -wingDepth, tailFin);

   //Right horiz frontplate
   glColor3f(0,1,.5);
   glNormal3f(1,0,.213);
   glTexCoord2f(0,1);      glVertex3d(strk-wid, 0, tailFin);
   glTexCoord2f(0,0);      glVertex3d(strk-wid, -wingDepth, tailFin);
   glTexCoord2f(1,0);      glVertex3d(strk, -wingDepth, 0);
   glTexCoord2f(1,1);      glVertex3d(strk, 0, 0);
   
   //Left Horiz tailfin
   glColor3f(1,0,0);
   glNormal3f( 0, 1,0);
   glTexCoord2f(1,1);      glVertex3d(strk - wid, 0.0, -tailFin);
   glTexCoord2f(0,1);      glVertex3d(tail, 0.0, -tailFin);
   glTexCoord2f(0,0);      glVertex3d(tail, 0.0, 0.0);
   glTexCoord2f(1,0);      glVertex3d(strk, 0.0, 0.0);

   //Lower Left 
   glColor3f(1,1,0);
   glNormal3f(0,-1,0);
   glTexCoord2f(1,1);      glVertex3d(strk - wid, -wingDepth, -tailFin);
   glTexCoord2f(1,0);      glVertex3d(strk, -wingDepth, 0.0);
   glTexCoord2f(0,0);      glVertex3d(tail, -wingDepth, 0.0);
   glTexCoord2f(0,1);      glVertex3d(tail, -wingDepth, -tailFin);
   
   //Left horiz backplate
   glColor3f(1,0,1);
   glNormal3f(-1,0,0);
   glTexCoord2f(1,0);      glVertex3d(tail, 0, -tailFin);
   glTexCoord2f(1,0);      glVertex3d(tail, -wingDepth, -tailFin);
   glTexCoord2f(1,0);      glVertex3d(tail, -wingDepth, -wid);
   glTexCoord2f(1,0);      glVertex3d(tail, 0, -wid);

   //Left horiz sideplate
   glColor3f(0,.4,1);
   glNormal3f(0,0,-1);
   glTexCoord2f(0,1);      glVertex3d(strk-wid, 0, -tailFin);
   glTexCoord2f(0,0);      glVertex3d(strk-wid, -wingDepth, -tailFin);
   glTexCoord2f(1,0);      glVertex3d(tail, -wingDepth, -tailFin);
   glTexCoord2f(1,1);      glVertex3d(tail, 0, -tailFin);

   //Left horiz frontplate
   glColor3f(0,1,.5);
   glNormal3f(1,0,-.213);
   glTexCoord2f(1,1);      glVertex3d(strk-wid, 0, -tailFin);
   glTexCoord2f(0,1);      glVertex3d(strk, 0, 0);
   glTexCoord2f(0,0);      glVertex3d(strk, -wingDepth, 0);
   glTexCoord2f(1,0);      glVertex3d(strk-wid, -wingDepth, -tailFin);

   glEnd();
   glDisable(GL_TEXTURE_2D);
   //  Undo transformations
   glPopMatrix();
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
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   glClearColor(.1,.1,.1,.1);
   const double len=1.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL and polygon offset
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position   
   //  Orthogonal - set world orientation

    

   if(mode == 0)
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }
   else if (mode == 1)
   {
      double Exw = -2*dim*Sin(th)*Cos(ph);
      double Eyw = +2*dim*Sin(ph);
      double Ezw = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Exw,Eyw,Ezw , 0,0,0 , 0,Cos(ph),0);
   }
   else if(mode == 2)
   {
      th = 0;
      ph = 0;
      Perx = +2*dim*Sin(Strafe);
      Perz = -2*dim*Cos(Strafe);
      gluLookAt(Ex,Ey,Ez , Ex+Perx,Ey,Ez+Perz, 0,1,0);
   }
   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   if(light)
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
      //  Two sided mode
      glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
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
   //  Solid Airplanes (Biplanes)
   if(num == 0)
   {
      //SolidPlane(3.0,0,0, 1,0,0, 0,1,0, scale);//Static plane
      SolidPlane(0,0,0, 1,0,0, 0,1,0, scale);//Static plane
      //SolidPlane(-2.5,0,0, 1,0,0, 0,1,0, scale);//Static plane
   }
   else if(num == 1)
   {
      SolidPlane(0,1.5,-1, -1,Cos(zh),0, Cos(zh),1,0, scale); //Pitch plane
   }
   else if (num == 2)
   {
      SolidPlane(moveplane,3, 1.5 ,1,0,0, 0,Cos(zh),Sin(zh), scale);
   }
   else if(num == 3)
   {
      SolidPlane(8*Cos(zh),2,8*Sin(zh), -Sin(zh),0,Cos(zh), -Cos(zh),.7,-Sin(zh), scale);
   }
   else if(num == 4)
   {
      SolidPlane(Cos(zh),2,Sin(zh), -Sin(zh),0,Cos(zh), -Cos(zh),.7,-Sin(zh), scale);
      SolidPlane(1,1, 1.5 ,1,0,0, 0,Cos(zh),Sin(zh), scale);
   }
   
   //Begin Airstrip
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
      glBindTexture(GL_TEXTURE_2D,texture[0]);

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
      glBindTexture(GL_TEXTURE_2D,texture[1]);

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

      glDisable(GL_LIGHTING);

      //  White

      glColor3f(1,1,1);
      //  Draw axes
      if (axes)
      {
      glBegin(GL_LINES);
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
    glWindowPos2i(5,30);
    if(mode == 0)
    {
        Print("Orthogonal Mode");
    }
    else if(mode == 1)
    {
        Print("Perspective Mode");
    }
    else if(mode == 2)
    {
        Print("First-Person Mode");
    }
    if (light)
    {
        glWindowPos2i(5,68);
        Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f TwoSided=%s",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight,side?"On":"Off");
        glWindowPos2i(5,50);
        Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
    }
    glWindowPos2i(5,10);
    //  Print the text string
    Print("Angle=%d,%d | Scale %.3f| FOV %d| Scene %d|zh = %d",th,ph,scale,fov,num,zh);
    //  Render the scene
    glFlush();
    //  Make the rendered scene visible
    glutSwapBuffers();
}

/*
 *  GLUT calls this routine when there is nothing else to do
 */
void idle()
{
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360);
   if(num == 2)
   {
      moveplane = fmod(4*t,30);
   }
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   if(mode != 2)
   {
      //  Right arrow key - increase angle by 5 degrees
      if (key == GLUT_KEY_RIGHT)
         th += 5;
      //  Left arrow key - decrease angle by 5 degrees
      else if (key == GLUT_KEY_LEFT)
         th -= 5;
      //  Up arrow key - increase elevation by 5 degrees
      else if (key == GLUT_KEY_UP && ph <180)
         ph += 5;
      //  Down arrow key - decrease elevation by 5 degrees
      else if (key == GLUT_KEY_DOWN && ph >0)
         ph -= 5;
      else if (key == GLUT_KEY_F3)
         distance = (distance==1) ? 7 : 1;
      //  Smooth color model
      else if (key == GLUT_KEY_F1)
         smooth = 1-smooth;
      //  Local Viewer
      else if (key == GLUT_KEY_F2)
         local = 1-local;
      //  Keep angles to +/-360 degrees
      th %= 360;
      ph %= 360;
      //  Tell GLUT it is necessary to redisplay the scene
   }
   display();
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == 'R')
   {
      th = ph = 0;
      Ex = 0;
      Ey = 0;
      Ez = 5.0;
      Perx = 0;
      Pery = 0;
      Perz = 0;
      Strafe = 0;
   }
   else if (ch == '=')
   {
      scale += .01;
   }
   //  =/- changes scale of the planes
   else if (ch == '-')
   {
      scale -= .01;
   }
   else if (ch == '.')
   {
      num += 1;
      if(num > 4)
      {
         num = 4;
      }
      if(num < 0)
      {
         num = 0;
      }
   }
   else if (ch == ',')
   {
      num -= 1;
      if(num > 4)
      {
         num = 4;
      }
      if(num < 0)
      {
         num = 0;
      }
   }
   //  Toggle axes
   else if (ch == 'g' || ch == 'G')
      axes = 1-axes;
   //  Switch display mode
   else if (ch == 'm')
   {
      mode+=1;
      if(mode > 2)
      {
         mode = 2;
      }
      if(mode < 0)
      {
         mode = 0;
      }
   }
   else if(ch =='n')
   {
      mode -=1;
      if(mode > 2)
      {
         mode = 2;
      }
      if(mode < 0)
      {
         mode = 0;
      }
   }
   //  Change field of view angle
   else if (ch == 'v' && ch>1)
   {
      fov--;
   }
   else if (ch == 'f' && ch<179)
   {
      fov++;
   }
   //Navigate in First person
   else if(ch == 'q')
   {
      Strafe -= 2;
   }   
   else if(ch == 'e')
   {
      Strafe += 2;
   }
   else if(ch == 'w')
   {
      Ez += Perz * stepSize;
      Ex += Perx * stepSize;
   }
   else if(ch == 's')
   {
      Ez -= Perz * stepSize;
      Ex -= Perx * stepSize;
   }
   else if(ch == 'd')
   {
      Ez += Perx * stepSize;
      Ex -= Perz * stepSize;
   }
   else if(ch == 'a')
   {
      Ez -= Perx * stepSize;
      Ex += Perz * stepSize;
   }
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Toggle light movement
   else if (ch == 'b' || ch == 'B')
      move = 1-move;
   //  Move light while stopped
   else if (ch == 'x')
      zh += 1;
   else if (ch == 'X')
      zh -= 1;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   else if(ch == 't')
      side = 1-side;
   //  Ambient level
   else if (ch=='1' && ambient>0)
      ambient -= 5;
   else if (ch=='2' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='3' && diffuse>0)
      diffuse -= 5;
   else if (ch=='4' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='5' && specular>0)
      specular -= 5;
   else if (ch=='6' && specular<100)
      specular += 5;
      //  Emission level
   else if (ch=='7' && emission>0)
      emission -= 5;
   else if (ch=='8' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='9' && shininess>-1)
      shininess -= 1;
   else if (ch=='0' && shininess<7)
      shininess += 1;
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(mode?fov:0,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
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
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitWindowSize(800,800);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("Zach Schwarz");
   //  Tell GLUT to call "idle" when there is nothing else to do
   glutIdleFunc(idle);
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user   
   texture[0] = LoadTexBMP("Grass01.bmp");
   texture[1] = LoadTexBMP("Airstrip.bmp");
   texture[2] = LoadTexBMP("metal.bmp");
   texture[3] = LoadTexBMP("wMetal.bmp");
   texture[4] = LoadTexBMP("wood.bmp");
   glutMainLoop();
   return 0;
}
