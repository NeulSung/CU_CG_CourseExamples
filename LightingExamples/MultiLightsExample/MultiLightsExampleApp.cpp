
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "../BaseCodes/Camera.h"
#include "../BaseCodes/GroundObj2.h"
#include "../BaseCodes/InitShader.h"
#include "../BaseCodes/BasicShapeObjs.h"
#include "MultiLightsExampleApp.h"



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



// Animtion Timer
// �ִϸ��̼� �ð��� ���õ� ������.
static const float FPS = 60.f;
void Timer(int value);
static float g_elapsed_time = 0.0f;  // ���α׷� ���� �������� �귯�� �ð� (�� ����).



/**
InitOpenGL: ���α׷� �ʱ� �� ������ ���� ���� �� �� ȣ��Ǵ� �Լ�. (main �Լ� ����)
OpenGL�� ������ �ʱ� ���� ���α׷��� �ʿ��� �ٸ� �ʱ� ���� �����Ѵ�.
�������, VAO�� VBO�� ���⼭ ������ �� �ִ�.
*/
void InitOpenGL()
{
	s_program_id = CreateFromFiles("../Shaders/vshader_MultiLights.glsl", "../Shaders/fshader_MultiLights.glsl");
	glUseProgram(s_program_id);

	glViewport(0, 0, (GLsizei)g_window_w, (GLsizei)g_window_h);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


	g_camera.lookAt(glm::vec3(0.f, 5.f, 6.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));


	//  meshes
	InitBasicShapeObjs();
	InitGround2();

	// Timer �Լ� ȣ��.
	g_elapsed_time = 0.f;
	glutTimerFunc((unsigned int)(1000.f/FPS), Timer, (int)(1000.f/FPS));
}



/**
ClearOpenGLResource: ���α׷��� ������ �޸� ������ ���� �� �� ȣ��Ǵ� �Լ�. (main �Լ� ����)
���α׷����� ����� �޸𸮸� ���⿡�� ������ �� �ִ�. 
�������, VAO�� VBO�� ���⼭ ���� �� �ִ�.
*/
void ClearOpenGLResource()
{
	// Delete Mesh Models (VA0, VBO)
	DeleteGround2();
	DeleteBasicShapeObjs();
}




/**
Timer: �ִϸ��̼��� ���� �ֱ������� ȣ��Ǵ� �Լ�.
�� ���α׷������� 1�ʿ� 60�� ȣ��ǵ��� �����Ѵ�.
�귯�� �ð��� ������Ʈ�ϰ� �̸� �������� ��ǳ�� ������ ������ ���� ����Ѵ�.
*/
void Timer(int value)
{
	g_elapsed_time += value/1000.f;


	glutPostRedisplay();
	glutTimerFunc((unsigned int)(1000.f/FPS), Timer, (int)(1000.f/FPS));
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
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	// Vertex shader �� matrix �������� location�� �޾ƿ´�.
	int m_proj_loc = glGetUniformLocation(s_program_id, "proj_matrix");
	int m_view_loc = glGetUniformLocation(s_program_id, "view_matrix");
	int m_model_loc = glGetUniformLocation(s_program_id, "model_matrix");



	// Projection Transform Matrix ����.
	glm::mat4 projection_matrix = glm::perspective(glm::radians(45.f), (float)g_window_w / g_window_h, 0.01f, 10000.f);
	glUniformMatrix4fv(m_proj_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	// Camera Transform Matrix ����.
	glm::mat4 view_matrix = g_camera.GetGLViewMatrix();
	glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));



	// Lights, �� ����.
	{
		// ���� ������ Shader ���� num_of_lights �� �Ѱ��ش�.
		int num_of_lights_loc = glGetUniformLocation(s_program_id, "num_of_lights");
		glUniform1i(num_of_lights_loc, 3);

		// Directional Light ����.
		{
			// ���� ���� ���� (0: Directionl Light, 1: Point Light, 2: Spot Light), fshader_MultiLights.glsl ����.
			int type_loc = glGetUniformLocation(s_program_id, "lights[0].type");
			glUniform1i(type_loc, 0);



			// ���� ���� ����.
			glm::vec3 dir(-1.f, -1.f, 0.f);
			dir = glm::normalize(dir);

			////// *** ���� ī�޶� ������ ����ϱ� ���� view transform ����  ***
			//  dir�� ������ ��Ÿ���� �����̹Ƿ� �̵�(Translation)��ȯ�� ���õǵ��� �Ѵ�. (�� ��° ��� 0.f���� ����)
			dir = glm::vec3(  view_matrix * glm::vec4(dir, 0.f) );

			int dir_loc = glGetUniformLocation(s_program_id, "lights[0].dir");
			glUniform3f(dir_loc, dir[0], dir[1], dir[2]);

			// ���� ���� ����.
			int intensity_loc = glGetUniformLocation(s_program_id, "lights[0].intensity");
			glUniform3f(intensity_loc, 0.3f, 0.3f, 0.3f);
		}


		// Point Light ����
		{
			// ���� ���� ���� (0: Directionl Light, 1: Point Light, 2: Spot Light), fshader_MultiLights.glsl ����.
			int type_loc = glGetUniformLocation(s_program_id, "lights[1].type");
			glUniform1i(type_loc, 1);


			// ���� ����ϴ� ��ġ(����) ����.
			// �ð��� ���� ��ġ�� ���ϵ��� ��.
			glm::vec3 pos(2.f * cos(g_elapsed_time), 0.5f, 2.f* sin(g_elapsed_time));

			// Apply Camera Matrices
			////// *** ���� ī�޶� ������ ����ϱ� ���� view transform ����  ***
			//  �̶� pos�� ��ġ�� ��Ÿ���� ����Ʈ�̹Ƿ� �̵�(Translation)��ȯ�� ����ǵ��� �Ѵ�. (�� ��° ��� 1.f���� ����)
			pos = glm::vec3(  view_matrix * glm::vec4(pos, 1.f) );

			int pos_loc = glGetUniformLocation(s_program_id, "lights[1].position");
			glUniform3f(pos_loc, pos[0], pos[1], pos[2]);

			
			// ���� ���� ����.
			int intensity_loc = glGetUniformLocation(s_program_id, "lights[1].intensity");
			glUniform3f(intensity_loc, 1.f, 1.f, 1.f);
		}



		// Spot Light ����.
		{
			// ���� ���� ���� (0: Directionl Light, 1: Point Light, 2: Spot Light), fshader_MultiLights.glsl ����.
			int type_loc = glGetUniformLocation(s_program_id, "lights[2].type");
			glUniform1i(type_loc, 2);

			// ���� ����ϴ� ��ġ(����) ����.
			glm::vec3 pos(0.f, 3.f, 0.f);

			// Apply Camera Matrices
			////// *** ���� ī�޶� ������ ����ϱ� ���� view transform ����  ***
			//  �̶� pos�� ��ġ�� ��Ÿ���� ����Ʈ�̹Ƿ� �̵�(Translation)��ȯ�� ����ǵ��� �Ѵ�. (�� ��° ��� 1.f���� ����)
			pos = glm::vec3(  view_matrix * glm::vec4(pos, 1.f) );

			int pos_loc = glGetUniformLocation(s_program_id, "lights[2].position");
			glUniform3f(pos_loc, pos[0], pos[1], pos[2]);


			// ���� ���� ����.
			// �ð��� ���� ������ ���ϵ��� ��.
			glm::vec3 dir(-1.0f * cos(g_elapsed_time), -1.f, 0.f);
			dir = glm::normalize(dir);

			////// *** ���� ī�޶� ������ ����ϱ� ���� view transform ����  ***
			//  dir�� ������ ��Ÿ���� �����̹Ƿ� �̵�(Translation)��ȯ�� ���õǵ��� �Ѵ�. (�� ��° ��� 0.f���� ����)
			dir = glm::vec3(  view_matrix * glm::vec4(dir, 0.f) );

			int dir_loc = glGetUniformLocation(s_program_id, "lights[2].dir");
			glUniform3f(dir_loc, dir[0], dir[1], dir[2]);
		


			// ���� ���� ���� (������).
			int intensity_loc = glGetUniformLocation(s_program_id, "lights[2].intensity");
			glUniform3f(intensity_loc, 1.f, 0.f, 0.f);

			// ���� ���� ���� ����.
			int light_cos_cutoff_loc = glGetUniformLocation(s_program_id, "lights[2].cos_cutoff");
			glUniform1f(light_cos_cutoff_loc, cos( 15.f/180.f * glm::pi<float>() ));
		}

	}




	// Ground
	{
		// Ground�� ���� Phong Shading ���� ���� ���� �����Ѵ�.
		int shininess_loc = glGetUniformLocation(s_program_id, "shininess_n");
		glUniform1f(shininess_loc, 50.f);

		int K_s_loc = glGetUniformLocation(s_program_id, "K_s");
		glUniform3f(K_s_loc, 0.3f, 0.3f, 0.3f);

		// ī�޶� ��ȯ ����� �����Ѵ�.
		glm::mat4 model_T(1.f); // �������
		glUniformMatrix4fv(m_model_loc, 1, GL_FALSE,  glm::value_ptr(model_T));

		// �׸���.
		DrawGround2();
	}

	// Sphere
	{
		// Sphere�� ���� Phong Shading ���� ���� ���� �����Ѵ�.
		int shininess_loc = glGetUniformLocation(s_program_id, "shininess_n");
		glUniform1f(shininess_loc, 100.f);

		int K_s_loc = glGetUniformLocation(s_program_id, "K_s");
		glUniform3f(K_s_loc, 0.7f, 0.7f, 0.7f);

		// ��ȯ ����� �����Ѵ�.
		glm::mat4 model_T;
		model_T = glm::translate(glm::vec3(0.f, 1.f, 0.f)) * glm::scale(glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(m_model_loc, 1, GL_FALSE,  glm::value_ptr(model_T));

		// ��ü �������� ����� Color ���� �����Ѵ�. 
		glVertexAttrib4f(2, 0.3f, 0.6f, 0.9f, 1.f);

		// ���� �׸���.
		DrawSphere();
	}

	glutSwapBuffers();
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





/////////////////////////////////////////////////////////////////////////////////////////
/// Keyboard and Mouse Input


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
















