#include <iostream>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"


GLuint g_window_w = 500;
GLuint g_window_h = 500;


//////////////////////////////////////////////////////////////////////
//// Define Mesh Model
//////////////////////////////////////////////////////////////////////
const int g_num_vertices = 24;
const int g_num_triangles = 12;

GLfloat m_positions[g_num_vertices * 3] =
{  0.25f,  0.25f,  0.25f,
  -0.25f,  0.25f,  0.25f,
  -0.25f, -0.25f,  0.25f,
   0.25f, -0.25f,  0.25f,
 						
   0.25f,  0.25f,  0.25f,
   0.25f,  0.25f, -0.25f,
  -0.25f,  0.25f, -0.25f,
  -0.25f,  0.25f,  0.25f,
 						
   0.25f,  0.25f,  0.25f,
   0.25f, -0.25f,  0.25f,
   0.25f, -0.25f, -0.25f,
   0.25f,  0.25f, -0.25f,
 						
   0.25f,  0.25f, -0.25f,
   0.25f, -0.25f, -0.25f,
  -0.25f, -0.25f, -0.25f,
  -0.25f,  0.25f, -0.25f,
 						
   0.25f, -0.25f,  0.25f,
  -0.25f, -0.25f,  0.25f,
  -0.25f, -0.25f, -0.25f,
   0.25f, -0.25f, -0.25f,
 						
  -0.25f,  0.25f,  0.25f,
  -0.25f,  0.25f, -0.25f,
  -0.25f, -0.25f, -0.25f,
  -0.25f, -0.25f,  0.25f
};

GLfloat m_normals[g_num_vertices * 3] =
{  0.f,  0.f,  1.f,
   0.f,  0.f,  1.f,
   0.f,  0.f,  1.f,
   0.f,  0.f,  1.f,
   		 	   	   
   0.f,  1.f,  0.f,
   0.f,  1.f,  0.f,
   0.f,  1.f,  0.f,
   0.f,  1.f,  0.f,
   		 	   	   
   1.f,  0.f,  0.f,
   1.f,  0.f,  0.f,
   1.f,  0.f,  0.f,
   1.f,  0.f,  0.f,
   		 	   	   
   0.f,  0.f, -1.f,
   0.f,  0.f, -1.f,
   0.f,  0.f, -1.f,
   0.f,  0.f, -1.f,

   0.f, -1.f,  0.f,
   0.f, -1.f,  0.f,
   0.f, -1.f,  0.f,
   0.f, -1.f,  0.f,

  -1.f,  0.f,  0.f,
  -1.f,  0.f,  0.f,
  -1.f,  0.f,  0.f,
  -1.f,  0.f,  0.f
};

GLfloat m_colors[g_num_vertices * 4] =
{ 1.f, 0.f, 0.f, 1.f,
  0.f, 1.f, 0.f, 1.f,
  0.f, 0.f, 1.f, 1.f,
  1.f, 1.f, 0.f, 1.f,
					
  1.f, 0.f, 0.f, 1.f,
  0.f, 1.f, 0.f, 1.f,
  0.f, 0.f, 1.f, 1.f,
  1.f, 1.f, 0.f, 1.f,

  1.f, 0.f, 0.f, 1.f,
  0.f, 1.f, 0.f, 1.f,
  0.f, 0.f, 1.f, 1.f,
  1.f, 1.f, 0.f, 1.f,

  1.f, 0.f, 0.f, 1.f,
  0.f, 1.f, 0.f, 1.f,
  0.f, 0.f, 1.f, 1.f,
  1.f, 1.f, 0.f, 1.f,

  1.f, 0.f, 0.f, 1.f,
  0.f, 1.f, 0.f, 1.f,
  0.f, 0.f, 1.f, 1.f,
  1.f, 1.f, 0.f, 1.f,

  1.f, 0.f, 0.f, 1.f,
  0.f, 1.f, 0.f, 1.f,
  0.f, 0.f, 1.f, 1.f,
  1.f, 1.f, 0.f, 1.f
};

GLuint m_indices[g_num_triangles * 3] =
{
   0, 1, 2,
   0, 2, 3,
   
   4, 5, 6,
   4, 6, 7,
   
   8,  9, 10,
   8, 10, 11,
  
  12, 13, 14,
  12, 14, 15,
  
  16, 17, 18,
  16, 18, 19,
  
  20, 21, 22,
  20, 22, 23
};


//////////////////////////////////////////////////////////////////////
//// Define Shader Programs
//////////////////////////////////////////////////////////////////////

GLuint f_shader_id;
GLuint v_shader_id;
GLuint s_program_id;

// vertex shader source code
const GLchar * v_shader_source =
R"(
	#version 330

	layout (location=0) in vec3 vs_position;
	layout (location=1) in vec3 vs_normal;
	layout (location=2) in vec4 vs_color;

	uniform mat4 proj_matrix;
	uniform mat4 view_matrix;
	uniform mat4 model_matrix;

	out vec3 fs_normal;
	out vec4 fs_color;

	void main()
	{
		gl_Position = proj_matrix * view_matrix * model_matrix * vec4(vs_position, 1.f);
		fs_normal = vec3(view_matrix * model_matrix * vec4(vs_normal, 0.f));
		fs_color = vs_color;
	}
		
)";

// fragament shader source code
const GLchar * f_shader_code =
R"(
	#version 330

	in vec4 fs_color;
	in vec3 fs_normal;

	uniform int shading_mode = 1;

	layout (location = 0) out vec4 color;
	
	void main()
	{
		vec3 N = normalize(fs_normal);

		vec3 L = vec3(1.f, 1.f, 1.f);
		L = normalize(L);

		float diffuse = clamp(dot(L, N), 0.2f, 1.f);

		if ( shading_mode == 1 )
			color = vec4(diffuse, diffuse, diffuse, 1.f) * fs_color;
		else if ( shading_mode == 2 )
			color = vec4(diffuse, diffuse, diffuse, 1.f);
	}
		
)";

//////////////////////////////////////////////////////////////////////
//// Declare VAO, VBO variables
//////////////////////////////////////////////////////////////////////
GLuint g_vao_id;
GLuint g_vbo_position_id;
GLuint g_vbo_normal_id;
GLuint g_vbo_color_id;
GLuint g_index_buffer_id;

// Index Buffer Object
GLuint index_buffer_id;


//////////////////////////////////////////////////////////////////////
//// Declare callback functions
//////////////////////////////////////////////////////////////////////
void Reshape(int w, int h);
void Display();
void Keyboard(unsigned char key, int x, int y);
void Timer(int value);



//////////////////////////////////////////////////////////////////////
//// Animation Parameters
//////////////////////////////////////////////////////////////////////
float g_y_angle = 0.f;


int main(int argc, char** argv)
{


	//////////////////////////////////////////////////////////////////////////////////////
	//// 1. freeglut �ʱ�ȭ, ������ �����ϱ�. 
	////    Ref: https://en.wikibooks.org/wiki/OpenGL_Programming/Installation/GLUT
	//////////////////////////////////////////////////////////////////////////////////////

	//// 1.1. �ֱ�ȭ
	glutInit(&argc, argv);
	
	//// 1.2. ������ ��� ����. 
	// ������۸�, RGB �����ӹ���, Depth ���۸� ����Ѵ�.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	//// 1.3. ������ ������ ���� (����, ���� �ȼ� ��).
	glutInitWindowSize(g_window_w, g_window_h);

	//// 1.4. OpenGL Context�� ����� ������ ����.
	glutCreateWindow("Computer Graphics");
	
	//// 1.5. Callback functions 
	// Ref: http://freeglut.sourceforge.net/docs/api.php#GlobalCallback
	//      http://freeglut.sourceforge.net/docs/api.php#WindowCallback
	//      https://www.opengl.org/resources/libraries/glut/spec3/node45.html
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc((unsigned int)(1000 /60), Timer, 0);





	//////////////////////////////////////////////////////////////////////////////////////
	//// 2. GLEW �ʱ�ȭ. 
	////    Ref: http://glew.sourceforge.net/basic.html
	//////////////////////////////////////////////////////////////////////////////////////
	glewExperimental = GL_TRUE;
	if ( glewInit() != GLEW_OK )
	{
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "GLEW OK\n";
	}


	


	//////////////////////////////////////////////////////////////////////////////////////
	//// 3. Shader Programs ���
	////    Ref: https://www.khronos.org/opengl/wiki/Shader_Compilation
	//////////////////////////////////////////////////////////////////////////////////////

	//// Vertex shader program
	//// 3.1. Vertex shader ��ü ����.
	GLuint v_shader_id = glCreateShader(GL_VERTEX_SHADER);

	//// 3.2. Vertex shader �ҽ��ڵ� �Է�.
	glShaderSource(v_shader_id, 1, &v_shader_source, NULL);

	//// 3.3. Vertex shader �ҽ��ڵ� ������.
	glCompileShader(v_shader_id);

	//// 3.4. ������ ���� �߻��� ���� �޼��� ���.
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


	//// Fragment shader program 
	//// 3.5. Fragment shader ��ü ����.
	GLuint f_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	//// 3.6. Fragment shader �ҽ��ڵ� �Է�.
	glShaderSource(f_shader_id, 1, &f_shader_code, NULL);

	//// 3.7. Fragment shader �ҽ��ڵ� ������.
	glCompileShader(f_shader_id);

	//// 3.8. ������ ���� �߻��� ���� �޼��� ���.
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

	//// Shader Program (vertex shader + fragment shader)
	//// 3.9. Shader Program ��ä ����.
	s_program_id = glCreateProgram();

	//// 3.10. Vertex shader�� Fragment shader�� Shader Program ��ü�� ���.
	glAttachShader(s_program_id, v_shader_id);
	glAttachShader(s_program_id, f_shader_id);

	//// 3.11. Shader Program ��ũ
	glLinkProgram(s_program_id);

	//// 3.12. Shader Program ������
	glUseProgram(s_program_id);






	////////////////////////////////////////////////////////////////////////////////////
	//// 4. OpenGL ����
	//////////////////////////////////////////////////////////////////////////////////////

	glViewport(0, 0, (GLsizei)g_window_w, (GLsizei)g_window_h);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


	

	

	
	////////////////////////////////////////////////////////////////////////////////////
	//// 5. VAO, VBO ����
	////    Ref: https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Array_Object
	///////////////////////////////////////////////////////////////////////////////////

	//// 5.1. VAO ��ü ���� �� ���ε�
	glGenVertexArrays(1, &g_vao_id);
	glBindVertexArray(g_vao_id);

	
	//// 5.2. vertex positions ������ ���� VBO ���� �� ���ε�.
	glGenBuffers(1, &g_vbo_position_id);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo_position_id);

	//// 5.3. vertex positions ������ �Է�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * g_num_vertices, m_positions, GL_STATIC_DRAW);

	//// 5.4. ���� ���ε��Ǿ��ִ� VBO�� shader program�� ����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	//// 5.5. vertex normal vectors ������ ���� VBO ���� �� ���ε�.
	glGenBuffers(1, &g_vbo_normal_id);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo_normal_id);

	//// 5.6. vertex positions ������ �Է�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * g_num_vertices, m_normals, GL_STATIC_DRAW);

	//// 5.7. ���� ���ε��Ǿ��ִ� VBO�� shader program�� ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);


	//// 5.8. vertex colors ������ ���� VBO ���� �� ���ε�.
	glGenBuffers(1, &g_vbo_color_id);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo_color_id);

	//// 5.9. vertex positions ������ �Է�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * g_num_vertices, m_colors, GL_STATIC_DRAW);

	//// 5.10. ���� ���ε��Ǿ��ִ� VBO�� shader program�� ����
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);


	//// 5.11. Index Buffer ��ü ���� �� ������ �Է�
	glGenBuffers(1, &g_index_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3 * g_num_triangles, m_indices, GL_STATIC_DRAW);

	


	//// 1.6. freeglut ������ �̺�Ʈ ó�� ����. �����찡 ���������� ���ѷ��� ����.
	glutMainLoop();

	return 0;
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Vertex shader �� matrix �������� location�� �޾ƿ´�.
	int m_proj_loc = glGetUniformLocation(s_program_id, "proj_matrix");
	int m_view_loc = glGetUniformLocation(s_program_id, "view_matrix");
	int m_model_loc = glGetUniformLocation(s_program_id, "model_matrix");


	// Projection Transform Matrix ����.
	glm::mat4 projection_matrix = glm::perspective(glm::radians(45.f), 1.f, 0.01f, 1000.f);
	glUniformMatrix4fv(m_proj_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	// Camera Transform Matrix ����.
	glm::mat4 view_matrix = glm::lookAt(glm::vec3(2.f, 1.5f, 2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));


	// ù��° ť���� Transform Matrix ����.
	glm::mat4 T1 = glm::rotate(glm::radians(g_y_angle), glm::vec3(0.f, 1.f, 0.f));
	glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, glm::value_ptr(T1));

	glBindVertexArray(g_vao_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_id);
	glDrawElements(GL_TRIANGLES, g_num_triangles * 3, GL_UNSIGNED_INT, 0);


	//// �ι�° ť���� Transform Matrix ����.
	//glm::mat4 trans_x = glm::translate(glm::vec3(0.5f, 0.f, 0.f));
	//glm::mat4 scale_half = glm::scale(glm::vec3(0.5f, 0.5f, 0.5f));
	//glm::mat4 T2 = T1 * trans_x * scale_half;
	//glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, glm::value_ptr(T2));

	//glBindVertexArray(g_vao_id);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_id);
	//glDrawElements(GL_TRIANGLES, g_num_triangles * 3, GL_UNSIGNED_INT, 0);

	

	
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
	g_y_angle += 1.f;

	// glutPostRedisplay�� ������ ���� �ð� �ȿ� ��ü �׸��� �ٽ� �׸� ���� �ý��ۿ� ��û�Ѵ�.
	// ��������� Display() �Լ��� ȣ�� �ȴ�.
	glutPostRedisplay();

	// 1/60 �� �Ŀ� Timer �Լ��� �ٽ� ȣ��Ƿη� �Ѵ�.
	// Timer �Լ� �� ������ �ð� �������� �ݺ� ȣ��ǰ��Ͽ�,
	// �ִϸ��̼� ȿ���� ǥ���� �� �ִ�
	glutTimerFunc((unsigned int)(1000 / 60), Timer, 0);
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
	// keyboard '1' �� �������� ��.
	if (key == '1')
	{
		// Fragment shader�� ���� �Ǿ��ִ� 'shading_mode' ������ location�� �޾ƿ´�.
		int shading_mode_loc = glGetUniformLocation(s_program_id, "shading_mode");

		// 'shading_mode' ������ 1�� ����.
		glUniform1i(shading_mode_loc, 1);


		// glutPostRedisplay�� ������ ���� �ð� �ȿ� ��ü �׸��� �ٽ� �׸� ���� �ý��ۿ� ��û�Ѵ�.
		// ��������� Display() �Լ��� ȣ���ϰ� �ȴ�.
		glutPostRedisplay();
	}

	// keyboard '2' �� �������� ��.
	else if (key == '2')
	{
		// Fragment shader�� ���� �Ǿ��ִ� 'shading_mode' ������ location�� �޾ƿ´�.
		int shading_mode_loc = glGetUniformLocation(s_program_id, "shading_mode");

		// 'shading_mode' ������ 2�� ����.
		glUniform1i(shading_mode_loc, 2);


		// glutPostRedisplay�� ������ ���� �ð� �ȿ� ��ü �׸��� �ٽ� �׸� ���� �ý��ۿ� ��û�Ѵ�.
		// ��������� Display() �Լ��� ȣ���ϰ� �ȴ�.
		glutPostRedisplay();
	}
}








