#pragma once
#include "renderer.h"
class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size);
	void Bind();
	void Unbind();
	~VertexBuffer();
};

