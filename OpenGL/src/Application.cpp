#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
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

	std::cout << glGetString(GL_VERSION);
	float positions[6] = { -0.5f, -0.5f, 0.5f, -0.5f, 0.3f, 0.3f };
	//Define vertex buffer
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);//By binding OpenGL can work with this buffer now
	glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), positions, GL_STATIC_DRAW);//Adding data to the buffer

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);//specify attributes for each vertex
	
	//Write shader source code
	std::string vertexShader, fragmentShader;
	parseShader((std::string)("res/shaders/basic.shader"), vertexShader, fragmentShader);

	unsigned int shaders = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shaders);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		/*glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.5f, -0.5f);
		glVertex2f(0.3f, 0.3f);
		glEnd();*/

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glDeleteProgram(shaders);
	//glDetachShader(program, fs);

	glfwTerminate();
	return 0;
}