#pragma once
#include <GL/glew.h>
#include <string>

#define ASSERT(x) if(x==false) __debugbreak()

#define GL_CALL(x) glClearErrors();\
	x;\
	ASSERT(!glLogError(#x,__FILE__,__LINE__))

void glClearErrors();
bool glLogError(std::string function, std::string file, long line);

class renderer
{
public:
	renderer();
	virtual ~renderer();
};

