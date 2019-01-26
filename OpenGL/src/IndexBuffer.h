#pragma once
#include "renderer.h"
class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;

public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	void Bind()const;
	void Unbind() const;
	~IndexBuffer();
};

