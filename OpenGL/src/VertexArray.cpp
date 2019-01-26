#include "VertexArray.h"


VertexArray::VertexArray()
{
	GL_CALL(glGenVertexArrays(1, &m_RendererID));
}


VertexArray::~VertexArray()
{
	GL_CALL(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout & layout)const
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GL_CALL(glEnableVertexAttribArray(i));
		GL_CALL(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		/*specify attributes for each vertex and bind the index (i) of the 
		current vertex array to the currently bound array buffer (vb)*/
		offset += element.count * element.GetSizeOfType(element.type);
	}
	Unbind();
	
}

void VertexArray::Bind() const
{
	GL_CALL(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GL_CALL(glBindVertexArray(0));
}
