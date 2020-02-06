#include "CSCIx229.h"

void wing(unsigned int* textures, double c10, double c11, double c12,
	double c20, double c21, double c22)
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,textures[11]);
	glPushMatrix();
	double x_inc = -1;
	double backx = -1;
	double frontx = 0;
	int i = 0;
	glScaled(.001,.001,.001);
	for (; i<100; i++)
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
   		glPolygonOffset(1,1);
		glBegin(GL_QUADS);
		glColor3f(c20,c21,c22);
		
		// Right Top
		glNormal3d(0,0,1);
		glVertex3d(backx, 0, 300);
		glVertex3d(backx, 0.5*sqrt(fabs(backx)), 300);
		glVertex3d(frontx, 0.5*sqrt(fabs(frontx)), 300);
		glVertex3d(frontx, 0, 300);
		// Right Bottom
		glVertex3d(backx, -1.5*(sqrt(fabs(backx))), 300);
		glVertex3d(backx, 0, 300);
		glVertex3d(frontx, 0, 300);
		glVertex3d(frontx, -1.5*(sqrt(fabs(frontx))), 300);
		glDisable(GL_POLYGON_OFFSET_FILL);
		// Top Section
		glColor3f(c10,c11,c12);
		glNormal3d((((.5*(sqrt(fabs(frontx))))-(.5*(sqrt(fabs(backx)))))/x_inc),1,0);
		glVertex3d(backx, 0.5*sqrt(fabs(backx)), -300);
		glVertex3d(frontx, 0.5*sqrt(fabs(frontx)), -300);
		glVertex3d(frontx, 0.5*sqrt(fabs(frontx)), 300);
		glVertex3d(backx, 0.5*sqrt(fabs(backx)), 300);

		glColor3f(c20,c21,c22);
		glNormal3d(0,0,-1);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1,1);
		
		// Left Top
		glVertex3d(backx, 0, -300);
		glVertex3d(backx, 0.5*sqrt(fabs(backx)), -300);
		glVertex3d(frontx, 0.5*sqrt(fabs(frontx)), -300);
		glVertex3d(frontx, 0, -300);
		// Left Bottom
		glVertex3d(backx, -1.5*(sqrt(fabs(backx))), -300);
		glVertex3d(backx, 0, -300);
		glVertex3d(frontx, 0, -300);
		glVertex3d(frontx, -1.5*(sqrt(fabs(frontx))), -300);
		glDisable(GL_POLYGON_OFFSET_FILL);
		
		// Bottom Section
		glColor3f(c10,c11,c12);
		glNormal3d((((1.5*(sqrt(fabs(frontx))))-(1.5*(sqrt(fabs(backx)))))/x_inc),-1,0);
		glVertex3d(backx, -1.5*sqrt(fabs(backx)), 300);
		glVertex3d(frontx, -1.5*sqrt(fabs(frontx)), 300);
		glVertex3d(frontx, -1.5*sqrt(fabs(frontx)), -300);
		glVertex3d(backx, -1.5*sqrt(fabs(backx)), -300);

		glEnd();
		backx += x_inc;
		frontx += x_inc;
	}
	i = 0;
	for (; i<80; i++)
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1,1);
		glColor3f(c20,c21,c22);
		glBegin(GL_QUADS);
		glNormal3d(0,0,1);
		
		// Right Top
		glVertex3d(backx, 0, 300);
		glVertex3d(backx, 0.5*sqrt(fabs(backx)), 300);
		glVertex3d(frontx, 0.5*sqrt(fabs(frontx)), 300);
		glVertex3d(frontx, 0, 300);
		// Right Bottom
		glVertex3d(backx, ((0.001661*pow(backx,2))+(0.2774*backx)-3.861), 300);
		glVertex3d(backx, 0, 300);
		glVertex3d(frontx, 0, 300);
		glVertex3d(frontx, ((0.001661*pow(frontx,2))+(0.2774*frontx)-3.861), 300);
		glDisable(GL_POLYGON_OFFSET_FILL);
		
		// Top Section
		glColor3f(c10,c11,c12);
		
		glNormal3d((((.5*(sqrt(fabs(frontx))))-(.5*(sqrt(fabs(backx)))))/x_inc),1,0);
		glVertex3d(backx, 0.5*sqrt(fabs(backx)), -300);
		glVertex3d(frontx, 0.5*sqrt(fabs(frontx)), -300);
		glVertex3d(frontx, 0.5*sqrt(fabs(frontx)), 300);
		glVertex3d(backx, 0.5*sqrt(fabs(backx)), 300);

		glColor3f(c20,c21,c22);
		glNormal3d(0,0,-1);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1,1);
		
		// Right Top
		glVertex3d(backx, 0, -300);
		glVertex3d(backx, 0.5*sqrt(fabs(backx)), -300);
		glVertex3d(frontx, 0.5*sqrt(fabs(frontx)), -300);
		glVertex3d(frontx, 0, -300);
		// Right Bottom
		glVertex3d(backx, ((0.001661*pow(backx,2))+(0.2774*backx)-3.861), -300);
		glVertex3d(backx, 0, -300);
		glVertex3d(frontx, 0, -300);
		glVertex3d(frontx, ((0.001661*pow(frontx,2))+(0.2774*frontx)-3.861), -300);
		glDisable(GL_POLYGON_OFFSET_FILL);
	
		// Bottom Section
		glColor3f(c10,c11,c12);
		
		glNormal3d(-((((0.001661*pow(frontx,2))+(0.2774*frontx)-3.861)-((0.001661*pow(backx,2))+(0.2774*backx)-3.861))/x_inc),-1,0);
		glVertex3d(backx, ((0.001661*pow(backx,2))+(0.2774*backx)-3.861), -300);
		glVertex3d(frontx, ((0.001661*pow(frontx,2))+(0.2774*frontx)-3.861), -300);
		glVertex3d(frontx, ((0.001661*pow(frontx,2))+(0.2774*frontx)-3.861), 300);
		glVertex3d(backx, ((0.001661*pow(backx,2))+(0.2774*backx)-3.861), 300);
		
		    
		glEnd();
		backx += x_inc;
		frontx += x_inc;
	}
	
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1,1);
	glColor3f(c20,c21,c22);
	glBegin(GL_QUADS);
	// Right Fin
	glNormal3d(0,0,1);
	glVertex3d(backx, 0, 300);
	glVertex3d(-185, 10, 300);
	glVertex3d(-170, 5, 300);
	glVertex3d(backx, 0, 300);
	// Left Fin
	glNormal3d(0,0,-1);
	glVertex3d(backx, 0, -300);
	glVertex3d(-185, 10, -300);
	glVertex3d(-170, 5, -300);
	glVertex3d(backx, 0, -300);
	// Face Positive X
	glColor3f(c10,c11,c12);
	glNormal3d(3.882,1,0);
	glVertex3d(-185,10,300);
	glVertex3d(-185,10,-300);
	glVertex3d(-170,5,-300);
	glVertex3d(-170,5,300);
	// Face Negative X
	glNormal3d(-2.387,1,0);
	glVertex3d(backx,0,-300);
	glVertex3d(-185,10,-300);
	glVertex3d(-185,10,300);
	glVertex3d(backx,0,300);
	// Face Negative Y
	glNormal3d(1,-2.387,0);
	glVertex3d(backx,0,300);
	glVertex3d(-170,5,300);
	glVertex3d(-170,5,-300);
	glVertex3d(backx,0,-300);
	glDisable(GL_POLYGON_OFFSET_FILL);


	glEnd();
	
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}