#version 330 core
#extension GL_NV_shadow_samplers_cube:enable
uniform samplerCube tex;
in vec3 texVec;//ȡ������
out vec4 fragOut;
void main()
{
	fragOut=textureCube(tex,texVec);
}