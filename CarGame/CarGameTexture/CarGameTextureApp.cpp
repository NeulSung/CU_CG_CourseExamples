


#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "CarGameTextureApp.h"
#include "../BaseCodes/Camera.h"
#include "../BaseCodes/InitShader.h"
#include "../BaseCodes/BasicShapeObjs.h"
#include "CarModel.h"
#include "GroundTexture.h"
#include "MoonTexture.h"
#include "RingModel.h"
#include "ProptopModel.h"
#include "PropbottomModel.h"
#include "EmissionLight.h"


// Window and User Interface
static bool g_left_button_pushed;
static bool g_right_button_pushed;
static int g_last_mouse_x;
static int g_last_mouse_y;

extern GLuint g_window_w;
extern GLuint g_window_h;

//////////////////////////////////////////////////////////////////////
// Camera 
//////////////////////////////////////////////////////////////////////
static Camera g_camera;

//////////////////////////////////////////////////////////////////////
//// Define Shader Programs
//////////////////////////////////////////////////////////////////////
GLuint s_program_id;



//////////////////////////////////////////////////////////////////////
//// Animation Parameters
//////////////////////////////////////////////////////////////////////
float g_elaped_time_s = 0.f;	// 


//////////////////////////////////////////////////////////////////////
//// Car Position, Rotation, Velocity
//// �ڵ��� ���� ������.
//////////////////////////////////////////////////////////////////////
glm::vec3 g_car_poisition(0.f, 0.f, 0.f); //��ġ
float g_car_speed = 0;			          // �ӵ� (�ʴ� �̵� �Ÿ�)
float g_car_rotation_y = 0;		          // ���� ���� (y�� ȸ��)
float g_car_angular_speed = 0;	          // ȸ�� �ӵ� (���ӵ� - �ʴ� ȸ�� ��)


//////////////////////////////////////////////////////////////////////
//// Light Parameters
//////////////////////////////////////////////////////////////////////
static int g_sunlight_mode = 0;

//////////////////////////////////////////////////////////////////////
//// Things Parameters
//////////////////////////////////////////////////////////////////////
glm::vec3 g_ring_position(0.f, 1.5f, -4.25f);
float g_ring_rotation_y = 0;
float g_ring_angular_speed = glm::radians(1.5f);

bool g_rotation = true;

bool g_ground_shiness = false;

float shininess_n = 50.f;


/**
InitOpenGL: ���α׷� �ʱ� �� ������ ���� ���� �� �� ȣ��Ǵ� �Լ�. (main �Լ� ����)
OpenGL�� ������ �ʱ� ���� ���α׷��� �ʿ��� �ٸ� �ʱ� ���� �����Ѵ�.
�������, VAO�� VBO�� ���⼭ ������ �� �ִ�.
*/
void InitOpenGL()
{
	s_program_id = CreateFromFiles("../Shaders/v_shader_texture.glsl", "../Shaders/f_shader_texture.glsl");
	glUseProgram(s_program_id);

	glViewport(0, 0, (GLsizei)g_window_w, (GLsizei)g_window_h);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


	// Initial State of Camera
	// ī�޶� �ʱ� ��ġ �����Ѵ�.
	g_camera.lookAt(glm::vec3(3.f, 2.f, 3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	
	// basic meshes
	InitBasicShapeObjs();


	// Car
	//InitCarModel();

	// ��ħ�� ����
	InitProptopModel();
	InitPropbottomModel();

	// �ٴ� VAO ����
	InitGroundTexture();

	// Moon VAO ����
	InitMoonTexture();

	// Ring ����
	InitRingModel();
	InitLightModel();
}







/**
ClearOpenGLResource: ���α׷��� ������ �޸� ������ ���� �� �� ȣ��Ǵ� �Լ�. (main �Լ� ����)
���α׷����� ����� �޸𸮸� ���⿡�� ������ �� �ִ�.
�������, VAO�� VBO�� ���⼭ ���� �� �ִ�.
*/
void ClearOpenGLResource()
{
	// Delete (VAO, VBO)
	DeleteBasicShapeObjs();
	DeleteProptopModel();
	DeletePropbottomModel();
	DeleteGroundTexture();
	DeleteMoonTexture();
	DeleteRingModel();
	DeleteLightModel();
}





/**
Display: ������ ȭ���� ������Ʈ �� �ʿ䰡 ���� �� ȣ��Ǵ� callback �Լ�.

������ �� ���� ����� ������ �ϴ� �ڵ�� �� �Լ� ���� �����ؾ��Ѵ�.
�����찡 ó�� ���� ��, ������ ũ�Ⱑ �ٲ� ��, �ٸ� �����쿡 ���� ȭ���� �Ϻ�
�Ǵ� ��ü�� �������ٰ� �ٽ� ��Ÿ�� �� �� �ý����� �ش� ������ ���� �׸��� ����
������Ʈ�� �ʿ��ϴٰ� �Ǵ��ϴ� ��� �ڵ����� ȣ��ȴ�.
���� ȣ���� �ʿ��� ��쿡�� glutPostRedisplay() �Լ��� ȣ���ϸ�ȴ�.

�� �Լ��� �ҽÿ� ����ϰ� ȣ��ȴٴ� ���� ����ϰ�, ������ ���� ��ȭ�� ������
1ȸ�� �ڵ�� �������� �� �Լ� �ۿ� �����ؾ��Ѵ�. Ư�� �޸� �Ҵ�, VAO, VBO ����
���� �ϵ���� ������ �õ��ϴ� �ڵ�� Ư���� ������ ���ٸ� ���� �� �Լ��� ���Խ�Ű��
�ȵȴ�. ���� ���, �޽� ���� �����ϰ� VAO, VBO�� �����ϴ� �κ��� ���� 1ȸ��
�����ϸ�ǹǷ� main() �Լ� �� �ܺο� �����ؾ��Ѵ�. ���ǵ� �޽� ���� ������ ���ۿ�
�׸����� �����ϴ� �ڵ常 �� �Լ��� �����ϸ� �ȴ�.

����, �� �Լ� ������ ���� �޸� �Ҵ��� �ؾ��ϴ� ��찡 �ִٸ� �ش� �޸𸮴� �ݵ��
�� �Լ��� ������ ���� ���� �ؾ��Ѵ�.

ref: https://www.opengl.org/resources/libraries/glut/spec3/node46.html#SECTION00081000000000000000
*/
void Display()
{
	// ��ü ȭ���� �����.
	// glClear�� Display �Լ� ���� �� �κп��� �� ���� ȣ��Ǿ���Ѵ�.
	if (g_sunlight_mode == 0)
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	if (g_sunlight_mode == 1)
		glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

	// Vertex shader �� matrix �������� location�� �޾ƿ´�.
	int m_proj_loc = glGetUniformLocation(s_program_id, "proj_matrix");
	int m_view_loc = glGetUniformLocation(s_program_id, "view_matrix");
	int m_model_loc = glGetUniformLocation(s_program_id, "model_matrix");

	glUniform1i(glGetUniformLocation(s_program_id, "light_mode"), g_sunlight_mode);
	
	// Projection Transform Matrix ����.
	glm::mat4 projection_matrix = glm::perspective(glm::radians(45.f), (float)g_window_w / g_window_h, 0.01f, 10000.f);
	glUniformMatrix4fv(m_proj_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	// Camera Transform Matrix ����.
	glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, glm::value_ptr(g_camera.GetGLViewMatrix()));

	// texture filtering ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Ring
	{
		glUniform1i(glGetUniformLocation(s_program_id, "flag_texture"), false);
		glBindTexture(GL_TEXTURE_2D, s_program_id);

		glm::mat4 ring_T = glm::rotate(g_ring_rotation_y, glm::vec3(0, 1, 0)) * glm::translate(g_ring_position);
		glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, glm::value_ptr(ring_T));
		DrawRingModel();

		//glm::mat4 ring_T = glm::rotate(g_ring_rotation_y, glm::vec3(0, 1, 0)) * glm::translate(g_ring_position);

		glUniform1i(glGetUniformLocation(s_program_id, "inverse"), true);
		glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, glm::value_ptr(ring_T));
		DrawLightModel();
	}

	glUniform1i(glGetUniformLocation(s_program_id, "inverse"), false);

	// light ����
	{
		glUniform1d(glGetUniformLocation(s_program_id, "light_mode"), g_sunlight_mode);

		// ���� ��� ����, ���� ���� ����.
		glm::vec3 light_position(0.f, 3.f, -5.f);
		//glm::vec3 light_dir(0.f, -1.f, 0.f);
		//light_dir = glm::normalize(light_dir);

		light_position = glm::rotateY(light_position, g_ring_rotation_y);
		glm::vec3 light_dir = glm::vec3(0.f,3.f,0.f)-light_position;

		light_dir = glm::normalize(light_dir);


		// Apply Camera Matrices
		////// *** ���� ī�޶� ������ ����ϱ� ���� view transform ����  ***
		//  light_position�� ��ġ�� ��Ÿ���� ����Ʈ�̹Ƿ� �̵�(Translation)��ȯ�� ����ǵ��� �Ѵ�. (�� ��° ��� 1.f���� ����)
		light_position = glm::vec3(g_camera.GetGLViewMatrix() * glm::vec4(light_position, 1.f));
		//  light_dir�� ������ ��Ÿ���� �����̹Ƿ� �̵�(Translation)��ȯ�� ������� �ʵ��� �Ѵ�. (�� ��° ��� 0.f���� ����)
		light_dir = glm::vec3(g_camera.GetGLViewMatrix() * glm::vec4(light_dir, 0.f));


		int light_position_loc = glGetUniformLocation(s_program_id, "light_position");
		glUniform3f(light_position_loc, light_position[0], light_position[1], light_position[2]);

		int light_dir_loc = glGetUniformLocation(s_program_id, "light_dir");
		glUniform3f(light_dir_loc, light_dir[0], light_dir[1], light_dir[2]);



		// Spot Light ���� ����.
		float light_cos_cutoff = cos(glm::radians(70.f));
		glm::vec3 light_indensity(1.0f, 1.0f, 1.0f);	// Red Light

		int light_cos_cutoff_loc = glGetUniformLocation(s_program_id, "light_cos_cutoff");
		glUniform1f(light_cos_cutoff_loc, light_cos_cutoff);

		int light_intensity_loc = glGetUniformLocation(s_program_id, "I_l");
		glUniform3f(light_intensity_loc, light_indensity[0], light_indensity[1], light_indensity[2]);
	}

	// texture �ٴ� 
	{
		glUniform1i(glGetUniformLocation(s_program_id, "flag_texture"), true);
		glUniform1i(glGetUniformLocation(s_program_id, "isGround"), true);
		glUniform1i(glGetUniformLocation(s_program_id, "isGround_shiness"), g_ground_shiness);

		// Ground�� ���� Phong Shading ���� ���� ���� �����Ѵ�.
		int shininess_loc = glGetUniformLocation(s_program_id, "shininess_n");
		glUniform1f(shininess_loc, shininess_n);

		int K_s_loc = glGetUniformLocation(s_program_id, "K_s");
		glUniform3f(K_s_loc, 0.3f, 0.3f, 0.3f);

		glm::mat4 T0(1.f); // ���� ���
		glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, glm::value_ptr(T0 * glm::translate(glm::vec3(0, -0.44, 0)) * glm::scale(glm::vec3(5.f,1.f,5.f))));
		DrawGroundTexture();

		glUniform1i(glGetUniformLocation(s_program_id, "isGround"), false);

	}

	// Moon
	{
		glUniform1i(glGetUniformLocation(s_program_id, "flag_texture"), true);
		glBindTexture(GL_TEXTURE_2D, s_program_id);

		glm::mat4 moon_T = glm::translate(glm::vec3(0.f, 3.f, 0.f)) * glm::scale(glm::vec3(0.7f, 0.7f, 0.7f));
		glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, glm::value_ptr(moon_T));
		DrawMoonTexture();
	}


	// prop
	{
		glUniform1i(glGetUniformLocation(s_program_id, "flag_texture"), false);
		glBindTexture(GL_TEXTURE_2D, s_program_id);

		glm::mat4 car_T = glm::translate(g_car_poisition);
		glUniformMatrix4fv(m_model_loc, 1, GL_FALSE,  glm::value_ptr(car_T));
		DrawProptopModel();

		//glm::mat4 car_T = glm::translate(g_car_poisition);
		glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, glm::value_ptr(car_T));
		DrawPropbottomModel();
	}

	
	

	

	// flipping the double buffers
	// glutSwapBuffers�� �׻� Display �Լ� ���� �Ʒ� �κп��� �� ���� ȣ��Ǿ���Ѵ�.
	glutSwapBuffers();
}


/**
Timer: ������ �ð� �Ŀ� �ڵ����� ȣ��Ǵ� callback �Լ�.
ref: https://www.opengl.org/resources/libraries/glut/spec3/node64.html#SECTION000819000000000000000
*/
void Timer(int value)
{
	// Timer ȣ�� �ð� ������ �����Ͽ�, ���� Timer�� ȣ��� �ĺ��� ������� �귯�� ����Ѵ�.
	g_elaped_time_s += value/1000.f;


	//// Turn
	//g_car_rotation_y += g_car_angular_speed;

	//// Calculate Velocity
	//glm::vec3 speed_v = glm::vec3(0.f, 0.f, g_car_speed);
	//glm::vec3 velocity = glm::rotateY(speed_v, g_car_rotation_y);	// speed_v �� y���� �������� g_car_rotation_y ��ŭ ȸ���Ѵ�.
	//// Move
	//g_car_poisition += velocity;

	if(g_rotation)
		g_ring_rotation_y += g_ring_angular_speed;


	// glutPostRedisplay�� ������ ���� �ð� �ȿ� ��ü �׸��� �ٽ� �׸� ���� �ý��ۿ� ��û�Ѵ�.
	// ��������� Display() �Լ��� ȣ�� �ȴ�.
	glutPostRedisplay();

	// 1/60 �� �Ŀ� Timer �Լ��� �ٽ� ȣ��Ƿη� �Ѵ�.
	// Timer �Լ� �� ������ �ð� �������� �ݺ� ȣ��ǰ��Ͽ�,
	// �ִϸ��̼� ȿ���� ǥ���� �� �ִ�
	glutTimerFunc((unsigned int)(1000 / 60), Timer, (1000 / 60));
}




/**
Reshape: �������� ũ�Ⱑ ������ ������ �ڵ����� ȣ��Ǵ� callback �Լ�.

@param w, h�� ���� ������ �������� ���� ũ��� ���� ũ�� (�ȼ� ����).
ref: https://www.opengl.org/resources/libraries/glut/spec3/node48.html#SECTION00083000000000000000
*/
void Reshape(int w, int h)
{
	//  w : window width   h : window height
	g_window_w = w;
	g_window_h = h;

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glutPostRedisplay();
}

/**
Keyboard: Ű���� �Է��� ���� ������ �ڵ����� ȣ��Ǵ� �Լ�.
@param key�� ������ Ű������ ���ڰ�.
@param x,y�� ���� ���콺 �������� ��ǥ��.
ref: https://www.opengl.org/resources/libraries/glut/spec3/node49.html#SECTION00084000000000000000

*/
void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		//case 's':
		//	g_car_speed = -0.01f;		// ���� �ӵ� ����
		//	glutPostRedisplay();
		//	break;

		//case 'w':
		//	g_car_speed = 0.01f;		// ���� �ӵ� ����
		//	glutPostRedisplay();
		//	break;

		//case 'a':
		//	g_car_angular_speed = glm::radians( 1.f );		// ��ȸ�� ���ӵ� ����
		//	glutPostRedisplay();
		//	break;

		//case 'd':
		//	g_car_angular_speed = -1 * glm::radians( 1.f );		//  ��ȸ�� ���ӵ� ����
		//	glutPostRedisplay();
		//	break;
	case 'n':
		if (g_sunlight_mode == 1) {
			g_sunlight_mode = 0;
		}
		else {
			g_sunlight_mode = 1;
		}
		glutPostRedisplay();
		break;
	case 'N':
		if (g_sunlight_mode == 1) {
			g_sunlight_mode = 0;
		}
		else {
			g_sunlight_mode = 1;
		}
		glutPostRedisplay();
		break;
	case 'p':
		if (g_rotation)
			g_rotation = false;
		else
			g_rotation = true;
		break;
	case 's':
		if (g_ground_shiness)
			g_ground_shiness = false;
		else
			g_ground_shiness = true;
		break;
	case '.':
		if(shininess_n<=500)
			shininess_n += 10;
		break;
	case ',':
		if (shininess_n >= 0)
			shininess_n -= 10;
		break;
	}

}

/**
KeyboardUp: �������� Ű�� ������ ������ �ڵ����� ȣ��Ǵ� �Լ�.
@param key�� �ش� Ű������ ���ڰ�.
@param x,y�� ���� ���콺 �������� ��ǥ��.
ref: https://www.opengl.org/resources/libraries/glut/spec3/node49.html#SECTION00084000000000000000

*/
void KeyboardUp(unsigned char key, int x, int y)
{
	switch (key)						
	{
	case 's':
		g_car_speed = 0.f;		// ���� �ӵ� ����
		glutPostRedisplay();
		break;

	case 'w':
		g_car_speed = 0.f;		// ���� �ӵ� ����
		glutPostRedisplay();
		break;

	case 'a':
		g_car_angular_speed = 0.f;		// ��ȸ�� ���ӵ� ����
		glutPostRedisplay();
		break;

	case 'd':
		g_car_angular_speed = 0.f;		//  ��ȸ�� ���ӵ� ����
		glutPostRedisplay();
		break;

	}

}



/**
Mouse: ���콺 ��ư�� �Էµ� ������ �ڵ����� ȣ��Ǵ� �Լ�.
�Ķ������ �ǹ̴� ������ ����.
@param button: ���� ��ư�� ����
  GLUT_LEFT_BUTTON - ���� ��ư
  GLUT_RIGHT_BUTTON - ������ ��ư
  GLUT_MIDDLE_BUTTON - ��� ��ư (���� �������� ��)
  3 - ���콺 �� (���� ���� ���� ����).
  4 - ���콺 �� (���� �Ʒ��� ���� ����).
@param state: ���� ����
  GLUT_DOWN - ���� ����
  GLUT_UP - ��������
@param x,y: ������ �Ͼ�� ��, ���콺 �������� ��ǥ��.
*/
void Mouse(int button, int state, int x, int y)
{
	float mouse_xd = (float)x / g_window_w;
	float mouse_yd = 1 - (float)y / g_window_h;
	float last_mouse_xd = (float)g_last_mouse_x / g_window_w;
	float last_mouse_yd = 1 - (float)g_last_mouse_y / g_window_h;


	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		g_left_button_pushed = true;

	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		g_left_button_pushed = false;

	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		g_right_button_pushed = true;

	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
		g_right_button_pushed = false;
	else if (button == 3)
	{
		g_camera.inputMouse(Camera::IN_TRANS_Z, 0, -1, 0.01f);
		glutPostRedisplay();
	}
	else if (button == 4)
	{
		g_camera.inputMouse(Camera::IN_TRANS_Z, 0, 1, 0.01f);
		glutPostRedisplay();
	}

	g_last_mouse_x = x;
	g_last_mouse_y = y;
}





/**
MouseMotion: ���콺 �����Ͱ� ������ ������ �ڵ����� ȣ��Ǵ� �Լ�.
@prarm x,y�� ���� ���콺 �������� ��ǥ���� ��Ÿ����.
*/
void MouseMotion(int x, int y)
{
	float mouse_xd = (float)x / g_window_w;
	float mouse_yd = 1 - (float)y / g_window_h;
	float last_mouse_xd = (float)g_last_mouse_x / g_window_w;
	float last_mouse_yd = 1 - (float)g_last_mouse_y / g_window_h;

	if (g_left_button_pushed)
	{
		g_camera.inputMouse(Camera::IN_ROTATION_Y_UP, last_mouse_xd, last_mouse_yd, mouse_xd, mouse_yd);
		glutPostRedisplay();
	}
	else if (g_right_button_pushed)
	{
		g_camera.inputMouse(Camera::IN_TRANS, last_mouse_xd, last_mouse_yd, mouse_xd, mouse_yd, 0.01f);
		glutPostRedisplay();
	}

	g_last_mouse_x = x;
	g_last_mouse_y = y;
}