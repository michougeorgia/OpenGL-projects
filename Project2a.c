/* Το αρθρωτό μοντέλο αναπαρίσταται ως δέντρο στην display μέσω της συνάρτησης traverse. Έχει
   κατασκευαστεί στην myInit() μέσω της μεθόδου sibling - child. Η κατασκευή αρχίζει από τον κορμό
   (torso) και όσοι κόμβοι έχουν και sibling και child, πρώτα γίνονται τα sibling κάθε σειράς κόμβων και
   ύστερα με τη σειρά τα children της επόμενης σειράς. Επιπλέον, στην myInit() έχει τεθεί χρώμα και σκιά
   στο μοντέλο.
   neck έχει 2 γωνίες αντί για μία: μία για να θέσω την αρχική θέση του λαιμού και μία για τις κινήσεις αργότερα
   όπως διαθέτουν όλοι οι κόμβοι
   οι γωνίες θ ενδέχεται να αλλάξουν, ανάλογα με τις κινήσεις που πρέπει να εμφανιστούν
   στην reshape υπάρχει σχόλιο για την κάμερα που οδηγεί στην άλλη ζητούμενη προβολή
   η κίνηση με το torso προαιρετικό για κατανόηση (free roam)
*/

/*
CONTROLS

A - D       :   Move camera (orthogonal plane) around object. DOES NOT CHANGE LIGHTING
Right Click :   Menu with options for resetting and animations.
Left Arrow  :   On free roam rotates the object counterclockwise. Otherwise proceeds the animation backwards
Right Arrow :   On free roam rotates the object clockwise. Otherwise proceeds the animation forwards

*/


#define NULL 0

#include <GL/glut.h>

#define UPPER_LEG_RADIUS  0.5
#define LOWER_LEG_RADIUS  0.5
#define LOWER_LEG_HEIGHT 3.0
#define UPPER_LEG_HEIGHT 2.0
#define HEAD_HEIGHT 1.5
#define HEAD_RADIUS 1.0
#define TORSO_HEIGHT 10.0
#define TORSO_RADIUS 3.0
#define NECK_HEIGHT 3.0
#define NECK_RADIUS 0.8
#define RIGHT_FOOT_RADIUS 0.5
#define RIGHT_FOOT_HEIGHT 2.0
#define LEFT_FOOT_RADIUS 0.5
#define LEFT_FOOT_HEIGHT 2.0
#define BACK_UPPER_LEG_RADIUS 0.5
#define BACK_LOWER_LEG_RADIUS 0.5
#define BACK_UPPER_LEG_HEIGHT 2.0
#define BACK_LOWER_LEG_HEIGHT 3.0
#define BACK_RIGHT_FOOT_RADIUS 0.5
#define BACK_RIGHT_FOOT_HEIGHT 2.0
#define BACK_LEFT_FOOT_RADIUS 0.5
#define BACK_LEFT_FOOT_HEIGHT 2.0



//Camvas related
GLsizei wh = 500, ww = 500;

//Light related
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess = { 100.0 };
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_position[] = { 10.0, 10.0, 10.0, 0.0 };

//Different Colors by diffusion
GLfloat light_diffuse_red[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat light_diffuse_yellow[] = { 1.0, 1.0, 0.0, 1.0 };


GLfloat light_diffuse_orange[] = { 1.0, 0.5, 0.0, 1.0 };
GLfloat light_diffuse_light_orange[] = { 1.0, 0.8, 0.5, 1.0 };
GLfloat light_diffuse_dark_orange[] = { 0.5, 0.2, 0.0, 1.0 };

GLfloat light_diffuse_green[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat light_diffuse_light_green[] = { 0.8, 1.0, 0.8, 1.0 };
GLfloat light_diffuse_dark_green[] = { 0.0, 0.4, 0.0, 1.0 };

GLfloat light_diffuse_blue[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat light_diffuse_light_blue[] = { 0.8, 0.8, 1.0, 1.0 };
GLfloat light_diffuse_dark_blue[] = { 0.0, 0.0, 0.4, 1.0 };

GLfloat light_diffuse_pink[] = { 1.0, 0.7, 0.8, 1.0 };
GLfloat light_diffuse_dark_pink[] = { 0.5, 0.3, 0.4, 1.0 };
GLfloat light_diffuse_light_pink[] = { 1.0, 0.8, 0.9, 1.0 };



typedef float point[3];

//Joint tree struct
//On every node we store:
//  1. Pointer to function that draws the object on starting position       -   (*f)()
//  2. Conversion matrix that places the object in relation to its parent   -   m[16]
//  3. Pointers for child and sibling respectively                          -   child, sibling 
typedef struct treenode
{
    GLfloat m[16];
    void (*f)();
    struct treenode* sibling;
    struct treenode* child;
}treenode;

typedef treenode* t_ptr;


static GLfloat theta[16] = { 0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 180.0, 0.0, 180.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }; /* initial joint angles */
static GLfloat default_angles[16] = { 0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 180.0, 0.0, 180.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }; /* initial joint angles */
static GLint menu_option = 0;   //Option selected on the menu
static GLint angle = 0;         //Current angle acceleration

//Joint objects
GLUquadricObj* t, * h, *n, * lua, * lla, * rua, * rla, * lll, * rll, * rul, * lul, * rff, * lff, * blul, * brul, * blll, * brll, * blf, * brf;

double size = 1.0;

//Joint tree nodes
treenode torso_node, head_node, n_node, lua_node, rua_node, lll_node, rll_node,
lla_node, rla_node, rul_node, lul_node, blul_node, brul_node, blll_node, brll_node, lff_node, rff_node, blf_node, brf_node;

//Camera related
static GLfloat cameraRotation = 0.0;
static GLfloat cameraRotationPos_y = 0.0;
static GLfloat cameraRotationFlag_y = 0.0;



void cameraRotate()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-30.0, 30.0, -30.0 * (GLfloat)wh / (GLfloat)ww, 30.0 * (GLfloat)wh / (GLfloat)ww, -90.0, 90.0);

    //Rotate camera via the pressing of the arrow keys
    glRotatef(cameraRotation, 0, cameraRotationFlag_y, 0);

    //glTranslatef(cameraTranslatePos_x, cameraTranslatePos_y, cameraTranslatePos_z);
    glMatrixMode(GL_MODELVIEW);
}

//Reset all camera values 
void resetCamera()
{
    cameraRotation = 0.0;
    cameraRotationPos_y = 0.0;
    cameraRotationFlag_y = 0.0;
    cameraRotate();
}

//Keyboard handler
void NormalKeyHandler(unsigned char key, int x, int y) {
    //WASD = rotate projection view in the direction of the keys
    // NOTE Lighting will not be cast differently on objects
    //A = Left
    //D = Right

    if (key == 'd')
    {
        cameraRotationFlag_y = 1.0f;
        cameraRotationPos_y -= 5.0f;
        cameraRotation = cameraRotationPos_y;
        cameraRotate();
        cameraRotationFlag_y = 0.0f;
        cameraRotation = 0.0f;
    }
    if (key == 'a')
    {
        cameraRotationFlag_y = 1.0f;
        cameraRotationPos_y += 5.0f;
        cameraRotation = cameraRotationPos_y;
        cameraRotate();
        cameraRotationFlag_y = 0.0f;
        cameraRotation = 0.0f;
    }

}


//Special Keyboard button handler
void SpecialKeyHandler(int key, int x, int y) {
    //Rotate the camera around the plane's center by the arrows movements
    //Camera eye and vectors are dependent on GlOrtho()
    //Right Arrow = Right
    //Left Arrow = Left
    //Up Arrow = Up
    //Down Arrow = Down

    if (key == GLUT_KEY_LEFT)
    {
        if (!menu_option)
            angle += 5.0;
        else
            angle = -1.0;

    }
    if (key == GLUT_KEY_RIGHT )
    {
      
        if (!menu_option)
            angle -= 5.0;
        else
            angle = 1.0;
    }


    glPushMatrix();

}




//Update the angles according to the position
void updateTheta(GLfloat new_angles[16])
{
   
    for (int i = 0; i < 16; i++)
        theta[i] = new_angles[i];
    


}

//Animate gradually the rotational angles of the object to the given angles
// If angle > 0 (right arrow key is pressed) animation goes forward
// If angle < 0 (left arrow key is pressed) animation goes backwards

void animateAngles(GLfloat new_angles[16])
{
    int i;
    for (i = 0; i < 16; i++)
    {
        if (angle > 0)
        {
            if (theta[i] < new_angles[i])
                theta[i] += angle;
            if (theta[i] > new_angles[i])
                theta[i] -= angle;
        }
        else
        {
            if (theta[i] > default_angles[i])
                theta[i] += angle;
            if (theta[i] < default_angles[i])
                theta[i] -= angle;
        }
        
    }
}

//Menu Options

    //Freely rotate the object
    //Rotates by the torso so torso position is reset
    void freeRoam()
    {   
        if (angle > 360.0)
            angle -= 360.0;
        if (angle < 360.0) 
            angle += 360.0;

        glLoadIdentity();
        glRotatef(theta[0]+angle, 0.0, 1.0, 0.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, torso_node.m);
    }

    //USAGE FOR EXERCISES
    /*
        0	-	  torso	
        1   -     neck
        2   -     head
        3   -     left upper leg lul
        4   -     right upper leg(rul)
        5   -     back left upper leg(blul)
        6   -     back right upper leg(brul)
        7   -     left lower leg(lll)
        8   -     right lower leg(rll)
        9   -     back left lower leg(blll)
        10  -     back right lower leg(brll)
        11  -     left front foot(lff)
        12  -     right front foot(rff)
        13  -     back left foot(blf)
        14  -     back right foot(brf)
        15  -     UNUSED
     */

    //Reset the position of every angle to their default values
    void resetPosition()
    {

        glLoadIdentity();
        glRotatef(theta[0], 0.0, 1.0, 0.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, torso_node.m);

        glLoadIdentity();

        glTranslatef(0.0, TORSO_HEIGHT, 0.0);
        glRotatef(theta[1], 1.0, 0.0, 0.0);
        glRotatef(theta[2], 0.0, 0.0, 1.0);
        glTranslatef(-3 * NECK_RADIUS, -2.5 * NECK_HEIGHT, 0.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, n_node.m);

        glLoadIdentity();
        glTranslatef(-(TORSO_RADIUS - 4.65 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, -1.5);   //front left upper leg
        glRotatef(theta[3], 1.0, 0.0, 0.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, lul_node.m);

        glLoadIdentity();
        glTranslatef(-(TORSO_RADIUS - 4.65 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, 1.5);    //front right upper leg
        glRotatef(theta[4], 1.0, 0.0, 0.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, rul_node.m);

        glLoadIdentity();
        glTranslatef(-(TORSO_RADIUS - 0.35 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, -1.5); //back left upper leg
        glRotatef(theta[5], 1.0, 0.0, 0.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, blul_node.m);

        glLoadIdentity();
        glTranslatef(-(TORSO_RADIUS - 0.35 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, 1.5);    //back right upper leg
        glRotatef(theta[4], 1.0, 0.0, 0.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, brul_node.m);

        glLoadIdentity();
        glTranslatef(5.5 * HEAD_HEIGHT, 2 * NECK_HEIGHT - 5 * HEAD_HEIGHT, 0.0);
        glRotatef(theta[5], 1.0, 0.0, 0.0);
        glRotatef(theta[6], 0.0, 1.0, 0.0);
        glTranslatef(0.0, -0.5 * HEAD_HEIGHT, 0.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, head_node.m);

        glLoadIdentity();
        glTranslatef(0.0, -1.5 * UPPER_LEG_HEIGHT, 0.0);

        glRotatef(theta[8], 1.0, 0.0, 0.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, lll_node.m);

        glLoadIdentity();
        glTranslatef(0.0, 0.0, 0.0);
        glRotatef(theta[9], 1.0, 0.0, 0.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, rll_node.m);

        glLoadIdentity();
        glTranslatef(0.0, -BACK_LOWER_LEG_HEIGHT, 0.0);
        glRotatef(theta[10], 1.0, 0.0, 0.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, blll_node.m);

        glLoadIdentity();
        glTranslatef(0.0, -BACK_LOWER_LEG_HEIGHT, 0.0);
        glRotatef(theta[11], 1.0, 0.0, 0.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, brll_node.m);

        glLoadIdentity();
        glTranslatef(-LEFT_FOOT_RADIUS, -LEFT_FOOT_RADIUS, 0.0);
        glRotatef(theta[12], 1.0, 0.0, 0.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, lff_node.m);

        glLoadIdentity();
        glTranslatef(-RIGHT_FOOT_RADIUS, LOWER_LEG_HEIGHT + RIGHT_FOOT_RADIUS, 0.0);
        glRotatef(theta[13], 1.0, 0.0, 0.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, rff_node.m);

        glLoadIdentity();
        glTranslatef(-BACK_LEFT_FOOT_RADIUS, -BACK_LEFT_FOOT_RADIUS, 0.0);
        glRotatef(theta[14], 1.0, 0.0, 0.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, blf_node.m);

        glLoadIdentity();
        glTranslatef(-BACK_RIGHT_FOOT_RADIUS, -BACK_RIGHT_FOOT_RADIUS, 0.0);
        glRotatef(theta[15], 1.0, 0.0, 0.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, brf_node.m);

        glLoadIdentity();
    }

    //Lower Neck and Head
    void initiatePos1()
    {
        GLfloat temp_angles[16] = { 0.0, -20.0, -10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
        animateAngles(temp_angles);

        //Neck

            glLoadIdentity();

            glTranslatef(2.0, 4, 0.0);
            glRotatef(theta[1], 0.0, 0.0, 1.0);
            glGetFloatv(GL_MODELVIEW_MATRIX, n_node.m);

            //Head

                glLoadIdentity();
                glTranslatef(5.5 * HEAD_HEIGHT, 2 * NECK_HEIGHT - 5 * HEAD_HEIGHT, 0.0);
                glRotatef(theta[2], 0.0, 0.0, 1.0);
                glTranslatef(0.0, -0.5 * HEAD_HEIGHT, 0.0);
                glGetFloatv(GL_MODELVIEW_MATRIX, head_node.m);
    }


    //Stand on Front Legs
    void initiatePos2()
    {
        GLfloat temp_angles[16] = { -30.0, 0.0, 0.0, 40.0, 40.0, -40.0, -50.0, 120.0, 40.0, 140.0, -60.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
        animateAngles(temp_angles);


        //Torso

            glLoadIdentity();
            glRotatef(theta[0], 0.0, 0.0, 1.0);
            glGetFloatv(GL_MODELVIEW_MATRIX, torso_node.m);


        //Right Front Leg

            glLoadIdentity();
            glTranslatef(-(TORSO_RADIUS - 4.65 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, 1.5);    //front right upper leg
            glRotatef(theta[4], 0.0, 0.0, 1.0);
            glGetFloatv(GL_MODELVIEW_MATRIX, rul_node.m);

            //Right Front Lower Leg

                glLoadIdentity();
                glRotatef(180, 1.0, 0.0, 0.0);
                glRotatef(theta[8], 0.0, 0.0, 1.0);
                glGetFloatv(GL_MODELVIEW_MATRIX, rll_node.m);

        //Left Front Leg

            glLoadIdentity();
            glTranslatef(-(TORSO_RADIUS - 4.65 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, -1.5);   //front left upper leg
            glRotatef(theta[3], 0.0, 0.0, 1.0);
            glGetFloatv(GL_MODELVIEW_MATRIX, lul_node.m);

            //Left Front Lower Leg

                glLoadIdentity();
                glRotatef(180, 0.0, 0.0, 1.0);
                glRotatef(theta[7], 0.0, 0.0, 1.0);
                glTranslatef(0.0, -1.5 * UPPER_LEG_HEIGHT, 0.0);
                glGetFloatv(GL_MODELVIEW_MATRIX, lll_node.m);

        //Right Back Leg

            glLoadIdentity();
            glTranslatef(-(TORSO_RADIUS - 0.35 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, 1.5);    //back right upper leg
            glRotatef(theta[6], 0.0, 0.0, 1.0);
            glGetFloatv(GL_MODELVIEW_MATRIX, brul_node.m);

            //Right Back Lower Leg

                glLoadIdentity();
                glRotatef(theta[10], 0.0, 0.0, 1.0);
                glTranslatef(0.0, -1.5 * UPPER_LEG_HEIGHT, 0.0);
                glGetFloatv(GL_MODELVIEW_MATRIX, brll_node.m);



        //Left Back Leg

            glLoadIdentity();
            glTranslatef(-(TORSO_RADIUS - 0.35 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, -1.5);    //back right upper leg
            glRotatef(theta[5], 0.0, 0.0, 1.0);
            glGetFloatv(GL_MODELVIEW_MATRIX, blul_node.m);

            //Left Back Lower Leg

                glLoadIdentity();
                glRotatef(180, 0.0, 0.0, 1.0);
                glRotatef(theta[9], 0.0, 0.0, 1.0);
                glTranslatef(0.0, -1.5 * UPPER_LEG_HEIGHT, 0.0);
                glGetFloatv(GL_MODELVIEW_MATRIX, blll_node.m);


    }


    //Stand on Hind Legs
    void initiatePos3()
    {
        GLfloat temp_angles[16] = { 70.0, 0.0, 0.0, 50.0, 40.0, -40.0, -40.0, 150.0, 40.0, 140.0, -40.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
        animateAngles(temp_angles);


        //Torso

            glLoadIdentity();
            glRotatef(theta[0], 0.0, 0.0, 1.0);
            glGetFloatv(GL_MODELVIEW_MATRIX, torso_node.m);


        //Right Front Leg

            glLoadIdentity();
            glTranslatef(-(TORSO_RADIUS - 4.65 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, 1.5);    //front right upper leg
            glRotatef(theta[4], 0.0, 0.0, 1.0);
            glGetFloatv(GL_MODELVIEW_MATRIX, rul_node.m);

            //Right Front Lower Leg

                glLoadIdentity();
                glRotatef(180, 1.0, 0.0, 0.0);
                glRotatef(theta[8], 0.0, 0.0, 1.0);
                glGetFloatv(GL_MODELVIEW_MATRIX, rll_node.m);

        //Left Front Leg

            glLoadIdentity();
            glTranslatef(-(TORSO_RADIUS - 4.65 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, -1.5);   //front left upper leg
            glRotatef(theta[3], 0.0, 0.0, 1.0);
            glGetFloatv(GL_MODELVIEW_MATRIX, lul_node.m);

            //Left Front Lower Leg

                glLoadIdentity();

                glRotatef(180, 0.0, 0.0, 1.0);
                glRotatef(theta[7], 0.0, 0.0, 1.0);
                glTranslatef(0.0, -1.5 * UPPER_LEG_HEIGHT, 0.0);
                glGetFloatv(GL_MODELVIEW_MATRIX, lll_node.m);

        //Right Back Leg

            glLoadIdentity();
            glTranslatef(-(TORSO_RADIUS - 0.35 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, 1.5);    //back right upper leg
            glRotatef(theta[6], 0.0, 0.0, 1.0);
            glGetFloatv(GL_MODELVIEW_MATRIX, brul_node.m);

            //Right Back Lower Leg

                glLoadIdentity();

                glRotatef(theta[10], 0.0, 0.0, 1.0);
                glTranslatef(0.0, -1.5 * UPPER_LEG_HEIGHT, 0.0);
                glGetFloatv(GL_MODELVIEW_MATRIX, brll_node.m);



            //Left Back Leg

            glLoadIdentity();
            glTranslatef(-(TORSO_RADIUS - 0.35 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, -1.5);    //back right upper leg
            glRotatef(theta[5], 0.0, 0.0, 1.0);
            glGetFloatv(GL_MODELVIEW_MATRIX, blul_node.m);

            //Left Back Lower Leg

                glLoadIdentity();
                glRotatef(180, 0.0, 0.0, 1.0);
                glRotatef(theta[9], 0.0, 0.0, 1.0);
                glTranslatef(0.0, -1.5 * UPPER_LEG_HEIGHT, 0.0);
                glGetFloatv(GL_MODELVIEW_MATRIX, blll_node.m);


    }

    //Lift Front Right Leg
    void initiatePos4()
    {
        GLfloat temp_angles[16] = { 0.0, 0.0, 0.0, 0.0, 40.0, 0.0, 0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
        animateAngles(temp_angles);

        //Right Front Leg

        glLoadIdentity();
        glTranslatef(-(TORSO_RADIUS - 4.65 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, 1.5);    //front right upper leg
        glRotatef(theta[4], 0.0, 0.0, 1.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, rul_node.m);

            //Right Front Lower Leg

            glLoadIdentity();
            glRotatef(180, 1.0, 0.0, 0.0);
            glRotatef(theta[8], 0.0, 0.0, 1.0);
            glGetFloatv(GL_MODELVIEW_MATRIX, rll_node.m);
    }

    //Dance Move
    void initiatePos5()
    {
        GLfloat temp_angles[16] = { -70.0, 0.0, 0.0, -50.0, -50.0, 40.0, -20.0, 100.0, 80.0, 150.0, -50.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
        animateAngles(temp_angles);


        //Torso

            glLoadIdentity();
            glRotatef(theta[0], 1.0, 1.0, 0.0);
            glGetFloatv(GL_MODELVIEW_MATRIX, torso_node.m);


        //Right Front Leg

            glLoadIdentity();
            glTranslatef(-(TORSO_RADIUS - 4.65 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, 1.5);    //front right upper leg
            glRotatef(theta[4], 0.0, 1.0, 0.0);
            glGetFloatv(GL_MODELVIEW_MATRIX, rul_node.m);

            //Right Front Lower Leg

                glLoadIdentity();
                glRotatef(180, 1.0, 0.0, 0.0);
                glRotatef(theta[8], 1.0, 0.0, 0.0);
                glGetFloatv(GL_MODELVIEW_MATRIX, rll_node.m);

        //Left Front Leg

            glLoadIdentity();
            glTranslatef(-(TORSO_RADIUS - 4.65 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, -1.5);   //front left upper leg
            glRotatef(theta[3], 0.0, 1.0, 0.0);
            glGetFloatv(GL_MODELVIEW_MATRIX, lul_node.m);

            //Left Front Lower Leg

                glLoadIdentity();

                glRotatef(180, 0.0, 0.0, 1.0);
                glRotatef(theta[7], 0.0, 0.0, 1.0);
                glTranslatef(0.0, -1.5 * UPPER_LEG_HEIGHT, 0.0);
                glGetFloatv(GL_MODELVIEW_MATRIX, lll_node.m);

        //Right Back Leg

            glLoadIdentity();
            glTranslatef(-(TORSO_RADIUS - 0.35 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, 1.5);    //back right upper leg
            glRotatef(theta[6], 1.0, 0.0, 0.0);
            glGetFloatv(GL_MODELVIEW_MATRIX, brul_node.m);

            //Right Back Lower Leg

                glLoadIdentity();

                glRotatef(theta[10], 0.0, 0.0, 1.0);
                glTranslatef(0.0, -1.5 * UPPER_LEG_HEIGHT, 0.0);
                glGetFloatv(GL_MODELVIEW_MATRIX, brll_node.m);



        //Left Back Leg

            glLoadIdentity();
            glTranslatef(-(TORSO_RADIUS - 0.35 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, -1.5);    //back right upper leg
            glRotatef(theta[5], 1.0, 0.0, 0.0);
            glGetFloatv(GL_MODELVIEW_MATRIX, blul_node.m);

            //Left Back Lower Leg

                glLoadIdentity();
                glRotatef(180, 0.0, 0.0, 1.0);
                glRotatef(theta[9], 0.0, 0.0, 1.0);
                glTranslatef(0.0, -1.5 * UPPER_LEG_HEIGHT, 0.0);
                glGetFloatv(GL_MODELVIEW_MATRIX, blll_node.m);


    }

    //Rest Position
    void initiatePos6()
    {
        GLfloat temp_angles[16] = { 0.0, -20.0, -10.0, 50.0, 50.0, -50.0, -50.0, 40.0, 140.0, 140.0, -40.0, -90.0, 90.0, -90.0, -90.0, 0.0 };
        animateAngles(temp_angles);

        //Neck

        glLoadIdentity();
        glTranslatef(2.0, 4, 0.0);
        glRotatef(theta[1], 0.0, 0.0, 1.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, n_node.m);

            //Head

                glLoadIdentity();
                glTranslatef(5.5 * HEAD_HEIGHT, 2 * NECK_HEIGHT - 5 * HEAD_HEIGHT, 0.0);
                glRotatef(theta[2], 0.0, 0.0, 1.0);
                glTranslatef(0.0, -0.5 * HEAD_HEIGHT, 0.0);
                glGetFloatv(GL_MODELVIEW_MATRIX, head_node.m);


        //Right Front Leg

            glLoadIdentity();
            glTranslatef(-(TORSO_RADIUS - 4.65 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, 1.5);    //front right upper leg
            glRotatef(theta[4], 0.0, 0.0, 1.0);
            glGetFloatv(GL_MODELVIEW_MATRIX, rul_node.m);

            //Right Front Lower Leg

                glLoadIdentity();
                glRotatef(180, 1.0, 0.0, 0.0);
                glRotatef(theta[8], 0.0, 0.0, 1.0);
                glGetFloatv(GL_MODELVIEW_MATRIX, rll_node.m);

                //Right Front Foot

                    glLoadIdentity();
                    glTranslatef(-RIGHT_FOOT_RADIUS + 0.5, LOWER_LEG_HEIGHT + RIGHT_FOOT_RADIUS - 0.5, 0.0);    //Altered Original Position a bit for smoother foot position
                    glRotatef(theta[12], 0.0, 0.0, 1.0);
                    glGetFloatv(GL_MODELVIEW_MATRIX, rff_node.m);

        //Left Front Leg

            glLoadIdentity();
            glTranslatef(-(TORSO_RADIUS - 4.65 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, -1.5);   //front left upper leg
            glRotatef(theta[3], 0.0, 0.0, 1.0);
            glGetFloatv(GL_MODELVIEW_MATRIX, lul_node.m);

            //Left Front Lower Leg

                glLoadIdentity();

                glRotatef(180, 0.0, 0.0, 1.0);
                glRotatef(theta[7], 0.0, 0.0, 1.0);
                glTranslatef(0.0, -1.5 * UPPER_LEG_HEIGHT, 0.0);
                glGetFloatv(GL_MODELVIEW_MATRIX, lll_node.m);

                //Left Front Foot

                    glLoadIdentity();
                    glTranslatef(-LEFT_FOOT_RADIUS + 0.5, -LEFT_FOOT_RADIUS + 0.5, 0.0);    //Altered Original Position a bit for smoother foot position
                    glRotatef(theta[11], 0.0, 0.0, 1.0);
                    glGetFloatv(GL_MODELVIEW_MATRIX, lff_node.m);

        //Right Back Leg

            glLoadIdentity();
            glTranslatef(-(TORSO_RADIUS - 0.35 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, 1.5);    //back right upper leg
            glRotatef(theta[6], 0.0, 0.0, 1.0);
            glGetFloatv(GL_MODELVIEW_MATRIX, brul_node.m);

            //Right Back Lower Leg

                glLoadIdentity();

                glRotatef(theta[10], 0.0, 0.0, 1.0);
                glTranslatef(0.0, -1.5 * UPPER_LEG_HEIGHT, 0.0);
                glGetFloatv(GL_MODELVIEW_MATRIX, brll_node.m);

                //Right Back Foot

                    glLoadIdentity();
                    glTranslatef(-BACK_RIGHT_FOOT_RADIUS + 0.5, -BACK_RIGHT_FOOT_RADIUS + 0.5, 0.0);    //Altered Original Position a bit for smoother foot position
                    glRotatef(theta[14], 0.0, 0.0, 1.0);
                    glGetFloatv(GL_MODELVIEW_MATRIX, brf_node.m);



        //Left Back Leg

            glLoadIdentity();
            glTranslatef(-(TORSO_RADIUS - 0.35 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, -1.5);    //back right upper leg
            glRotatef(theta[5], 0.0, 0.0, 1.0);
            glGetFloatv(GL_MODELVIEW_MATRIX, blul_node.m);

            //Left Back Lower Leg

                glLoadIdentity();
                glRotatef(180, 0.0, 0.0, 1.0);
                glRotatef(theta[9], 0.0, 0.0, 1.0);
                glTranslatef(0.0, -1.5 * UPPER_LEG_HEIGHT, 0.0);
                glGetFloatv(GL_MODELVIEW_MATRIX, blll_node.m);

                //Left Back Foot

                glLoadIdentity();
                glTranslatef(-BACK_LEFT_FOOT_RADIUS + 0.5, -BACK_LEFT_FOOT_RADIUS + 0.5, 0.0);    //Altered Original Position a bit for smoother foot position
                glRotatef(theta[13], 0.0, 0.0, 1.0);
                glGetFloatv(GL_MODELVIEW_MATRIX, blf_node.m);
    }


//Preorder recursive traversion function (universal from theory)
void traverse(treenode* root)
{
    if (root == NULL) return;
    glPushMatrix();
    glMultMatrixf(root->m);
    root->f();
    if (root->child != NULL)
    {
        traverse(root->child);
    }
    glPopMatrix();
    if (root->sibling != NULL)
    {
        traverse(root->sibling);
    }

}

    //Draw and reshape cylinders for every joints

    //t
        void torso()
        {
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_red);

            glPushMatrix();
            glRotatef(90, 0.0, 1.0, 0.0);
            glTranslatef(0.0, 2, -3);
            gluCylinder(t, TORSO_RADIUS, TORSO_RADIUS, TORSO_HEIGHT, 10, 10);
            glPopMatrix();
        }

    //n
        void neck()
        {
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_orange);

            glPushMatrix();
            glTranslatef(2.5 * NECK_HEIGHT, -1.5, 0.0);
            glRotatef(-90.0, 0.0, 1.0, 0.0);
            gluCylinder(n, NECK_RADIUS, NECK_RADIUS, NECK_HEIGHT, 10, 10);
            glPopMatrix();
        }

    //h
        void head()
        {
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_yellow);

            glPushMatrix();
            glTranslatef(0.0, 0.5 * HEAD_HEIGHT, 0.0);
            glScalef(HEAD_RADIUS, HEAD_HEIGHT, HEAD_RADIUS);
            gluSphere(h, 1.0, 10, 10);
            glPopMatrix();
        }

    //lul
        void left_upper_leg()
        {
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_green);

            glPushMatrix();

            glRotatef(-90.0, 1.0, 0.0, 0.0);
            gluCylinder(lul, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS, UPPER_LEG_HEIGHT, 10, 10);
            glPopMatrix();
        }

    //blul
        void back_left_upper_leg()
        {
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_pink);

            glPushMatrix();

            glRotatef(-90.0, 1.0, 0.0, 0.0);
            gluCylinder(blul, BACK_UPPER_LEG_RADIUS, BACK_UPPER_LEG_RADIUS, BACK_UPPER_LEG_HEIGHT, 10, 10);
            glPopMatrix();
        }

    //lll
        void left_lower_leg()
        {
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_light_green);
            glPushMatrix();

            glRotatef(-90.0, 1.0, 0.0, 0.0);
            gluCylinder(lll, LOWER_LEG_RADIUS, LOWER_LEG_RADIUS, LOWER_LEG_HEIGHT, 10, 10);
            glPopMatrix();
        }

    //blll
        void back_left_lower_leg()
        {
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_light_pink);

            glPushMatrix();

            glRotatef(-90.0, 1.0, 0.0, 0.0);
            gluCylinder(blll, BACK_LOWER_LEG_RADIUS, BACK_LOWER_LEG_RADIUS, BACK_LOWER_LEG_HEIGHT, 10, 10);
            glPopMatrix();
        }

    //rul
        void right_upper_leg()
        {
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_blue);

            glPushMatrix();
    
            glRotatef(-90.0, 1.0, 0.0, 0.0);
            gluCylinder(rul, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS, UPPER_LEG_HEIGHT, 10, 10);
            glPopMatrix();
        }

    //brul
        void back_right_upper_leg()
        {
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_orange);

            glPushMatrix();

            glRotatef(-90.0, 1.0, 0.0, 0.0);
            gluCylinder(brul, BACK_UPPER_LEG_RADIUS, BACK_UPPER_LEG_RADIUS, BACK_UPPER_LEG_HEIGHT, 10, 10);
            glPopMatrix();
        }
    
    //rll
        void right_lower_leg()
        {
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_light_blue);

            glPushMatrix();

            glRotatef(-90.0, 1.0, 0.0, 0.0);
            gluCylinder(rll, LOWER_LEG_RADIUS, LOWER_LEG_RADIUS, LOWER_LEG_HEIGHT, 10, 10);
            glPopMatrix();
        }

    //brll
        void back_right_lower_leg()
        {
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_light_orange);

            glPushMatrix();

            glRotatef(-90.0, 1.0, 0.0, 0.0);
            gluCylinder(brll, BACK_LOWER_LEG_RADIUS, BACK_LOWER_LEG_RADIUS, BACK_LOWER_LEG_HEIGHT, 10, 10);
            glPopMatrix();
        }

    //rff
        void right_front_foot()
        {
    
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_dark_blue);

            glPushMatrix();

            glRotatef(90.0, 0.0, 1.0, 0.0);
            gluCylinder(rff, RIGHT_FOOT_RADIUS, RIGHT_FOOT_RADIUS, RIGHT_FOOT_HEIGHT, 10, 10);
            glPopMatrix();
        }

    //lff
        void left_front_foot()
        {
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_dark_green);

            glPushMatrix();

            glRotatef(90.0, 0.0, 1.0, 0.0);
            gluCylinder(lff, LEFT_FOOT_RADIUS, LEFT_FOOT_RADIUS, LEFT_FOOT_HEIGHT, 10, 10);
            glPopMatrix();
        }

    //brf
        void back_right_foot()
        {
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_dark_orange);

            glPushMatrix();

            glRotatef(90.0, 0.0, 1.0, 0.0);
            gluCylinder(brf, BACK_RIGHT_FOOT_RADIUS, BACK_RIGHT_FOOT_RADIUS, BACK_RIGHT_FOOT_HEIGHT, 10, 10);
            glPopMatrix();
        }

    //bff
        void back_left_foot()
        {
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_dark_pink);
            glPushMatrix();

            glRotatef(90.0, 0.0, 1.0, 0.0);
            gluCylinder(blf, BACK_LEFT_FOOT_RADIUS, BACK_LEFT_FOOT_RADIUS, BACK_LEFT_FOOT_HEIGHT, 10, 10);
            glPopMatrix();
        }



void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
      traverse(&torso_node);

    switch (menu_option)
    {
    //Free Roam
    case 0:   
        freeRoam(); 
        break;

    //Reset Object Position
    case 1:
        resetPosition();
        break;

    //Execute 'Lower Neck'
    case 2:
        initiatePos1();
        break;

    //Execute 'Kick Back'
    case 3:
        initiatePos2();
        break;

    //Execute 'Stand on Hind Legs'
    case 4:
        initiatePos3();
        break;

    //Execute 'Lift Front Leg'
    case 5:
        initiatePos4();
        break;

    //Execute 'Dance Move'
    case 6:
        initiatePos5();
        break;

    //Execute 'Rest'
    case 7:
        initiatePos6();
        break;
    }

    glPopMatrix();
    glutSwapBuffers();
}


void myReshape(int w, int h)
{
    //Create a glOrtho window inside OpenGL that is dependent on the width and height of the Windows window
    // and is 60 by 60 viewport inside OpenGL with culling on objects between -90 and 90

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-30.0, 30.0, -30.0 * (GLfloat)h / (GLfloat)w, 30.0 * (GLfloat)h / (GLfloat)w, -90.0, 90.0);
       
    glMatrixMode(GL_MODELVIEW);

    ww = w;
    wh = h;
}

void myinit()
{

    //Enable lighting, materials, depth, smoothness and clear blackness from field

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
   // glEnable(GL_COLOR_MATERIAL);

    glClearColor(1.0, 1.0, 1.0, 1.0);


    /* allocate quadrics with filled drawing style */

    h = gluNewQuadric();
    gluQuadricDrawStyle(h, GLU_FILL);
    t = gluNewQuadric();
    gluQuadricDrawStyle(t, GLU_FILL);
    n = gluNewQuadric();
    gluQuadricDrawStyle(n, GLU_FILL);
    lua = gluNewQuadric();
    gluQuadricDrawStyle(lua, GLU_FILL);
    lla = gluNewQuadric();
    gluQuadricDrawStyle(lla, GLU_FILL);
    rua = gluNewQuadric();
    gluQuadricDrawStyle(rua, GLU_FILL);
    rla = gluNewQuadric();
    gluQuadricDrawStyle(rla, GLU_FILL);
    lul = gluNewQuadric();
    gluQuadricDrawStyle(lul, GLU_FILL);
    lll = gluNewQuadric();
    gluQuadricDrawStyle(lll, GLU_FILL);
    rul = gluNewQuadric();
    gluQuadricDrawStyle(rul, GLU_FILL);
    rll = gluNewQuadric();
    gluQuadricDrawStyle(rll, GLU_FILL);
    brll = gluNewQuadric();
    gluQuadricDrawStyle(brll, GLU_FILL);
    blll = gluNewQuadric();
    gluQuadricDrawStyle(blll, GLU_FILL);
    blul = gluNewQuadric();
    gluQuadricDrawStyle(blul, GLU_FILL);
    brul = gluNewQuadric();
    gluQuadricDrawStyle(brul, GLU_FILL);
    lff = gluNewQuadric();
    gluQuadricDrawStyle(lff, GLU_FILL);
    rff = gluNewQuadric();
    gluQuadricDrawStyle(rff, GLU_FILL);
    blf = gluNewQuadric();
    gluQuadricDrawStyle(blf, GLU_FILL);
    brf = gluNewQuadric();
    gluQuadricDrawStyle(brf, GLU_FILL);



    /* Set up tree */
    //Self explanatory (as said above) tree construction with 1 sibling and 1 child per node max


    glLoadIdentity();
    glRotatef(theta[0], 0.0, 1.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, torso_node.m);
    torso_node.f = torso;
    torso_node.sibling = NULL;
    torso_node.child = &n_node;

    glLoadIdentity();

    glTranslatef(0.0, TORSO_HEIGHT, 0.0);
    glRotatef(theta[1], 1.0, 0.0, 0.0);
    glRotatef(theta[2], 0.0, 0.0, 1.0);
    glTranslatef(-3 * NECK_RADIUS, -2.5 *NECK_HEIGHT, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, n_node.m);
    n_node.f = neck;
    n_node.sibling = &lul_node;
    n_node.child = &head_node;

    
    glLoadIdentity();
    glTranslatef(-(TORSO_RADIUS - 4.65 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, -1.5);   //front left upper leg
    glRotatef(theta[3], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, lul_node.m);
    lul_node.f = left_upper_leg;
    lul_node.sibling = &rul_node;
    lul_node.child = &lll_node;


    glLoadIdentity();
    glTranslatef(-(TORSO_RADIUS - 4.65 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, 1.5);    //front right upper leg
    glRotatef(theta[4], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, rul_node.m);
    rul_node.f = right_upper_leg;
    rul_node.sibling = &blul_node;
    rul_node.child = &rll_node;


    glLoadIdentity();
    glTranslatef(-(TORSO_RADIUS - 0.35 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, -1.5); //back left upper leg
    glRotatef(theta[5], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, blul_node.m);
    blul_node.f = back_left_upper_leg;
    blul_node.sibling = &brul_node;
    blul_node.child = &blll_node;


    glLoadIdentity();
    glTranslatef(-(TORSO_RADIUS - 0.35 * UPPER_LEG_HEIGHT), -UPPER_LEG_HEIGHT, 1.5);    //back right upper leg
    glRotatef(theta[4], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, brul_node.m);
    brul_node.f = back_right_upper_leg;
    brul_node.sibling = NULL;
    brul_node.child = &brll_node;


    glLoadIdentity();
    glTranslatef(5.5 * HEAD_HEIGHT, 2 * NECK_HEIGHT - 5 * HEAD_HEIGHT, 0.0);
    glRotatef(theta[5], 1.0, 0.0, 0.0);
    glRotatef(theta[6], 0.0, 1.0, 0.0);
    glTranslatef(0.0, -0.5 * HEAD_HEIGHT, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, head_node.m);
    head_node.f = head;
    head_node.sibling = NULL;
    head_node.child = NULL;

    glLoadIdentity();
    glTranslatef(0.0, -1.5 * UPPER_LEG_HEIGHT, 0.0);

    glRotatef(theta[8], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, lll_node.m);
    lll_node.f = left_lower_leg;
    lll_node.sibling = NULL;
    lll_node.child = &lff_node;


    glLoadIdentity();
    glTranslatef(0.0, 0.0, 0.0);
    glRotatef(theta[9], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, rll_node.m);
    rll_node.f = right_lower_leg;
    rll_node.sibling = NULL;
    rll_node.child = &rff_node;

    glLoadIdentity();
    glTranslatef(0.0, -BACK_LOWER_LEG_HEIGHT, 0.0);
    glRotatef(theta[10], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, blll_node.m);
    blll_node.f = back_left_lower_leg;
    blll_node.sibling = NULL;
    blll_node.child = &blf_node;

    glLoadIdentity();
    glTranslatef(0.0, -BACK_LOWER_LEG_HEIGHT, 0.0);
    glRotatef(theta[11], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, brll_node.m);
    brll_node.f = back_right_lower_leg;
    brll_node.sibling = NULL;
    brll_node.child = &brf_node;

    glLoadIdentity();
    glTranslatef(-LEFT_FOOT_RADIUS, -LEFT_FOOT_RADIUS, 0.0);
    glRotatef(theta[12], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, lff_node.m);
    lff_node.f = left_front_foot;
    lff_node.sibling = NULL;
    lff_node.child = NULL;

    glLoadIdentity();
    glTranslatef(-RIGHT_FOOT_RADIUS, LOWER_LEG_HEIGHT + RIGHT_FOOT_RADIUS, 0.0);
    glRotatef(theta[13], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, rff_node.m);
    rff_node.f = right_front_foot;
    rff_node.sibling = NULL;
    rff_node.child = NULL;

    glLoadIdentity();
    glTranslatef(-BACK_LEFT_FOOT_RADIUS, -BACK_LEFT_FOOT_RADIUS, 0.0);
    glRotatef(theta[14], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, blf_node.m);
    blf_node.f = back_left_foot;
    blf_node.sibling = NULL;
    blf_node.child = NULL;

    glLoadIdentity();
    glTranslatef(-BACK_RIGHT_FOOT_RADIUS, -BACK_RIGHT_FOOT_RADIUS, 0.0);
    glRotatef(theta[15], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, brf_node.m);
    brf_node.f = back_right_foot;
    brf_node.sibling = NULL;
    brf_node.child = NULL;

    glLoadIdentity();

}


void processMenuEvents(int option) {

    menu_option = option;

    switch (option) {

        //Free mode
    case 0:
        angle = 0.0;
        break;
        
        //Reset Position
    case 1:
        angle = 0.0;
        updateTheta(default_angles);
        resetPosition();
        break;

        //Select Exit Program
    case 8:
        exit(1);
        break;

        //Reset Camera
    case 9:
        resetCamera();
        break;

        //Default on Exercises
    default:
        angle = 0.0;
        updateTheta(default_angles);
        break;
    }

}


void create_menus() 
{

    int menu = glutCreateMenu(processMenuEvents);

    //add entries to our menus
    glutAddMenuEntry("Reset Camera", 9);
    glutAddMenuEntry("0. Free Mode", 0);
    glutAddMenuEntry("1. Reset Position", 1);
    glutAddMenuEntry("2. Lower Neck", 2);
    glutAddMenuEntry("3. Kick Back", 3);
    glutAddMenuEntry("4. Stand on Hind Legs", 4);
    glutAddMenuEntry("5. Lift Front Leg", 5);
    glutAddMenuEntry("6. Dance Move", 6);
    glutAddMenuEntry("7. Rest", 7);
    glutAddMenuEntry("Exit", 8);

    // attach the menu to the right button
    glutAttachMenu(2);

}

void Update(void)
{
    //Constantly redraw
    glutPostRedisplay();
}

void main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(2000, 2000);
    glutCreateWindow("Project 2 a");
    myinit();
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);

    glutIdleFunc(Update);
    glutSpecialFunc(SpecialKeyHandler);
    glutKeyboardFunc(NormalKeyHandler);


    create_menus();



    glutMainLoop();
}