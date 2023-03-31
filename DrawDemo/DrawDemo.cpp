// DrawDemo.cpp : �������̨Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include <iostream>
#include "glut.h"//OpenGLͷ�ļ�
#pragma comment(lib,"glut32.lib")//OpenGL��

int windowWidth = 440, windowHeight = 400;//���ڳ�����
const int x_cone = 1;
const int x_cylinder = 2;
const int y_cone = 3;
const int y_cylinder = 4;
const int z_cone = 5;
const int z_cylinder = 6;
const GLint pickSize = 512;//ʰȡ�������Ĵ�С
GLfloat m_tranlate[3] = { 0, 0, -10 };//����ƽ�ƣ���Ӧx�ᡢy�ᡢz��ƽ��������W��A��S��D��������
GLfloat m_rorate[2] = { 0, 0 };//������ת���ֱ�����x���y����ת�ĽǶȣ�������������
GLfloat m_scale = 1.0;//�������ţ�������м���ֿ���
GLint m_MouseDownPT[2] = { 0, 0 };//��¼�������㣬���ڿ�����ת�Ƕ�
bool m_bMouseDown = false;//��¼�������Ƿ��£�����Ϊtrue��δ����Ϊfalse
bool m_editor = false;//��¼�Ƿ����༭ģʽ��trueΪ�ǣ�falseΪ��
bool m_operator[3] = { false ,false ,false };//��¼�������Ƿ�ѡ�в�������x��y��z��

void ProcessPicks(GLint nPicks, GLuint pickBuffer[]);//��ӡʰȡ��Ϣ

//���»���
void Display()
{
	glClearColor(1, 1, 1, 0);//���ñ�����ɫΪ��ɫ����ȫ͸��
	glEnable(GL_LIGHT0);//����0�ŵ�
	glEnable(GL_LIGHTING);//��������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//��������ɫ����Ȼ���
	glPushMatrix();//ѹջ
	glTranslatef(m_tranlate[0], m_tranlate[1], m_tranlate[2]);//ƽ��(x,y,z)����
	glRotatef(m_rorate[0], 1, 0, 0);//��x����ת
	glRotatef(m_rorate[1], 0, 1, 0);//��y����ת
	glScalef(m_scale, m_scale, m_scale);//����(x,y,z)����
	if (m_editor == false)
	{
		glutSolidCube(1.0);//����һ��ʵ�������壬����1.0ָ���Ǽ������С
	}
	else
	{
		glutWireCube(1.0);//����һ�����������壬����1.0ָ���Ǽ������С
	}
	if (m_editor == true)
	{
		GLUquadric *gluQ = gluNewQuadric();//����һ�������������
		//����x�������
		if (m_operator[0] == true)//��ѡ��x�������
		{
			glEnable(GL_COLOR_MATERIAL);
			glColor4d(0.0, 0.0, 0.0, 0.0);//����Ϊ��ɫ
		}
		else
		{
			glColor4d(1.0, 1.0, 1.0, 0.0);//��������Ϊ��ɫ
		}
		glPushName(x_cylinder);
		gluCylinder(gluQ, 0.05, 0.05, 1.5, 20, 20);//����һ��Բ��
		glPopName();
		glTranslatef(0, 0, 1.5);
		glPushName(x_cone);
		glutSolidCone(0.07, 0.2, 20, 20);//����һ��ʵ��Բ׶
		glPopName();
		glTranslatef(0, 0, -1.5);
		glRotatef(90, 0, 1, 0);//��y����ת90��
		//����y�������
		if (m_operator[1] == true)//��ѡ��y�������
		{
			glEnable(GL_COLOR_MATERIAL);
			glColor4d(0.0, 0.0, 0.0, 0.0);//����Ϊ��ɫ
		}
		else
		{
			glColor4d(1.0, 1.0, 1.0, 0.0);//��������Ϊ��ɫ
		}
		glPushName(y_cylinder);
		gluCylinder(gluQ, 0.05, 0.05, 1.5, 20, 20);//����һ��Բ��
		glPopName();
		glTranslatef(0, 0, 1.5);
		glPushName(y_cone);
		glutSolidCone(0.07, 0.2, 20, 20);//����һ��ʵ��Բ׶
		glPopName();
		glTranslatef(0, 0, -1.5);
		glRotatef(270, 1, 0, 0);//��x����ת270��
		//����z�������
		if (m_operator[2] == true)//��ѡ��z�������
		{
			glEnable(GL_COLOR_MATERIAL);
			glColor4d(0.0, 0.0, 0.0, 0.0);//����Ϊ��ɫ
		}
		else
		{
			glColor4d(1.0, 1.0, 1.0, 0.0);//��������Ϊ��ɫ
		}
		glPushName(z_cylinder);
		gluCylinder(gluQ, 0.05, 0.05, 1.5, 20, 20);//����һ��Բ��
		glPopName();
		glTranslatef(0, 0, 1.5);
		glPushName(z_cone);
		glutSolidCone(0.07, 0.2, 20, 20);//����һ��ʵ��Բ׶
		glPopName();
		glTranslatef(0, 0, -1.5);
	}
	glPopMatrix();//��ջ
	glutSwapBuffers();//����ǰ�󻺳�
}

//���ڴ�С�ı��¼�  
void ReshapeEvent(int width, int height)
{
	glViewport(0, 0, width, height);//�ӿ�����Ļ�Ĵ�Сλ�� 
	glMatrixMode(GL_PROJECTION);//ͶӰ����
	glLoadIdentity();//����λ
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);//����ͶӰ����
	glMatrixMode(GL_MODELVIEW);//ģ�;���
	glLoadIdentity();//��λ����
	glEnable(GL_DEPTH_TEST);//������ȼ��
}

//�����¼������¼�
void IdleEvent()
{
	glutPostRedisplay();//ˢ�º���
}

//�����¼�
void KeyboardEvent(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'W':
	case 'w':
		m_tranlate[1] += 0.1;//�����ƶ�
		break;
	case 'S':
	case 's':
		m_tranlate[1] -= 0.1;//�����ƶ�
		break;
	case 'A':
	case 'a':
		m_tranlate[0] -= 0.1;//�����ƶ�
		break;
	case 'D':
	case 'd':
		m_tranlate[0] += 0.1;//�����ƶ�
		break;
	}
}

//����¼�
void MouseEvent(int button, int state, int x, int y)
{
	static GLuint pickBuffer[pickSize];
	GLint nPicks, viewport[4];
	//�������
	if (state == GLUT_UP && button == GLUT_WHEEL_UP)//�������Ϲ������Ŵ�
	{
		m_scale += 0.1;
	}
	else if (state == GLUT_UP && button == GLUT_WHEEL_DOWN)//�������¹�������С
	{
		m_scale -= 0.1;
		if (m_scale < 0.1)
		{
			m_scale = 0.1;
		}
	}
	//GLUT_DOWN�����������£�GLUT_UP���������ɿ�
	//GLUT_LEFT_BUTTON�������������»��ɿ���GLUT_MIDDLE_BUTTON������м������»��ɿ���GLUT_RIGHT_BUTTON������Ҽ������»��ɿ�
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)//������������
	{
		if (m_editor == true)//���ڱ༭ģʽ�У�����������Ϊ������������ƽ�Ʋ���
		{
			glSelectBuffer(pickSize, pickBuffer);//����ѡ�񻺳���
			glMatrixMode(GL_PROJECTION);//�л���ͶӰ����
			glPushMatrix();//����ǰ��ͶӰ������һ����ѹ���ջ���Ա�ָ�
			glLoadIdentity();
			glGetIntegerv(GL_VIEWPORT, viewport);//��õ�ǰ������ʾ����Ĳ���
			glRenderMode(GL_SELECT);//�л���ѡ��ģʽ
			glInitNames();//��ʼ�����ֶ�ջ
			gluPickMatrix(GLdouble(x), GLdouble(viewport[3] - y + viewport[1]), 2.0, 2.0, viewport);//����һ��2.0*2.0��ѡ������
			gluPerspective(35.0, (GLfloat)viewport[2] / (GLfloat)viewport[3], 1.0, 200.0);
			Display();//��ѡ��ģʽ���ػ�ͼԪ
			//���ѡ�񼯲����
			nPicks = glRenderMode(GL_RENDER);//���ص����¼��
			ProcessPicks(nPicks, pickBuffer);//���ѡ����
			glMatrixMode(GL_PROJECTION);//�л���ͶӰ����
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}
		else//�����ڱ༭ģʽ�У�����������Ϊ��ת
		{
			m_bMouseDown = true;//����������
			m_MouseDownPT[0] = x;//��¼��ǰx���꣬�����Ͻ�Ϊԭ��
			m_MouseDownPT[1] = y;//��¼��ǰy���꣬�����Ͻ�Ϊԭ��
		}
	}
	else
	{
		m_bMouseDown = false;//���������𣬽�����ת
		//���������𣬲�����ֹͣ��Ӧ
		m_operator[0] = false;
		m_operator[1] = false;
		m_operator[2] = false;
	}
}

//������갴ť�ƶ�����¼�
void MotionEvent(int x, int y)
{
	if (m_bMouseDown == true)//���������������
	{
		if (m_editor == true)//���ڱ༭ģʽ�У�����������Ϊ������������ƽ�Ʋ���
		{

		}
		else//�����ڱ༭ģʽ�У�����������Ϊ��ת
		{
			//ͨ���������ı���ת�ĽǶ�
			m_rorate[0] += y - m_MouseDownPT[1];
			m_rorate[1] += x - m_MouseDownPT[0];
			m_MouseDownPT[0] = x;//��¼��ǰx����
			m_MouseDownPT[1] = y;//��¼��ǰy����
		}
	}
}

//�����������뿪����
void MouseEntry(int state)
{
	//*state������ֵ��GLUT_LEFT�����뿪���ڣ�GLUT_ENTERED�������봰��
}

//����˵���Ӧ�¼�
void MenuEvent(int choose)
{
	switch (choose)
	{
		//��λ������תƽ�����ŵ�ֵ�ָ�Ϊ��ʼֵ
	case 1:
		//����ƽ�ƣ���Ӧx�ᡢy�ᡢz��ƽ��������W��A��S��D��������
		m_tranlate[0] = 0;
		m_tranlate[1] = 0;
		m_tranlate[2] = -10;
		//������ת���ֱ�����x���y����ת�ĽǶȣ�������������
		m_rorate[0] = 0;
		m_rorate[1] = 0;
		//�������ţ�������м���ֿ���
		m_scale = 1.0;
		//��¼�������㣬���ڿ�����ת�Ƕ�
		m_MouseDownPT[0] = 0;
		m_MouseDownPT[1] = 0;
		//��¼�������Ƿ��£�����Ϊtrue��δ����Ϊfalse
		m_bMouseDown = false;
		break;
	case 2:
		//�򿪲�����������༭ģʽ
		m_editor = true;
		break;
	case 3:
		//�رղ��������˳��༭ģʽ
		m_editor = false;
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, (char**)argv);//��GLUT���г�ʼ�� 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//��ʼ������ģʽ��˫���壬ʹ��RGB��ɫ����Ȼ�����
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - windowWidth) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - windowHeight) / 2);//��ʼ������λ��Ϊ��Ļ����
	glutInitWindowSize(windowWidth, windowHeight);//��ʼ�����ڴ�С
	glutCreateWindow("DrawDemo");//�������ڣ����ô�������ΪDrawDemo
	glutDisplayFunc(Display);//���»����¼�
	//ע��ص�����
	glutKeyboardFunc(KeyboardEvent);//�����¼�
	glutMouseFunc(MouseEvent);//����¼�
	glutReshapeFunc(ReshapeEvent);//���ڴ�С�����仯�¼�
	glutMotionFunc(MotionEvent);//������갴ť�ƶ�����¼�
	glutIdleFunc(IdleEvent);//�����¼������¼�
	glutEntryFunc(MouseEntry);//�����������뿪����
	{
		glutCreateMenu(MenuEvent);//�����˵�
		glutAddMenuEntry("��λ", 1);//�˵���1
		glutAddMenuEntry("�༭", 2);//�˵���2
		glutAddMenuEntry("�˳��༭", 3);//�˵���3
		glutAttachMenu(GLUT_RIGHT_BUTTON);//����Ҽ����µ����˵�
	}
	glutMainLoop();//������ע��Ļص�����

	return 0;
}

//��ӡʰȡ��Ϣ
void ProcessPicks(GLint nPicks, GLuint pickBuffer[])
{
	printf("��ε����%d������\n", nPicks);
	for (int i = 0; i < nPicks; i++)
	{
		GLuint nameNum = *pickBuffer;
		printf("ѡ�е���ĿΪ%d����\n", *pickBuffer);
		pickBuffer++;
		printf("minZ is %f\n", (float)*pickBuffer / 0x7FFFFFFF);
		pickBuffer++;
		printf("maxZ is %f\n", (float)*pickBuffer / 0x7FFFFFFF);
		pickBuffer++;
		for (int i = 0; i < (int)nameNum; i++)
		{
			printf("name is %d\n", *pickBuffer);
			if (*pickBuffer == x_cone || *pickBuffer == x_cylinder)
			{
				m_operator[0] = true;//������ѡ�в�����x��
				printf("��ѡ��x�������\n");
			}
			if (*pickBuffer == y_cone || *pickBuffer == y_cylinder)
			{
				m_operator[1] = true;//������ѡ�в�����y��
				printf("��ѡ��y�������\n");
			}
			if (*pickBuffer == z_cone || *pickBuffer == z_cylinder)
			{
				m_operator[2] = true;//������ѡ�в�����z��
				printf("��ѡ��z�������\n");
			}
			pickBuffer++;
		}
	}
}