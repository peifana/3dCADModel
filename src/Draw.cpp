#include"Draw.hpp"
#include<gl/glut.h>
#include <GL/freeglut_ext.h>
Solid* solid;
Camera cam;
int mousex, mousey,times=1;
bool mouseLeftDown, mouseRightDown;
double cameraAngleX, cameraAngleY;
double cameraDistanceX, cameraDistanceY;
void KeyBoards(unsigned char key, int x, int y){}
void MouseFunc(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouseLeftDown = true;
		}
		else mouseLeftDown = false;
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouseRightDown = true;
		}
		else mouseRightDown = false;
	}
	mousex = x;
	mousey = y;
}

void ActiveMotionFunc(int x, int y) {
	cameraAngleX = cameraAngleY = 0;
	cameraDistanceX = cameraDistanceY = 0;
	if (mouseLeftDown) {
		cameraAngleX = (x - mousex) * 0.6;
		cameraAngleY = (y - mousey) * 0.6;
		printf("%f,%f",cameraAngleX,cameraAngleY);
	}
	else if (mouseRightDown) {
		cameraDistanceX = (x - mousex) * 0.02;
		cameraDistanceY = (y - mousey) * 0.02;
	}
	mousex = x;
	mousey = y;
	glutPostRedisplay();
}


void Draw(int argc, char** argv, Solid* firstSolid) {
	solid = firstSolid;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(1024, 768);
	glutCreateWindow("CADProject");


	glEnable(GL_DEPTH);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor4f(1, 1, 1, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cam.eyex, cam.eyey,cam.eyez,cam.centerx, cam.centery, cam.centerz, cam.upx, cam.upy, cam.upz);
	glutKeyboardFunc(KeyBoards);
	glutMouseFunc(MouseFunc);
	glutMotionFunc(ActiveMotionFunc);
	glutDisplayFunc(Display);
	glutMainLoop();
}
void CALLBACK tessBeginCB(GLenum which) { glBegin(which); }

void CALLBACK tessEndCB() { glEnd(); }

void CALLBACK tessVertexCB(const GLvoid* data)
{
	const GLdouble* ptr = (const GLdouble*)data;
	glVertex3dv(ptr);
}

void CALLBACK tessErrorCB(GLenum errorCode)
{
	const GLubyte* errorStr;
	errorStr = gluErrorString(errorCode);
}

void DrawLoop(GLUtesselator* tess, Loop* loop)
{
	GLdouble* pos;
	gluTessBeginContour(tess);
	HalfEdge* he = loop->GetFirstHalfEdge();
	Vertex* firstVertex = he->getBeg();
	Point point = firstVertex->Getpos();
	pos = new GLdouble[3]{ point.x,point.y,point.z};
	gluTessVertex(tess, pos, pos);
	while (he->getEnd() != firstVertex)
	{
		he = he->getNextHalfEdge();
		point = he->getBeg()->Getpos();
		pos = new GLdouble[3]{ point.x,point.y,point.z};
		gluTessVertex(tess, pos, pos);
	}
	gluTessEndContour(tess);
}


void Display() {
	glClear(GL_COLOR_BUFFER_BIT);
	GLUtesselator* tess = gluNewTess();
	gluTessCallback(tess, GLU_TESS_BEGIN, (void (CALLBACK*)())tessBeginCB);
	gluTessCallback(tess, GLU_TESS_END, (void (CALLBACK*)())tessEndCB);
	gluTessCallback(tess, GLU_TESS_VERTEX, (void (CALLBACK*)())tessVertexCB);
	gluTessCallback(tess, GLU_TESS_ERROR, (void (CALLBACK*)())tessErrorCB);
	glScalef(times, times, times);
	glTranslatef(cameraDistanceX, cameraDistanceY, 0);
	glRotatef(cameraAngleX, 1, 0, 0);
	glRotatef(cameraAngleY, 0, 1, 0);
	Solid* drawingSolid=solid;
	while (drawingSolid)
	{
		Face* face = drawingSolid->GetFirstFace();
		while (face)
		{
			gluTessBeginPolygon(tess, 0);
			Loop* loop = face->GetouterLoop();
			DrawLoop(tess, loop);
			loop = face->GetInnerLoop();
			while (loop)
			{
				DrawLoop(tess, loop);
				loop = loop->GetNextInnerLoop();
			}
			gluTessEndPolygon(tess);
			face = face->GetnextFace();
		}
		drawingSolid = drawingSolid->GetNextSolid();
	}
	gluDeleteTess(tess);
	glFlush();
	glutSwapBuffers();
}