// Pool Game.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<stdlib.h>
#include<glut.h>
#include<math.h>
#include"simulation.h"
#include <string>
using std::string;
//level variables
int level = 0;

//Putter variables
float gPutterAngle = 0.0;
float gPutterPower = 0.25;
bool gPutterControl[4] = {false,false,false,false};
float gPutterAngleSpeed = 2.0f; //radians per second
float gPutterPowerSpeed = 0.25f;
float gPutterPowerMax = 0.75;
float gPutterPowerMin = 0.1;
float gPutterBallFactor = 8.0;
bool gDoPutter = true;

//turn based
int current_player = 0;//which ball the Putter is drawn at. Plus players turn.
int scoredPutters = 0;
bool scored[2] = { false, false };

//scoring
int playerscore[2] = { 0,0 };

//camera variables
vec3 gCamPos(0.0,0.7,2.1);
vec3 gCamLookAt(0.0,0.0,0.0);
bool gCamRotate = true;
float gCamRotSpeed = 0.2;
float gCamMoveSpeed = 0.5;
bool gCamL = false;
bool gCamR = false;
bool gCamU = false;
bool gCamD = false;
bool gCamZin = false;
bool gCamZout = false;

//rendering options
#define DRAW_SOLID	(0)

void DoCamera(int ms)
{
	
	static const vec3 up(0.0,1.0,0.0);

	if(gCamRotate)
	{
		if(gCamL)
		{
			vec3 camDir = (gCamLookAt - gCamPos).Normalised();
			vec3 localL = up.Cross(camDir);
			vec3 inc = (localL* ((gCamRotSpeed*ms)/1000.0) );
			gCamLookAt = gCamPos + camDir + inc;
		}
		if(gCamR)
		{
			vec3 camDir = (gCamLookAt - gCamPos).Normalised();
			vec3 localR = up.Cross(camDir);
			vec3 inc = (localR* ((gCamRotSpeed*ms)/1000.0) );
			gCamLookAt = gCamPos + camDir - inc;
		}
		if(gCamU)
		{
			vec3 camDir = (gCamLookAt - gCamPos).Normalised();
			vec3 localR = camDir.Cross(up);
			vec3 localUp = localR.Cross(camDir);
			vec3 inc = (localUp* ((gCamMoveSpeed*ms)/1000.0) );
			gCamLookAt = gCamPos + camDir + inc;
		}
		if(gCamD)
		{
			vec3 camDir = (gCamLookAt - gCamPos).Normalised();
			vec3 localR = camDir.Cross(up);
			vec3 localUp = localR.Cross(camDir);
			vec3 inc = (localUp* ((gCamMoveSpeed*ms)/1000.0) );
			gCamLookAt = gCamPos + camDir - inc;
		}		
	}
	else
	{
		if(gCamL)
		{
			vec3 camDir = (gCamLookAt - gCamPos).Normalised();
			vec3 localL = up.Cross(camDir);
			vec3 inc = (localL* ((gCamMoveSpeed*ms)/1000.0) );
			gCamPos += inc;
			gCamLookAt += inc;
		}
		if(gCamR)
		{
			vec3 camDir = (gCamLookAt - gCamPos).Normalised();
			vec3 localR = camDir.Cross(up);
			vec3 inc = (localR* ((gCamMoveSpeed*ms)/1000.0) );
			gCamPos += inc;
			gCamLookAt += inc;
		}
		if(gCamU)
		{
			vec3 camDir = (gCamLookAt - gCamPos).Normalised();
			vec3 localR = camDir.Cross(up);
			vec3 localUp = localR.Cross(camDir);
			vec3 inc = (localUp* ((gCamMoveSpeed*ms)/1000.0) );
			gCamPos += inc;
			gCamLookAt += inc;
		}
		if(gCamD)
		{
			vec3 camDir = (gCamLookAt - gCamPos).Normalised();
			vec3 localR = camDir.Cross(up);
			vec3 localDown = camDir.Cross(localR);
			vec3 inc = (localDown* ((gCamMoveSpeed*ms)/1000.0) );
			gCamPos += inc;
			gCamLookAt += inc;
		}
	}

	if(gCamZin)
	{
		vec3 camDir = (gCamLookAt - gCamPos).Normalised();
		vec3 inc = (camDir* ((gCamMoveSpeed*ms)/1000.0) );
		gCamPos += inc;
		gCamLookAt += inc;
	}
	if(gCamZout)
	{
		vec3 camDir = (gCamLookAt - gCamPos).Normalised();
		vec3 inc = (camDir* ((gCamMoveSpeed*ms)/1000.0) );
		gCamPos -= inc;
		gCamLookAt -= inc;
	}
}

void RenderText(void) {
	string first = "null", last = "null";
	if (playerscore[0] <= playerscore[1])
	{
		first = "Green: " + std::to_string(playerscore[0]);
		last = "Blue: " + std::to_string(playerscore[1]);
	}
	else
	{
		first = "Blue: " + std::to_string(playerscore[1]);
		last = "Green: " + std::to_string(playerscore[0]);
	}
	string temp = "Score | " + first + " " + last;
	unsigned char score[100]; 
	for (int i = 0; i < temp.length(); i++)
	{
		score[i] = (unsigned char)temp[i];
	}

	float x = .5;
	glRasterPos2f(-1, 1.5);
	glColor3f(1., 0., 0.);
	int len = temp.length();
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, score[i]);
	}
	return;
}
void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderText();
	glLoadIdentity();
	//set camera
	gluLookAt(gCamPos(0), gCamPos(1), gCamPos(2), gCamLookAt(0), gCamLookAt(1), gCamLookAt(2), 0.0f, 1.0f, 0.0f);

	//Check current player not out of bounds
	if (scored[0] == true)
	{
		current_player = 1;
	}
	else if (scored[1] == true)
	{
		current_player = 0;
	}

	if (current_player > 1)
	{
		current_player = 0;
	}


	//draw the ball	
	for(int i=0;i<NUM_BALLS;i++)
	{
		//check if ball at 'I' has scored. If not run content
		if (scored[i] == false)
		{
			//set ball colour
			if (i == 0)
			{
				glColor3f(0.0, 1.0, 0.0);
			}
			else
			{
				glColor3f(0.0, 0.0, 1.0);
			}
			//find the difference between the hole position and the ball position
			vec2 diff = gTable.balls[i].position - gTable.holes[0].position;
			//check if in range
			if (diff.Magnitude() < 0.075)
			{
				//set the ball scored as true
				scored[i] = true;
				scoredPutters++;
				//set do putter to true
				gDoPutter = true;

				//stop ball moving
				gTable.balls[i].velocity(0) = 0;
				gTable.balls[i].velocity(1) = 0;
			}

			glPushMatrix();
			glTranslatef(gTable.balls[i].position(0), (BALL_RADIUS / 2.0), gTable.balls[i].position(1));
			#if DRAW_SOLID
			glutSolidSphere(gTable.balls[i].radius, 32, 32);
			#else
			glutWireSphere(gTable.balls[i].radius, 12, 12);
			#endif
			glPopMatrix();
		}
	}

	//reset play field
	if (scoredPutters == 2)
	{
		for (int i = 0; i < NUM_BALLS; i++)
		{
			scored[i] = false;
			gTable.balls[i].position(0) = 0;

			gTable.balls[i].position(1) = 0;

			gTable.balls[i].velocity(0) = 0;

			gTable.balls[i].velocity(1) = 0;

			gTable.balls[i].in_hole = 0;
		}
		//call setup functions to reset play field
		gTable.SetupCushions();
		gTable.SetupHoles();
		gCamPos = (0.0, 0.7, 2.1) + (gTable.tx, 0.0f, gTable.tz);
		scoredPutters = 0;
	}

	glColor3f(1.0,1.0,1.0);


	//draw the holes
	glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i < NUM_HOLES; i++)
	{
		glPushMatrix();
		glTranslatef(gTable.holes[i].position(0), (BALL_RADIUS / 2.0), gTable.holes[i].position(1));
		#if DRAW_SOLID
		glutSolidSphere(gTable.holes[i].radius, 32, 32);
		#else
		glutWireSphere(gTable.holes[i].radius, 12, 12);
		#endif
		glPopMatrix();
		glColor3f(1.0, 0.0, 0.0);
	}
	glColor3f(1.0, 1.0, 1.0);

	//draw the table
	for(int i=0;i<NUM_CUSHIONS;i++)
	{	
		glBegin(GL_LINE_LOOP);
		glVertex3f (gTable.cushions[i].vertices[0](0), 0.0, gTable.cushions[i].vertices[0](1));
		glVertex3f (gTable.cushions[i].vertices[0](0), 0.1, gTable.cushions[i].vertices[0](1));
		glVertex3f (gTable.cushions[i].vertices[1](0), 0.1, gTable.cushions[i].vertices[1](1));
		glVertex3f (gTable.cushions[i].vertices[1](0), 0.0, gTable.cushions[i].vertices[1](1));
		glEnd();
	}

	/*for (int i = 0; i < gTable.parts.num; i++)
	{
		glColor3f(1.0, 0.0, 0.0);
		glPushMatrix();
		glTranslatef(gTable.parts.particles[i]->position(0), gTable.parts.particles[i]->position(1), gTable.parts.particles[i]->position(2));
#if DRAW_SOLID
		glutSolidSphere(0.002f, 32, 32);
#else
		glutWireSphere(0.002f, 12, 12);
#endif
		glPopMatrix();
	}*/

	//draw the Putter
	if(gDoPutter)
	{
		glBegin(GL_LINES);
		float Putterx = sin(gPutterAngle) * gPutterPower;
		float Putterz = cos(gPutterAngle) * gPutterPower;
		glColor3f(1.0,0.0,0.0);
		glVertex3f (gTable.balls[current_player].position(0), (BALL_RADIUS/2.0f), gTable.balls[current_player].position(1));
		glVertex3f ((gTable.balls[current_player].position(0)+Putterx), (BALL_RADIUS/2.0f), (gTable.balls[current_player].position(1)+Putterz));
		glColor3f(1.0,1.0,1.0);
		glEnd();
	}
	
	//glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

void SpecKeyboardFunc(int key, int x, int y) 
{
	switch(key)
	{
		case GLUT_KEY_LEFT:
		{
			gPutterControl[0] = true;
			break;
		}
		case GLUT_KEY_RIGHT:
		{
			gPutterControl[1] = true;
			break;
		}
		case GLUT_KEY_UP:
		{
			gPutterControl[2] = true;
			break;
		}
		case GLUT_KEY_DOWN:
		{
			gPutterControl[3] = true;
			break;
		}
	}
}

void SpecKeyboardUpFunc(int key, int x, int y) 
{
	switch(key)
	{
		case GLUT_KEY_LEFT:
		{
			gPutterControl[0] = false;
			break;
		}
		case GLUT_KEY_RIGHT:
		{
			gPutterControl[1] = false;
			break;
		}
		case GLUT_KEY_UP:
		{
			gPutterControl[2] = false;
			break;
		}
		case GLUT_KEY_DOWN:
		{
			gPutterControl[3] = false;
			break;
		}
	}
}

void KeyboardFunc(unsigned char key, int x, int y) 
{
	switch(key)
	{
	case(13):
		{
			if(gDoPutter)
			{
				playerscore[current_player]++;
				vec2 imp(	(-sin(gPutterAngle) * gPutterPower * gPutterBallFactor),
							(-cos(gPutterAngle) * gPutterPower * gPutterBallFactor));
				gTable.balls[current_player++].ApplyImpulse(imp);
			}
			break;
		}
	case(27):
		{
			for(int i=0;i<NUM_BALLS;i++)
			{
				gTable.balls[i].Reset();
			}
			break;
		}
	case(32):
		{
			gCamRotate = false;
			break;
		}
	case('z'):
		{
			gCamL = true;
			break;
		}
	case('c'):
		{
			gCamR = true;
			break;
		}
	case('s'):
		{
			gCamU = true;
			break;
		}
	case('x'):
		{
			gCamD = true;
			break;
		}
	case('f'):
		{
			gCamZin = true;
			break;
		}
	case('v'):
		{
			gCamZout = true;
			break;
		}
	}

}

void KeyboardUpFunc(unsigned char key, int x, int y) 
{
	switch(key)
	{
	case(32):
		{
			gCamRotate = true;
			break;
		}
	case('z'):
		{
			gCamL = false;
			break;
		}
	case('c'):
		{
			gCamR = false;
			break;
		}
	case('s'):
		{
			gCamU = false;
			break;
		}
	case('x'):
		{
			gCamD = false;
			break;
		}
	case('f'):
		{
			gCamZin = false;
			break;
		}
	case('v'):
		{
			gCamZout = false;
			break;
		}
	}
}

void ChangeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0) h = 1;
	float ratio = 1.0* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,0.2,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0.0,0.7,2.1, 0.0,0.0,0.0, 0.0f,1.0f,0.0f);
	gluLookAt(gCamPos(0),gCamPos(1),gCamPos(2),gCamLookAt(0),gCamLookAt(1),gCamLookAt(2),0.0f,1.0f,0.0f);
}

void InitLights(void)
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	GLfloat light_ambient[] = { 2.0, 2.0, 2.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);
}



void UpdateScene(int ms) 
{
	if(gTable.AnyBallsMoving()==false) gDoPutter = true;
	else gDoPutter = false;

	if(gDoPutter)
	{
		if(gPutterControl[0]) gPutterAngle -= ((gPutterAngleSpeed * ms)/1000);
		if(gPutterControl[1]) gPutterAngle += ((gPutterAngleSpeed * ms)/1000);
		if (gPutterAngle <0.0) gPutterAngle += TWO_PI;
		if (gPutterAngle >TWO_PI) gPutterAngle -= TWO_PI;

		if(gPutterControl[2]) gPutterPower += ((gPutterPowerSpeed * ms)/1000);
		if(gPutterControl[3]) gPutterPower -= ((gPutterPowerSpeed * ms)/1000);
		if(gPutterPower > gPutterPowerMax) gPutterPower = gPutterPowerMax;
		if(gPutterPower < gPutterPowerMin) gPutterPower = gPutterPowerMin;
	}

	DoCamera(ms);

	gTable.Update(ms);

	glutTimerFunc(SIM_UPDATE_MS, UpdateScene, SIM_UPDATE_MS);
	glutPostRedisplay();
}

int _tmain(int argc, _TCHAR* argv[])
{
	gTable.SetupCushions();
	gTable.SetupHoles();
	gCamPos = (0.0, 0.7, 2.1) + (gTable.tx, 0.0f, gTable.tz);
	glutInit(&argc, ((char **)argv));
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE| GLUT_RGBA);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(1000,700);
	//glutFullScreen();
	glutCreateWindow("CMP9133M WIL12446446");
	#if DRAW_SOLID
	InitLights();
	#endif
	glutDisplayFunc(RenderScene);
	glutTimerFunc(SIM_UPDATE_MS, UpdateScene, SIM_UPDATE_MS);
	glutReshapeFunc(ChangeSize);
	glutIdleFunc(RenderScene);
	
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(KeyboardFunc);
	glutKeyboardUpFunc(KeyboardUpFunc);
	glutSpecialFunc(SpecKeyboardFunc);
	glutSpecialUpFunc(SpecKeyboardUpFunc);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}
