//////////////////////////////////////////////////////////////////////////////
//animation.h - 动画
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "base/base.h"
#include "animation/bone.h"
#include <string>
#include <map>
#include <glm/glm.hpp>
#include "misc/convert.hpp"
#include <assimp/scene.h>
#include "shader/shader_program.h"
//////////////////////////////////////////////////////////////////////////////
//动画类(管理一个动画序列)
F_REPLICABLE class FAnimation
{
public:
	FAnimation()
	{
	}
	FAnimation(const std::string& _name, const aiNode* _pRootNode, const std::map<std::string, FBone*>* _pBones,
		const aiAnimation* _pAnimation, GLdouble _fps, GLdouble _cntFrame)
	{
		_set(_name, _pRootNode, _pBones, _pAnimation, _fps, _cntFrame);
	}
	virtual ~FAnimation()
	{
	}
public:
	//@brief 播放动画
	//@param _time 程序运行时间
	void play(FShaderProgram& _sp, GLdouble _time)
	{
		if (_prevTime == 0.) { _prevTime = _time; return; }
		GLdouble dt = _time - _prevTime;
		if (dt < (1.0 / fps))return;
		_prevTime = _time;
		realFps = 1.0 / dt;//实际FPS
		if (currFrame >= cntFrame)currFrame = 0.;
		pose(_sp, currFrame);
		currFrame += dt * fps;
	}
	//显示第_frameID帧动画
	void pose(FShaderProgram& _sp, GLdouble _frameID)
	{
		assert(pRoot != nullptr && pAnimation != nullptr && pBones != nullptr);
		_transform(_frameID);
		_setUniformData(_sp);
	}
	//设置动画数据
	void _set(const std::string& _name, const aiNode* _pRootNode, const std::map<std::string, FBone*>* _pBones,
		const aiAnimation* _pAnimation, GLdouble _fps, GLdouble _cntFrame)
	{
		name = _name;
		pRoot = _pRootNode;
		pBones = _pBones;
		pAnimation = _pAnimation;
		fps = _fps;
		cntFrame = _cntFrame;
	}
protected:
	//设置Uniform变量数据
	void _setUniformData(FShaderProgram& _sp)
	{
		char name[40];
		for (auto iter = pBones->begin(); iter != pBones->end(); iter++)
		{
			sprintf_s(name, F_UNAME_BONES_TRANSF, iter->second->id);
			_sp._setMat4(name, iter->second->matFinal);
		}
	}
	//生成第_frameID帧的骨骼变换矩阵
	void _transform(GLdouble _frameID)
	{
		_matGlobalInv = inverse(convert(pRoot->mTransformation));

		_frameID = fmod(_frameID, cntFrame);

		aiMatrix4x4 I;//单位矩阵
		_readTree(pRoot, I, _frameID);
	}
	//遍历场景树
	void _readTree(const aiNode* _pNode, const aiMatrix4x4& _parent, GLdouble _frameID)
	{
		std::string nodeName(_pNode->mName.C_Str());//节点名称
		aiMatrix4x4 nodeTransf(_pNode->mTransformation);//节点变换矩阵
		const aiNodeAnim* pNodeAnim = _getNodeAnim(_pNode);//该节点中的动画数据
		//插值获取第_frameID的动画变换矩阵:
		if (pNodeAnim)//该节点是骨骼且该骨骼有动画数据
		{
			aiVector3D Translation = _interpolateTranslation(pNodeAnim, _frameID);
			aiQuaternion RotationQ = _interpolateRotation(pNodeAnim, _frameID);
			aiVector3D Scaling = _interpolateScaling(pNodeAnim, _frameID);
			nodeTransf = aiMatrix4x4(Scaling, RotationQ, Translation);//合成矩阵
		}
		aiMatrix4x4 GlobalTransformation = _parent * nodeTransf;
		if (pBones->find(nodeName) != pBones->end()) //该节点是骨骼
		{
			FBone* p = pBones->at(nodeName);//这块骨骼的指针
			p->matFinal = _matGlobalInv * convert(_parent) * convert(nodeTransf) * p->matOffset;
		}
		//遍历子节点:
		for (GLuint i = 0; i < _pNode->mNumChildren; i++)
			_readTree(_pNode->mChildren[i], GlobalTransformation, _frameID);
	}
	//获取节点_pNode的动画数据
	aiNodeAnim* _getNodeAnim(const aiNode* _pNode)
	{
		aiNodeAnim* p = nullptr;
		for (GLuint i = 0; i < pAnimation->mNumChannels; i++)
			if (pAnimation->mChannels[i]->mNodeName == _pNode->mName)
			{
				p = pAnimation->mChannels[i];
				break;
			}
		return p;
	}
	//平移插值
	aiVector3D& _interpolateTranslation(const aiNodeAnim* _pAnim, GLdouble _frameID)
	{
		static aiVector3D ret;
		//只有一帧动画:
		if (_pAnim->mNumPositionKeys == 1)
			return ret = _pAnim->mPositionKeys[0].mValue;//返回第一关键点
		//获取前后关键点的数组索引:
		GLuint prevKeyIndex = _prevKeyTranslation(_pAnim, _frameID);//前一关键点
		GLuint nextKeyIndex = prevKeyIndex + 1;//后一关键点
		assert(nextKeyIndex < _pAnim->mNumPositionKeys);
		//计算插值参数:
		GLdouble dt = _pAnim->mPositionKeys[nextKeyIndex].mTime - _pAnim->mPositionKeys[prevKeyIndex].mTime;
		GLfloat factor = (GLfloat)((_frameID - _pAnim->mPositionKeys[prevKeyIndex].mTime) / dt);
		assert(factor >= 0.0f && factor <= 1.0f);
		const aiVector3D& prevKeyVal = _pAnim->mPositionKeys[prevKeyIndex].mValue;//前一帧平移
		const aiVector3D& nextKeyVal = _pAnim->mPositionKeys[nextKeyIndex].mValue;//后一帧平移

		//ret = prevKeyVal * factor + nextKeyVal * (1 - factor);//插值
		ret = prevKeyVal + factor * (nextKeyVal - prevKeyVal);//插值
		return ret;
	}
	//旋转插值
	aiQuaternion& _interpolateRotation(const aiNodeAnim* _pAnim, GLdouble _frameID)
	{
		static aiQuaternion ret;
		//只有一帧动画:
		if (_pAnim->mNumRotationKeys == 1)
			return ret = _pAnim->mRotationKeys[0].mValue;//返回第一帧
		//获取前后关键点的数组索引:
		GLuint prevKeyIndex = _prevKeyRotation(_pAnim, _frameID);//前一关键点
		GLuint nextKeyIndex = prevKeyIndex + 1;//后一关键点
		assert(nextKeyIndex < _pAnim->mNumRotationKeys);
		//计算插值参数:
		GLdouble dt = _pAnim->mRotationKeys[nextKeyIndex].mTime - _pAnim->mRotationKeys[prevKeyIndex].mTime;
		GLfloat factor = (GLfloat)((_frameID - _pAnim->mRotationKeys[prevKeyIndex].mTime) / dt);
		assert(factor >= 0.0f && factor <= 1.0f);
		const aiQuaternion& prevKeyVal = _pAnim->mRotationKeys[prevKeyIndex].mValue;//前一关键点旋转数据
		const aiQuaternion& nextKeyVal = _pAnim->mRotationKeys[nextKeyIndex].mValue;//后一关键点旋转数据

		aiQuaternion::Interpolate(ret, prevKeyVal, nextKeyVal, factor);//作插值
		ret = ret.Normalize();//标准化
		return ret;
	}
	//缩放插值
	aiVector3D& _interpolateScaling(const aiNodeAnim* _pAnim, GLdouble _frameID)
	{
		static aiVector3D ret;
		//只有一帧动画:
		if (_pAnim->mNumScalingKeys == 1)
			return ret = _pAnim->mScalingKeys[0].mValue;//返回第一关键点
		//获取前后关键点的数组索引:
		GLuint prevKeyIndex = _prevKeyScaling(_pAnim, _frameID);//前一关键点
		GLuint nextKeyIndex = prevKeyIndex + 1;//后一关键点
		assert(nextKeyIndex < _pAnim->mNumScalingKeys);
		//计算插值参数:
		GLdouble dt = _pAnim->mScalingKeys[nextKeyIndex].mTime - _pAnim->mScalingKeys[prevKeyIndex].mTime;
		GLfloat factor = (GLfloat)((_frameID - _pAnim->mScalingKeys[prevKeyIndex].mTime) / dt);
		assert(factor >= 0.0f && factor <= 1.0f);
		const aiVector3D& prevKeyVal = _pAnim->mScalingKeys[prevKeyIndex].mValue;//前一帧缩放
		const aiVector3D& nextKeyVal = _pAnim->mScalingKeys[nextKeyIndex].mValue;//后一帧缩放

		//ret = prevKeyVal * factor + nextKeyVal * (1 - factor);//插值
		ret = prevKeyVal + factor * (nextKeyVal - prevKeyVal);//插值
		return ret;
	}
	//获取_frameID的前一关键点的数组索引
	GLuint _prevKeyTranslation(const aiNodeAnim* _pAnim, GLdouble _frameID)
	{
		assert(_pAnim->mNumPositionKeys > 0);
		for (GLuint i = 0; i < _pAnim->mNumPositionKeys - 1; i++)
			if (_frameID < _pAnim->mPositionKeys[i + 1].mTime)
				return i;
		assert(0);
		return 0;
	}
	//获取_frameID的前一关键点的数组索引
	GLuint _prevKeyRotation(const aiNodeAnim* _pAnim, GLdouble _frameID)
	{
		assert(_pAnim->mNumRotationKeys > 0);
		for (GLuint i = 0; i < _pAnim->mNumRotationKeys - 1; i++)
			if (_frameID < _pAnim->mRotationKeys[i + 1].mTime)
				return i;
		assert(0);
		return 0;
	}
	//获取_frameID的前一关键点的数组索引
	GLuint _prevKeyScaling(const aiNodeAnim* _pAnim, GLdouble _frameID)
	{
		assert(_pAnim->mNumScalingKeys > 0);
		for (GLuint i = 0; i < _pAnim->mNumScalingKeys - 1; i++)
			if (_frameID < _pAnim->mScalingKeys[i + 1].mTime)
				return i;
		assert(0);
		return 0;
	}
public:
	std::string name = "";
	const aiAnimation* pAnimation = nullptr;
	const aiNode* pRoot = nullptr;
	const std::map<std::string, FBone*>* pBones = nullptr;
	GLdouble cntFrame = 0.;
	GLdouble fps = 25.;

	GLdouble realFps = 0.;
	GLdouble currFrame = 0.;
protected:
	glm::mat4 _matGlobalInv;
	GLdouble _prevTime = 0.;

};