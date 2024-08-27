#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

enum MENU_TYPE
{
    S1,
    S2,
    S3,
    S4,
    S5,
    S6,
};

int n=4;
static GLfloat thetaR = 0.0;
GLfloat lightIntensity = 0.3f;
static GLfloat theta[] = { 0.0,0.0,0.0 };

GLboolean spotlightOn = TRUE;
GLenum shadingMode = GL_SMOOTH;
GLboolean onePolygon = TRUE;

typedef float point3[4];
point3 v[] = { {0.0, 0.0, 1.0}, {0.0, 0.942809, -0.33333}, {-0.816497, -0.471405, -0.333333}, {0.816497, -0.471405, -0.333333} };

float cameraRadius = 70.0f;
float cameraAngle = 0.0f;
float lookAtX = 0.0f;
float lookAtY = 40.0f;
float lookAtZ = 0.0f;

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100.0f, (float)width / (float)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void triangle(point3 a, point3 b, point3 c)
{
    glBegin(GL_POLYGON);
    glNormal3fv(a);
    glVertex3fv(a);
    glNormal3fv(b);
    glVertex3fv(b);
    glNormal3fv(c);
    glVertex3fv(c);
    glEnd();
}

void normal(point3 p)
{
    double sqrt();
    float d = 0.0;
    int i;
    for (i = 0; i < 3; i++) d += p[i] * p[i];
    d = sqrt(d);
    if (d > 0.0) for (i = 0; i < 3; i++) p[i] /= d;
}

void divide_triangle(point3 a, point3 b, point3 c, int m)
{
    point3 v1, v2, v3;
    int j;
    if (m > 0)
    {
        for (j = 0; j < 3; j++)
            v1[j] = a[j] + b[j];
        normal(v1);
        for (j = 0; j < 3; j++)
            v2[j] = a[j] + c[j];
        normal(v2);
        for (j = 0; j < 3; j++)
            v3[j] = b[j] + c[j];
        normal(v3);

        divide_triangle(a, v1, v2, m - 1);
        divide_triangle(c, v2, v3, m - 1);
        divide_triangle(b, v3, v1, m - 1);
        divide_triangle(v1, v3, v2, m - 1);
    }

    else(triangle(a, b, c));
}

void tetrahedron(int m)
{
    divide_triangle(v[0], v[1], v[2], m);
    divide_triangle(v[3], v[2], v[1], m);
    divide_triangle(v[0], v[3], v[1], m);
    divide_triangle(v[0], v[2], v[3], m);
}

void Rotate()
{
    thetaR -= 0.05;
    if (thetaR <= -180.0)
    {
        thetaR = 0.0;
    }

    if (thetaR > -90.0) {
        lightIntensity = lightIntensity + 0.7 / 90.0 * 0.05;
    }    
    else
        lightIntensity = lightIntensity - 0.7 / 90.0 * 0.05;

    glutPostRedisplay();
}

void drawRectangles(float x1, float x2, float z1, float z2) {

    float X = (x2 - x1) / 10;
    float Z = (z2 - z1) / 10;

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            float startX = x1 + i * X;
            float endX = startX + X;
            float startZ = z1 + j * Z;
            float endZ = startZ + Z;

            glBegin(GL_QUADS);
            glVertex3f(startX, 0.0f, startZ);
            glVertex3f(startX, 0.0f, endZ);
            glVertex3f(endX, 0.0f, endZ);
            glVertex3f(endX, 0.0f, startZ);
            glEnd();
        }
    }
}

void Vector(point3 p, point3 q, point3 r)
{
    point3 n;
    n[0] = (q[1] - p[1]) * (r[2] - p[2]) - (q[2] - p[2]) * (r[1] - p[1]);
    n[1] = (q[2] - p[2]) * (r[0] - p[0]) - (q[0] - p[0]) * (r[2] - p[2]);
    n[2] = (q[0] - p[0]) * (r[1] - p[1]) - (q[1] - p[1]) * (r[0] - p[0]);
    glNormal3fv(n);

}


void processMenuEventsPolygon(int selection) {
    switch (selection) {
    case S1:
        spotlightOn = TRUE;
        break;
    case S2:
        spotlightOn = FALSE;
        break;
    case S3:
        shadingMode = GL_SMOOTH;
        break;
    case S4:
        shadingMode = GL_FLAT;
        break;
    case S5:
        onePolygon = TRUE;
        break;
    case S6:
        onePolygon = FALSE;
        break;
    }
    glutPostRedisplay();
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float cameraX = lookAtX + cameraRadius * sin(cameraAngle);
    float cameraZ = lookAtZ + cameraRadius * cos(cameraAngle);
    gluLookAt(cameraX, lookAtY, cameraZ, lookAtX, lookAtY, lookAtZ, 0.0f, 1.0f, 0.0f);

    GLfloat grass_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat grass_diffuse[] = { 0.6f, 0.8f, 0.2f, 1.0f };
    GLfloat grass_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat grass_shininess[] = { 0.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, grass_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, grass_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, grass_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, grass_shininess);

    glNewList(1, GL_COMPILE);
    point3 v1 = { -50, 0, -30 };
    point3 v2 = { -50, 0, 30 };
    point3 v3 = { 50, 0, 30 };
    point3 v4 = { 50, 0, -30 };

    glBegin(GL_POLYGON);
    Vector(v1, v2, v4);
    glVertex3f(-50, 0, -30);
    glVertex3f(-50, 0, 30);
    glVertex3f(50, 0, 30);
    glVertex3f(50, 0, -30);
    glEnd();
    glEndList();


    if (onePolygon) {
        glCallList(1);
    }
    else
    {
        Vector(v1, v2, v4);
        drawRectangles(-50.0f, 50.0f, -30.0f, 30.0f);
    }

    GLfloat roofDiffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat roofSpecular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat roof_shininess[] = { 100.0f };

    glMaterialfv(GL_FRONT, GL_DIFFUSE, roofDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, roofSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, roof_shininess);

    glNewList(2, GL_COMPILE);
    point3 s1 = { 5, 10, 10 };
    point3 s2 = { -5, 10, 10 };
    point3 s3 = { 0, 18.66, 10 };

    glBegin(GL_TRIANGLES);
    Vector(s1, s2, s3);
    glVertex3f(5, 10, 10);
    glVertex3f(-5, 10, 10);
    glVertex3f(0, 18.66, 10);
    glEnd();

    point3 d1 = { 5, 10, -10 };
    point3 d2 = { -5, 10, -10 };
    point3 d3 = { 0, 18.66, -10 };

    glBegin(GL_TRIANGLES);
    Vector(d1, d2, d3);
    glVertex3f(5, 10, -10);
    glVertex3f(-5, 10, -10);
    glVertex3f(0, 18.66, -10);
    glEnd();

    point3 f1 = { -5, 10, 10 };
    point3 f2 = { 0, 18.66, 10 };
    point3 f3 = { 0, 18.66, -10 };
    point3 f4 = { -5, 10, -10 };

    glBegin(GL_POLYGON);
    Vector(f1, f2, f4);
    glVertex3f(-5, 10, 10);
    glVertex3f(0, 18.66, 10);
    glVertex3f(0, 18.66, -10);
    glVertex3f(-5, 10, -10);
    glEnd();

    point3 x1 = { 5, 10, 10 };
    point3 x2 = { 0, 18.66, 10 };
    point3 x3 = { 0, 18.66, -10 };
    point3 x4 = { 5, 10, -10 };

    glBegin(GL_POLYGON);
    Vector(x1, x4, x2);
    glVertex3f(5, 10, 10);
    glVertex3f(0, 18.66, 10);
    glVertex3f(0, 18.66, -10);
    glVertex3f(5, 10, -10);
    glEnd();

    glEndList();

    glCallList(2);

    GLfloat houseDiffuse[] = { 0.8f, 0.2f, 0.2f, 1.0f };
    GLfloat houseSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat houseAmbient[] = { 1.0f, 0.0f, 0.0f, 1.0f };

    glMaterialfv(GL_FRONT, GL_DIFFUSE, houseDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, houseSpecular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, houseAmbient);

    glNewList(3, GL_COMPILE);
    point3 a1 = { 5,10,10 };
    point3 a2 = { -5,10,10 };
    point3 a3 = { -5,0,10 };
    point3 a4 = { 5,0,10 };

    glBegin(GL_POLYGON);
    Vector(a1, a2, a4);
    glVertex3f(5, 10, 10);
    glVertex3f(-5, 10, 10);
    glVertex3f(-5, 0, 10);
    glVertex3f(5, 0, 10);
    glEnd();

    point3 b1 = { 5,10,-10 };
    point3 b2 = { -5,10,-10 };
    point3 b3 = { -5,0,-10 };
    point3 b4 = { 5,0,-10 };

    glBegin(GL_POLYGON);
    Vector(b1, b2, b4);
    glVertex3f(5, 10, -10);
    glVertex3f(-5, 10, -10);
    glVertex3f(-5, 0, -10);
    glVertex3f(5, 0, -10);
    glEnd();

    point3 c1 = { -5,10,10 };
    point3 c2 = { -5,10,-10 };
    point3 c3 = { -5, 0, -10 };
    point3 c4 = { -5, 0, 10 };

    glBegin(GL_POLYGON);
    Vector(c1, c2, c4);
    glVertex3f(-5, 10, 10);
    glVertex3f(-5, 10, -10);
    glVertex3f(-5, 0, -10);
    glVertex3f(-5, 0, 10);
    glEnd();

    point3 k1 = { 5, 10, 10 };
    point3 k2 = { 5,10,-10 };
    point3 k3 = { 5,0,-10 };
    point3 k4 = { 5, 10, -10 };

    glBegin(GL_POLYGON);
    Vector(k2, k1, k3);
    glVertex3f(5, 10, 10);
    glVertex3f(5, 10, -10);
    glVertex3f(5, 0, -10);
    glVertex3f(5, 0, 10);
    glEnd();

    point3 l1 = { 5,10,10 };
    point3 l2 = { -5,10,10 };
    point3 l3 = { -5,10,-10 };
    point3 l4 = { 5,10,-10 };

    glBegin(GL_POLYGON);
    Vector(l1, l2, l4);
    glVertex3f(5, 10, 10);
    glVertex3f(-5, 10, 10);
    glVertex3f(-5, 10, -10);
    glVertex3f(5, 10, -10);
    glEnd();

    point3 m1 = { -5, 0, 10 };
    point3 m2 = { 5, 0, 10 };
    point3 m3 = { -5, 0, -10 };
    point3 m4 = { 5, 0, -10 };

    glBegin(GL_POLYGON);
    Vector(m1, m2, m4);
    glVertex3f(-5, 0, 10);
    glVertex3f(5, 0, 10);
    glVertex3f(-5, 0, -10);
    glVertex3f(5, 0, -10);
    glEnd();
    glEndList();

    glShadeModel(shadingMode);
    glCallList(3);

    if (spotlightOn) {
        glEnable(GL_LIGHT0);

        GLfloat spotPosition[] = { 0.0f, 16.0f, 10.0f, 1.0f };
        GLfloat spotDirection[] = { 0.0f, -16.0f, 10.0f };
        GLfloat spotExponent = 30.0f;
        GLfloat spotCutoff = 30.0f;

        glLightfv(GL_LIGHT0, GL_POSITION, spotPosition);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection);
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotCutoff);

        GLfloat spotlightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        GLfloat spotlightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        GLfloat spotlightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

        glLightfv(GL_LIGHT0, GL_AMBIENT, spotlightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, spotlightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, spotlightSpecular);
    }
    else {
        glDisable(GL_LIGHT0);
    }

    glNewList(4, GL_COMPILE);

    GLfloat position[] = { -50.0f, 0.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, position);

    GLfloat diffuse[] = { lightIntensity, lightIntensity, lightIntensity, 1.0f };
    GLfloat specular[] = { lightIntensity, lightIntensity, lightIntensity, 1.0f };

    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    glEnable(GL_LIGHT1);

    glEndList();

    glNewList(5, GL_COMPILE);

    GLfloat emission[] = { 1.0, 1.0, 0.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);

    glTranslatef(-50, 0, 0);
    tetrahedron(n);

    GLfloat default_emission[] = { 0.0, 0.0, 0.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_EMISSION, default_emission);

    glEndList();

    glRotatef(thetaR, 0, 0, 1);

    glCallList(4);
    glCallList(5);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    glFlush();
    glutSwapBuffers();
}


void updateCamera(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, updateCamera, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'a' || key == 'A') {
        cameraAngle += 0.1f;
    }
    else if (key == 'd' || key == 'D') {
        cameraAngle -= 0.1f;
    }

    glutPostRedisplay();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Project 3");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(Rotate);

    int menu1;
    menu1 = glutCreateMenu(processMenuEventsPolygon);
    glutAddMenuEntry("on", S1);
    glutAddMenuEntry("of", S2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    int menu2 = glutCreateMenu(processMenuEventsPolygon);
    glutAddMenuEntry("smooth", S3);
    glutAddMenuEntry("flat", S4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    int menu3 = glutCreateMenu(processMenuEventsPolygon);
    glutAddMenuEntry("low", S5);
    glutAddMenuEntry("high", S6);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    int sub_menu;
    sub_menu = glutCreateMenu(processMenuEventsPolygon);

    glutAddSubMenu("Spotlight", menu1);
    glutAddSubMenu("Shades", menu2);
    glutAddSubMenu("Polygon", menu3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, updateCamera, 0);

    glutMainLoop();
}