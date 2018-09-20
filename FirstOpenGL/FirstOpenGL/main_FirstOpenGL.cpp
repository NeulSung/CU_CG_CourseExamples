#include <iostream>
#include "GL/glew.h"
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
	glutTimerFunc((unsigned int)(1000 /60), Timer, 0);





	///////////////////////////////////////////////////////////////////////
	//// Initialize GLEW
	glewExperimental = TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "GLEW OK\n";
	}

	

	///////////////////////////////////////////////////////////////////////
	//// Vertex shader program 

	// create vertex shader and obejct the id.
	GLuint v_shader_id = glCreateShader(GL_VERTEX_SHADER);

	// vertex shader source code
	const GLchar * v_shader_source = 
		R"(
			#version 330

			layout (location=0) in vec4 vs_position;
			layout (location=1) in vec4 vs_normal;
			layout (location=2) in vec4 vs_color;

			//uniform mat4 projection_matrix;
			//uniform mat4 modelview_matrix;

			out vec4 fs_color;

			void main()
			{
				gl_Position = vs_position; //projection_matrix * modelview_matrix * vs_position;
				fs_color = vs_color;
			}
		
		)";

	glShaderSource(v_shader_id, 1, &v_shader_source, NULL);
	glCompileShader(v_shader_id);

	{
		GLint compiled;
		glGetShaderiv(v_shader_id, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			GLsizei len;
			glGetShaderiv(v_shader_id, GL_INFO_LOG_LENGTH, &len);

			GLchar* log = new GLchar[len + 1];
			glGetShaderInfoLog(v_shader_id, len, &len, log);
			std::cerr << "Shader compilation failed: " << log << std::endl;
			delete[] log;
			return 0;
		}
	}


	///////////////////////////////////////////////////////////////////////
	//// Fragment shader program 

	// create vertex shader and obejct the id.
	GLuint f_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	// fragament shader source code
	const GLchar * f_shader_code =
		R"(
			#version 330

			in vec4 fs_color;
	
			layout (location = 0) out vec4 color;

			void main()
			{
				color = fs_color;
			}
		
		)";

	glShaderSource(f_shader_id, 1, &f_shader_code, NULL);
	glCompileShader(f_shader_id);

	{
		GLint compiled;
		glGetShaderiv(f_shader_id, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			GLsizei len;
			glGetShaderiv(f_shader_id, GL_INFO_LOG_LENGTH, &len);

			GLchar* log = new GLchar[len + 1];
			glGetShaderInfoLog(f_shader_id, len, &len, log);
			std::cerr << "Shader compilation failed: " << log << std::endl;
			delete[] log;
			return 0;
		}
	}


	///////////////////////////////////////////////////////////////////////
	//// Shader Program (vertex shader + fragment shader)

	// Create shader program and obtain the id. 
	GLuint s_program_id = glCreateProgram();

	// Attache vertex and fragment shaders to the program
	glAttachShader(s_program_id, v_shader_id);
	glAttachShader(s_program_id, f_shader_id);

	glLinkProgram(s_program_id);
	glUseProgram(s_program_id);





	///////////////////////////////////////////////////////////////////////
	//// Initialize OpenGL

	glViewport(0, 0, (GLsizei)g_window_w, (GLsizei)g_window_h);
	glOrthof(-100.f, 100.f, -100.f, 100.f, -100.f, 100.f);
	
	// ������ ���Ѵ�.
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);

	

	///////////////////////////////////////////////////////////////////////
	//// Mesh Model
	GLfloat m_positions[g_num_vertices * 4] = 
	{     0.f,  0.f,  0.f, 1.f
		, 0.5f, 0.f,  0.f, 1.f
		, 0.5f, 0.5f, 0.f, 1.f
		, 0.f,  0.5f, 0.f, 1.f
	};

	GLfloat m_normals[g_num_vertices * 4] =
	{     0.f, 0.f, 1.f, 0.f
		, 0.f, 0.f, 1.f, 0.f
		, 0.f, 0.f, 1.f, 0.f
		, 0.f, 0.f, 1.f, 0.f
	};

	GLfloat m_colors[g_num_vertices * 4] =
	{     1.f, 0.f, 0.f, 1.f
		, 0.f, 1.f, 0.f, 1.f
		, 0.f, 0.f, 1.f, 1.f
		, 1.f, 1.f, 0.f, 1.f
	};


	GLuint m_indices[g_num_triangles * 3] =
	{
		0, 1, 2,
		0, 2, 3
	};


	///////////////////////////////////////////////////////////////////////
	//// Vertex Array Object
	// VAO�� �����ϰ� �ڵ����� �ο��� �̸��� g_two_tri_vao�� ��´�.
	glGenVertexArrays(1, &g_vao_id);
	glBindVertexArray(g_vao_id);


	//// VBO for positions
	// ������(vertex)�� ��ǥ���� �����ϱ� ���� VBO�� �����ϰ� �ڵ����� �ο��� VBO�� �̸��� g_two_tri_vbo_pos�� ��´�.
	glGenBuffers(1, &g_vbo_position_id);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo_position_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * g_num_vertices, m_positions, GL_STATIC_DRAW);	
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &g_vbo_normal_id);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo_normal_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * g_num_vertices, m_normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &g_vbo_color_id);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo_color_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * g_num_vertices, m_colors, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);


	// Index Buffer Object
	glGenBuffers(1, &g_index_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3 * g_num_triangles, m_indices, GL_STATIC_DRAW);





	glutMainLoop();

	return 0;
}



/**
Display: �׸��� �׸��� ���� ȣ��Ǵ� callback �Լ�.
ȭ�鿡 �׸��� ���� �׷��� �ʿ䰡 ���� ������ �ڵ����� �ݺ��ؼ� ���� �� ȣ��ȴ�.
(��. �ٸ� �����쿡 ���� ȭ���� �Ϻκ��� �������ٰ� �ٽ� ��Ÿ���� ���.)
�ݺ������� ȣ��ȴٴ� ���� ����ϰ�, ���ʿ��� ������ ���� �̰��� �־�� �ȵȴ�.
���� ���, VAO�� VBO�� �����ϴ� �ڵ带 �̰��� �־�� �ȵȴ�. �� ������ �̹� ������
VAO�� VBO�� ����ؼ� �׸��� �׸��� �ڵ常 ���ԵǾ�� �Ѵ�.

�� ������ ��쿡�� g_draw_mode�� ���Կ� ���� �ٸ� �׸��� ��Ÿ������ �Ͽ���.
*/
void Display()
{
	// ��ü ȭ���� �����.
	// glClear�� Display �Լ� ���� �� �κп��� �� ���� ȣ��Ǿ���Ѵ�.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glBindVertexArray(g_vao_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_id);
	glDrawElements(GL_TRIANGLES, g_num_triangles * 3, GL_UNSIGNED_INT, 0);

	
	// ������۸� ������ ���� �Լ� ȣ��.
	// glutSwapBuffers�� �׻� Display �Լ� ���� �Ʒ� �κп��� �� ���� ȣ��Ǿ���Ѵ�.
	glutSwapBuffers();
}



/**
Timer: �ִϸ��̼��� ���� �ֱ������� ȣ��Ǵ� �Լ�.
�� ���α׷������� 1�ʿ� 60�� ȣ��ǵ��� �����Ѵ�.
�귯�� �ð��� ������Ʈ�ϰ� �̸� �������� ��ǳ�� ������ ������ ���� ����Ѵ�.
*/
void Timer(int value)
{

	// glutPostRedisplay�� ������ ���� �ð� �ȿ� ��ü �׸��� �ٽ� �׸� ���� �ý��ۿ� ��û�Ѵ�.
	// ��������� Display() �Լ��� ȣ���ϰ� �ȴ�.
	glutPostRedisplay();

	// 1/60 �� �Ŀ� Timer �Լ��� �ٽ� ȣ��Ƿη� �Ѵ�.
	glutTimerFunc((unsigned int)(1000 / 60), Timer, 0);
}



/**
Reshape: �������� ũ�Ⱑ ������ ������ �ڵ����� ȣ��Ǵ� callback �Լ�.
w�� h�� ���� ������ �������� ���� ũ��� ���� ũ�⸦ ��Ÿ���� (�ȼ� ����).
ȭ�� ũ�Ⱑ ��ȭ�ϸ�, Viewport�� ũ��, ī�޶��� ���μ��� ���� ���� �ٽ� �����ؾ��Ѵ�.
*/
void Reshape(int w, int h)
{
	//  w : window width   h : window height
	g_window_w = w;
	g_window_h = h;


	//  w : window width   h : window height
	//glViewport(0, 0, (GLsizei)w, (GLsizei)h);

}









/////////////////////////////////////////////////////////////////////////////////////////
/// Keyboard and Mouse Input



/**
Mouse: ���콺 ��ư�� �Էµ� ������ �ڵ����� ȣ��Ǵ� �Լ�.
�Ķ������ �ǹ̴� ������ ����.
1. button: ���� ��ư�� ����
GLUT_LEFT_BUTTON - ���� ��ư
GLUT_RIGHT_BUTTON - ������ ��ư
GLUT_MIDDLE_BUTTON - ��� ��ư (���� �������� ��)
3 - ���콺 �� (���� ���� ���� ����).
4 - ���콺 �� (���� �Ʒ��� ���� ����).
2. state: ���� ����
GLUT_DOWN - ���� ����
GLUT_UP - ��������
3. x,y: ������ �Ͼ�� ��, ���콺 �������� ��ǥ��.
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
MouseMotion: ���콺 �����Ͱ� ������ ������ �ڵ����� ȣ��Ǵ� �Լ�.
�Ķ���� x,y�� ���� ���콺 �������� ��ǥ���� ��Ÿ����.
*/
void MouseMotion(int x, int y)
{
	std::cout << "mouse dragged : (" << x << ", " << y << ")" << std::endl;

}



/**
Keyboard: Ű���� �Է��� ���� ������ �ڵ����� ȣ��Ǵ� �Լ�.
�Ķ���� key �� ������ Ű������ ���ڰ��� ��Ÿ����.
�Ķ���� x,y�� ���� ���콺 �������� ��ǥ���� ��Ÿ����.

�� ���������� Ű���� '1', '2', '3', '4' �� �������� g_draw_mode�� �����Ѵ�.
*/
void Keyboard(unsigned char key, int x, int y)
{
	std::cout << "key '" << key << "' pushed : (" << x << ", " << y << ")" << std::endl;
	


	// glutPostRedisplay�� ������ ���� �ð� �ȿ� ��ü �׸��� �ٽ� �׸� ���� �ý��ۿ� ��û�Ѵ�.
	// ��������� Display() �Լ��� ȣ���ϰ� �ȴ�.
	glutPostRedisplay();
}




/**
SpeicalKeyboard: ���ڰ����� ǥ���ϱ� ����� Ű���� �Է��� ���� ������ �ڵ����� ȣ��Ǵ� �Լ�.
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

	glutPostRedisplay();
}






