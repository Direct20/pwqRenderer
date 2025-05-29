#version 330 core
in vec4 fragPos;
uniform vec3 lightPos;
uniform float farMnear;
void main()
{
	float dist=length(fragPos.xyz-lightPos);
	dist/=farMnear;
	gl_FragDepth=dist;
}