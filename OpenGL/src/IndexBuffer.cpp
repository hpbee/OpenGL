#include "IndexBuffer.h"



IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count):m_Count(count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));
	GL_CALL(glGenBuffers(1, &m_RendererID));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));//By binding OpenGL can work with this buffer now
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW));//Adding data to the buffer
}


void IndexBuffer::Bind() const
{
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

IndexBuffer::~IndexBuffer()
{
	GL_CALL(glDeleteBuffers(1, &m_RendererID));
}
