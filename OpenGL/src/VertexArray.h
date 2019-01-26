#pragma once
#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
class VertexArray
{
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)const;
	void Bind() const;
	void Unbind() const;
};

