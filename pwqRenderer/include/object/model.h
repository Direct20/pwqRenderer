#pragma once
#include "object/object_base.h"
#include "shader/shader_program.h"
#include <string>
#include <assimp/Importer.hpp>
#include "misc/vector_manager.hpp"
#include "misc/queue_manager.hpp"
#include "misc/map_manager.hpp"
#include "object.h"
#include "animation/bone.h"
#include "animation/animation.h"
//////////////////////////////////////////////////////////////////////////////
//ģ����
class FModel :public FBaseObject
{
public:
	using MeshType = FShaderProgram::MeshType;
public:
	FModel(const std::string& _path)
	{
		load(_path);
	}
	virtual ~FModel() {}
public:
	//����ģ��
	void load(const std::string& _path);

	//@brief ����ģ��
	//@param _type ��������
	void analyse(MeshType _type = MeshType::meshStatic)
	{
		meshType = _type;
		_analyseMeshes(_type);
		_analyseAnimations(_type);
	}
	//��̬��ʽ����ģ��
	void draw(FShaderProgram& _sp);

	//�Ǽ������巽ʽ����ģ�͵�_idAnimation�����������еĵ�_idFrame֡����
	void draw(FShaderProgram& _sp, GLuint _idAnimation, GLdouble _idFrame);

	//���Ź�������
	void play(FShaderProgram& _sp, GLuint _idAnimation, GLdouble _time);

protected:
	//���Ź�������
	void _play(FShaderProgram& _sp, GLuint _idAnimation, GLdouble _time);

	//ת��Ϊ��_idAnimation�����������еĵ�_idFrame֡����
	void _pose(FShaderProgram& _sp, GLuint _idAnimation, GLdouble _idFrame);

	//������������
	void _analyseMeshes(MeshType _type);

	//������������
	void _analyseAnimations(MeshType _type);
public:
	FModel(FModel& _x) = delete;
	void operator=(FModel& _x) = delete;
public:
	//������(ֻ��)
	Assimp::Importer imp;
	//��������(ֻ��)
	const aiScene* pScene = nullptr;
	//ģ������Ŀ¼(ֻ��)
	std::string dir = "";
	//ģ����չ��(ֻ��)
	std::string ext = "";
	//ģ�������������(ֻ��)
	MeshType meshType = MeshType::meshStatic;
	//����(ֻ��)
	FManagerVector<FObject> meshes;
	//������(ֻ��)
	FManagerMap<std::string, FBone> bones;
	//������(ֻ��)
	FManagerVector<FAnimation> animations;
protected:
	//��һ������ID
	GLuint _nextBoneID = 0;
};