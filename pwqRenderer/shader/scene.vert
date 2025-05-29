#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec3 aTexCoord;
layout(location = 3) in vec3 aNormal;
layout(location = 4) in vec3 aTangent;
layout(location=5) in uvec4 aBoneIndices;
layout(location=6) in vec4 aBoneWeights;
//////////////////////////////////////////////////////////////////////////////
#define MESH_TYPE_STATIC 0
#define MESH_TYPE_SKELETON 1
uniform int meshType=MESH_TYPE_STATIC;//���������� MESH_TYPE_STATIC��̬������ MESH_TYPE_SKELETON�Ǽ�������
//////////////////////////////////////////////////////////////////////////////
//�任����
struct TransMatrices
{ 
	//ģ��
	mat4 model;
	//��ͼ
	mat4 view;
	//ͶӰ
	mat4 projection;
};
//////////////////////////////////////////////////////////////////////////////
uniform TransMatrices trans;//�任����
//////////////////////////////////////////////////////////////////////////////
#define MAX_BONES_COUNT	100
uniform mat4 matBones[MAX_BONES_COUNT];//�����任����
//////////////////////////////////////////////////////////////////////////////
out VS_OUT
{
	vec3 fragPos;
	vec4 fragColor;
	vec2 fragTexCoord;
	vec3 fragNormal;
	vec3 fragTangent;
	mat3 TBN;
}vsout;
void main()
{

	mat4 boneTransf=mat4(1.0f);
	if(meshType==MESH_TYPE_SKELETON)
	{
		boneTransf=matBones[aBoneIndices.x]*aBoneWeights.x;
		boneTransf+=matBones[aBoneIndices.y]*aBoneWeights.y;
		boneTransf+=matBones[aBoneIndices.z]*aBoneWeights.z;
		boneTransf+=matBones[aBoneIndices.w]*aBoneWeights.w;
	}
	
	vsout.fragPos = (trans.model *boneTransf* vec4(aPos, 1.)).xyz;
	vsout.fragColor=aColor;
	vsout.fragTexCoord=aTexCoord.xy;
	vsout.fragNormal=(trans.model *boneTransf* vec4(aNormal,0.)).xyz;
	vsout.fragTangent=(trans.model * boneTransf*vec4(aTangent,0.)).xyz;

	vec3 T=normalize(vsout.fragTangent);
	vec3 N=normalize(vsout.fragNormal);
	T = normalize(T - dot(T, N) * N);
	vec3 B=cross(T,N);
	vsout.TBN=mat3(T,B,N);
	
	gl_Position = trans.projection * trans.view * trans.model *boneTransf* vec4(aPos, 1.);
}