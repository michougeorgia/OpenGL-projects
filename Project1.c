#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

//Camvas related
GLsizei wh = 500, ww = 500;

//Camera related
static GLfloat cameraTranslatePos_x = 0.0;
static GLfloat cameraTranslatePos_y = 0.0;
static GLfloat cameraTranslatePos_z = 0.0;
static GLfloat cameraRotation = 0.0;
static GLfloat cameraRotationPos_x = 0.0;
static GLfloat cameraRotationPos_y = 0.0;
static GLfloat cameraRotationFlag_x = 0.0;
static GLfloat cameraRotationFlag_y = 0.0;

//Menu related
static GLint menu_option = 0;


//Exercises Starting Values

//Global
	//Control Point Movement
		GLdouble var_d = 10.0;		//Declared minimum distance that the closest control point should have to the mouse in order for it to move
		GLint min_i = 0;
		GLdouble min_d = 0.0;
		GLdouble temp_d = 0.0;
		GLboolean point_found = 0;

	//Reset
	GLboolean resetFlag = 0;		//Flag that resets and exercises values if needed

	//Selected point counter
	GLint currentPointNum1 = 0;
	GLint currentPointNum2 = 0;

//Exercise 1 + 2
	
	GLint numberOfPointsEx1_1 = 4;			//Number of control points for 1st curve
	GLfloat cubic_points1[4][3];			//Matrix of the 1st curve
	GLfloat cubicPoints1_converted[4][3];	//Matrix of the 1st curve converted to cubic interpolation
	GLint numberOfPointsEx1_2 = 4;			//Number of control points for 2nd curve
	GLfloat cubic_points2[4][3];			//Matrix of the 2nd curve
	GLfloat cubicPoints2_converted[4][3];	//Matrix of the 2nd curve converted to cubic interpolation




//Exercise 2
	GLint numberOfPointsEx2 = 7;				//Number of control points for Bezier curve
	GLfloat bezier_points1[4][3];				//Matrix for the Bezier



//Exercise 3
	GLint numberOfPointsEx3_1 = 4;				//Number of control points for 1st Bezier curve

	//Create Bezier matrix for 1st curve
	GLfloat bezier_points3_1[4][3] = { {-15.0,0.0,0.0 }, {-10.0,10.0,0.0 } , {-5.0,10.0,0.0} , {0.0, 0.0, 0.0} };

	GLint numberOfPointsEx3_2 = 4;				//Number of control points for 2nd Bezier curve

	//Create Bezier matrix for 2nd curve
	GLfloat bezier_points3_2[4][3]= { { 0.0, 0.0, 0.0 }, { 5.0, -10.0, 0.0 }, { 15, -15, 0.0 }, { 20.00, -5.0, 0.0 } };


//Exercise 4
	GLint numberOfPointsEx4 = 16;						//Number of total control points for the surface
	GLfloat cubicPointsConnected1_converted[4][3];		//Matrix for the 1st curve converted for cubic interpolation
	GLfloat cubicPointsConnected2_converted[4][3];		//Matrix for the 2nd curve converted for cubic interpolation
	GLfloat cubicPointsConnected3_converted[4][3];		//Matrix for the 3rd curve converted for cubic interpolation
	GLfloat cubicPointsConnected4_converted[4][3];		//Matrix for the 4th curve converted for cubic interpolation
	GLfloat converted_points0[4][4][3];					//3D matrix for the interpolated surface

	//Matrix multiplication Mb^-1 * Mi
	static GLfloat conversion[4][4] =
	{
		{1.0,0.0,0.0,0.0},
		{-0.8333,3.0,-1.5,0.3333},
		{0.3333,-1.5,3.0,-0.8333},
		{0.0,0.0,0.0,1.0}
	};

	// (Mb * Mi ^-1) * (Mb ^T * Mi ^T ^-1)

	static GLfloat conversion2[4][4] =
	{
		{1.0,0.0,0.0,0.0},
		{0.0,1.12345654321,0.296295703704,0.024691308642},
		{0.0,-0.296295703704,0.888887111112,0.148147851852},
		{0.0,0.024691308642,0.148147851852,0.049382617284}
	};

	//(Mb ^ -1 x Mi) ^ -1
	static GLfloat conversion2_1[4][4] =
	{
		{1.0,0.0,0.0,0.0},
		{18518.888,-55555.222,55555.888,-18518.555},
		{148148.444,-444444.111,444444.777,-148148.111},
		{500001,-1500000.0,1500000.0,-500000.0}
	};

	//(Mi^ T x Mb^ T ^ -1) ^ -1
	static GLfloat conversion2_2[4][4] =
	{
		{1.0,0.2962962222222,0.0370371111111,0.0},
		{0.0,0.444444444444,0.222222222222,0.0},
		{0.0,0.222222222222,0.444444444444,0.0},
		{0.0,-0.333333111111,-0.444444222222,1.0}
	};

	//Mi^-1 * Mb^-1
	static GLfloat conversion2_3[4][4] =
	{
		{1.0,0.0,0.0,0.0},
		{0.0,0.222222,0.074073,0.037037},
		{2.407406,0.814813,0.444444444444,0.296296},
		{4.0,2.0,1.333333,1.0}
	};

	//Mi^T
	static GLfloat conversion2_4[4][4] =
	{
		{1.0,-5.5,9.0,-4.5},
		{0.0,9.0,-22.5,13.5},
		{0.0,-4.5,18.0,-13.5},
		{0.0,1.0,-4.5,4.5}
	};

//Calculate Euclidean Distance
	GLdouble distance(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)
	{
		return sqrt( ( (x2 - x1) * (x2 - x1) ) + ( (y2 - y1) * (y2 - y1) ) );
	}

//Mouse related
	GLdouble mouseOgl[3] = { 0.0,0.0,0.0 }; //Position of mouse in 3D space
	static GLdouble mousePosX = 0.0;
	static GLdouble mousePosY = 0.0;
	static GLdouble mouseLastPosX = 0.0;
	static GLdouble mouseLastPosY = 0.0;
	static GLboolean mousePressed = 0;
	static GLboolean mouseDrag = 0;

//Update observer's view
void reshape_rotate() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-40.0, 40.0, -40.0 * (GLfloat)wh / (GLfloat)ww, 40.0 * (GLfloat)wh / (GLfloat)ww, -90.0, 90.0);

	//glOrtho(-60.0, 60.0, -60.0, 60.0, -500.0, 500.0);
	//glOrtho(-60.0, 60.0, -60.0, 60.0, -1000.0, 1000.0);
	//gluLookAt(0.0 + cameraTranslatePos_x, 100.0 + cameraTranslatePos_y, 100.0 + cameraTranslatePos_z, 0.0 , 0.0 , 0.0 , 0.0, 1.0, 0.0);
	
	
	//Rotate camera via the pressing of the arrow keys
	glRotatef(cameraRotation, cameraRotationFlag_x, cameraRotationFlag_y, 0);
	//Pan camera (entire plane) via the pressing of WASD
	glTranslatef(cameraTranslatePos_x, cameraTranslatePos_y, 0);
	//glTranslatef(cameraTranslatePos_x, cameraTranslatePos_y, cameraTranslatePos_z);
}


//Reset all camera values 
void resetCamera() 
{
	
	cameraTranslatePos_x = 0.0;
	cameraTranslatePos_y = 0.0;
	cameraTranslatePos_z = 0.0;
	cameraRotation = 0.0;
	cameraRotationPos_x = 0.0;
	cameraRotationPos_y = 0.0;
	cameraRotationFlag_x = 0.0;
	cameraRotationFlag_y = 0.0;
	reshape_rotate();
}

//Secial Keyboard button handler
void SpecialKeyHandler(int key, int x, int y) {
	//Rotate the camera around the plane's center by the arrows movements
	//Camera eye and vectors are dependent on GlOrtho()
	//Right Arrow = Right
	//Left Arrow = Left
	//Up Arrow = Up
	//Down Arrow = Down
	if (key == GLUT_KEY_RIGHT)
	{
		cameraRotationFlag_y = 1.0f;
		cameraRotationPos_y += 5.0f;
		cameraRotation = cameraRotationPos_y;
		reshape_rotate();
		cameraRotationFlag_y = 0.0f;
		cameraRotation = 0.0f;
	}
	if (key == GLUT_KEY_LEFT)
	{
		cameraRotationFlag_y = 1.0f;
		cameraRotationPos_y -= 5.0f;
		cameraRotation = cameraRotationPos_y;
		reshape_rotate();
		cameraRotationFlag_y = 0.0f;
		cameraRotation = 0.0f;
	}
	if (key == GLUT_KEY_UP)
	{
		cameraRotationFlag_x = 1.0f;
		cameraRotationPos_x += 5.0f;
		cameraRotation = cameraRotationPos_x;
		reshape_rotate();
		cameraRotationFlag_x = 0.0f;
		cameraRotation = 0.0f;
	}
	if (key == GLUT_KEY_DOWN)
	{
		cameraRotationFlag_x = 1.0f;
		cameraRotationPos_x -= 5.0f;
		cameraRotation = cameraRotationPos_x;
		reshape_rotate();
		cameraRotationFlag_x = 0.0f;
		cameraRotation = 0.0f;
	}

}

//Keyboard handler
void NormalKeyHandler(unsigned char key, int x, int y) {
	//WASD = pan plane in the direction of the keys
	//W = Up
	//S = Down
	//A = Left
	//D = Right
	if (key == 'w')
	{
		cameraTranslatePos_y -= 5.0f;
		reshape_rotate();
	}
	if (key == 's')
	{
		cameraTranslatePos_y += 5.0f;
		reshape_rotate();
	}
	if (key == 'd')
	{
		cameraTranslatePos_x -= 5.0f;
		reshape_rotate();
	}
	if (key == 'a')
	{
		cameraTranslatePos_x += 5.0f;
		reshape_rotate();
	}

	//Unused
	if (key == 'r')
	{
		cameraTranslatePos_z -= 5.0f;
		reshape_rotate();
	}
	if (key == 'f')
	{
		cameraTranslatePos_z += 5.0f;
		reshape_rotate();
	}
}


//handle click events of the mouse
void myMouse(int button, int state, int x, int y)
{
	//mouse coords to gl coords

	mousePosX = x;
	mousePosY = y;
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_UP) {   //on release left mouse button
			
			mousePressed = 1;	 //declare the mouse as clicked
			point_found = 0;	 // in case the mouse is dragged for a point, reset it if the mouse is unclicked 
			glutPostRedisplay(); //redisplay and calculate gl coords
		}

		if (state == GLUT_DOWN) {
			mouseDrag = 1;		//if the mouse stays clicked, declare it as moving
			glutPostRedisplay();
		}
		else
		{
			mouseDrag = 0;		//if the mosue is unclicked, declare is as not moving
		}

		break;
	}
}

void myDrag(int x, int y)
{
	//If the mouse is moving while it is clicked, update its position variables
	if (mouseDrag == 1)
	{
		mousePosX = x;
		mousePosY = y;
	}
}

// detailed information: 
// http://nehe.gamedev.net/article/using_gluunproject/16013/

void GetOGLPos(int x, int y)
{
	//init vars:
	GLint viewport[4];			// holds the x,y and height, width of our current window
	GLdouble modelview[16];		//The modelview matrix is 4x4
	GLdouble projection[16];	//The projection matrix is 4x4
	GLfloat winX, winY, winZ;	//x,y,z OpenGL mouse coordinates
	GLdouble posX, posY, posZ;	// Hold The Final Values
	//get gl specs

	//GL_MODELVIEW_MATRIX stores the modelview matrix information which
	//determines how the vertices of OpenGL primitives are transformed to eye coordinates
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);     //get Modelmatrix   
	
	//GL_PROJECTION_MATRIX stores the projection matrix which
	//transforms vertices in eye coordinates to clip coordinates
	glGetDoublev(GL_PROJECTION_MATRIX, projection);   //get projection matrix

	//GL_VIEWPORT stores on viewport the following info:
	//	viewport[0] = x
	//	viewport[1] = y
	//	viewport[2] = width
	//	viewport[3] = height
	glGetIntegerv(GL_VIEWPORT, viewport);             //get viewport values

	//calculate the gl mouseposition
	winX = (float)x;
	// Subtract The Current Mouse Y Coordinate From The Screen Height, since
	// Windows sees (0,0) at the top left while OpenGL coordinates start at the bottom left
	winY = (float)viewport[3] - (float)y;

	//Find the winZ (z mouse coordinates) from the framebuffer (this case depth buffer - GL_DEPTH_COMPONENT)
	//minimum value is 0 and maximum value is 1 multiplied by internal constants with final value in range [0,1]
	//glReadPixels(x , y, width, height, format, type, *pixels);
	// in this situation x,y are our mouse selection coordinates converted into OpenGL coordinates
	// and width and height are 1 indicating a single pixel
	glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);


	//gluUnProject converts Windows screen coordinates to OpenGL coordinates.
	// it maps the specified window coordinates (winX, winY, winZ)
	// into object coordinates (posX, posY, posZ)
	// using modelMatrix, projMatrix, and viewport
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	//Store the mouse coordinates on a global mouse array
	mouseOgl[0] = posX;
	mouseOgl[1] = posY;
	mouseOgl[2] = posZ;

	//NOTE winZ and posZ are not strictly necessary because the z variable is not used in this exercise,
	//     but will be used anyway for future reference and experimentation
}

void myInit()
{	//Enable functionality usage
	glEnable(GL_MAP1_VERTEX_3);
	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_DEPTH_TEST);
}


//Control point conversion for evaluator
void convertBezier2Cubic4(GLfloat cubicPoints[4][3], const GLfloat bezierPoints[4][3], GLint maxPoints, GLint maxElements) {
	int i, j, k;
	for (i = 0; i < maxPoints; i++) 
	{
		for (j = 0; j < maxElements; j++)
		{
			cubicPoints[i][j] = 0;
			for (k = 0; k < 4; k++) 
			{
				cubicPoints[i][j] += conversion[i][k] * bezierPoints[k][j];
			}
		}
	}
}

void convertBezier2Cubic16(GLfloat cubicPoints16[4][4][3], const GLfloat bezierPoints16[4][4][3], GLint maxPoints, GLint maxElements) {
	int i, j, k,l;
	GLfloat temp[4][4][3];
	
	for (l = 0; l < maxPoints; l++)
	{
		for (i = 0; i < maxPoints; i++) 
		{
			for (j = 0; j < maxElements; j++)
			{
				temp[l][i][j] = 0;
				for (k = 0; k < 4; k++) 
				{
					temp[l][i][j] += conversion2_3[l][k] * bezierPoints16[k][i][j];
				}
			}
		}
	}

	for (l = 0; l < maxPoints; l++)
	{
		for (i = 0; i < maxPoints; i++)
		{
			for (j = 0; j < maxElements; j++)
			{
				cubicPoints16[l][i][j] = 0;
				for (k = 0; k < 4; k++)
				{
					cubicPoints16[l][i][j] += bezierPoints16[l][k][j] * conversion2_4[k][i];
				}
			}
		}
	}
	

}



void exercise1() 
{
	//ερώτημα 1 με δύο καμπύλες και Co συνέχεια

	//Drag the completed final curve

		//If the bezier curves have been created
		//and the player clicks to drag a point
		//this tries to find the nearest point
		//which doesn't exceed var_d
		//and keeps it for moving it until the mouse is unclicked
		if (mouseDrag == 1 && currentPointNum1 == numberOfPointsEx1_1 && currentPointNum2 == numberOfPointsEx1_2 && !point_found)
		{
			//Gain the current relative position of the mouse for further calculations
			GetOGLPos(mousePosX, mousePosY);

			//For mouse movement later, store said position
			mouseLastPosX = mouseOgl[0];
			mouseLastPosY = mouseOgl[1];

			//min_i = position of the point with the shortest distance in the final curve
			//min_d = shortest euclidean distance between mouse position and current point
			//		  its default value is double the max distance declared by the programmer
			//temp_d = stores the euclidean distance of a point and the mouse's position to be used on calculations for ease
			min_i = 0;
			min_d = var_d*2;
			temp_d = 0.0;

			//Repeat for both curves jointed as one
			for (int i = 0; i < numberOfPointsEx1_1 + numberOfPointsEx1_2 - 1; i++)
			{
				//Check for first curve
				if (i < numberOfPointsEx1_1)
				{
					temp_d = distance(cubic_points1[i][0], cubic_points1[i][1], mouseOgl[0], mouseOgl[1]);
					//Check if said distance is the shortest and also less than the minimum distance set by the programmer
					if (temp_d < min_d && temp_d < var_d)
					{
						min_d = temp_d;
						min_i = i;
					}
				}
				//Check for second curve
				else
				{
					temp_d = distance(cubic_points2[i - (numberOfPointsEx1_2 - 1)][0], cubic_points2[i - (numberOfPointsEx1_2 - 1)][1], mouseOgl[0], mouseOgl[1]);
					//Check if said distance is the shortest and also less than the minimum distance set by the programmer
					if (temp_d < min_d && temp_d < var_d)
					{
						min_d = temp_d;
						min_i = i;
					}
				}

			}
			//printf("min_d: %f min_i: %d \n", min_d, min_i);

			//Update the point flag to proceed with mouse movement later
			//This variable is reset in myMouse() when themouse is unclicked
			point_found = 1;

		}

		//If the closest point to the mouse is found after 
		//the user attempted to drag his mouse and the
		//curves have been created,
		//this moves said point towards the mouse if the distance
		//is less than var_d
		if (point_found)
		{
			//Update and store the current mouse position
			GetOGLPos(mousePosX, mousePosY);

			//Proceed with the movement so long as the shortest distance of the found point is less than
			//the constant distance declared by the programmer
			if (min_d <= var_d)
			{
				//Update said point

				//Each point's x,y value is updated according to the difference between
				//the old mouse position and the new, updating said distance on said point

				//Check if said point exists in the first curve
				if (min_i < numberOfPointsEx1_1)
				{
					cubic_points1[min_i][0] += (mouseOgl[0] - mouseLastPosX);
					cubic_points1[min_i][1] += (mouseOgl[1] - mouseLastPosY);

					//Check if said point is shared between curves
					if (min_i == numberOfPointsEx1_1 - 1)
					{
						cubic_points2[min_i - (numberOfPointsEx1_1 - 1)][0] += (mouseOgl[0] - mouseLastPosX);
						cubic_points2[min_i - (numberOfPointsEx1_1 - 1)][1] += (mouseOgl[1] - mouseLastPosY);
					}
				}
				//Check if said point exists in the second curve
				else
				{
					cubic_points2[min_i - (numberOfPointsEx1_2 - 1)][0] += (mouseOgl[0] - mouseLastPosX);
					cubic_points2[min_i - (numberOfPointsEx1_2 - 1)][1] += (mouseOgl[1] - mouseLastPosY);
				}
			}
		
			//After succesfful movement, update the old mouse position
			//for calculation on the next iteration
			mouseLastPosX = mouseOgl[0];
			mouseLastPosY = mouseOgl[1];
		
		}
	



	//Create first curve

		//Create the curve's control points

			//If the mouse is clicked within the window and the curve has not been completed
			//create a point and append it on the curve
			if (mousePressed==1 && currentPointNum1 < numberOfPointsEx1_1)
			{
				printf("Point %d:\n", currentPointNum1);
				//Update the mouse as not pressed
				mousePressed = 0;

				//Get the current mouse's position after converting it for ease
				GetOGLPos(mousePosX, mousePosY);

				//Update the current curve's point by said mouse position
				cubic_points1[currentPointNum1][0] = mouseOgl[0];
				cubic_points1[currentPointNum1][1] = mouseOgl[1];
				cubic_points1[currentPointNum1][2] = 0;
				printf("x: %f y: %f z: %f\n", cubic_points1[currentPointNum1][0], cubic_points1[currentPointNum1][1], cubic_points1[currentPointNum1][2]);

				//Append
				currentPointNum1++;

			}

	
		//If all points on the first curve have been created, draw it

			if (currentPointNum1 == numberOfPointsEx1_1)
			{

				//Convert the curve's matrix for use in Bezier functions to be displayed as a cubic interpolation curve
				convertBezier2Cubic4(cubicPoints1_converted, cubic_points1, numberOfPointsEx1_1, 3);

				// Connect the control points on a line and draw it in light gray.
				glColor3f(0.7, 0.7, 0.7);	//Gray
				glBegin(GL_LINE_STRIP);
				for (int indx = 0; indx < numberOfPointsEx1_1; indx++)
					glVertex3fv(cubic_points1[indx]);
				glEnd();

				//Specify and Enable Bezier Curve

				glColor3f(0.0, 0.0, 1.0);	//Blue

				//Create the Bezier map for the first curve's points
				// glMap1f( points_type, u_min, u_max, u_order, u_stride, control_points);
				glMap1f(GL_MAP1_VERTEX_3, 0, 1, 3, numberOfPointsEx1_1, &cubicPoints1_converted[0][0]);

		
				//Evaluate 100 in between points of the calculated p(u) with u from 0 to 1
				glBegin(GL_LINE_STRIP);
				for (int i = 0; i <= 100; i++)
				{
					glEvalCoord1f((GLfloat)i / 100 );
				}
				glEnd();

				// Draw the Bezier control points as dots.
				glPointSize(5.0);	//Make the points large
				glColor3f(0.0, 1.0, 0.0);	//Green
				glBegin(GL_POINTS);
				for (int indx = 0; indx < numberOfPointsEx1_1; indx++)
					glVertex3fv(cubic_points1[indx]);
				glEnd();

			}

	//Create second curve

		//Create the second curve's control points

			//If the mouse is clicked within the window and the curve has not been completed
			//create a point and append it on the curve
			//additionally, check whether the first curve as been completed
			if ( (mousePressed == 1 && currentPointNum2 < numberOfPointsEx1_2) || (currentPointNum2 == 0 && currentPointNum1== numberOfPointsEx1_1))
			{
				printf("Point %d:\n", currentPointNum2);

				//Update the mouse as not pressed
				mousePressed = 0;

				//Exception on the first point
				//Make it equal to the last point of the first curve to simulate conjointment
				if (currentPointNum2 == 0)
				{
					cubic_points2[currentPointNum2][0] = cubic_points1[3][0];
					cubic_points2[currentPointNum2][1] = cubic_points1[3][1];
					cubic_points2[currentPointNum2][2] = cubic_points1[3][2];
				}
				else
				{
					//Get the current mouse's position after converting it for ease
					GetOGLPos(mousePosX, mousePosY);


					//Update the current curve's point by said mouse position
					cubic_points2[currentPointNum2][0] = mouseOgl[0];
					cubic_points2[currentPointNum2][1] = mouseOgl[1];
					cubic_points2[currentPointNum2][2] = 0;
				}
				printf("x: %f y: %f z: %f\n", cubic_points2[currentPointNum2][0], cubic_points2[currentPointNum2][1], cubic_points2[currentPointNum2][2]);
		
				//Append
				currentPointNum2++;

			}


		//If all points on the first curve have been created, draw it

			if (currentPointNum2 == numberOfPointsEx1_2)
			{

				//Convert the curve's matrix for use in Bezier functions to be displayed as a cubic interpolation curve
				convertBezier2Cubic4(cubicPoints2_converted, cubic_points2, numberOfPointsEx1_2, 3);


				// Connect the control points on a line and draw it in light gray.
				glColor3f(0.7, 0.7, 0.7);	//Gray
				glBegin(GL_LINE_STRIP);
				for (int indx = 0; indx < numberOfPointsEx1_2; indx++)
					glVertex3fv(cubic_points2[indx]);
				glEnd();

				//Specify and Enable Bezier Curve

				glColor3f(0.0, 1.0, 1.0);	//Cyan

				//Create the Bezier map for the second curve's points
				// glMap1f( points_type, u_min, u_max, u_order, u_stride, control_points);
				glMap1f(GL_MAP1_VERTEX_3, 0, 1, 3, numberOfPointsEx1_2, &cubicPoints2_converted[0][0]);


				//Evaluate 100 in between points of the calculated p(u) with u from 0 to 1
				glBegin(GL_LINE_STRIP);
				for (int i = 0; i <= 100; i++)
				{
					glEvalCoord1f((GLfloat)i / 100);
				}
				glEnd();

				// Draw the Bezier control points as dots.
				glPointSize(5.0);	//Make points larger
				glColor3f(0.0, 1.0, 0.0);	//Green
				glBegin(GL_POINTS);
				for (int indx = 0; indx < numberOfPointsEx1_2; indx++)
					glVertex3fv(cubic_points2[indx]);
				glEnd();

			}

}

void exercise2() 
{
	//ερώτημα 2 με κλειστή καμπύλη bezier και 7 σημεία ελέγχου


	//Drag the completed curve

		//If the bezier curves have been created
		//and the player clicks to drag a point
		//this tries to find the nearest point
		//which doesn't exceed var_d
		//and keeps it for moving it until the mouse is unclicked
		if (mouseDrag == 1 && currentPointNum1 == numberOfPointsEx2 && !point_found)
		{
			//Gain the current relative position of the mouse for further calculations
			GetOGLPos(mousePosX, mousePosY);

			//For mouse movement later, store said position
			mouseLastPosX = mouseOgl[0];
			mouseLastPosY = mouseOgl[1];

			//min_i = position of the point with the shortest distance in the final curve
			//min_d = shortest euclidean distance between mouse position and current point
			//		  its default value is double the max distance declared by the programmer
			//temp_d = stores the euclidean distance of a point and the mouse's position to be used on calculations for ease
			min_i = 0;
			min_d = var_d * 2;
			temp_d = 0.0;

			//Repeat until all control points of the curve have been checked
			for (int i = 0; i < numberOfPointsEx2 - 1; i++)
			{

				temp_d = distance(bezier_points1[i][0], bezier_points1[i][1], mouseOgl[0], mouseOgl[1]);
				//Check if said distance is the shortest and also less than the minimum distance set by the programmer
				if (temp_d < min_d && temp_d < var_d)
				{
					min_d = temp_d;
					min_i = i;
				}

			}
			//printf("min_d: %f min_i: %d \n", min_d, min_i);

			//Update the point flag to proceed with mouse movement later
			//This variable is reset in myMouse() when themouse is unclicked
			point_found = 1;

		}

		//If the closest point to the mouse is found after 
		//the user attempted to drag his mouse and the
		//curves have been created,
		//this moves said point towards the mouse if the distance
		//is less than var_d
		if (point_found)
		{
			//Update and store the current mouse position
			GetOGLPos(mousePosX, mousePosY);

			//Proceed with the movement so long as the shortest distance of the found point is less than
			//the constant distance declared by the programmer
			if (min_d <= var_d)
			{
				//Update said point

				//Each point's x,y value is updated according to the difference between
				//the old mouse position and the new, updating said distance on said point


				bezier_points1[min_i][0] += (mouseOgl[0] - mouseLastPosX);
				bezier_points1[min_i][1] += (mouseOgl[1] - mouseLastPosY);
				if (min_i == 0)
				{
					bezier_points1[numberOfPointsEx2 - 1][0] += (mouseOgl[0] - mouseLastPosX);
					bezier_points1[numberOfPointsEx2 - 1][1] += (mouseOgl[1] - mouseLastPosY);
				}

			}

			//After succesfful movement, update the old mouse position
			//for calculation on the next iteration
			mouseLastPosX = mouseOgl[0];
			mouseLastPosY = mouseOgl[1];

		}



	//Create the curve

		//Create the control points of the curve
	
			//If the mouse is clicked within the window and the curve has not been completed
			//create a point and append it on the curve
			//Exception on the last point append in which we enter the iteration anyway
			//as we have created a specific interaction for it withing the point creation
			if ( (mousePressed == 1 && currentPointNum1 < numberOfPointsEx2) || (currentPointNum1 == numberOfPointsEx2-1) )
			{
				printf("Point %d:\n", currentPointNum1);

				//Update the mouse as not pressed
				mousePressed = 0;

				//If we have found the last point of the curve, create it to have
				//equal values to the first point, so that the curve loops in a circle
				if (currentPointNum1 == numberOfPointsEx2 - 1)
				{
					bezier_points1[currentPointNum1][0] = bezier_points1[0][0];
					bezier_points1[currentPointNum1][1] = bezier_points1[0][1];
					bezier_points1[currentPointNum1][2] = bezier_points1[0][2];
				}
				//Otherwise create its point by the mouse's position
				else
				{
					//Get the current mouse's position after converting it for ease
					GetOGLPos(mousePosX, mousePosY);

					//Update the current curve's point by said mouse position
					bezier_points1[currentPointNum1][0] = mouseOgl[0];
					bezier_points1[currentPointNum1][1] = mouseOgl[1];
					bezier_points1[currentPointNum1][2] = 0;
				}
				printf("x: %f y: %f z: %f\n", bezier_points1[currentPointNum1][0], bezier_points1[currentPointNum1][1], bezier_points1[currentPointNum1][2]);

				//Append
				currentPointNum1++;

			}


		//If all points have been created, draw the curve

			if (currentPointNum1 == numberOfPointsEx2)
			{
				// Connect the control points on a line and draw it in light gray.
				glColor3f(0.7, 0.7, 0.7);	//Gray
				glBegin(GL_LINE_STRIP);
				for (int indx = 0; indx < numberOfPointsEx2; indx++)
					glVertex3fv(bezier_points1[indx]);
				glEnd();


				glColor3f(0.0, 1.0, 0.0);	//Green

				//Create the Bezier map for the second curve's points
				// glMap1f( points_type, u_min, u_max, u_order, u_stride, control_points);
				glMap1f(GL_MAP1_VERTEX_3, 0, 1, 3, numberOfPointsEx2, &bezier_points1[0][0]);


				//Evaluate 100 in between points of the calculated p(u) with u from 0 to 1
				glBegin(GL_LINE_STRIP);
				for (int i = 0; i <= 100; i++)
				{
					glEvalCoord1f((GLfloat)i / 100);
				}
				glEnd();


				// Draw the Bezier control points as dots.
				glPointSize(5.0);
				glColor3f(1.0, 0.0, 0.0);	//Red
				glBegin(GL_POINTS);
				for (int indx = 0; indx < numberOfPointsEx2; indx++)
					glVertex3fv(bezier_points1[indx]);
				glEnd();

			}



}
/* exercise 3 - solution1 : 2 bezier with defined control points and Co,C1.
	* C1 thought: p'(1) = q'(0) => q1 - q0 = p3 - p2 => q1 = -p2 (αν το σημείο ένωσης είναι το (0,0,0)).
	*/
void exercise3()
{


	//Drag the completed final curve

		//If the user clicks to drag a point
		//this tries to find the nearest point
		//which doesn't exceed var_d
		//and keeps it for moving it until the mouse is unclicked
	if (mouseDrag == 1 && !point_found)
	{
		//Gain the current relative position of the mouse for further calculations
		GetOGLPos(mousePosX, mousePosY);

		//For mouse movement later, store said position
		mouseLastPosX = mouseOgl[0];
		mouseLastPosY = mouseOgl[1];

		//min_i = position of the point with the shortest distance in the final curve
		//min_d = shortest euclidean distance between mouse position and current point
		//		  its default value is double the max distance declared by the programmer
		//temp_d = stores the euclidean distance of a point and the mouse's position to be used on calculations for ease
		min_i = 0;
		min_d = var_d * 2;
		temp_d = 0.0;

		//Repeat for both curves jointed as one
		for (int i = 0; i < numberOfPointsEx3_1 + numberOfPointsEx3_2 - 1; i++)
		{
			//Check for first curve
			if (i < numberOfPointsEx3_1)
			{
				temp_d = distance(bezier_points3_1[i][0], bezier_points3_1[i][1], mouseOgl[0], mouseOgl[1]);
				//Check if said distance is the shortest and also less than the minimum distance set by the programmer
				if (temp_d < min_d && temp_d < var_d)
				{
					min_d = temp_d;
					min_i = i;
				}
			}
			//Check for second curve
			else
			{
				temp_d = distance(bezier_points3_2[i - (numberOfPointsEx3_2 - 1)][0], bezier_points3_2[i - (numberOfPointsEx3_2 - 1)][1], mouseOgl[0], mouseOgl[1]);
				//Check if said distance is the shortest and also less than the minimum distance set by the programmer
				if (temp_d < min_d && temp_d < var_d)
				{
					min_d = temp_d;
					min_i = i;
				}
			}

		}
		//printf("min_d: %f min_i: %d \n", min_d, min_i);

		//Update the point flag to proceed with mouse movement later
		//This variable is reset in myMouse() when themouse is unclicked
		point_found = 1;

	}

	//If the closest point to the mouse is found after 
	//the user attempted to drag his mouse and the
	//curves have been created,
	//this moves said point towards the mouse if the distance
	//is less than var_d
	if (point_found)
	{
		//Update and store the current mouse position
		GetOGLPos(mousePosX, mousePosY);

		//Proceed with the movement so long as the shortest distance of the found point is less than
		//the constant distance declared by the programmer
		if (min_d <= var_d)
		{
			//Update said point

			//Each point's x,y value is updated according to the difference between
			//the old mouse position and the new, updating said distance on said point

			//Check if said point exists in the first curve
			if (min_i < numberOfPointsEx3_1)
			{
				bezier_points3_1[min_i][0] += (mouseOgl[0] - mouseLastPosX);
				bezier_points3_1[min_i][1] += (mouseOgl[1] - mouseLastPosY);

				//Check if said point is shared between curves
				if (min_i == numberOfPointsEx3_1 - 1)
				{
					bezier_points3_2[min_i - (numberOfPointsEx3_1 - 1)][0] += (mouseOgl[0] - mouseLastPosX);
					bezier_points3_2[min_i - (numberOfPointsEx3_1 - 1)][1] += (mouseOgl[1] - mouseLastPosY);
				}
			}
			//Check if said point exists in the second curve
			else
			{
				bezier_points3_2[min_i - (numberOfPointsEx3_2 - 1)][0] += (mouseOgl[0] - mouseLastPosX);
				bezier_points3_2[min_i - (numberOfPointsEx3_2 - 1)][1] += (mouseOgl[1] - mouseLastPosY);
			}
		}

		//After succesfful movement, update the old mouse position
		//for calculation on the next iteration
		mouseLastPosX = mouseOgl[0];
		mouseLastPosY = mouseOgl[1];

	}




	//Draw curve 1
	
		// Connect the control points on a line and draw it in light gray.
		glColor3f(0.7, 0.7, 0.7);	//Gray
		glBegin(GL_LINE_STRIP);
		for (int indx = 0; indx < numberOfPointsEx3_1; indx++)
			glVertex3fv(bezier_points3_1[indx]);
		glEnd();


		glColor3f(0.0, 1.0, 0.0);	//Green

		//Create the Bezier map for the second curve's points
		// glMap1f( points_type, u_min, u_max, u_order, u_stride, control_points);
		glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &bezier_points3_1[0][0]);



		//Evaluate 100 in between points of the calculated p(u) with u from 0 to 1
		glBegin(GL_LINE_STRIP);
		glColor3f(0.0, 0.0, 1.0);
		for (int i = 0; i <= 100; i++)
		{
			glEvalCoord1f((GLfloat)i / 100);
		}
		glEnd();


		// Draw the Bezier control points as dots.
		glPointSize(5.0);
		glColor3f(0.0, 1.0, 0.0);		//Green
		glBegin(GL_POINTS);
		for (int indx = 0; indx < numberOfPointsEx3_1; indx++)
			glVertex3fv(bezier_points3_1[indx]);
		glEnd();





	//Draw curve 2
	
		// Connect the control points on a line and draw it in light gray.
		glColor3f(0.7, 0.7, 0.7);	//Gray
		glBegin(GL_LINE_STRIP);
		for (int indx = 0; indx < numberOfPointsEx3_2; indx++)
			glVertex3fv(bezier_points3_2[indx]);
		glEnd();


		glColor3f(0.0, 1.0, 0.0);	//Green

		//Create the Bezier map for the second curve's points
		// glMap1f( points_type, u_min, u_max, u_order, u_stride, control_points);
		glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &bezier_points3_2[0][0]);



		//Evaluate 100 in between points of the calculated p(u) with u from 0 to 1
		glBegin(GL_LINE_STRIP);
		glColor3f(0.0, 0.0, 1.0);
		for (int i = 0; i <= 100; i++)
		{
			glEvalCoord1f((GLfloat)i / 100);
		}
		glEnd();


		// Draw the Bezier control points as dots.
		glPointSize(5.0);
		glColor3f(0.0, 1.0, 0.0);		//Green
		glBegin(GL_POINTS);
		for (int indx = 0; indx < numberOfPointsEx3_2; indx++)
			glVertex3fv(bezier_points3_2[indx]);
		glEnd();

}


void exercise4()
{

	//ερώτημα 4 - δικυβική επιφάνεια bezier με 16 προκαθορισμένα σημεία ελέγχου, x = 0

	

	//int uSize = 30;
	//int vSize = 30;


	// ο αριθμός των υποδιαιρέσεων στο πλέγμα
	int gridSize = 40;



	//Extreme example
	/*
	//p00 - p03
	{0, 20, 10},
	{ -10,-10,20 },
	{ 10,10,-10 },
	{ 0,0,30 },

		//p10 - p13
	{ 30,-20,0 },
	{ 10,20,-10 },
	{ 30,-10,-30 },
	{ 20,0,10 },

		//p20 - p23
	{ -10,10,0 },
	{ -30,-20,10 },
	{ 30,20,20 },
	{ 10,-10,30 },

		//p30 - p33
	{ 0,10,10 },
	{ 20,20,20 },
	{ -20,-20,-10 },
	{ 0,-10,0 }

	*/

	GLfloat points1[16][3] = { 
		//px0 and px3 always have x=0 so that
		//the surface lies on zy


		//p00 - p03
		{0,0,10}, 
		{-10,0,10}, 
		{10,0,20}, 
		{0,0,30},

		//p10 - p13
		{20,10,0}, 
		{10,10,10}, 
		{10,10,20}, 
		{20,10,30},

		//p20 - p23
		{-10,20,0}, 
		{10,20,10}, 
		{10,20,20},
		{-15,20,30}, 

		//p30 - p33
		{0,30,0} , 
		{10,30,10}, 
		{10,30,20}, 
		{0,30,30}
	};



	GLfloat points2[4][4][3] = {
		//px0 and px3 always have x=0 so that
		//the surface lies on zy


		//p00 - p03
		{
			{0,0,10},
			{-10,0,10},
			{10,0,20},
			{0,0,30}
		},

		//p10 - p13
		{
			{20,10,0},
			{10,10,10},
			{10,10,20},
			{20,10,30}
		},

		//p20 - p23
		{
			{-10,20,0},
			{10,20,10},
			{10,20,20},
			{-15,20,30}
		},

		//p30 - p33
		{
			{0,30,0} ,
			{10,30,10},
			{10,30,20},
			{0,30,30}
		}

	};


	GLfloat cubic_connected_points1[4][3] =
	{
		//p00 - p03
		{0,0,10},
		{-10,0,10},
		{10,0,20},
		{0,0,30}
	};

	GLfloat cubic_connected_points2[4][3] =
	{
		//p10 - p13
		{20,10,0},
		{10,10,10},
		{10,10,20},
		{20,10,30}

	};

	GLfloat cubic_connected_points3[4][3] =
	{
		//p20 - p23
		{-10,20,0},
		{10,20,10},
		{10,20,20},
		{-15,20,30}

	};

	GLfloat cubic_connected_points4[4][3] =
	{
		//p30 - p33
		{0,30,0} ,
		{10,30,10},
		{10,30,20},
		{0,30,30}
	};

	convertBezier2Cubic4(cubicPointsConnected1_converted, cubic_connected_points1, 4, 3);
	convertBezier2Cubic4(cubicPointsConnected2_converted, cubic_connected_points2, 4, 3);
	convertBezier2Cubic4(cubicPointsConnected3_converted, cubic_connected_points3, 4, 3);
	convertBezier2Cubic4(cubicPointsConnected4_converted, cubic_connected_points4, 4, 3);


	GLfloat converted_points1[16][3] = {

		//p00 - p03
		{cubicPointsConnected1_converted[0][0], cubicPointsConnected1_converted[0][1], cubicPointsConnected1_converted[0][2] },
		{cubicPointsConnected1_converted[1][0], cubicPointsConnected1_converted[1][1], cubicPointsConnected1_converted[1][2] },
		{cubicPointsConnected1_converted[2][0], cubicPointsConnected1_converted[2][1], cubicPointsConnected1_converted[2][2] },
		{cubicPointsConnected1_converted[3][0], cubicPointsConnected1_converted[3][1], cubicPointsConnected1_converted[3][2] },


		//p10 - p13
		{cubicPointsConnected2_converted[0][0], cubicPointsConnected2_converted[0][1], cubicPointsConnected2_converted[0][2] },
		{cubicPointsConnected2_converted[1][0], cubicPointsConnected2_converted[1][1], cubicPointsConnected2_converted[1][2] },
		{cubicPointsConnected2_converted[2][0], cubicPointsConnected2_converted[2][1], cubicPointsConnected2_converted[2][2] },
		{cubicPointsConnected2_converted[3][0], cubicPointsConnected2_converted[3][1], cubicPointsConnected2_converted[3][2] },


		//p20 - p23
		{cubicPointsConnected3_converted[0][0], cubicPointsConnected3_converted[0][1], cubicPointsConnected3_converted[0][2] },
		{cubicPointsConnected3_converted[1][0], cubicPointsConnected3_converted[1][1], cubicPointsConnected3_converted[1][2] },
		{cubicPointsConnected3_converted[2][0], cubicPointsConnected3_converted[2][1], cubicPointsConnected3_converted[2][2] },
		{cubicPointsConnected3_converted[3][0], cubicPointsConnected3_converted[3][1], cubicPointsConnected3_converted[3][2] },


		//p30 - p33
		{cubicPointsConnected4_converted[0][0], cubicPointsConnected4_converted[0][1], cubicPointsConnected4_converted[0][2] },
		{cubicPointsConnected4_converted[1][0], cubicPointsConnected4_converted[1][1], cubicPointsConnected4_converted[1][2] },
		{cubicPointsConnected4_converted[2][0], cubicPointsConnected4_converted[2][1], cubicPointsConnected4_converted[2][2] },
		{cubicPointsConnected4_converted[3][0], cubicPointsConnected4_converted[3][1], cubicPointsConnected4_converted[3][2] }
	};

	/*

	GLfloat converted_points2[16][3] = {

		//p00 - p30
		{cubicPointsConnected1_converted[0][0], cubicPointsConnected1_converted[0][1], cubicPointsConnected1_converted[0][2] },
		{cubicPointsConnected2_converted[0][0], cubicPointsConnected2_converted[0][1], cubicPointsConnected2_converted[0][2] },
		{cubicPointsConnected3_converted[0][0], cubicPointsConnected3_converted[0][1], cubicPointsConnected3_converted[0][2] },
		{cubicPointsConnected4_converted[0][0], cubicPointsConnected4_converted[0][1], cubicPointsConnected4_converted[0][2] },


		//p01 - p31
		{cubicPointsConnected1_converted[1][0], cubicPointsConnected1_converted[1][1], cubicPointsConnected1_converted[1][2] },
		{cubicPointsConnected2_converted[1][0], cubicPointsConnected2_converted[1][1], cubicPointsConnected2_converted[1][2] },
		{cubicPointsConnected3_converted[1][0], cubicPointsConnected3_converted[1][1], cubicPointsConnected3_converted[1][2] },
		{cubicPointsConnected4_converted[1][0], cubicPointsConnected4_converted[1][1], cubicPointsConnected4_converted[1][2] },


		//p02 - p32
		{cubicPointsConnected1_converted[2][0], cubicPointsConnected1_converted[2][1], cubicPointsConnected1_converted[2][2] },
		{cubicPointsConnected2_converted[2][0], cubicPointsConnected2_converted[2][1], cubicPointsConnected2_converted[2][2] },
		{cubicPointsConnected3_converted[2][0], cubicPointsConnected3_converted[2][1], cubicPointsConnected3_converted[2][2] },
		{cubicPointsConnected4_converted[2][0], cubicPointsConnected4_converted[2][1], cubicPointsConnected4_converted[2][2] },


		//p03 - p33
		{cubicPointsConnected1_converted[3][0], cubicPointsConnected1_converted[3][1], cubicPointsConnected1_converted[3][2] },
		{cubicPointsConnected2_converted[3][0], cubicPointsConnected2_converted[3][1], cubicPointsConnected2_converted[3][2] },
		{cubicPointsConnected3_converted[3][0], cubicPointsConnected3_converted[3][1], cubicPointsConnected3_converted[3][2] },
		{cubicPointsConnected4_converted[3][0], cubicPointsConnected4_converted[3][1], cubicPointsConnected4_converted[3][2] },

	};

	*/

	//converted_points1 = converted_points2


	GLfloat converted_points3[4][4][3] = {

		//p00 - p03
		{
			{cubicPointsConnected1_converted[0][0], cubicPointsConnected1_converted[0][1], cubicPointsConnected1_converted[0][2] },
			{cubicPointsConnected1_converted[1][0], cubicPointsConnected1_converted[1][1], cubicPointsConnected1_converted[1][2] },
			{cubicPointsConnected1_converted[2][0], cubicPointsConnected1_converted[2][1], cubicPointsConnected1_converted[2][2] },
			{cubicPointsConnected1_converted[3][0], cubicPointsConnected1_converted[3][1], cubicPointsConnected1_converted[3][2] }
		},


		//p10 - p13
		{
			{cubicPointsConnected2_converted[0][0], cubicPointsConnected2_converted[0][1], cubicPointsConnected2_converted[0][2] },
			{cubicPointsConnected2_converted[1][0], cubicPointsConnected2_converted[1][1], cubicPointsConnected2_converted[1][2] },
			{cubicPointsConnected2_converted[2][0], cubicPointsConnected2_converted[2][1], cubicPointsConnected2_converted[2][2] },
			{cubicPointsConnected2_converted[3][0], cubicPointsConnected2_converted[3][1], cubicPointsConnected2_converted[3][2] }
		},


		//p20 - p23
		{
			{cubicPointsConnected3_converted[0][0], cubicPointsConnected3_converted[0][1], cubicPointsConnected3_converted[0][2] },
			{cubicPointsConnected3_converted[1][0], cubicPointsConnected3_converted[1][1], cubicPointsConnected3_converted[1][2] },
			{cubicPointsConnected3_converted[2][0], cubicPointsConnected3_converted[2][1], cubicPointsConnected3_converted[2][2] },
			{cubicPointsConnected3_converted[3][0], cubicPointsConnected3_converted[3][1], cubicPointsConnected3_converted[3][2] }
		},


		//p30 - p33
		{
			{cubicPointsConnected4_converted[0][0], cubicPointsConnected4_converted[0][1], cubicPointsConnected4_converted[0][2] },
			{cubicPointsConnected4_converted[1][0], cubicPointsConnected4_converted[1][1], cubicPointsConnected4_converted[1][2] },
			{cubicPointsConnected4_converted[2][0], cubicPointsConnected4_converted[2][1], cubicPointsConnected4_converted[2][2] },
			{cubicPointsConnected4_converted[3][0], cubicPointsConnected4_converted[3][1], cubicPointsConnected4_converted[3][2] }
		}
	};

	//Trial with separate forms





	/*
	GLfloat converted_points4_1[2][4][3] = {

		//p00 - p03
		{
			{cubicPointsConnected1_converted[0][0], cubicPointsConnected1_converted[0][1], cubicPointsConnected1_converted[0][2] },
			{cubicPointsConnected1_converted[1][0], cubicPointsConnected1_converted[1][1], cubicPointsConnected1_converted[1][2] },
			{cubicPointsConnected1_converted[2][0], cubicPointsConnected1_converted[2][1], cubicPointsConnected1_converted[2][2] },
			{cubicPointsConnected1_converted[3][0], cubicPointsConnected1_converted[3][1], cubicPointsConnected1_converted[3][2] }
		},


		//p10 - p13
		{
			{cubicPointsConnected2_converted[0][0], cubicPointsConnected2_converted[0][1], cubicPointsConnected2_converted[0][2] },
			{cubicPointsConnected2_converted[1][0], cubicPointsConnected2_converted[1][1], cubicPointsConnected2_converted[1][2] },
			{cubicPointsConnected2_converted[2][0], cubicPointsConnected2_converted[2][1], cubicPointsConnected2_converted[2][2] },
			{cubicPointsConnected2_converted[3][0], cubicPointsConnected2_converted[3][1], cubicPointsConnected2_converted[3][2] }
		}


	};

	GLfloat converted_points4_2[2][4][3] = {

		//p10 - p13
		{
			{cubicPointsConnected2_converted[0][0], cubicPointsConnected2_converted[0][1], cubicPointsConnected2_converted[0][2] },
			{cubicPointsConnected2_converted[1][0], cubicPointsConnected2_converted[1][1], cubicPointsConnected2_converted[1][2] },
			{cubicPointsConnected2_converted[2][0], cubicPointsConnected2_converted[2][1], cubicPointsConnected2_converted[2][2] },
			{cubicPointsConnected2_converted[3][0], cubicPointsConnected2_converted[3][1], cubicPointsConnected2_converted[3][2] }
		},

		//p20 - p23
		{
			{cubicPointsConnected3_converted[0][0], cubicPointsConnected3_converted[0][1], cubicPointsConnected3_converted[0][2] },
			{cubicPointsConnected3_converted[1][0], cubicPointsConnected3_converted[1][1], cubicPointsConnected3_converted[1][2] },
			{cubicPointsConnected3_converted[2][0], cubicPointsConnected3_converted[2][1], cubicPointsConnected3_converted[2][2] },
			{cubicPointsConnected3_converted[3][0], cubicPointsConnected3_converted[3][1], cubicPointsConnected3_converted[3][2] }
		},


	};

	GLfloat converted_points4_3[2][4][3] = {

		//p20 - p23
		{
			{cubicPointsConnected3_converted[0][0], cubicPointsConnected3_converted[0][1], cubicPointsConnected3_converted[0][2] },
			{cubicPointsConnected3_converted[1][0], cubicPointsConnected3_converted[1][1], cubicPointsConnected3_converted[1][2] },
			{cubicPointsConnected3_converted[2][0], cubicPointsConnected3_converted[2][1], cubicPointsConnected3_converted[2][2] },
			{cubicPointsConnected3_converted[3][0], cubicPointsConnected3_converted[3][1], cubicPointsConnected3_converted[3][2] }
		},


		//p30 - p33
		{
			{cubicPointsConnected4_converted[0][0], cubicPointsConnected4_converted[0][1], cubicPointsConnected4_converted[0][2] },
			{cubicPointsConnected4_converted[1][0], cubicPointsConnected4_converted[1][1], cubicPointsConnected4_converted[1][2] },
			{cubicPointsConnected4_converted[2][0], cubicPointsConnected4_converted[2][1], cubicPointsConnected4_converted[2][2] },
			{cubicPointsConnected4_converted[3][0], cubicPointsConnected4_converted[3][1], cubicPointsConnected4_converted[3][2] }
		}
	};

	//Draw as Interpolation Surface

	glColor3f(1.0, 0.0, 0.0); //Red

	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 2, converted_points4_1);
	glMapGrid2f(gridSize, 0, 1, gridSize, 0, 1);
	glEvalMesh2(GL_LINE, 0, gridSize, 0, gridSize);
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 2, converted_points4_2);
	glMapGrid2f(gridSize, 0, 1, gridSize, 0, 1);
	glEvalMesh2(GL_LINE, 0, gridSize, 0, gridSize);
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 2, converted_points4_3);
	glMapGrid2f(gridSize, 0, 1, gridSize, 0, 1);
	glEvalMesh2(GL_LINE, 0, gridSize, 0, gridSize);
	//glEvalMesh2(GL_FILL, 0, gridSize, 0, gridSize);
	*/
	
	
	/*
	//Draw as Interpolation Surface

	glColor3f(1.0, 1.0, 0.0); //Yellow

	// glMap2f( points_type, u_min, u_max, u_order, u_stride, v_min, v_max, v_order, v_stride, control_points);
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, converted_points1);

	//Create a grid of drawn points from u and v (0 to 1 for each)
	//and divide each segment by the gridSize recursively
	glMapGrid2f(gridSize, 0, 1, gridSize, 0, 1);

	//Draw said grid with a line connection on the same division count
	glEvalMesh2(GL_LINE, 0, gridSize, 0, gridSize);
	//glEvalMesh2(GL_FILL, 0, gridSize, 0, gridSize);
	*/

	//Direct Conversion
	
	convertBezier2Cubic16(converted_points0, points2, 4, 3);





	//Draw as Interpolation Surface

	glColor3f(1.0, 1.0, 0.0); //Yellow

	// DRAW BEZIER SURFACE OF THE INITIAL POINTS
	// change 'points1' to other final 4x4x3 or 16x3 values for different surfaces
	// 
	// glMap2f( points_type, u_min, u_max, u_order, u_stride, v_min, v_max, v_order, v_stride, control_points);
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, points1[0]);
	

	//Create a grid of drawn points from u and v (0 to 1 for each)
	//and divide each segment by the gridSize recursively
	glMapGrid2f(gridSize, 0, 1, gridSize, 0, 1);

	//Draw said grid with a line connection on the same division count
	glEvalMesh2(GL_LINE, 0, gridSize, 0, gridSize);
	//glEvalMesh2(GL_FILL, 0, gridSize, 0, gridSize);
	
	

	//Draw separate Interpolation curves
	glColor3f(0.0, 1.0, 1.0);	//Cyan

	//Draw curve 1
	glMap1f(GL_MAP1_VERTEX_3, 0, 1, 3, 4, cubicPointsConnected1_converted[0]);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 100; i++)
		glEvalCoord1f((GLfloat)i / 100);
	glEnd();

	//Draw curve 2
	glMap1f(GL_MAP1_VERTEX_3, 0, 1, 3, 4, cubicPointsConnected2_converted[0]);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 100; i++)
		glEvalCoord1f((GLfloat)i / 100);
	glEnd();

	//Draw curve 3
	glMap1f(GL_MAP1_VERTEX_3, 0, 1, 3, 4, cubicPointsConnected3_converted[0]);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 100; i++)
		glEvalCoord1f((GLfloat)i / 100);
	glEnd();

	//Draw curve 4
	glMap1f(GL_MAP1_VERTEX_3, 0, 1, 3, 4, cubicPointsConnected4_converted[0]);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 100; i++)
		glEvalCoord1f((GLfloat)i / 100);
	glEnd();





	// Draw the Bezier angle control points as dots.

	glPointSize(5.0);
	glColor3f(1.0, 0.0, 0.0);	//Red
	glBegin(GL_POINTS);
	glVertex3fv(cubic_connected_points1[0]);
	glVertex3fv(cubic_connected_points1[3]);
	glVertex3fv(cubic_connected_points4[0]);
	glVertex3fv(cubic_connected_points4[3]);
	glEnd();

	// Draw the Bezier control points as dots.

	glPointSize(5.0);
	glColor3f(0.0, 1.0, 0.0);	//Green
	glBegin(GL_POINTS);
	for (int indx = 0; indx < 4; indx++)
	{
		glVertex3fv(cubic_connected_points1[indx]);
		glVertex3fv(cubic_connected_points2[indx]);
		glVertex3fv(cubic_connected_points3[indx]);
		glVertex3fv(cubic_connected_points4[indx]);
	}
	glEnd();




	/*

	//Draw as Bezier
	glColor3f(0.0, 1.0, 0.0); //Green
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, points);
	glMapGrid2f(gridSize, 0, 1, gridSize, 0, 1);
	glEvalMesh2(GL_LINE, 0, gridSize, 0, gridSize);
	*/





}

void processMenuEvents(int option) {


	switch (option) {
	//Select Exercise 1
	case 1:
		//Start Exercise 1
		menu_option = 1;
		//Reset Exercise 1 values
		resetFlag = 1;
		break;

	//Select Exercise 2
	case 2:
		//Start Exercise 2
		menu_option = 2;
		//Reset Exercise 2 values
		resetFlag = 1;
		break;

	//Select Exercise 3
	case 3:
		//Start Exercise 3
		menu_option = 3;
		break;

	//Select Exercise 4
	case 4:
		//Start Exercise 4
		menu_option = 4;
		break;

	//Select Reset Camera
	case 5:
		resetCamera();
		break;

	//Select Exit Program
	case 6:
		exit(1);
		break;

	//Exception
	default:
		menu_option = 0;
		break;
	}

}

void create_menus() {
	// create the menus and
	// tell glut that "processMenuEvents" will
	// handle the events


	int menu = glutCreateMenu(processMenuEvents);

	//add entries to our menus
	glutAddMenuEntry("1. Create 2 cubic interpolation curve segments", 1);
	glutAddMenuEntry("2. Create 1 6th degree cubic Bezier closed curve segments", 2);
	glutAddMenuEntry("3. Draw 2 cubic Bezier curve segments with continuity", 3);
	glutAddMenuEntry("4. Draw 1 bicubic interpolation surface segment", 4);
	glutAddMenuEntry("Reset Camera Position", 5);
	glutAddMenuEntry("Exit", 6);

	// attach the menu to the right button
	glutAttachMenu(2);
}



void display(void)
{
	myInit(); /* set attributes */
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	//Start selected exercise depending on menu_option number
	switch (menu_option) 
	{
	//Selected Exercise 1
	case 1:
		//Reset Exercise 1 values if needed
		if (resetFlag)
		{
			currentPointNum1 = 0;
			currentPointNum2 = 0;
			resetFlag = 0;
		}
		//Initiate Exercise 1
		exercise1();
		break;

	//Selected Exercise 2
	case 2:
		//Reset Exercise 2 values if needed
		if (resetFlag)
		{
			currentPointNum1 = 0;
			resetFlag = 0;
		}
		//Initiate Exercise 2
		exercise2();
		break;

	//Selected Exercise 3
	case 3:
		//Initiate Exercise 3
		exercise3();
		break;

	//Selected Exercise 4
	case 4:
		//Initiate Exercise 4
		exercise4();
		break;
	default:
		break;
	}

	glFlush();
	glutSwapBuffers();

}

void myReshape(int w, int h)
{
	//Create a glOrtho window inside OpenGL that is dependent on the width and height of the Windows window
	// and is 80 by 80 viewport inside OpenGL with culling on objects between -90 and 90

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



void Update(void)
{
	//Constantly redraw
	glutPostRedisplay();
}


void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("Project 1");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_STRIP);

	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutIdleFunc(Update);

	create_menus();

	glutSpecialFunc(SpecialKeyHandler);
	glutKeyboardFunc(NormalKeyHandler);
	glutMouseFunc(myMouse);
	glutMotionFunc(myDrag);
	glutMainLoop();
}