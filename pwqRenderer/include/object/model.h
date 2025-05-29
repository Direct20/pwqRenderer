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
//模型类
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
	//加载模型
	void load(const std::string& _path);

	//@brief 解析模型
	//@param _type 解析类型
	void analyse(MeshType _type = MeshType::meshStatic)
	{
		meshType = _type;
		_analyseMeshes(_type);
		_analyseAnimations(_type);
	}
	//静态方式绘制模型
	void draw(FShaderProgram& _sp);

	//骨架网格体方式绘制模型的_idAnimation个动画序列中的第_idFrame帧动画
	void draw(FShaderProgram& _sp, GLuint _idAnimation, GLdouble _idFrame);

	//播放骨骼动画
	void play(FShaderProgram& _sp, GLuint _idAnimation, GLdouble _time);

protected:
	//播放骨骼动画
	void _play(FShaderProgram& _sp, GLuint _idAnimation, GLdouble _time);

	//转变为第_idAnimation个动画序列中的第_idFrame帧动画
	void _pose(FShaderProgram& _sp, GLuint _idAnimation, GLdouble _idFrame);

	//解析网格数据
	void _analyseMeshes(MeshType _type);

	//解析动画数据
	void _analyseAnimations(MeshType _type);
public:
	FModel(FModel& _x) = delete;
	void operator=(FModel& _x) = delete;
public:
	//加载器(只读)
	Assimp::Importer imp;
	//场景数据(只读)
	const aiScene* pScene = nullptr;
	//模型所在目录(只读)
	std::string dir = "";
	//模型扩展名(只读)
	std::string ext = "";
	//模型网格解析类型(只读)
	MeshType meshType = MeshType::meshStatic;
	//网格集(只读)
	FManagerVector<FObject> meshes;
	//骨骼集(只读)
	FManagerMap<std::string, FBone> bones;
	//动画集(只读)
	FManagerVector<FAnimation> animations;
protected:
	//下一个骨骼ID
	GLuint _nextBoneID = 0;
};