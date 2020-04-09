/**
	@mainpage Computer graphics assignment
	OpenGL project : Space Bar
	
	09-04-2020

	Simple OpenGL 3D scene depicting a humble bar in the middle of space

	
	Instructions :
	
	*Go to Files 

	*Click main.cpp
		
	Made by:
	* Hitesh Shetty - 2017A7PS1541H
	* Lovedeep Singh Sidhu -2017A7PS1735H
 
**/

#include <stdio.h>           /// Standard C/C++ Input-Output
#include <math.h>            /// Math Functions
#include <windows.h>         /// Standard Header For MSWindows Applications
#include <gl/glut.h>            /// The GL Utility Toolkit (GLUT) Header
#include <time.h>
#include <windowsx.h>
#include <stdlib.h>
#include "SOIL.h"


// Global Variables
bool g_gamemode;				/// GLUT GameMode ON/OFF
bool g_fullscreen;				/// Fullscreen Mode ON/OFF (When g_gamemode Is OFF)
bool b_culling = false;			
float aspect = 1;
float strafez=0;
int camx=-45,camz=0;
float camy=25;
const float PI = 3.1415926535897932384626433832795028;
const float epsilon = 0.001;
GLuint texid;
int strafex=0;
int strafey=0;
float move=1.2;


//Function Prototypes
void render(void);
void initLights(void);
bool init(void);
void reshape(int w,int h);
void keyboard(unsigned char key, int x, int y);
void special_keys(int a_keys, int x, int y);
void tableLeg(double thick, double len);   
void table(double topWid, double topThick, double legThick, double legLen);
void drawchair(double topWid, double topThick, double legThick, double legLen); 
void chairLeg(double thick, double len); 
static void drawBoxtexture(GLfloat size, GLenum type, double t1,double t2,double t3,double t4,double t5,double t6,double t7,double t8);
void DrawVase(float x, float y, float z, float size);
void DrawRoom(float x, float y, float z, float size);
void makeCylinder(float height, float base1,float base2,int rgb);

void drawchair(double topWid, double topThick, double legThick, double legLen)
{
	glColor3f(.1f, .3f, .3f);//NL chairs color

	glPushMatrix();
	glTranslated(0, legLen, 0);
	glScaled(topWid, topThick, topWid);
	drawBoxtexture(1.0,GL_QUADS,0.65,1,0.65,0.7,1,0.7,1,1); 
	glPopMatrix();

	
	glPushMatrix();
	glTranslated(-1.4,4.5,0);
	glScaled(0.2,topWid,topWid);
	drawBoxtexture(1.0,GL_QUADS,0,0.65,0,0,0.5,0,0.5,0.65);
	glPopMatrix();

	
	double dist = 0.95 * topWid/2.0 - legThick / 2.0; 
	glPushMatrix();
	glTranslated(dist, 0, dist);
	chairLeg(legThick, legLen);
	glTranslated(0, 0, -2*dist);
	chairLeg(legThick, legLen);
	glTranslated(-2*dist, 0, 2*dist);
	chairLeg(legThick, legLen);
	glTranslated(0, 0, -2*dist);
	chairLeg(legThick, legLen);
	glPopMatrix();
	glColor3f(.7f, .7f, .7f);//NL turn it back to normal

}

void chairLeg(double thick, double len){
	glPushMatrix();
	glTranslated(0, len/2, 0);
	glScaled(thick, len, thick);

	drawBoxtexture(1.0,GL_QUADS,0,0.65,0,0,0.6,0,0.6,0.65);
	glPopMatrix();
}

void tableLeg(double thick, double len){
	glPushMatrix();
	glTranslated(0, len/2, 0);
	glScaled(thick, len, thick);

	drawBoxtexture(1.0,GL_QUADS,0.3,1,0.3,0.7,0.6,0.7,0.6,1);
	glPopMatrix();
}

void table(double topWid, double topThick, double legThick, double legLen){
	
	glPushMatrix();
	//glTranslated(-1-move, 0, -3-move);
	DrawVase(0,3,0,0.4);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, legLen, 0);
	glScaled(topWid, topThick, topWid);

	glColor3f(22.2f, 22.2f, .0f);//NL table color
	drawBoxtexture(1.0,GL_QUADS,0,1,0,0.7,0.2,0.7,0.2,1);
	glPopMatrix();

	double dist = 0.95 * topWid/2.0 - legThick / 2.0; 
	glPushMatrix();
	glTranslated(dist, 0, dist);
	tableLeg(legThick, legLen);
	glTranslated(0, 0, -2*dist);
	tableLeg(legThick, legLen);
	glTranslated(-2*dist, 0, 2*dist);
	tableLeg(legThick, legLen);
	glTranslated(0, 0, -2*dist);
	tableLeg(legThick, legLen);
	glPopMatrix();
	move=1.2;
	glColor3f(.7f, .7f, .7f);//NL turn it back to normal
}


void initLights(void) {

	glEnable(GL_LIGHT0);							   // Quick And Dirty Lighting (Assumes Light0 Is Set Up)
	//glEnable(GL_LIGHTING);						       // Enable Lighting      //NT try GL_FRONT_FACE
}
///this function makes the tabletop
static void drawBoxtexture(GLfloat size, GLenum type, double t1,double t2,double t3,double t4,double t5,double t6,double t7,double t8){
	static GLfloat n[6][3] =
	{
		{-1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0},
		{1.0, 0.0, 0.0},
		{0.0, -1.0, 0.0},
		{0.0, 0.0, 1.0},
		{0.0, 0.0, -1.0}
	};
	static GLint faces[6][4] =
	{
		{0, 1, 2, 3},
		{3, 2, 6, 7},
		{7, 6, 5, 4},
		{4, 5, 1, 0},
		{5, 6, 2, 1},
		{7, 4, 0, 3}
	};
	GLfloat v[8][3];
	GLint i;

	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

	for (i = 5; i >= 0; i--) {
		glBegin(type);
		glNormal3fv(&n[i][0]);
		
		glVertex3fv(&v[faces[i][0]][0]);
		
		glVertex3fv(&v[faces[i][1]][0]);
		
		glVertex3fv(&v[faces[i][2]][0]);
		
		glVertex3fv(&v[faces[i][3]][0]);
		glEnd();
	}

}

void DrawVase(float x, float y, float z, float size){

	makeCylinder(0, 7.75, 8, 7);
	//Back side
	glBegin( GL_QUADS );
	glColor3f(.0f, .7f, .0f);//NL turn it back to normal
	glVertex3f(x-size,y,z-size);
	glVertex3f(x+size,y,z-size); 
	glVertex3f(x+size+0.2,y+size*3,z-size-0.2);
	glVertex3f(x-size-0.2,y+size*3,z-size-0.2);
	glEnd();

	//Front side
	glBegin( GL_QUADS );
	glVertex3f(x+size,y,z+size);
	glVertex3f(x+size+0.2,y+size*3,z+size+0.2);
	glVertex3f(x-size-0.2,y+size*3,z+size+0.2);
	glVertex3f(x-size,y,z+size);
	glEnd();

	//Right side
	glBegin( GL_QUADS );
	glVertex3f(x+size,y,z-size);
	glVertex3f(x+size,y,z+size);
	glVertex3f(x+size+0.2,y+size*3,z+size+0.2);	
	glVertex3f(x+size+0.2,y+size*3,z-size-0.2);
	glEnd();

	//Left side
	glBegin( GL_QUADS );
	glVertex3f(x-size,y,z-size);
	glVertex3f(x-size-0.2,y+size*3,z-size-0.2);
	glVertex3f(x-size-0.2,y+size*3,z+size+0.2);	
	glVertex3f(x-size,y,z+size);
	glEnd();
	
	glColor3f(.7f, .7f, .7f);//NL turn it back to normal

}

void DrawRoom(float x, float y, float z, float size) {

	//Back side
	glBegin(GL_QUADS);
	glColor4f(.7f, .7f, .7f,0.4);//NL turn it back to normal
	glVertex3f(x - size, y, z - size);
	glVertex3f(x + size, y, z - size);
	glVertex3f(x + size, y + size , z - size );
	glVertex3f(x - size , y + size , z - size );
	glEnd();
	
	/*
	//Front side
	glBegin(GL_QUADS);
	glVertex3f(x + size, y, z + size);
	glVertex3f(x + size + 0.2, y + size * 3, z + size + 0.2);
	glVertex3f(x - size - 0.2, y + size * 3, z + size + 0.2);
	glVertex3f(x - size, y, z + size);
	glEnd();
	*/

	//Left side
	glBegin(GL_QUADS);
	glColor4f(.5f, .5f, .5f,0.4);//NL turn it back to normal
	glVertex3f(x - size, y, z - size);
	glVertex3f(x - size , y + size , z - size );
	glVertex3f(x - size , y + size , z + size );
	glVertex3f(x - size, y, z + size);
	glEnd();

	//Bottom side
	glColor4f(.1f, .1f, .1f,0.4);//NL turn floor black
	glBegin(GL_QUADS);
	glVertex3f(x - size, y, z - size);
	glVertex3f(x - size, y, z + size);
	glVertex3f(x + size, y, z + size);
	glVertex3f(x + size, y, z - size);
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);//NL turn it back to normal
}

void makeCylinder(float height, float base1,float base2,int rgb)//rgb is for color syntax is rrggbb rr =r.rf, dont add any unnecessary zeroes in msb side.
{
    GLUquadric *obj = gluNewQuadric();
    //gluQuadricDrawStyle(obj, GLU_LINE); //NL kinda like the texture
	//printf("%d %d %d %d\n", rgb,(rgb / 10000), (rgb / 100) % 100, rgb % 100);
    glColor3f((rgb/100000.0),((rgb/100)%100)/10.0,(rgb%100)/10.0);
    glPushMatrix();
    glRotatef(-90, 1.0,0.0,0.0);
    gluCylinder(obj, base1,base2, height, 20,20);
    glPopMatrix();
	glColor3f(.9f, .9f, .9f);//NL change color of a specific object by changing current colour 
    //glutSwapBuffers();  //NL caused the scene to flicker so commented out
}


// Our GL Specific Initializations. Returns true On Success, false On Fail.
bool init(void)
{	glGenTextures(1,&texid);
glEnable( GL_TEXTURE_2D );
texid = 1;// SOIL_load_OGL_texture("final.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
if(!texid){
	printf("can't find texture");
	exit(-1);
}
glBindTexture( GL_TEXTURE_2D,texid);
glPixelStorei(GL_UNPACK_ALIGNMENT, 1);             // Pixel Storage Mode To Byte Alignment
glEnable(GL_TEXTURE_2D);                           // Enable Texture Mapping 
glClearColor(0.05f, 0.0f, 0.25f, 0.5f);			   //Background //NL null void ka color
glClearDepth(1.0f);								   // Depth Buffer Setup
glDepthFunc(GL_LEQUAL);							   // The Type Of Depth Testing To Do
glEnable(GL_DEPTH_TEST);						   // Enables Depth Testing
glShadeModel(GL_SMOOTH);						   // Enable Smooth Shading
initLights();
glEnable(GL_COLOR_MATERIAL);					   // Enable Material Coloring
glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Hint for nice perspective interpolation
//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);	// Set the color tracking for both faces for both the ambient and diffuse components
glEnable(GL_NORMALIZE);
//glFrontFace(GL_CCW);                               //Counter Clock Wise definition of the front and back side of faces
glCullFace(GL_BACK);                               //Hide the back side


return true;
}

void positionCamera(){


	glMatrixMode(GL_PROJECTION);     // Select The Projection Matrix

	glLoadIdentity();                // Reset The Projection Matrix
	gluPerspective(55.0f, aspect, 0.1, 1000.0);  //NL para: perspective distortion amount, aspect??, how close things start redering from camera,how far  
	gluLookAt(0.0f,0.0f,245,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f);

	//camera transformations go here

	glMatrixMode(GL_MODELVIEW);      // Select The Modelview Matrix



}

// Our Reshaping Handler (Required Even In Fullscreen-Only Modes)
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	// Calculate The Aspect Ratio And Set The Clipping Volume
	if (h == 0) h = 1;
	aspect = (float)w/(float)h;
	positionCamera();
	glMatrixMode(GL_MODELVIEW);      // Select The Modelview Matrix
	glLoadIdentity(); // Reset The Modelview Matrix
	initLights();
}

///Keyboard Handler : Normal
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
		strafez+=1;

		break;
	case 's':
		strafez-=1;

		break;
	case 'a':
		strafex++;
		break;
	case 'd':
		strafex--;
		break;
	case '\'':
		strafey--;
		break;
	case '\/':
		strafey++;
		break;
	case 27:        // When Escape Is Pressed...
		exit(0);    // Exit The Program
		break;      // Ready For Next Case
	default:
		break;
	}
	glutPostRedisplay();
}

///Keyboard Handler : Special Keys 
void special_keys(int a_keys, int x, int y)
{

	switch (a_keys) {
	case GLUT_KEY_F1:
		// We Can Switch Between Windowed Mode And Fullscreen Mode Only
		if (!g_gamemode) {
			g_fullscreen = !g_fullscreen;       // Toggle g_fullscreen Flag
			if (g_fullscreen) glutFullScreen(); // We Went In Fullscreen Mode
			else glutReshapeWindow(500, 500);   // We Went In Windowed Mode
		}
		break;
	case GLUT_KEY_UP:
		camy+=1;
		break;
	case GLUT_KEY_DOWN:
		camy-=1;
		break;
	case GLUT_KEY_LEFT:
		camx+=1;
		break;
	case GLUT_KEY_RIGHT:
		camx-=1;
		break;
	default:
		;
	}

	glutPostRedisplay();
}

/// Main Function 
int main(int argc, char** argv)
{	
	glutInit(&argc, argv);                           // GLUT Initializtion
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE); // (CHANGED)|
	if (g_gamemode) {
		glutGameModeString("1024x768:32");            // Select 1024x768 In 32bpp Mode
		if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
			glutEnterGameMode();                     // Enter Full Screen
		else
			g_gamemode = false;                     // Cannot Enter Game Mode, Switch To Windowed
	}
	if (!g_gamemode) {
		glutInitWindowPosition(100, 100);
		glutInitWindowSize(500, 500);                  // Window Size If We Start In Windowed Mode
		glutCreateWindow("COMPGRAPHA2"); // Window Title 
	}
	if (!init()) {                                   // Our Initialization
		fprintf(stderr,"Initialization failed.");
		return -1;
	}
	glutDisplayFunc(render);                     // Register The Display Function
	glutReshapeFunc(reshape);                    // Register The Reshape Handler
	glutKeyboardFunc(keyboard);                  // Register The Keyboard Handler
	glutSpecialFunc(special_keys);               // Register Special Keys Handler
	glutIdleFunc(NULL);                        	 // We Do Rendering In Idle Time
	glutMainLoop();                              // Go To GLUT Main Loop
	return 0;
}

// Our Rendering Is Done Here
void render(void)   
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer

	// Do we have culling enabled?
	if (b_culling == true)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

	glLoadIdentity();

	//camera movement
	glTranslated(strafex,strafey,strafez);
	glRotatef(camy,1,0,0);
	glRotatef(camx,0,1,0);
    
	//NL lamp?
	glTranslated(0, 0, -10);
	makeCylinder(10, 0.1,0,0);
	makeCylinder(2, 2,0,20);
	makeCylinder(0.5,0.2,0,600060 );
	glTranslated(0, 0, 10);
	DrawRoom(0, -10, -10, 25);

	//NL stars
	for (int i = 0; i < 500; i++) {
		glPushMatrix();
		srand(i*i+1);
		glTranslated( 300*rand()/(RAND_MAX+1.0)-150, 300*rand()/(RAND_MAX + 1.0) -150 ,300*rand()/(RAND_MAX + 1.0)-150);
		glRotatef(45, 0, 0, 1);
		glScaled(0.5, 0.5, 0.5);
		drawBoxtexture(1.0, GL_QUADS, 0, 0, 0, 0, 0, 0, 0, 0);
		glPopMatrix();
	}

	//NL fireflies
	//NL green dynamic
	for (int i = 0; i < 20; i++) {
		glPushMatrix();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glTranslated(50 * rand() / (RAND_MAX + 1.0) - sin(time(0)*rand())-25, 50 * rand() / (RAND_MAX + 1.0) - sin(rand()*time(0)) - 25, 50 * rand() / (RAND_MAX + 1.0) - sin(time(0)*rand()) - 25);
		//glRotatef(time(0)%12*15, 0, 0, 1);
		glScaled(0.35, 0.35, 0.35);
		glColor4f(0.6, 1, 0.2, 0.5+0.5*sin((time(0))/2.0));
		drawBoxtexture(1.0, GL_QUADS, 0, 0, 0, 0, 0, 0, 0, 0);
		glPopMatrix();
	}
	//NL yellow static
	for (int i = 0; i < 15; i++) {
		glPushMatrix();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glTranslated(30 * rand() / (RAND_MAX + 1.0) - 15, 30 * rand() / (RAND_MAX + 1.0) - 15, 30 * rand() / (RAND_MAX + 1.0) - 15);
		glRotatef(45, 0, 0, 1);
		glScaled(0.35, 0.35, 0.35);
		//printf("%f\n", 0.5 + 0.5 * sin((time(0))/2.0) );
		glColor4f(256, 256, 0, 0.5+0.5*(sin((time(NULL)))));
		drawBoxtexture(1.0, GL_QUADS, 0, 0, 0, 0, 0, 0, 0, 0);
		glPopMatrix();
	}


	//NL Tables
	glTranslated(-5, -10, 0.4);
	table(6, 0.2, 0.2, 3);
	glPushMatrix();//NL dublicates top of stack matrix and push it so the following ops till pop can be done on dupe
	glTranslated(-5,0,0);
	drawchair(3,0.1,0.1,3);
	glTranslated(5,0,0);
	glTranslated(0,0,-5);
	glRotatef(-90,0,1,0);
	drawchair(3,0.1,0.1,3);
	glPopMatrix();

	glTranslated(8, 0, 0.4);//NL next table to the right
	table(6, 0.2, 0.2, 3);
	glPushMatrix();
	glTranslated(0,0,-5);
	glRotatef(-90,0,1,0);
	drawchair(3,0.1,0.1,3);
	glRotatef(90,0,1,0);//NL go back to normal rotation 
	glTranslated(5,0,5);
	glRotatef(180,0,1,0);
	drawchair(3,0.1,0.1,3);
	glPopMatrix();

	glTranslated(0, 0, -20);
	table(6, 0.2, 0.2, 3);
	glPushMatrix();
	glTranslated(0,0,-5);
	glRotatef(-90,0,1,0);
	drawchair(3,0.1,0.1,3);
	glRotatef(90,0,1,0);
	glTranslated(5,0,5);
	glRotatef(180,0,1,0);
	drawchair(3,0.1,0.1,3);
	glPopMatrix();

	glTranslated(-8, 0, 0);
	table(6, 0.2, 0.2, 3);
	glPushMatrix();
	glTranslated(-5,0,0);
	drawchair(3,0.1,0.1,3);
	glTranslated(5,0,0);
	glTranslated(0,0,-5);
	glRotatef(-90,0,1,0);
	drawchair(3,0.1,0.1,3);
	glPopMatrix();
	/*
	glTranslated(0, 0, -20);
	table(6, 0.2, 0.2, 3);
	glPushMatrix();
	glTranslated(-5,0,0);
	drawchair(3,0.1,0.1,3);
	glTranslated(5,0,0);
	glTranslated(0,0,-5);
	glRotatef(-90,0,1,0);
	drawchair(3,0.1,0.1,3);
	glPopMatrix();

	glTranslated(8, 0, 0);
	table(6, 0.2, 0.2, 3);
	glPushMatrix();
	glTranslated(0,0,-5);
	glRotatef(-90,0,1,0);
	drawchair(3,0.1,0.1,3);
	glRotatef(90,0,1,0);
	glTranslated(5,0,5);
	glRotatef(180,0,1,0);
	drawchair(3,0.1,0.1,3);
	glPopMatrix();
	*/
	//DrawRoom(5, 0, 9, 25); //NL room

	positionCamera();

	
	// Swap The Buffers To Make Our Rendering Visible
	glutSwapBuffers();
	glutPostRedisplay(); //animation
	
}
