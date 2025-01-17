


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
//// 자동차 제어 변수들.
//////////////////////////////////////////////////////////////////////
glm::vec3 g_car_poisition(0.f, 0.f, 0.f); //위치
float g_car_speed = 0;			          // 속도 (초당 이동 거리)
float g_car_rotation_y = 0;		          // 현재 방향 (y축 회전)
float g_car_angular_speed = 0;	          // 회전 속도 (각속도 - 초당 회전 각)


//////////////////////////////////////////////////////////////////////
//// Light Parameters
//////////////////////////////////////////////////////////////////////
static int g_sunlight_mode = 1;

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
InitOpenGL: 프로그램 초기 값 설정을 위해 최초 한 번 호출되는 함수. (main 함수 참고)
OpenGL에 관련한 초기 값과 프로그램에 필요한 다른 초기 값을 설정한다.
예를들어, VAO와 VBO를 여기서 생성할 수 있다.
*/
void InitOpenGL()
{
	s_program_id = CreateFromFiles("../Shaders/v_shader_texture.glsl", "../Shaders/f_shader_texture.glsl");
	glUseProgram(s_program_id);

	glViewport(0, 0, (GLsizei)g_window_w, (GLsizei)g_window_h);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


	// Initial State of Camera
	// 카메라 초기 위치 설정한다.
	g_camera.lookAt(glm::vec3(3.f, 2.f, 3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	
	// basic meshes
	InitBasicShapeObjs();


	// Car
	//InitCarModel();

	// 받침대 생성
	InitProptopModel();
	InitPropbottomModel();

	// 바닥 VAO 생성
	InitGroundTexture();

	// Moon VAO 생성
	InitMoonTexture();

	// Ring 생성
	InitRingModel();
	InitLightModel();
}







/**
ClearOpenGLResource: 프로그램이 끝나기 메모리 해제를 위해 한 번 호출되는 함수. (main 함수 참고)
프로그램에서 사용한 메모리를 여기에서 해제할 수 있다.
예를들어, VAO와 VBO를 여기서 지울 수 있다.
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
Display: 윈도우 화면이 업데이트 될 필요가 있을 때 호출되는 callback 함수.

윈도우 상에 최종 결과를 렌더링 하는 코드는 이 함수 내에 구현해야한다.
원도우가 처음 열릴 때, 윈도우 크기가 바뀔 때, 다른 윈도우에 의해 화면의 일부
또는 전체가 가려졌다가 다시 나타날 때 등 시스템이 해당 윈도우 내의 그림에 대한
업데이트가 필요하다고 판단하는 경우 자동으로 호출된다.
강제 호출이 필요한 경우에는 glutPostRedisplay() 함수를 호출하면된다.

이 함수는 불시에 빈번하게 호출된다는 것을 명심하고, 윈도우 상태 변화와 무관한
1회성 코드는 가능한한 이 함수 밖에 구현해야한다. 특히 메모리 할당, VAO, VBO 생성
등의 하드웨어 점유를 시도하는 코드는 특별한 이유가 없다면 절대 이 함수에 포함시키면
안된다. 예를 들어, 메시 모델을 정의하고 VAO, VBO를 설정하는 부분은 최초 1회만
실행하면되므로 main() 함수 등 외부에 구현해야한다. 정의된 메시 모델을 프레임 버퍼에
그리도록 지시하는 코드만 이 함수에 구현하면 된다.

만일, 이 함수 내에서 동적 메모리 할당을 해야하는 경우가 있다면 해당 메모리는 반드시
이 함수가 끝나기 전에 해제 해야한다.

ref: https://www.opengl.org/resources/libraries/glut/spec3/node46.html#SECTION00081000000000000000
*/
void Display()
{
	// 전체 화면을 지운다.
	// glClear는 Display 함수 가장 윗 부분에서 한 번만 호출되어야한다.
	if (g_sunlight_mode == 0)
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	if (g_sunlight_mode == 1)
		glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

	// Vertex shader 의 matrix 변수들의 location을 받아온다.
	int m_proj_loc = glGetUniformLocation(s_program_id, "proj_matrix");
	int m_view_loc = glGetUniformLocation(s_program_id, "view_matrix");
	int m_model_loc = glGetUniformLocation(s_program_id, "model_matrix");

	glUniform1i(glGetUniformLocation(s_program_id, "light_mode"), g_sunlight_mode);
	
	// Projection Transform Matrix 설정.
	glm::mat4 projection_matrix = glm::perspective(glm::radians(45.f), (float)g_window_w / g_window_h, 0.01f, 10000.f);
	glUniformMatrix4fv(m_proj_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	// Camera Transform Matrix 설정.
	glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, glm::value_ptr(g_camera.GetGLViewMatrix()));

	// texture filtering 설정
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

	// light 설정
	{
		glUniform1d(glGetUniformLocation(s_program_id, "light_mode"), g_sunlight_mode);

		// 빛의 출발 점과, 진행 방향 설정.
		glm::vec3 light_position(0.f, 3.f, -5.f);
		//glm::vec3 light_dir(0.f, -1.f, 0.f);
		//light_dir = glm::normalize(light_dir);

		light_position = glm::rotateY(light_position, g_ring_rotation_y);
		glm::vec3 light_dir = glm::vec3(0.f,3.f,0.f)-light_position;

		light_dir = glm::normalize(light_dir);


		// Apply Camera Matrices
		////// *** 현재 카메라 방향을 고려하기 위해 view transform 적용  ***
		//  light_position는 위치를 나타내는 포인트이므로 이동(Translation)변환이 적용되도록 한다. (네 번째 요소 1.f으로 셋팅)
		light_position = glm::vec3(g_camera.GetGLViewMatrix() * glm::vec4(light_position, 1.f));
		//  light_dir는 방향을 나타내는 벡터이므로 이동(Translation)변환이 적용되지 않도록 한다. (네 번째 요소 0.f으로 셋팅)
		light_dir = glm::vec3(g_camera.GetGLViewMatrix() * glm::vec4(light_dir, 0.f));


		int light_position_loc = glGetUniformLocation(s_program_id, "light_position");
		glUniform3f(light_position_loc, light_position[0], light_position[1], light_position[2]);

		int light_dir_loc = glGetUniformLocation(s_program_id, "light_dir");
		glUniform3f(light_dir_loc, light_dir[0], light_dir[1], light_dir[2]);



		// Spot Light 변수 설정.
		float light_cos_cutoff = cos(glm::radians(70.f));
		glm::vec3 light_indensity(1.0f, 1.0f, 1.0f);	// Red Light

		int light_cos_cutoff_loc = glGetUniformLocation(s_program_id, "light_cos_cutoff");
		glUniform1f(light_cos_cutoff_loc, light_cos_cutoff);

		int light_intensity_loc = glGetUniformLocation(s_program_id, "I_l");
		glUniform3f(light_intensity_loc, light_indensity[0], light_indensity[1], light_indensity[2]);
	}

	// texture 바닥 
	{
		glUniform1i(glGetUniformLocation(s_program_id, "flag_texture"), true);
		glUniform1i(glGetUniformLocation(s_program_id, "isGround"), true);
		glUniform1i(glGetUniformLocation(s_program_id, "isGround_shiness"), g_ground_shiness);

		// Ground를 위한 Phong Shading 관련 변수 값을 설정한다.
		int shininess_loc = glGetUniformLocation(s_program_id, "shininess_n");
		glUniform1f(shininess_loc, shininess_n);

		int K_s_loc = glGetUniformLocation(s_program_id, "K_s");
		glUniform3f(K_s_loc, 0.3f, 0.3f, 0.3f);

		glm::mat4 T0(1.f); // 단위 행렬
		glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, glm::value_ptr(T0 * glm::translate(glm::vec3(0, -0.44, 0)) * glm::scale(glm::vec3(5.f,1.f,5.f))));
		DrawGroundTexture();

		// Left Wall
		glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, glm::value_ptr(T0 * glm::translate(glm::vec3(25.f, 15.44f, 0.f))* glm::rotate(glm::radians(90.f),glm::vec3(0,0,1))* glm::scale(glm::vec3(5.f,1.f,5.f))));
		DrawGroundTexture();

		// Right Wall
		glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, glm::value_ptr(T0* glm::translate(glm::vec3(-25.f, 15.44f, 0.f))* glm::rotate(glm::radians(-90.f), glm::vec3(0, 0, 1))* glm::scale(glm::vec3(5.f, 1.f, 5.f))));
		DrawGroundTexture();

		// Out Wall
		glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, glm::value_ptr(T0* glm::translate(glm::vec3(0.f, 15.44f, -25.f))* glm::rotate(glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f))* glm::scale(glm::vec3(5.f, 1.f, 5.f))));
		DrawGroundTexture();

		// In Wall
		glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, glm::value_ptr(T0* glm::translate(glm::vec3(0.f, 15.44f, 25.f))* glm::rotate(glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f))* glm::scale(glm::vec3(5.f, 1.f, 5.f))));
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
	// glutSwapBuffers는 항상 Display 함수 가장 아래 부분에서 한 번만 호출되어야한다.
	glutSwapBuffers();
}


/**
Timer: 지정된 시간 후에 자동으로 호출되는 callback 함수.
ref: https://www.opengl.org/resources/libraries/glut/spec3/node64.html#SECTION000819000000000000000
*/
void Timer(int value)
{
	// Timer 호출 시간 간격을 누적하여, 최초 Timer가 호출된 후부터 현재까지 흘러간 계산한다.
	g_elaped_time_s += value/1000.f;


	//// Turn
	//g_car_rotation_y += g_car_angular_speed;

	//// Calculate Velocity
	//glm::vec3 speed_v = glm::vec3(0.f, 0.f, g_car_speed);
	//glm::vec3 velocity = glm::rotateY(speed_v, g_car_rotation_y);	// speed_v 를 y축을 기준으로 g_car_rotation_y 만큼 회전한다.
	//// Move
	//g_car_poisition += velocity;

	if(g_rotation)
		g_ring_rotation_y += g_ring_angular_speed;


	// glutPostRedisplay는 가능한 빠른 시간 안에 전체 그림을 다시 그릴 것을 시스템에 요청한다.
	// 결과적으로 Display() 함수가 호출 된다.
	glutPostRedisplay();

	// 1/60 초 후에 Timer 함수가 다시 호출되로록 한다.
	// Timer 함수 가 동일한 시간 간격으로 반복 호출되게하여,
	// 애니메이션 효과를 표현할 수 있다
	glutTimerFunc((unsigned int)(1000 / 60), Timer, (1000 / 60));
}




/**
Reshape: 윈도우의 크기가 조정될 때마다 자동으로 호출되는 callback 함수.

@param w, h는 각각 조정된 윈도우의 가로 크기와 세로 크기 (픽셀 단위).
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
Keyboard: 키보드 입력이 있을 때마다 자동으로 호출되는 함수.
@param key는 눌려진 키보드의 문자값.
@param x,y는 현재 마우스 포인터의 좌표값.
ref: https://www.opengl.org/resources/libraries/glut/spec3/node49.html#SECTION00084000000000000000

*/
void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		//case 's':
		//	g_car_speed = -0.01f;		// 후진 속도 설정
		//	glutPostRedisplay();
		//	break;

		//case 'w':
		//	g_car_speed = 0.01f;		// 전진 속도 설정
		//	glutPostRedisplay();
		//	break;

		//case 'a':
		//	g_car_angular_speed = glm::radians( 1.f );		// 좌회전 각속도 설정
		//	glutPostRedisplay();
		//	break;

		//case 'd':
		//	g_car_angular_speed = -1 * glm::radians( 1.f );		//  우회전 각속도 설정
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
		if (shininess_n >= 10)
			shininess_n -= 10;
		break;
	}

}

/**
KeyboardUp: 눌려졌던 키가 놓여질 때마다 자동으로 호출되는 함수.
@param key는 해당 키보드의 문자값.
@param x,y는 현재 마우스 포인터의 좌표값.
ref: https://www.opengl.org/resources/libraries/glut/spec3/node49.html#SECTION00084000000000000000

*/
void KeyboardUp(unsigned char key, int x, int y)
{
	switch (key)						
	{
	case 's':
		g_car_speed = 0.f;		// 후진 속도 설정
		glutPostRedisplay();
		break;

	case 'w':
		g_car_speed = 0.f;		// 전진 속도 설정
		glutPostRedisplay();
		break;

	case 'a':
		g_car_angular_speed = 0.f;		// 좌회전 각속도 설정
		glutPostRedisplay();
		break;

	case 'd':
		g_car_angular_speed = 0.f;		//  우회전 각속도 설정
		glutPostRedisplay();
		break;

	}

}



/**
Mouse: 마우스 버튼이 입력될 때마다 자동으로 호출되는 함수.
파라메터의 의미는 다음과 같다.
@param button: 사용된 버튼의 종류
  GLUT_LEFT_BUTTON - 왼쪽 버튼
  GLUT_RIGHT_BUTTON - 오른쪽 버튼
  GLUT_MIDDLE_BUTTON - 가운데 버튼 (휠이 눌러졌을 때)
  3 - 마우스 휠 (휠이 위로 돌아 갔음).
  4 - 마우스 휠 (휠이 아래로 돌아 갔음).
@param state: 조작 상태
  GLUT_DOWN - 눌러 졌음
  GLUT_UP - 놓여졌음
@param x,y: 조작이 일어났을 때, 마우스 포인터의 좌표값.
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
MouseMotion: 마우스 포인터가 움직일 때마다 자동으로 호출되는 함수.
@prarm x,y는 현재 마우스 포인터의 좌표값을 나타낸다.
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