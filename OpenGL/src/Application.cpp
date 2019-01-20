#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>

void glClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}
bool glLogError(std::string function, std::string file, long line)
{
	if (GLenum error = glGetError())
	{
		std::cout << "OpenGL error : " << error << " in line " << line << " in function " << function << " in file " << file << "\n";
		return false;
	}
	return true;
}

#define ASSERT(x) if(!x) __debugbreak()

#define GL_CALL(x) glClearErrors();\
	x;\
	ASSERT(glLogError(#x,__FILE__,__LINE__))

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
	//Define vertex buffer
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);//By binding OpenGL can work with this buffer now
	glBufferData(GL_ARRAY_BUFFER, 8*sizeof(float), positions, GL_STATIC_DRAW);//Adding data to the buffer

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);//specify attributes for each vertex

	unsigned int ibo; //Index buffer object
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);//By binding OpenGL can work with this ibo now
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);//Adding data to the buffer
	
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

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		GL_CALL(glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,NULL));//Draw triangles from indices bound to gl_element_array_buffer
		//while (GLenum error = glGetError()) std::cout << "Error : " << error << std::endl;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glDeleteProgram(shaders);
	//glDetachShader(program, fs);

	glfwTerminate();
	getchar();
	return 0;
}