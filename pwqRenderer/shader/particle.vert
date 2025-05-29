#version 330 core
layout(location=0) in vec3 aPos;
struct Transform
{
	mat4 model;
	mat4 view;
	mat4 projection;
};
uniform Transform trans;
//out vec2 texCoord;
void main()
{
	gl_Position=trans.projection*trans.view*trans.model*vec4(aPos,1.);
	//texCoord=vec2(0.5f,0.5f);
}