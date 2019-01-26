#include "renderer.h"
#include <iostream>
void glClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}
bool glLogError(std::string function, std::string file, long line)
{
	if (GLenum error = glGetError())
	{
		std::cout << "OpenGL error : " << error << " in line " << line << " in function " << function << " in file " << file << "\n";
		return true;
	}
	return false;
}

renderer::renderer()
{
}


renderer::~renderer()
{
}
