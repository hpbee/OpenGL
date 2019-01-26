#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include "renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"


void parseShader(std::string filePath, std::string& vertexShader, std::string& fragmentShader)
{
	std::ifstream stream(filePath);
	std::string line("");
	std::string type;
	std::string* target=&vertexShader;
	while (std::getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			type = line.substr(line.find("#shader") + 8);
			while (type.find_first_of(" ") != std::string::npos) type.replace(type.find_first_of(" "), 1, "");
			if (type.find("GL_VERTEX_SHADER")!=std::string::npos) target = &vertexShader;
			if (type.find("GL_FRAGMENT_SHADER") != std::string::npos) target = &fragmentShader;
			continue;
		}
		*target += (line+"\n");
	}
}

static unsigned int CompileShader(const std::string& source, unsigned int type)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	//Handle Errors
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message = (char*)(alloca(length * sizeof(char*)));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Compile Error in shader. " << ((type == GL_VERTEX_SHADER) ? "Vertex Shader" : "Fragment Shader") << std::endl;
		std::cout << message << std::endl;
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram(); //ID of program created
	unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);
	//glUseProgram(program);
	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	//Create GLFW window with core profile unlike the default compatability profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//Change opengl version being used
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);//

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) //This needs to be called only after creating opengl context
		std::cout << "glewInit Error\n";
	{
		std::cout << glGetString(GL_VERSION);
		float positions[] =
		{
		-0.5f, -0.5f, //0
		 0.5f, -0.5f, //1
		 0.5f, 0.5f, //2
		-0.5f, 0.5f //3
		};
		unsigned int indices[] =
		{
			0,1,2,
			2,3,0
		}; //Index buffer we can use to not duplicate vertices again

		//Create vertex array to be used with core profile
		unsigned int vao;
		GL_CALL(glGenVertexArrays(1, &vao));
		GL_CALL(glBindVertexArray(vao));

		//Define vertex buffer
		VertexBuffer vb(positions, 8 * sizeof(float));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);//specify attributes for each vertex
		//		and bind the index (0) of the current vertex array (vao) to the currently bound array buffer (buffer)

		IndexBuffer ibo(indices, 6); //Index buffer object


		//Write shader source code
		std::string vertexShader, fragmentShader;
		parseShader((std::string)("res/shaders/basic.shader"), vertexShader, fragmentShader);

		unsigned int shaders = CreateShader(vertexShader, fragmentShader);
		glUseProgram(shaders);
		//Setting values for u_Color defined in shader code.
		int u_ColorLocation = glGetUniformLocation(shaders, "u_Color");//Get uniform color location
		ASSERT(u_ColorLocation != -1);//assert location is found
		float d = 0.01f;
		float c[3]{ 0.6, 0.3, 0.8 };
		float incr[3] = { 0.0005, 0.0002, 0.0003 };

		//Unbind buffers and shaders
		GL_CALL(glBindVertexArray(0));
		GL_CALL(glUseProgram(0));
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			//Bind shaders and buffers
			GL_CALL(glUseProgram(shaders));
			/*GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
			//If any changes in occur in vertices ,we have to do this. To avoid this we can use different vertex arrays
			//	for different vertices and bind the vertex arrays as needed.
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);//This binds the specification of the vertex to vertex array that's enabled.*/
			GL_CALL(glBindVertexArray(vao));
			ibo.Bind();

			GL_CALL(glUniform4f(u_ColorLocation, c[0], c[1], c[2], 1.0f));
			for (int i = 0; i < 3; i++)
			{
				if ((c[i] + incr[i]) > 1.0 || (c[i] + incr[i]) < 0) incr[i] *= -1;
				c[i] += incr[i];
			}

			//glDrawArrays(GL_TRIANGLES, 0, 3);
			GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));//Draw triangles from indices bound to gl_element_array_buffer
			//while (GLenum error = glGetError()) std::cout << "Error : " << error << std::endl;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
		glDeleteProgram(shaders);
		//glDetachShader(program, fs);
	}
	glfwTerminate();
	//getchar();
	return 0;
}