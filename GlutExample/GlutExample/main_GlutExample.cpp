#include <iostream>
#include "GL/freeglut.h"


GLuint g_window_w = 500;
GLuint g_window_h = 500;


const int g_num_vertices = 4;
const int g_num_triangles = 2;


GLuint g_vao_id;
GLuint g_vbo_position_id;
GLuint g_vbo_normal_id;
GLuint g_vbo_color_id;
GLuint g_index_buffer_id;


// Index Buffer Object
GLuint index_buffer_id;

void Reshape(int w, int h);
void Display();
void Keyboard(unsigned char key, int x, int y);
void SpeicalKeyboard(int key, int x, int y);
void Mouse(int button, int state, int x, int y);
void MouseMotion(int x, int y);
void Timer(int value);



int main(int argc, char** argv)
{


	///////////////////////////////////////////////////////////////////////
	//// Create and Initialize Window
	glutInit(&argc, argv);
	
	// specifies display mode
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// specifies the size, in pixels, of your window
	glutInitWindowSize(g_window_w, g_window_h);

	//glutInitContextVersion(3, 2);
	//glutInitContextProfile(GLUT_CORE_PROFILE);

	// creates a window with an OpenGL context
	glutCreateWindow("Computer Graphics");
	
	// Callback functions
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpeicalKeyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);
	glutTimerFunc(1000, Timer, 0);



	glutMainLoop();

	return 0;
}



/**
Display: �׸��� �׸��� ���� ȣ��Ǵ� callback �Լ�.
ȭ�鿡 �׸��� ���� �׷��� �ʿ䰡 ���� ������ �ڵ����� �ݺ��ؼ� ���� �� ȣ��ȴ�.
(��. �ٸ� �����쿡 ���� ȭ���� �Ϻκ��� �������ٰ� �ٽ� ��Ÿ���� ���.)
�ݺ������� ȣ��ȴٴ� ���� ����ϰ�, ���ʿ��� ������ ���� �̰��� �־�� �ȵȴ�.
*/
void Display()
{
	std::cout << "Display() is called." << std::endl;
}



/**
Timer: glutTimerFunc�� ȣ�� �� �� ���� �ð� �Ŀ� ȣ��Ǵ� callback �Լ�.
*/
void Timer(int value)
{
	std::cout << "Timer() is called." << std::endl;
	

	// 1000�и�������(1��) �Ŀ� Timer �Լ��� �ٽ� ȣ��Ƿη� �Ѵ�.
	glutTimerFunc(1000, Timer, 0);
}



/**
Reshape: �������� ũ�Ⱑ ������ ������ �ڵ����� ȣ��Ǵ� callback �Լ�.
w�� h�� ���� ������ �������� ���� ũ��� ���� ũ�⸦ ��Ÿ���� (�ȼ� ����).
*/
void Reshape(int w, int h)
{
	std::cout << "Reshape() is called. (width=" << w << ", height=" << h << ")" << std::endl;


}









/////////////////////////////////////////////////////////////////////////////////////////
/// Keyboard and Mouse Input



/**
Mouse: ���콺 ��ư�� �Էµ� ������ �ڵ����� ȣ��Ǵ� �Լ�.
�Ķ������ �ǹ̴� ������ ����.
1. button: ���� ��ư�� ����
  GLUT_LEFT_BUTTON - ���� ��ư
  GLUT_RIGHT_BUTTON - ������ ��ư
  GLUT_MIDDLE_BUTTON - ��� ��ư (�� ��ư�� �������� ��)
  3 - ���콺 �� (���� ���� ���� ����).
  4 - ���콺 �� (���� �Ʒ��� ���� ����).
2. state: ��ư �׼� ����
  GLUT_DOWN - ���� ����
  GLUT_UP - ���� ����
3. x,y: �ش� �̺�Ʈ �߻� ��, ���콺 �������� ��ǥ��.
*/
void Mouse(int button, int state, int x, int y)
{


	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		std::cout << "mouse left button pushed : (" << x << ", " << y << ")" << std::endl;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		std::cout << "mouse left button released : (" << x << ", " << y << ")" << std::endl;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		std::cout << "mouse right button pushed : (" << x << ", " << y << ")" << std::endl;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		std::cout << "mouse right button released : (" << x << ", " << y << ")" << std::endl;
	}
	else if (button == 3)
	{
		std::cout << "mouse wheel up" << std::endl;
	}
	else if (button == 4)
	{
		std::cout << "mouse wheel down" << std::endl;
	}

}





/**
MouseMotion: ���콺 ��ư�� ������ ���¿��� �����Ͱ� ������ �� (�巡���� ��)���� �ڵ����� ȣ��Ǵ� �Լ�.
 �Ķ���� x,y�� ���콺 �������� ��ǥ��.
*/
void MouseMotion(int x, int y)
{
	std::cout << "mouse dragged : (" << x << ", " << y << ")" << std::endl;

}



/**
Keyboard: Ű���� �Է��� ���� ������ �ڵ����� ȣ��Ǵ� �Լ�.
�Ķ���� key �� ������ Ű������ ���ڰ��� ��Ÿ����.
�Ķ���� x,y�� ���� ���콺 �������� ��ǥ���� ��Ÿ����.

�� ���������� Ű���� 'r' �� �������� �׸��� ������Ʈ �� ���� ��û�Ѵ�.
*/
void Keyboard(unsigned char key, int x, int y)
{
	std::cout << "key '" << key << "' pushed : (" << x << ", " << y << ")" << std::endl;
	
	if (key == 'r')
	{
		// glutPostRedisplay�� ������ ���� �ð� �ȿ� ��ü �׸��� �ٽ� �׸� ���� �ý��ۿ� ��û�Ѵ�.
		// ��������� Display() �Լ��� ȣ���ϰ� �ȴ�.
		glutPostRedisplay();
	}
}




/**
SpeicalKeyboard: ���ڰ��� �ƴ� Ư��Ű�� �Է� �� ������ �ڵ����� ȣ��Ǵ� �Լ�.
�Ķ���� key �� ������ Ű���带 ��Ÿ���� ��ũ�� �� (freeglut_std.h ����).
�Ķ���� x,y�� ���� ���콺 �������� ��ǥ��.
*/
void SpeicalKeyboard(int key, int x, int y)
{
	switch (key)
	{
		std::cout << key << std::endl;
		/* fill here */
	case GLUT_KEY_DOWN:
		std::cout << "down" << std::endl;
		break;

	case GLUT_KEY_UP:
		std::cout << "up" << std::endl;
		break;

	case GLUT_KEY_LEFT:
		std::cout << "left" << std::endl;
		break;

	case GLUT_KEY_RIGHT:
		std::cout << "right" << std::endl;
		break;

	}
}






