#version 330 core
layout(location=0) in vec3 aPos;
struct Transf
{
	mat4 model;
	mat4 view;
	mat4 projection;
};
uniform Transf trans;
out vec3 texVec;
void main()
{
	vec4 tmp=trans.projection*trans.view*trans.model*vec4(aPos,1.0);
	texVec=aPos;
	gl_Position=tmp.xyww;//令天空盒顶点深度为最大值
}