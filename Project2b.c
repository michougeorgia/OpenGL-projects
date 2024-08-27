/* Particle system in a box and test for collision */

/*
CONTROLS

Left Click  :   Menu Options for changing particle speed, number and size

*/



#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

#define MAX_NUM_PARTICLES 1000
#define INITIAL_NUM_PARTICLES 25
#define INITIAL_POINT_SIZE 5.0
#define INITIAL_SPEED 1.0

typedef int bool;
#define TRUE 1
#define FALSE 0

void myDisplay();
void myIdle();
void myReshape(int, int);
void main_menu(int);
void collision(int);
float forces(int, int);
void myinit();

/* particle struct */

typedef struct particle
{
    int color;
    float position[3];
    float velocity[3];
    float mass;
} particle;

particle particles[MAX_NUM_PARTICLES]; /* particle system */

/* initial state of particle system */

int present_time;
int last_time;
int num_particles = INITIAL_NUM_PARTICLES;
float point_size = INITIAL_POINT_SIZE;
float speed = INITIAL_SPEED;
bool gravity = FALSE; /* gravity off */
bool repulsion = FALSE; /* repulsion off */
float d2[MAX_NUM_PARTICLES][MAX_NUM_PARTICLES]; /* array for interparticle distances */

GLsizei wh = 500, ww = 500; /* initial window size */

GLfloat colors[8][3] = { {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0},{0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0}, {0.0, 1.0, 1.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 0.0},
    {1.0, 1.0, 1.0} };


void myReshape(int w, int h)
{

    /* adjust clipping box */

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.0, 2.0, -2.0, 2.0, -4.0, 4.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1.5, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    /* adjust viewport and  clear */

    if (w < h)
        glViewport(0, 0, w, w);
    else
        glViewport(0, 0, h, h);


    /* set global size for use by drawing routine */

    ww = w;
    wh = h;
}


/* Function that sets up particles with random locations and velocities */
void myinit()
{
    int  i, j;
    for (i = 0; i < num_particles; i++)
    {
        particles[i].mass = 1.0;
        particles[i].color = i % 8;
        for (j = 0; j < 3; j++)
        {
            particles[i].position[j] = 2.0 * ((float)rand() / RAND_MAX) - 1.0;
            particles[i].velocity[j] = speed * 2.0 * ((float)rand() / RAND_MAX) - 1.0;
        }
    }
    glPointSize(point_size);

    /* set clear color to grey */
    glClearColor(0.5, 0.5, 0.5, 1.0);
}


/* Euler method in collision to calculate new position and velocity. Check if there is repulsion => calculate distance
  between particles that have collided => calculate new force in force function
*/
void myIdle()
{
    int i, j, k;
    float dt;
    present_time = glutGet(GLUT_ELAPSED_TIME);
    dt = 0.001 * (present_time - last_time);
    for (i = 0; i < num_particles; i++)
    {
        for (j = 0; j < 3; j++)
        {
            particles[i].position[j] += dt * particles[i].velocity[j];
            particles[i].velocity[j] += dt * forces(i, j) / particles[i].mass;
        }
        collision(i);
    }
    if (repulsion)
        for (i = 0; i < num_particles; i++) for (k = 0; k < i; k++)
        {
            d2[i][k] = 0.0;
            for (j = 0; j < 3; j++) d2[i][k] += (particles[i].position[j] -
                particles[k].position[j]) * (particles[i].position[j] -
                    particles[k].position[j]);
            d2[k][i] = d2[i][k];
        }
    last_time = present_time;
    glutPostRedisplay();
}


/* Gravity and repulsive force in case of collision. Gravity is calculated by the type force =  mass * g
with negative value because of the direction (g: universal gravitational constant) */
float forces(int i, int j)
{
    int k;
    float force = 0.0;
    if (gravity && j == 1)
        force = -9.8; /* gravity */
    if (repulsion)
        for (k = 0; k < num_particles; k++)  /* repulsive force */
        {
            if (k != i)
                force += 0.001 * (particles[i].position[j] - particles[k].position[j]) / (0.001 + d2[i][k]);
        }
    return(force);
}


/* Function that tests if there are collisions between particles and the cube. If there is collision, it reflects these
  particles */
void collision(int n)

{
    int i;
    for (i = 0; i < 3; i++)
    {
        if (particles[n].position[i] >= 1.0)
        {
            particles[n].velocity[i] = -particles[n].velocity[i];
            particles[n].position[i] = 1.0 - (particles[n].position[i] - 1.0);
        }
        if (particles[n].position[i] <= -1.0)
        {
            particles[n].velocity[i] = -particles[n].velocity[i];
            particles[n].position[i] = -1.0 - (particles[n].position[i] + 1.0);
        }
    }
}

/* Menu for options about the number, size and velocity of particles, clicked with left button */
void main_menu(int index)
{
    switch (index)
    {
    case(1):
    {
        num_particles = 2 * num_particles;
        myinit();
        break;
    }
    case(2):
    {
        num_particles = num_particles / 2;
        myinit();
        break;
    }
    case(3):
    {
        speed = 2.0 * speed;
        myinit();
        break;
    }
    case(4):
    {
        speed = speed / 2.0;
        myinit();
        break;
    }
    case(5):
    {
        point_size = 2.0 * point_size;
        myinit();
        break;
    }
    case(6):
    {
        point_size = point_size / 2.0;
        if (point_size < 1.0) point_size = 1.0;
        myinit();
        break;
    }

    }
}

void myDisplay()
{
    int i;

    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    for (i = 0; i < num_particles; i++)
    {
        glColor3fv(colors[particles[i].color]);
        glVertex3fv(particles[i].position);
    }
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    glutWireCube(2.2);
    glutSwapBuffers();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("particle system in a box");
    glutDisplayFunc(myDisplay);
    myinit();

    glutCreateMenu(main_menu);
    glutAddMenuEntry("more particles", 1);
    glutAddMenuEntry("fewer particles", 2);
    glutAddMenuEntry("faster", 3);
    glutAddMenuEntry("slower", 4);
    glutAddMenuEntry("larger particles", 5);
    glutAddMenuEntry("smaller particles", 6);
    glutAttachMenu(GLUT_LEFT_BUTTON);

    glutIdleFunc(myIdle);
    glutReshapeFunc(myReshape);
    glutMainLoop();
}