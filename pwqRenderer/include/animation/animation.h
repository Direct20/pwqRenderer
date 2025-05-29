//////////////////////////////////////////////////////////////////////////////
//animation.h - ����
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
//������(����һ����������)
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
	//@brief ���Ŷ���
	//@param _time ��������ʱ��
	void play(FShaderProgram& _sp, GLdouble _time)
	{
		if (_prevTime == 0.) { _prevTime = _time; return; }
		GLdouble dt = _time - _prevTime;
		if (dt < (1.0 / fps))return;
		_prevTime = _time;
		realFps = 1.0 / dt;//ʵ��FPS
		if (currFrame >= cntFrame)currFrame = 0.;
		pose(_sp, currFrame);
		currFrame += dt * fps;
	}
	//��ʾ��_frameID֡����
	void pose(FShaderProgram& _sp, GLdouble _frameID)
	{
		assert(pRoot != nullptr && pAnimation != nullptr && pBones != nullptr);
		_transform(_frameID);
		_setUniformData(_sp);
	}
	//���ö�������
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
	//����Uniform��������
	void _setUniformData(FShaderProgram& _sp)
	{
		char name[40];
		for (auto iter = pBones->begin(); iter != pBones->end(); iter++)
		{
			sprintf_s(name, F_UNAME_BONES_TRANSF, iter->second->id);
			_sp._setMat4(name, iter->second->matFinal);
		}
	}
	//���ɵ�_frameID֡�Ĺ����任����
	void _transform(GLdouble _frameID)
	{
		_matGlobalInv = inverse(convert(pRoot->mTransformation));

		_frameID = fmod(_frameID, cntFrame);

		aiMatrix4x4 I;//��λ����
		_readTree(pRoot, I, _frameID);
	}
	//����������
	void _readTree(const aiNode* _pNode, const aiMatrix4x4& _parent, GLdouble _frameID)
	{
		std::string nodeName(_pNode->mName.C_Str());//�ڵ�����
		aiMatrix4x4 nodeTransf(_pNode->mTransformation);//�ڵ�任����
		const aiNodeAnim* pNodeAnim = _getNodeAnim(_pNode);//�ýڵ��еĶ�������
		//��ֵ��ȡ��_frameID�Ķ����任����:
		if (pNodeAnim)//�ýڵ��ǹ����Ҹù����ж�������
		{
			aiVector3D Translation = _interpolateTranslation(pNodeAnim, _frameID);
			aiQuaternion RotationQ = _interpolateRotation(pNodeAnim, _frameID);
			aiVector3D Scaling = _interpolateScaling(pNodeAnim, _frameID);
			nodeTransf = aiMatrix4x4(Scaling, RotationQ, Translation);//�ϳɾ���
		}
		aiMatrix4x4 GlobalTransformation = _parent * nodeTransf;
		if (pBones->find(nodeName) != pBones->end()) //�ýڵ��ǹ���
		{
			FBone* p = pBones->at(nodeName);//��������ָ��
			p->matFinal = _matGlobalInv * convert(_parent) * convert(nodeTransf) * p->matOffset;
		}
		//�����ӽڵ�:
		for (GLuint i = 0; i < _pNode->mNumChildren; i++)
			_readTree(_pNode->mChildren[i], GlobalTransformation, _frameID);
	}
	//��ȡ�ڵ�_pNode�Ķ�������
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
	//ƽ�Ʋ�ֵ
	aiVector3D& _interpolateTranslation(const aiNodeAnim* _pAnim, GLdouble _frameID)
	{
		static aiVector3D ret;
		//ֻ��һ֡����:
		if (_pAnim->mNumPositionKeys == 1)
			return ret = _pAnim->mPositionKeys[0].mValue;//���ص�һ�ؼ���
		//��ȡǰ��ؼ������������:
		GLuint prevKeyIndex = _prevKeyTranslation(_pAnim, _frameID);//ǰһ�ؼ���
		GLuint nextKeyIndex = prevKeyIndex + 1;//��һ�ؼ���
		assert(nextKeyIndex < _pAnim->mNumPositionKeys);
		//�����ֵ����:
		GLdouble dt = _pAnim->mPositionKeys[nextKeyIndex].mTime - _pAnim->mPositionKeys[prevKeyIndex].mTime;
		GLfloat factor = (GLfloat)((_frameID - _pAnim->mPositionKeys[prevKeyIndex].mTime) / dt);
		assert(factor >= 0.0f && factor <= 1.0f);
		const aiVector3D& prevKeyVal = _pAnim->mPositionKeys[prevKeyIndex].mValue;//ǰһ֡ƽ��
		const aiVector3D& nextKeyVal = _pAnim->mPositionKeys[nextKeyIndex].mValue;//��һ֡ƽ��

		//ret = prevKeyVal * factor + nextKeyVal * (1 - factor);//��ֵ
		ret = prevKeyVal + factor * (nextKeyVal - prevKeyVal);//��ֵ
		return ret;
	}
	//��ת��ֵ
	aiQuaternion& _interpolateRotation(const aiNodeAnim* _pAnim, GLdouble _frameID)
	{
		static aiQuaternion ret;
		//ֻ��һ֡����:
		if (_pAnim->mNumRotationKeys == 1)
			return ret = _pAnim->mRotationKeys[0].mValue;//���ص�һ֡
		//��ȡǰ��ؼ������������:
		GLuint prevKeyIndex = _prevKeyRotation(_pAnim, _frameID);//ǰһ�ؼ���
		GLuint nextKeyIndex = prevKeyIndex + 1;//��һ�ؼ���
		assert(nextKeyIndex < _pAnim->mNumRotationKeys);
		//�����ֵ����:
		GLdouble dt = _pAnim->mRotationKeys[nextKeyIndex].mTime - _pAnim->mRotationKeys[prevKeyIndex].mTime;
		GLfloat factor = (GLfloat)((_frameID - _pAnim->mRotationKeys[prevKeyIndex].mTime) / dt);
		assert(factor >= 0.0f && factor <= 1.0f);
		const aiQuaternion& prevKeyVal = _pAnim->mRotationKeys[prevKeyIndex].mValue;//ǰһ�ؼ�����ת����
		const aiQuaternion& nextKeyVal = _pAnim->mRotationKeys[nextKeyIndex].mValue;//��һ�ؼ�����ת����

		aiQuaternion::Interpolate(ret, prevKeyVal, nextKeyVal, factor);//����ֵ
		ret = ret.Normalize();//��׼��
		return ret;
	}
	//���Ų�ֵ
	aiVector3D& _interpolateScaling(const aiNodeAnim* _pAnim, GLdouble _frameID)
	{
		static aiVector3D ret;
		//ֻ��һ֡����:
		if (_pAnim->mNumScalingKeys == 1)
			return ret = _pAnim->mScalingKeys[0].mValue;//���ص�һ�ؼ���
		//��ȡǰ��ؼ������������:
		GLuint prevKeyIndex = _prevKeyScaling(_pAnim, _frameID);//ǰһ�ؼ���
		GLuint nextKeyIndex = prevKeyIndex + 1;//��һ�ؼ���
		assert(nextKeyIndex < _pAnim->mNumScalingKeys);
		//�����ֵ����:
		GLdouble dt = _pAnim->mScalingKeys[nextKeyIndex].mTime - _pAnim->mScalingKeys[prevKeyIndex].mTime;
		GLfloat factor = (GLfloat)((_frameID - _pAnim->mScalingKeys[prevKeyIndex].mTime) / dt);
		assert(factor >= 0.0f && factor <= 1.0f);
		const aiVector3D& prevKeyVal = _pAnim->mScalingKeys[prevKeyIndex].mValue;//ǰһ֡����
		const aiVector3D& nextKeyVal = _pAnim->mScalingKeys[nextKeyIndex].mValue;//��һ֡����

		//ret = prevKeyVal * factor + nextKeyVal * (1 - factor);//��ֵ
		ret = prevKeyVal + factor * (nextKeyVal - prevKeyVal);//��ֵ
		return ret;
	}
	//��ȡ_frameID��ǰһ�ؼ������������
	GLuint _prevKeyTranslation(const aiNodeAnim* _pAnim, GLdouble _frameID)
	{
		assert(_pAnim->mNumPositionKeys > 0);
		for (GLuint i = 0; i < _pAnim->mNumPositionKeys - 1; i++)
			if (_frameID < _pAnim->mPositionKeys[i + 1].mTime)
				return i;
		assert(0);
		return 0;
	}
	//��ȡ_frameID��ǰһ�ؼ������������
	GLuint _prevKeyRotation(const aiNodeAnim* _pAnim, GLdouble _frameID)
	{
		assert(_pAnim->mNumRotationKeys > 0);
		for (GLuint i = 0; i < _pAnim->mNumRotationKeys - 1; i++)
			if (_frameID < _pAnim->mRotationKeys[i + 1].mTime)
				return i;
		assert(0);
		return 0;
	}
	//��ȡ_frameID��ǰһ�ؼ������������
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