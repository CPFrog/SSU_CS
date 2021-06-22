#define _CRT_SECURE_NO_WARNINGS
#include <glut.h>
#include <cmath>
#include <iostream>
#include <GLAUX.H>
#define GL_PI 3.1415f

float zoom = 1.1;
float scale = 1.0;
float angle[3];

int mouse_state, mouse_button;
GLfloat mouse_X, mouse_Y;
GLUquadricObj* q = gluNewQuadric();
GLUquadricObj* head = gluNewQuadric();
GLUquadricObj* body = gluNewQuadric();
GLuint tex[2]; // �Ӹ�, �� �ؽ���

void myDisplay();
void myMouse(int button, int state, int x, int y);
void myKeyboard(unsigned char key, int x, int y);
void myMotion(int x, int y);

int LoadGLTextures();
void DrawArm();
void DrawLeg();
void HalfSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat radius);
AUX_RGBImageRec* LoadBMPFile(const char* filename);

//���� ��ƾ
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("20163340 ������");
	glClearColor(0, 0, 0, 0.0);

	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);

	LoadGLTextures();
	glutMainLoop();
	return 0;
}

/*�⺻���� ������Ʈ ��� �� PostRedisplay ȣ��� ��� ȣ��Ǵ� �Լ�.*/
void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, 0.1, 0.0, 0.0, 0.0, -1.0, 0, 1, 0);

	glRotatef(angle[0], 1.0f, 0.0f, 0.0f);
	glRotatef(angle[1], 0.0f, 1.0f, 0.0f);
	glRotatef(angle[2], 0.0f, 0.0f, 1.0f);
	glScalef(scale, scale, scale);

	//������ �𵨸� ����

	//������ �ٸ�
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslatef(0.12, -.42, 0);
	glRotatef(-90, 0, 1, 0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	DrawLeg();
	glPopMatrix();

	//���� �ٸ�
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-0.12, -.42, 0);
	glRotatef(-90, 0, 1, 0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	DrawLeg();
	glPopMatrix();

	//������ ��
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(0.15, 0.05, 0.0);
	glRotatef(135, 1.0, 0.0, 1.0);
	DrawArm();
	glPopMatrix();

	//���� ��
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-0.15, .05, 0);
	glRotatef(135, 1.0, 0.0, -1.0);
	DrawArm();
	glPopMatrix();

	//��
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	glColor3f(1, 1, 1);
	glTranslatef(0.0, -.23, 0.0);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(body, 0.22, 0.15, 0.27, 25, 10);
	glPopMatrix();

	//�Ӹ�
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glTranslatef(0.0, 0.3, 0);
	glRotatef(-100, 1, 0, 0);
	glColor3f(1, 1, 1);
	gluSphere(head, 0.3, 25, 25);
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

//�ؽ��� �̹����� �����ϴ� �Լ�
int LoadGLTextures() {
	gluQuadricDrawStyle(body, GLU_FILL);
	gluQuadricTexture(body, GL_TRUE);
	gluQuadricDrawStyle(head, GLU_FILL);
	gluQuadricTexture(head, GL_TRUE);

	AUX_RGBImageRec* textureimg[2];
	memset(textureimg, 0, sizeof(void*));

	if ((textureimg[0] = LoadBMPFile("face.bmp")) && (textureimg[1] = LoadBMPFile("body.bmp"))) {
		glGenTextures(2, &tex[0]);

		for (int i = 0; i < 2; i++) {
			glBindTexture(GL_TEXTURE_2D, tex[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureimg[i]->sizeX,
				textureimg[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, textureimg[i]->data);
		}
	}
	else return FALSE;

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	return TRUE;
}

//�ؽ��� �̹����� ���� BMP ������ �о���� �Լ�
AUX_RGBImageRec* LoadBMPFile(const char* filename) {
	FILE* hFile = NULL;
	if (!filename) return NULL;

	hFile = fopen(filename, "r");
	if (hFile) {
		fclose(hFile);
		return auxDIBImageLoad(filename);
	}

	return NULL;
}

//���콺 Ŭ�� �� ���� ���콺�� ��ġ�� �����ϴ� �Լ�
void myMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouse_state = state;
			mouse_button = button;
			mouse_X = x;
			mouse_Y = y;
		}
		else if (state == GLUT_UP) {
			mouse_state = -1;
		}
		else return;
	}
	else return;
	glutPostRedisplay();
}

//Ű���� ���� �� ȭ�� ó�� �Լ�
void myKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
	case 'A':
		scale *= zoom;
		break;

	case 'z':
	case 'Z':
		scale /= zoom;
		break;
	}

	glutPostRedisplay();
}

//�巡�׶�� �Ǵ� ���� �� ó��
void myMotion(int x, int y) {
	if (mouse_button == GLUT_LEFT_BUTTON && mouse_state == GLUT_DOWN) {
		angle[1] -= (mouse_X - x) / 5.0;
		angle[0] -= (mouse_Y - y) / 5.0;
		glutPostRedisplay();
		mouse_X = x;
		mouse_Y = y;
	}
}

//�� �׷��ִ� �Լ�
void DrawArm() {
	gluQuadricDrawStyle(q, GLU_FILL);
	gluQuadricNormals(q, GLU_SMOOTH);

	glColor3f(1, 1, 1);
	gluCylinder(q, 0.06, 0.06, .18, 25, 10);
	glTranslatef(0, 0, .18);
	glutSolidSphere(0.06, 25, 25);
}

//�ٸ� �׷��ִ� �Լ�
void DrawLeg() {
	gluQuadricDrawStyle(q, GLU_FILL);
	gluQuadricNormals(q, GLU_SMOOTH);

	glColor3f(1, 1, 1);
	gluCylinder(q, 0.07, 0.07, .20, 25, 10);
	glRotatef(90, 1, 0, 0);
	glTranslatef(0.04, 0, 0);
	HalfSphere(0, 0, 0, 0.09);
}

/* ���� ���·� ���� �ݱ��� �׷��ִ� �Լ�
* x: �ݱ� �߽��� x��ǥ
* y: �ݱ� �߽��� y��ǥ
* z: �ݱ� �߽��� z��ǥ
* radius : �ݱ��� ������
	*/
void HalfSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat radius) {
	GLfloat angley;
	GLfloat nexty;
	GLfloat anglex;

	glColor3f(1, 1.0, 1);
	glBegin(GL_QUAD_STRIP);
	for (angley = 0.0f; angley <= (0.5f * GL_PI); angley += (0.5f * GL_PI) / 8) {
		y = radius * sin(angley); //y�� �� ���
		nexty = angley + ((0.5f * GL_PI) / 8.0f); //���� angley�� ���� 
		for (anglex = 0.0f; anglex < (2.0f * GL_PI); anglex += (GL_PI / 8.0f)) {
			x = radius * cos(angley) * sin(anglex);
			z = radius * cos(angley) * cos(anglex);
			glNormal3f(-cos(angley) * sin(anglex), -sin(angley), -cos(angley) * cos(anglex)); //�ݱ��� �������� normal ���� ����
			glVertex3f(x, y, z);
			x = radius * cos(nexty) * sin(anglex);
			z = radius * cos(nexty) * cos(anglex);
			glNormal3f(-cos(nexty) * sin(anglex), -sin(nexty), -cos(nexty) * cos(anglex));
			glVertex3f(x, radius * sin(nexty), z);
		}
	}
	glEnd();
}