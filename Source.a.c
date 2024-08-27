#include <stdlib.h>
#include <GL/glut.h>

GLsizei wh = 500, ww = 500;

static GLfloat theta = 0.0;
static GLfloat scale = 1.0;
static GLfloat s = 1.0;


void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glNewList(1, GL_COMPILE);
	glBegin(GL_POLYGON);
	glVertex3f(1, 1, 1);
	glVertex3f(-1, 1, 1);
	glVertex3f(-1, -1, 1);
	glVertex3f(1, -1, 1);
	glEnd();
	glEndList();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(0.0, 0.0, -70.0);
	glRotatef(theta, 1.0, 2.0, 2.0);
	glScalef(scale, scale, scale);
	glPushMatrix();

	glScalef(5.0, 5.0, 5.0);
	glColor3f(1.0, 0.0, 0.0);
	glCallList(1);

	glPopMatrix();
	glPushMatrix();

	glScalef(5.0, 5.0, 5.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	glCallList(1);

	glPopMatrix();
	glPushMatrix();


	glScalef(5.0, 5.0, 5.0);
	glRotatef(90.0, 0.0, -1.0, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	glCallList(1);

	glPopMatrix();
	glPushMatrix();


	glScalef(5.0, 5.0, 5.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glColor3f(0.0, 1.0, 1.0);
	glCallList(1);

	glPopMatrix();
	glPushMatrix();

	glScalef(5.0, 5.0, 5.0);
	glRotatef(90.0, -1.0, 0.0, 0.0);
	glColor3f(0.8, 0.8, 0.8);
	glCallList(1);

	glPopMatrix();
	glPushMatrix();

	glScalef(5.0, 5.0, 5.0);
	glRotatef(180.0, 1.0, 0.0, 0.0);
	glColor3f(1.0, 0.0, 1.0);
	glCallList(1);

	glPopMatrix();
	glLoadIdentity();

	glFlush();
	glutSwapBuffers();
}

void myReshape(int w, int h)
{

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-40.0, 40.0, -40.0 * (GLfloat)h / (GLfloat)w, 40.0 * (GLfloat)h / (GLfloat)w, -90.0, 90.0);
	else
		glOrtho(-40.0 * (GLfloat)w / (GLfloat)h, 40.0 * (GLfloat)w / (GLfloat)h, -40.0, 40.0, -90.0, 90.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void SpinScaleCube()
{
	theta += 0.1;
	if (theta > 360.0) theta -= 360.0;

	if (scale < 2.0 && scale >= 1.0 && s == 1.0)
	{
		scale += 0.01;
	}
	else if (s == 1.0)
	{
		s = 0.0;
	}
	else if (s == 0.0)
	{
		scale -= 0.01;
		if (scale == 1.0)
			s = 1.0;
	}
	glutPostRedisplay();
}


void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Spinning Cube");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutIdleFunc(SpinScaleCube);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}