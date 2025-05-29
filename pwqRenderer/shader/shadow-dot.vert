#version 330 core
layout(location=0) in vec3 aPos;
struct Transf
{
	mat4 model;
	mat4 projection;
};
uniform Transf trans;
void main()
{
	gl_Position=trans.model*vec4(aPos,1.0);
}