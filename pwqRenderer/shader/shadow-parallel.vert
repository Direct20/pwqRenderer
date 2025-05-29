#version 330 core
layout(location=0) in vec3 aPos;
layout(location=5) in uvec4 aBoneIndices;
layout(location=6) in vec4 aBoneWeights;
struct Transf
{
	mat4 model;
	mat4 view;
	mat4 projection;
};
uniform Transf trans;
//////////////////////////////////////////////////////////////////////////////
#define MESH_TYPE_STATIC 0
#define MESH_TYPE_SKELETON 1
uniform int meshType=MESH_TYPE_STATIC;//网格体类型 MESH_TYPE_STATIC静态网格体 MESH_TYPE_SKELETON骨架网格体
//////////////////////////////////////////////////////////////////////////////
#define MAX_BONES_COUNT	100
uniform mat4 matBones[MAX_BONES_COUNT];//骨骼变换矩阵
//////////////////////////////////////////////////////////////////////////////
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

	gl_Position=trans.projection*trans.view*trans.model*boneTransf*vec4(aPos,1.0f);
}