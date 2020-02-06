#include "CSCIx229.h"
int start,grand,grand2,track;

int th=-75;        //  Azimuth of view angle
int ph=20;        //  Elevation of view angle
int axes=0;       //  Display axes
int mode=3;       //  Projection mode
int Width;
int Height;
double carSize=1; //  Size of the cars
double asp = 1;   //  Aspect ratio
int fov=55;       //  Field of view (for perspective)
double dim=5.0;   //  Eye Position to start
double increment = 0.01;//  increment the forward/backward movement
float Lpos[4];
int Day = 0;
/* Eye coordinates stored globally */
double Ex = 0;
double Ey = 0;
double Ez = 5;    
double yVal = 1;

/* Camera perspective coordinates stored globally - no looking up as of yet */
double Cx = 0;
double Cz = 0;

int turn = 0; // rotation in degrees. needs to be an int to use modulo

/* Light values */
int one       =   1;  // Unit value
int inc       =  10;  // Ball increment
unsigned int textures[20];
float N[] = {0, -1, 0}; // Normal vector for the plane
float E[] = {0, -.45, 0 }; // Point of the plane

int pause = 1;
int readout = 1;
int populated = 0;
int sky[5];
int shader[] = {0,0,0}; //  Shader programs

// translation rotatation and tire rotation matrix/array
double translate[2][10000];
double rotate[2][10000];
double tireRotation[100000];

double translate2[2][10000];
double rotate2[2][10000];
double tireRotation2[100000];

/* Animation time */
int displayTime = 0;

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
   float Emission[]  = {0.5,0.5,0.5,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
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

void timer()
{
   if (!pause)
   {
      displayTime += 1;
      if (displayTime == 1339)
      {
         pause = 1;
      }
   }
   
   glutPostRedisplay();
   glutTimerFunc(1000.0/60.0, timer, 0);
}

void population()
{
   // CAR ONE VARIABLES
   int i = 0; // timestamp
   double j = 0; // tire rotation
   translate[0][0] = 0;
   translate[1][0] = 0;
   rotate[0][0] = 1;
   rotate[1][0] = 0;
   tireRotation[0] = 0;

   // CAR ONE INPUTS
   //starting at 0 -- ACCELERATE STRAIGHT +x
   for (i = 1; i < 20; i++)
   {
      translate[0][i] = translate[0][i-1]+i*.00953; // accelerate, going to half for the distance
      translate[1][i] = 0;
      rotate[0][i] = 1;
      rotate[1][i] = 0;
      tireRotation[i]=0;
   }
   //starting at 20 -- STRAIGHT +x
   for (; i < 225; i++)
   {
      translate[0][i] = translate[0][i-1]+0.207; // compensate for acceleration time
      translate[1][i] = 0;
      rotate[0][i] = 1;
      rotate[1][i] = 0;
      tireRotation[i]=0;
   }
   //starting at 225 -- RIGHT TURN +x +z
   for (; i < 325; i++)
   {
      translate[0][i] = translate[0][i-1] + (.2 - ((i-225)*.002)); // .2 (or speed) / # of ticks for increment
      translate[1][i] = translate[1][i-1] + ((i-225)*.002);
      rotate[0][i] = rotate[0][i-1] - 0.01; // 1 / # of ticks for increment
      rotate[1][i] = rotate[1][i-1] + 0.01;
      tireRotation[i] = j;
      j -= .2; // 20 / # of ticks for increment
   }
   //starting at 325 -- Right TURN -x +z
   for (; i < 395; i++)
   {
      translate[0][i] = translate[0][i-1] - ((i-325)*.002857);
      translate[1][i] = translate[1][i-1] + (.2 - ((i-325)*.002857));
      rotate[0][i] = rotate[0][i-1] - 0.01429;
      rotate[1][i] = rotate[1][i-1] - 0.01429;
      tireRotation[i] = j;
      j += .2857143;
   }
   j=0;
   //starting at 395 -- STRAIGHT -x
   for (; i < 580; i++)
   {
      translate[0][i] = translate[0][i-1]-0.2;
      translate[1][i] = translate[1][i-1];
      rotate[0][i] = -1;
      rotate[1][i] = 0;
      tireRotation[i]= 0;
   }
   //starting at 580 -- LEFT TURN -x +z
   for (; i < 690; i++)
   {
      translate[0][i] = translate[0][i-1] - (.2 - ((i-580)*.001818182));
      translate[1][i] = translate[1][i-1] + ((i-580)*.001818182);
      rotate[0][i] = rotate[0][i-1] + 0.009091;
      rotate[1][i] = rotate[1][i-1] + 0.009091;
      tireRotation[i] = j;
      j += .18182;  
   }
   j=0;
   for (; i < 765; i++)
   {
      translate[0][i] = translate[0][i-1] - ((i-690)*.0026667);
      translate[1][i] = translate[1][i-1] + (.2 - ((i-690)*.0026667));
      rotate[0][i] = rotate[0][i-1] - 0.0133333;
      rotate[1][i] = rotate[1][i-1] - 0.0133333;
      tireRotation[i] = j;
      j -= .2666667;  
   }
   j=0;
   //starting at 785 STRAIGHT -- -x
   for (; i < 835; i++)
   {
      translate[0][i] = translate[0][i-1]-0.2;
      translate[1][i] = translate[1][i-1];
      rotate[0][i] = -1;
      rotate[1][i] = 0;
      tireRotation[i]= 0;
   }
   //starting at 835 -- RIGHT TURN -- -x -z
   for (; i < 995; i++)
   {
      translate[0][i] = translate[0][i-1] - (.2 - ((i-835)*.00125));
      translate[1][i] = translate[1][i-1] - ((i-835)*.00125);
      rotate[0][i] = rotate[0][i-1] + 0.00625;
      rotate[1][i] = rotate[1][i-1] - 0.00625;
      tireRotation[i] = j;
      j -= .125;  
   }
   //starting at 995 -- RIGHT TURN -- +x -z
   for (; i < 1175; i++)
   {
      translate[0][i] = translate[0][i-1] + ((i-995)*.00111111);
      translate[1][i] = translate[1][i-1] - (.2 - ((i-995)*.00111111));
      rotate[0][i] = rotate[0][i-1] + 0.00555556;
      rotate[1][i] = rotate[1][i-1] + 0.00555556;
      tireRotation[i] = j;
      j += .111111; 
   }
   //starting at 1175 STRAIGHT -- x
   j=0;
   for (; i < 1235; i++)
   {
      translate[0][i] = translate[0][i-1]+(.2+((i-1175)*.0007));
      translate[1][i] = translate[1][i-1];
      rotate[0][i] = 1;
      rotate[1][i] = 0;
      tireRotation[i]= 0;
   }
   for (; i < 1280; i++)
   {
      translate[0][i] = translate[0][i-1]+.24;
      translate[1][i] = translate[1][i-1];
      rotate[0][i] = 1;
      rotate[1][i] = 0;
      tireRotation[i]= 0;
   }
   //starting at 1280 STRAIGHT -- x
   for (; i < 1600; i++)
   {
      translate[0][i] = translate[0][i-1]+(.25-((i-1270)*.0007));
      translate[1][i] = translate[1][i-1]-0.002;
      rotate[0][i] = 1;
      rotate[1][i] = 0;
      tireRotation[i]= 0;
   }

   // CAR TWO VARIABLES
   i = 0; // timestamp
   j = 0; // tire rotation
   translate2[0][0] = -.9;
   translate2[1][0] = 1.5;
   rotate2[0][0] = 1;
   rotate2[1][0] = 0;
   tireRotation2[0] = 0;

   // CAR TWO INPUTS
   //starting at 0 -- ACCELERATE STRAIGHT +x
   for (i = 1; i < 30; i++)
   {
      translate2[0][i] = translate2[0][i-1]+i*.007; // accelerate, going to half for the distance
      translate2[1][i] = translate2[1][i-1];
      rotate2[0][i] = 1;
      rotate2[1][i] = 0;
      tireRotation2[i]=0;
   }
   //starting at 20 -- STRAIGHT +x
   for (; i < 245; i++)
   {
      translate2[0][i] = translate2[0][i-1]+0.205; // compensate for acceleration time
      translate2[1][i] = translate2[1][i-1]-.008;
      rotate2[0][i] = 1;
      rotate2[1][i] = 0;
      tireRotation2[i]=0;
   }
   
   //starting at 245 -- RIGHT TURN +x +z
   for (; i < 335; i++)
   {
      translate2[0][i] = translate2[0][i-1] + (.2 - ((i-245)*.00222222)); // .2 (or speed) / # of ticks for increment
      translate2[1][i] = translate2[1][i-1] + ((i-245)*.00222222);
      rotate2[0][i] = rotate2[0][i-1] - 0.0111111; // 1 / # of ticks for increment
      rotate2[1][i] = rotate2[1][i-1] + 0.0111111;
      tireRotation2[i] = j;
      j -= .222222; // 20 / # of ticks for increment
   }
   //starting at 335 -- Right TURN -x +z
   for (; i < 415; i++)
   {
      translate2[0][i] = translate2[0][i-1] - ((i-335)*.0025);
      translate2[1][i] = translate2[1][i-1] + (.2 - ((i-335)*.0025));
      rotate2[0][i] = rotate2[0][i-1] - 0.0125;
      rotate2[1][i] = rotate2[1][i-1] - 0.0125;
      tireRotation2[i] = j;
      j += .25;
   }
   j=0;
   //starting at 415 -- ACCELERATE STRAIGHT -x
   for (; i < 440; i++)
   {
      translate2[0][i] = translate2[0][i-1]-(0.2+(i-415)*.002);
      translate2[1][i] = translate2[1][i-1];
      rotate2[0][i] = -1;
      rotate2[1][i] = 0;
      tireRotation2[i]= 0;
   }
   //starting at 430 -- STRAIGHT -x
   for (; i < 470; i++)
   {
      translate2[0][i] = translate2[0][i-1]-0.232;
      translate2[1][i] = translate2[1][i-1];
      rotate2[0][i] = -1;
      rotate2[1][i] = 0;
      tireRotation2[i]= 0;
   }
   //starting at 470 -- STRAIGHT PASS -x
   for (; i < 540; i++)
   {
      translate2[0][i] = translate2[0][i-1]-0.232;
      translate2[1][i] = translate2[1][i-1]+.022;
      rotate2[0][i] = -1;
      rotate2[1][i] = 0;
      tireRotation2[i]= .1;
   }
   j=0;
   //starting at 540 -- STRAIGHT -x
   for (; i < 575; i++)
   {
      translate2[0][i] = translate2[0][i-1]-0.232;
      translate2[1][i] = translate2[1][i-1];
      rotate2[0][i] = -1;
      rotate2[1][i] = 0;
      tireRotation2[i]= 0;
   }
   //starting at 575 -- LEFT TURN -x +z
   for (; i < 665; i++)
   {
      translate2[0][i] = translate2[0][i-1] - (.2 - ((i-575)*.0021));
      translate2[1][i] = translate2[1][i-1] + ((i-575)*.0021);
      rotate2[0][i] = rotate2[0][i-1] + 0.0105;
      rotate2[1][i] = rotate2[1][i-1] + 0.0105;
      tireRotation2[i] = j;
      j += .21;  
   }
   //straighten out
   for (; i < 675; i++)
   {
      translate2[0][i] = translate2[0][i-1];
      translate2[1][i] = translate2[1][i-1] + 0.2;
      rotate2[0][i] = rotate2[0][i-1];
      rotate2[1][i] = rotate2[1][i-1];
      tireRotation2[i] = j;
      j -= 2;  
   }
   j=0;
   //starting at 675 -- RIGHT TURN -x +z
   for (; i < 765; i++)
   {
      translate2[0][i] = translate2[0][i-1] - ((i-665)*.0021);
      translate2[1][i] = translate2[1][i-1] + (.2 - ((i-665)*.0021));
      rotate2[0][i] = rotate2[0][i-1] - 0.0105;
      rotate2[1][i] = rotate2[1][i-1] - 0.0105;
      tireRotation2[i] = j;
      j -= .21;  
   }
   j=0;
   //starting at 765 STRAIGHT -- -x
   for (; i < 820; i++)
   {
      translate2[0][i] = translate2[0][i-1]-0.215;
      translate2[1][i] = translate2[1][i-1]-.011;
      rotate2[0][i] = -1;
      rotate2[1][i] = 0;
      tireRotation2[i]= 0;
      j=-1.1;
   } 
   //starting at 825 -- RIGHT TURN -- -x -z
   for (; i < 980; i++)
   {
      translate2[0][i] = translate2[0][i-1] - (.2 - ((i-820)*.0012903));
      translate2[1][i] = translate2[1][i-1] - ((i-820)*.0012903);
      rotate2[0][i] = rotate2[0][i-1] + 0.006451613;
      rotate2[1][i] = rotate2[1][i-1] - 0.006451613;
      tireRotation2[i] = j;
      j -= .12903;  
   }
   j=0;
   // STRAIGHTEN OUT
   for (; i < 985; i++)
   {
      translate2[0][i] = translate2[0][i-1];
      translate2[1][i] = translate2[1][i-1] - .2;
      rotate2[0][i] = rotate2[0][i-1];
      rotate2[1][i] = rotate2[1][i-1];
      tireRotation2[i] = j; 
   }
   //starting at 985 -- RIGHT TURN -- +x -z
   for (; i < 1135; i++)
   {
      translate2[0][i] = translate2[0][i-1] + ((i-995)*.001333333);
      translate2[1][i] = translate2[1][i-1] - (.2 - ((i-995)*.001333333));
      rotate2[0][i] = rotate2[0][i-1] + 0.0066667;
      rotate2[1][i] = rotate2[1][i-1] + 0.0066667;
      tireRotation2[i] = j;
      j -= .1333333;  
   }
   //starting at 1135 STRAIGHT -- -x
   j=0;
   for (; i < 1280; i++)
   {
      translate2[0][i] = translate2[0][i-1]+.19;
      translate2[1][i] = translate2[1][i-1]-0.004;
      rotate2[0][i] = 1;
      rotate2[1][i] = 0;
      tireRotation2[i]= 0;
   }
   for (; i < 1600; i++)
   {
      translate2[0][i] = translate2[0][i-1]+(.2-((i-1280)*.0005));
      translate2[1][i] = translate2[1][i-1];
      rotate2[0][i] = 1;
      rotate2[1][i] = 0;
      tireRotation2[i]= 0;
   }
}

static void Sky(double D)
{
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);
   int deValley = 90;
   //  Sides
   glBindTexture(GL_TEXTURE_2D,sky[2]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.00,0); glVertex3f(-D,-deValley-1,-D);
   glTexCoord2f(1,0); glVertex3f(+D,-deValley-1,-D);
   glTexCoord2f(1,1); glVertex3f(+D,+D-deValley,-D);
   glTexCoord2f(0.00,1); glVertex3f(-D,+D-deValley,-D);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,sky[0]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(+D,-deValley-1,-D);
   glTexCoord2f(1,0); glVertex3f(+D,-deValley-1,+D);
   glTexCoord2f(1,1); glVertex3f(+D,+D-deValley,+D);
   glTexCoord2f(0,1); glVertex3f(+D,+D-deValley,-D);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,sky[3]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(+D,-deValley-1,+D);
   glTexCoord2f(1,0); glVertex3f(-D,-deValley-1,+D);
   glTexCoord2f(1,1); glVertex3f(-D,+D-deValley,+D);
   glTexCoord2f(0,1); glVertex3f(+D,+D-deValley,+D);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,sky[1]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-D,-deValley-1,+D);
   glTexCoord2f(1,0); glVertex3f(-D,-deValley-1,-D);
   glTexCoord2f(1.00,1); glVertex3f(-D,+D-deValley,-D);
   glTexCoord2f(0,1); glVertex3f(-D,+D-deValley,+D);
   glEnd();

   //  Top and bottom
   glBindTexture(GL_TEXTURE_2D,sky[4]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,1); glVertex3f(+D,+D-deValley,-D);
   glTexCoord2f(0,0); glVertex3f(+D,+D-deValley,+D);
   glTexCoord2f(1,0); glVertex3f(-D,+D-deValley,+D);
   glTexCoord2f(1,1); glVertex3f(-D,+D-deValley,-D);
   glEnd();

   glDisable(GL_TEXTURE_2D);
}

static void startTow(float x,float y,float z , float th,float ph , float S)
{
   glPushMatrix();
   glTranslated(4,3.4,5);
   glRotatef(180,0,1,0);
   glScaled(1.5,1.5,1.5);
   glCallList(start);
   glPopMatrix();
}

static void Track()
{
   glPushMatrix();
   glTranslated(0,0,2);
   glCallList(track);
   glPopMatrix();
}

static void GrandStand(float x,float y,float z , float th,float ph , float S)
{
   float white[] = { 	0.774597, 	0.774597, 	0.774597,1};
   float Diff[] = {0.4,0.4,0.4,1};
   float Ambient[] = {.25,.25,.25,1}; 
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,.5*128);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,Diff);
   glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, Ambient);
   glEnable(GL_COLOR_MATERIAL);
   glPushMatrix();
   glTranslated(x,y,z);
   glRotatef(th,0,1,0);
   glCallList(grand);
   glPopMatrix();
}

void Light(int on)
{
   if(on == 1)
   {
      Lpos[0] = -25;
      Lpos[1] = 30;
      Lpos[2] = -25;
      Lpos[3] = 1;
      float Med[]  = {0.45,0.45,0.45,1.0};
      float Specular[]  = {1.0,1.0,1.0,1.0};
      //  Draw light position as ball (still no lighting here)
      ball(Lpos[0],Lpos[1],Lpos[2] , 0.1);

      glShadeModel(GL_SMOOTH); //Smooth shading
      glEnable(GL_NORMALIZE);   //  OpenGL should normalize normal vectors
      glEnable(GL_LIGHTING);     //  Enable lighting
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);//  Location of viewer for specular calculations
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);//  glColor sets ambient and diffuse color materials
      glEnable(GL_COLOR_MATERIAL);
      //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
      glEnable(GL_LIGHT0);
      glEnable(GL_LIGHT1);
      glEnable(GL_LIGHT2);
      //  Set ambient, diffuse, specular components and position of light 0
      if(Day == 1)
      {
         glDisable(GL_LIGHT1);
         glDisable(GL_LIGHT2);
         glLightfv(GL_LIGHT0,GL_AMBIENT, Med);
         glLightfv(GL_LIGHT0,GL_DIFFUSE, Med);
         glLightfv(GL_LIGHT0,GL_SPECULAR, Specular);
         glLightfv(GL_LIGHT0,GL_POSITION,Lpos);
      }
      else
      {     
         glDisable(GL_LIGHT0);
         //  Set ambient, diffuse, specular components and position of light 1
         float pos[] = {translate[0][displayTime],-.2,translate[1][displayTime],1};
         float Dir[] = {rotate[0][displayTime],-.2,rotate[1][displayTime],0};
         glLightfv(GL_LIGHT1,GL_AMBIENT, Med);
         glLightfv(GL_LIGHT1,GL_DIFFUSE, Med);
         glLightfv(GL_LIGHT1,GL_SPECULAR, Specular);
         glLightfv(GL_LIGHT1,GL_POSITION, pos);
         //  Set spotlight parameters
         glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,Dir);
         glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,90);
         glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,60);
         //  Set attenuation
         glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION ,0.0);
         glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION ,0.001);
         glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION,0.001);

         
         //  Set ambient, diffuse, specular components and position of light 2
         float pos2[] = {translate2[0][displayTime],-.2,translate2[1][displayTime],1};
         float Dir2[] = {rotate2[0][displayTime],-.2,rotate2[1][displayTime],0};
         glLightfv(GL_LIGHT2,GL_AMBIENT, Med);
         glLightfv(GL_LIGHT2,GL_SPECULAR, Specular);
         glLightfv(GL_LIGHT2,GL_DIFFUSE, Med);
         glLightfv(GL_LIGHT2,GL_POSITION, pos2);
         //  Set spotlight parameters
         glLightfv(GL_LIGHT2,GL_SPOT_DIRECTION,Dir2);
         glLightf(GL_LIGHT2,GL_SPOT_CUTOFF,90);
         glLightf(GL_LIGHT2,GL_SPOT_EXPONENT,60);
         //  Set attenuation
         glLightf(GL_LIGHT2,GL_CONSTANT_ATTENUATION ,0.0);
         glLightf(GL_LIGHT2,GL_LINEAR_ATTENUATION ,0.001);
         glLightf(GL_LIGHT2,GL_QUADRATIC_ATTENUATION,0.001);
      }
   }
   else
   {
      glDisable(GL_LIGHTING);
      glDisable(GL_COLOR_MATERIAL);
      glDisable(GL_NORMALIZE);
   }
}

void Scene(int Lig)
{ 
   Light(Lig);//LUMOS!
   Track();
   glUseProgram(shader[1]);
   startTow(0,1,0.0,0,0,0);
   GrandStand(-6,-.45,-8,180,0,1);
   GrandStand(10,-.45,-8,180,0,1);
   glUseProgram(shader[0]);
   FormulaOne(translate2[0][displayTime],-0.4,translate2[1][displayTime], rotate2[0][displayTime],0,rotate2[1][displayTime], 0,1,0, 0,0.6,0.8, 1,1,1, carSize, displayTime, tireRotation2, textures);
   FormulaOne(translate[0][displayTime],-0.4,translate[1][displayTime], rotate[0][displayTime],0,rotate[1][displayTime], 0,1,0, 0.8,0,0, 1,1,1, carSize, displayTime, tireRotation, textures);
}

void display()
{
   if (!populated)
   {
      population();
      populated = 1;
   }
   const double len=1.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   
   /* Switch Mode for demonstration purposes */
   if (displayTime == 500)
      mode = 1;
   if (displayTime == 1100)
      mode = 3;

   /* In car mode */
   if (mode == 3)
   {
      fov = 55;
      th = 0;
      ph = 0;
      double Aa = 0.6*Sin(90*rotate[0][displayTime]);
      double Ab = 0.6*Sin(90*rotate[1][displayTime]);
      gluLookAt(translate[0][displayTime]-Aa, 0, translate[1][displayTime]-Ab, translate[0][displayTime+10], 0, translate[1][displayTime+10], 0,1,0);
   }

   /* first person mode */
   if (mode == 2)
   {
      th = 0;
      ph = 0;
      Cx = +2*dim*Sin(turn); // changing perspective based on the rotation
      Cz = -2*dim*Cos(turn);
      Ey = yVal;
      gluLookAt(Ex,Ey,Ez, Cx+Ex,Ey,Cz+Ez, 0,1,0);
   }

   /* Helicopter Mode */
   if (mode == 1)
   {
      gluLookAt(translate[0][displayTime]-10, 15, translate[1][displayTime]-10, translate[0][displayTime], 0, translate[1][displayTime], 0,1,0);
   }
   Scene(1);

   glEnable(GL_TEXTURE_2D);
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(1,1);
   glColor3f(1,1,1);
   glNormal3f(0,1,0);
   glBindTexture(GL_TEXTURE_2D,textures[12]);
   for (int j=-(50*dim);j<(50*dim);j++)
   {
      glBegin(GL_QUAD_STRIP);
      for (int i=-(50*dim);i<=(50*dim);i++)
      {
         glTexCoord2f(i,j); glVertex3f(i,-.45,j);
         glTexCoord2f(i,(j+1)); glVertex3f(i,-.45,j+1);
      }
      glEnd();
   }
   glDisable(GL_POLYGON_OFFSET_FILL);
   glDisable(GL_TEXTURE_2D);

   Sky(50*dim);

   glColor3f(1.0,1.0,1.0);
   glDisable(GL_LIGHTING);
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
   glColor3f(1,1,1);
   if (readout)
   {
      glWindowPos2i(5,40);   
      Print("Ylight=%.1f Dim=%.1f VERSION= %s", Lpos[1],dim,glGetString(GL_VERSION));
      glWindowPos2i(5,20);
      Print("mx1 = %f mz1 = %f rx1 = %f rz1 = %f time = %d, populated = %d", translate[0][displayTime],translate[1][displayTime],rotate[0][displayTime],rotate[1][displayTime],displayTime,populated);
      glWindowPos2i(5,60);
      Print("Current X = %f  Current Y = %f  Current Z = %f Current MODE = %d", Ex,Ey,Ez,mode);
   }

   //  Render the scene
   glFlush();
   //  Make the rendered scene visible
   glutSwapBuffers();
}

void idle()
{
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
   else if (ch == 'm' || ch == 'M')
   {
      mode = mode+1;
      if (mode > 3)
         mode = 1;
      th = 0;
      ph = 0;
      fov = 55;
      Ex = 0;
      Ey = 0;
      Ez = 5;
      turn = 0;
      yVal = 1;
   }
   else if(ch == 'y')
   {
      yVal = yVal - .1;
   }
   else if(ch == 'Y')
   {
      yVal = yVal + .1;
   }
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   else if (ch=='t')
      displayTime += 1;
   else if (ch=='z')
      pause = 1-pause;
   else if (ch=='r')
      readout = 1-readout;      
   else if(ch == 'l')
   {
      Day = 1 - Day;
   }
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
   glutIdleFunc(idle);
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
 *  Read text file
 */
char* ReadText(char *file)
{
   int   n;
   char* buffer;
   //  Open file
   FILE* f = fopen(file,"rt");
   if (!f) Fatal("Cannot open text file %s\n",file);
   //  Seek to end to determine size, then rewind
   fseek(f,0,SEEK_END);
   n = ftell(f);
   rewind(f);
   //  Allocate memory for the whole file
   buffer = (char*)malloc(n+1);
   if (!buffer) Fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
   //  Snarf the file
   if (fread(buffer,n,1,f)!=1) Fatal("Cannot read %d bytes for text file %s\n",n,file);
   buffer[n] = 0;
   //  Close and return
   fclose(f);
   return buffer;
}

/*
 *  Print Shader Log
 */
void PrintShaderLog(int obj,char* file)
{
   int len=0;
   glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for shader log\n",len);
      glGetShaderInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s:\n%s\n",file,buffer);
      free(buffer);
   }
   glGetShaderiv(obj,GL_COMPILE_STATUS,&len);
   if (!len) Fatal("Error compiling %s\n",file);
}

/*
 *  Print Program Log
 */
void PrintProgramLog(int obj)
{
   int len=0;
   glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for program log\n",len);
      glGetProgramInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s\n",buffer);
   }
   glGetProgramiv(obj,GL_LINK_STATUS,&len);
   if (!len) Fatal("Error linking program\n");
}

/*
 *  Create Shader
 */
int CreateShader(GLenum type,char* file)
{
   //  Create the shader
   int shader = glCreateShader(type);
   //  Load source code from file
   char* source = ReadText(file);
   glShaderSource(shader,1,(const char**)&source,NULL);
   free(source);
   //  Compile the shader
   fprintf(stderr,"Compile %s\n",file);
   glCompileShader(shader);
   //  Check for errors
   PrintShaderLog(shader,file);
   //  Return name
   return shader;
}

/*
 *  Create Shader Program
 */
int CreateShaderProg(char* VertFile,char* FragFile)
{
   //  Create program
   int prog = glCreateProgram();
   //  Create and compile vertex shader
   int vert = CreateShader(GL_VERTEX_SHADER  ,VertFile);
   //  Create and compile fragment shader
   int frag = CreateShader(GL_FRAGMENT_SHADER,FragFile);
   //  Attach vertex shader
   glAttachShader(prog,vert);
   //  Attach fragment shader
   glAttachShader(prog,frag);
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
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
   glutCreateWindow("Tyach Final Project of Awesomeness!!!");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);

   textures[1] = LoadTexBMP("tiretread.bmp");
   textures[5] = LoadTexBMP("metFlo.bmp");
   textures[7] = LoadTexBMP("S71B.bmp");
   textures[8] = LoadTexBMP("metalPlate.bmp");
   textures[9] = LoadTexBMP("taillight.bmp");
   textures[10] = LoadTexBMP("Gauges1.bmp");
   textures[12] = LoadTexBMP("Grass.bmp");

   sky[0] = LoadTexBMP("desertsky_bk.bmp");
   sky[1] = LoadTexBMP("desertsky_ft.bmp");
   sky[2] = LoadTexBMP("desertsky_lf.bmp");
   sky[3] = LoadTexBMP("desertsky_rt.bmp");
   sky[4] = LoadTexBMP("desertsky_up.bmp");

   start = LoadOBJ("StartTower.obj");
   grand = LoadOBJ("grandstand.obj");
   grand2 = LoadOBJ("grandstand.obj");
   track = LoadOBJ("Track.obj");

   shader[1] = CreateShaderProg("texture.vert","texture.frag");
   shader[2] = CreateShaderProg("pixtex.vert","pixtex.frag");
   
   /* timer function for car movement */
   glutTimerFunc(1000.0/60.0, timer, 0);
   glutIdleFunc(idle);
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}