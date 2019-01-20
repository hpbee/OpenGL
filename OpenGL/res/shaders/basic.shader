#shader GL_VERTEX_SHADER

#version 330 core
	layout ( location = 0) in vec4 position;
	void main()
	{
		gl_Position = position;		
	};


#shader GL_FRAGMENT_SHADER

#version 330 core
	layout ( location = 0) out vec4 color;
	uniform vec4 u_Color;
	void main()
	{
		color = u_Color;/*vec4(0.5,1.0,0.8,1.0)*/// Will be set in Application.cpp
	};