// DrawDemo.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
#include <iostream>
#include "glut.h"//OpenGL头文件
#pragma comment(lib,"glut32.lib")//OpenGL库

int windowWidth = 440, windowHeight = 400;//窗口长、宽
const int x_cone = 1;
const int x_cylinder = 2;
const int y_cone = 3;
const int y_cylinder = 4;
const int z_cone = 5;
const int z_cylinder = 6;
const GLint pickSize = 512;//拾取缓冲区的大小
GLfloat m_tranlate[3] = { 0, 0, -10 };//用于平移，对应x轴、y轴、z轴平移量，用W、A、S、D按键控制
GLfloat m_rorate[2] = { 0, 0 };//用于旋转，分别是绕x轴和y轴旋转的角度，用鼠标左键控制
GLfloat m_scale = 1.0;//用于缩放，用鼠标中间滚轮控制
GLint m_MouseDownPT[2] = { 0, 0 };//记录鼠标坐标点，用于控制旋转角度
bool m_bMouseDown = false;//记录鼠标左键是否按下，按下为true，未按下为false
bool m_editor = false;//记录是否进入编辑模式，true为是，false为否
bool m_operator[3] = { false ,false ,false };//记录鼠标左键是否选中操作器的x、y、z轴

void ProcessPicks(GLint nPicks, GLuint pickBuffer[]);//打印拾取信息

//重新绘制
void Display()
{
	glClearColor(1, 1, 1, 0);//设置背景颜色为白色，完全透明
	glEnable(GL_LIGHT0);//启动0号灯
	glEnable(GL_LIGHTING);//开启光照
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//擦除背景色和深度缓存
	glPushMatrix();//压栈
	glTranslatef(m_tranlate[0], m_tranlate[1], m_tranlate[2]);//平移(x,y,z)坐标
	glRotatef(m_rorate[0], 1, 0, 0);//绕x轴旋转
	glRotatef(m_rorate[1], 0, 1, 0);//绕y轴旋转
	glScalef(m_scale, m_scale, m_scale);//缩放(x,y,z)坐标
	if (m_editor == false)
	{
		glutSolidCube(1.0);//绘制一个实心立方体，参数1.0指的是几何体大小
	}
	else
	{
		glutWireCube(1.0);//绘制一个空心立方体，参数1.0指的是几何体大小
	}
	if (m_editor == true)
	{
		GLUquadric *gluQ = gluNewQuadric();//生成一个二次曲面对象
		//绘制x轴操作器
		if (m_operator[0] == true)//若选中x轴操作器
		{
			glEnable(GL_COLOR_MATERIAL);
			glColor4d(0.0, 0.0, 0.0, 0.0);//设置为黑色
		}
		else
		{
			glColor4d(1.0, 1.0, 1.0, 0.0);//否则设置为白色
		}
		glPushName(x_cylinder);
		gluCylinder(gluQ, 0.05, 0.05, 1.5, 20, 20);//绘制一个圆柱
		glPopName();
		glTranslatef(0, 0, 1.5);
		glPushName(x_cone);
		glutSolidCone(0.07, 0.2, 20, 20);//绘制一个实心圆锥
		glPopName();
		glTranslatef(0, 0, -1.5);
		glRotatef(90, 0, 1, 0);//绕y轴旋转90°
		//绘制y轴操作器
		if (m_operator[1] == true)//若选中y轴操作器
		{
			glEnable(GL_COLOR_MATERIAL);
			glColor4d(0.0, 0.0, 0.0, 0.0);//设置为黑色
		}
		else
		{
			glColor4d(1.0, 1.0, 1.0, 0.0);//否则设置为白色
		}
		glPushName(y_cylinder);
		gluCylinder(gluQ, 0.05, 0.05, 1.5, 20, 20);//绘制一个圆柱
		glPopName();
		glTranslatef(0, 0, 1.5);
		glPushName(y_cone);
		glutSolidCone(0.07, 0.2, 20, 20);//绘制一个实心圆锥
		glPopName();
		glTranslatef(0, 0, -1.5);
		glRotatef(270, 1, 0, 0);//绕x轴旋转270°
		//绘制z轴操作器
		if (m_operator[2] == true)//若选中z轴操作器
		{
			glEnable(GL_COLOR_MATERIAL);
			glColor4d(0.0, 0.0, 0.0, 0.0);//设置为黑色
		}
		else
		{
			glColor4d(1.0, 1.0, 1.0, 0.0);//否则设置为白色
		}
		glPushName(z_cylinder);
		gluCylinder(gluQ, 0.05, 0.05, 1.5, 20, 20);//绘制一个圆柱
		glPopName();
		glTranslatef(0, 0, 1.5);
		glPushName(z_cone);
		glutSolidCone(0.07, 0.2, 20, 20);//绘制一个实心圆锥
		glPopName();
		glTranslatef(0, 0, -1.5);
	}
	glPopMatrix();//出栈
	glutSwapBuffers();//交互前后缓冲
}

//窗口大小改变事件  
void ReshapeEvent(int width, int height)
{
	glViewport(0, 0, width, height);//视口在屏幕的大小位置 
	glMatrixMode(GL_PROJECTION);//投影矩阵
	glLoadIdentity();//矩阵单位
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);//设置投影矩阵
	glMatrixMode(GL_MODELVIEW);//模型矩阵
	glLoadIdentity();//单位矩阵化
	glEnable(GL_DEPTH_TEST);//启动深度检测
}

//空闲事件处理事件
void IdleEvent()
{
	glutPostRedisplay();//刷新函数
}

//键盘事件
void KeyboardEvent(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'W':
	case 'w':
		m_tranlate[1] += 0.1;//向上移动
		break;
	case 'S':
	case 's':
		m_tranlate[1] -= 0.1;//向下移动
		break;
	case 'A':
	case 'a':
		m_tranlate[0] -= 0.1;//向左移动
		break;
	case 'D':
	case 'd':
		m_tranlate[0] += 0.1;//向右移动
		break;
	}
}

//鼠标事件
void MouseEvent(int button, int state, int x, int y)
{
	static GLuint pickBuffer[pickSize];
	GLint nPicks, viewport[4];
	//鼠标缩放
	if (state == GLUT_UP && button == GLUT_WHEEL_UP)//滚轮往上滚动，放大
	{
		m_scale += 0.1;
	}
	else if (state == GLUT_UP && button == GLUT_WHEEL_DOWN)//滚轮往下滚动，缩小
	{
		m_scale -= 0.1;
		if (m_scale < 0.1)
		{
			m_scale = 0.1;
		}
	}
	//GLUT_DOWN：鼠标键被按下，GLUT_UP：鼠标键被松开
	//GLUT_LEFT_BUTTON：鼠标左键被按下或松开，GLUT_MIDDLE_BUTTON：鼠标中键被按下或松开，GLUT_RIGHT_BUTTON：鼠标右键被按下或松开
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)//鼠标左键被按下
	{
		if (m_editor == true)//若在编辑模式中，按下鼠标左键为拉动操作器作平移操作
		{
			glSelectBuffer(pickSize, pickBuffer);//设置选择缓冲区
			glMatrixMode(GL_PROJECTION);//切换到投影矩阵
			glPushMatrix();//将当前的投影矩阵复制一个并压入堆栈，以便恢复
			glLoadIdentity();
			glGetIntegerv(GL_VIEWPORT, viewport);//获得当前窗口显示区域的参数
			glRenderMode(GL_SELECT);//切换到选择模式
			glInitNames();//初始化名字堆栈
			gluPickMatrix(GLdouble(x), GLdouble(viewport[3] - y + viewport[1]), 2.0, 2.0, viewport);//定义一个2.0*2.0的选择区域
			gluPerspective(35.0, (GLfloat)viewport[2] / (GLfloat)viewport[3], 1.0, 200.0);
			Display();//在选择模式下重绘图元
			//获得选择集并输出
			nPicks = glRenderMode(GL_RENDER);//返回点击记录数
			ProcessPicks(nPicks, pickBuffer);//输出选择结果
			glMatrixMode(GL_PROJECTION);//切换到投影矩阵
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}
		else//若不在编辑模式中，按下鼠标左键为旋转
		{
			m_bMouseDown = true;//按下鼠标左键
			m_MouseDownPT[0] = x;//记录当前x坐标，以左上角为原点
			m_MouseDownPT[1] = y;//记录当前y坐标，以左上角为原点
		}
	}
	else
	{
		m_bMouseDown = false;//鼠标左键弹起，结束旋转
		//鼠标左键弹起，操作器停止响应
		m_operator[0] = false;
		m_operator[1] = false;
		m_operator[2] = false;
	}
}

//按下鼠标按钮移动鼠标事件
void MotionEvent(int x, int y)
{
	if (m_bMouseDown == true)//如果鼠标左键被按下
	{
		if (m_editor == true)//若在编辑模式中，按下鼠标左键为拉动操作器作平移操作
		{

		}
		else//若不在编辑模式中，按下鼠标左键为旋转
		{
			//通过滑动鼠标改变旋转的角度
			m_rorate[0] += y - m_MouseDownPT[1];
			m_rorate[1] += x - m_MouseDownPT[0];
			m_MouseDownPT[0] = x;//记录当前x坐标
			m_MouseDownPT[1] = y;//记录当前y坐标
		}
	}
}

//检测鼠标进入或离开窗口
void MouseEntry(int state)
{
	//*state有两个值：GLUT_LEFT表明离开窗口，GLUT_ENTERED表明进入窗口
}

//点击菜单响应事件
void MenuEvent(int choose)
{
	switch (choose)
	{
		//复位：把旋转平移缩放的值恢复为初始值
	case 1:
		//用于平移，对应x轴、y轴、z轴平移量，用W、A、S、D按键控制
		m_tranlate[0] = 0;
		m_tranlate[1] = 0;
		m_tranlate[2] = -10;
		//用于旋转，分别是绕x轴和y轴旋转的角度，用鼠标左键控制
		m_rorate[0] = 0;
		m_rorate[1] = 0;
		//用于缩放，用鼠标中间滚轮控制
		m_scale = 1.0;
		//记录鼠标坐标点，用于控制旋转角度
		m_MouseDownPT[0] = 0;
		m_MouseDownPT[1] = 0;
		//记录鼠标左键是否按下，按下为true，未按下为false
		m_bMouseDown = false;
		break;
	case 2:
		//打开操作器，进入编辑模式
		m_editor = true;
		break;
	case 3:
		//关闭操作器，退出编辑模式
		m_editor = false;
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, (char**)argv);//对GLUT进行初始化 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//初始化设置模式：双缓冲，使用RGB颜色，深度缓存区
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - windowWidth) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - windowHeight) / 2);//初始化窗口位置为屏幕中央
	glutInitWindowSize(windowWidth, windowHeight);//初始化窗口大小
	glutCreateWindow("DrawDemo");//创建窗口，设置窗口名称为DrawDemo
	glutDisplayFunc(Display);//重新绘制事件
	//注册回调函数
	glutKeyboardFunc(KeyboardEvent);//键盘事件
	glutMouseFunc(MouseEvent);//鼠标事件
	glutReshapeFunc(ReshapeEvent);//窗口大小发生变化事件
	glutMotionFunc(MotionEvent);//按下鼠标按钮移动鼠标事件
	glutIdleFunc(IdleEvent);//空闲事件处理事件
	glutEntryFunc(MouseEntry);//检测鼠标进入或离开窗口
	{
		glutCreateMenu(MenuEvent);//创建菜单
		glutAddMenuEntry("复位", 1);//菜单项1
		glutAddMenuEntry("编辑", 2);//菜单项2
		glutAddMenuEntry("退出编辑", 3);//菜单项3
		glutAttachMenu(GLUT_RIGHT_BUTTON);//鼠标右键按下弹出菜单
	}
	glutMainLoop();//调用已注册的回调函数

	return 0;
}

//打印拾取信息
void ProcessPicks(GLint nPicks, GLuint pickBuffer[])
{
	printf("这次点击了%d个物体\n", nPicks);
	for (int i = 0; i < nPicks; i++)
	{
		GLuint nameNum = *pickBuffer;
		printf("选中的数目为%d个：\n", *pickBuffer);
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
				m_operator[0] = true;//鼠标左键选中操作器x轴
				printf("已选中x轴操作器\n");
			}
			if (*pickBuffer == y_cone || *pickBuffer == y_cylinder)
			{
				m_operator[1] = true;//鼠标左键选中操作器y轴
				printf("已选中y轴操作器\n");
			}
			if (*pickBuffer == z_cone || *pickBuffer == z_cylinder)
			{
				m_operator[2] = true;//鼠标左键选中操作器z轴
				printf("已选中z轴操作器\n");
			}
			pickBuffer++;
		}
	}
}