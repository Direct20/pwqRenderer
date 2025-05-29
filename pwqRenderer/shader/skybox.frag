#version 330 core
#extension GL_NV_shadow_samplers_cube:enable
uniform samplerCube tex;
in vec3 texVec;//取样向量
out vec4 fragOut;
void main()
{
	fragOut=textureCube(tex,texVec);
}