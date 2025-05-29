#include "object/model.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <vector>
#include "misc/def1.h"
#include "misc/global_manager.h"

using namespace std;
using namespace glm;
void FModel::load(const std::string& _path)
{
	//����ģ��
	pScene = imp.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_OptimizeMeshes);
	//����ʧ��:
	if (!pScene || !pScene->mRootNode)throw ex("Failed to load the model \"" + _path + "\".");
	//ģ�Ͳ�����:
	if (pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)throw ex("The model \"" + _path + "\" had been damaged.");
	//��ȡģ������·��
	dir = _path.substr(0, _path.find_last_of('/'));
	ext = _path.substr(1 + _path.find_last_of('.'));
}

void FModel::draw(FShaderProgram& _sp)
{
	for (GLuint i = 0; i < meshes.lib.size(); i++)
		meshes.lib[i]->draw_static(_sp, _genModelMatrix(), FObject::dtIndices, GL_TRIANGLES);
}

void FModel::draw(FShaderProgram& _sp, GLuint _idAnimation, GLdouble _idFrame)
{
	_pose(_sp, _idAnimation, _idFrame);
	for (GLuint i = 0; i < meshes.lib.size(); i++)
		meshes.lib[i]->draw(_sp, _genModelMatrix(), FObject::dtIndices, GL_TRIANGLES);
}

void FModel::play(FShaderProgram& _sp, GLuint _idAnimation, GLdouble _time)
{
	_play(_sp, _idAnimation, _time);
	for (GLuint i = 0; i < meshes.lib.size(); i++)
		meshes.lib[i]->draw(_sp, _genModelMatrix(), FObject::dtIndices, GL_TRIANGLES);
}

void FModel::_play(FShaderProgram& _sp, GLuint _idAnimation, GLdouble _time)
{
	if (animations.lib.size() <= _idAnimation)
	{
		war_excep(0, "Invalid animation id.");
		return;
	}
	animations.lib[_idAnimation]->play(_sp, _time);
}

void FModel::_pose(FShaderProgram& _sp, GLuint _idAnimation, GLdouble _idFrame)
{
	if (animations.lib.size() <= _idAnimation)
	{
		war_excep(0, "Invalid animation id.");
		return;
	}
	animations.lib[_idAnimation]->pose(_sp, _idFrame);
}

void FModel::_analyseMeshes(MeshType _type)
{
	//��������:
	for (GLuint i = 0; i < pScene->mNumMeshes; i++)
	{
		aiMesh& currMesh = *pScene->mMeshes[i];
		FObject* pMyMesh = meshes.newOne();
		pMyMesh->meshType = _type;//����������
		//��������:
		{
			//��������
			if (currMesh.HasFaces())
			{
				vector<GLuint> tmp;
				for (GLuint j = 0; j < currMesh.mNumFaces; j++)
					tmp.insert(tmp.begin(), &currMesh.mFaces[j].mIndices[0],
						&currMesh.mFaces[j].mIndices[currMesh.mFaces[j].mNumIndices]);
				pMyMesh->setIndices(tmp.data(), tmp.size() * sizeof(GLuint));
			}
			//����λ��
			if (currMesh.HasPositions())
			{
				pMyMesh->addVertexAttr(FObject::aiPosition, 3, GL_FLOAT, sizeof(float));
				pMyMesh->setVertices(currMesh.mVertices, currMesh.mNumVertices * sizeof(aiVector3D), FObject::aiPosition);
			}
			else
				pMyMesh->addVertexAttr(FObject::aiPosition, 3, GL_FLOAT, 0);
			//���㷨��
			if (currMesh.HasNormals())
			{
				pMyMesh->addVertexAttr(FObject::aiNormal, 3, GL_FLOAT, sizeof(float));
				pMyMesh->setVertices(currMesh.mNormals, currMesh.mNumVertices * sizeof(aiVector3D), FObject::aiNormal);
			}
			else
				pMyMesh->addVertexAttr(FObject::aiNormal, 3, GL_FLOAT, 0);
			//��������
			if (currMesh.HasTangentsAndBitangents())
			{
				pMyMesh->addVertexAttr(FObject::aiTangent, 3, GL_FLOAT, sizeof(float));
				pMyMesh->setVertices(currMesh.mTangents, currMesh.mNumVertices * sizeof(aiVector3D), FObject::aiTangent);
			}
			else
				pMyMesh->addVertexAttr(FObject::aiTangent, 3, GL_FLOAT, 0);
			//������������
			if (currMesh.HasTextureCoords(0))
			{
				pMyMesh->addVertexAttr(FObject::aiTexCoord, 3, GL_FLOAT, sizeof(float));
				pMyMesh->setVertices(currMesh.mTextureCoords[0], currMesh.mNumVertices * sizeof(aiVector3D), FObject::aiTexCoord);
			}
			else
				pMyMesh->addVertexAttr(FObject::aiTexCoord, 3, GL_FLOAT, 0);
			//������ɫ
			if (currMesh.HasVertexColors(0))
			{
				pMyMesh->addVertexAttr(FObject::aiColor, 4, GL_FLOAT, sizeof(float));
				pMyMesh->setVertices(currMesh.mColors[0], currMesh.mNumVertices * sizeof(aiColor4D), FObject::aiColor);
			}
			else
				pMyMesh->addVertexAttr(FObject::aiColor, 4, GL_FLOAT, 0);

			if (_type == MeshType::meshSkeleton)//�ԹǼ����������
			{
				//��������
				if (currMesh.HasBones())
				{
					//�Զ�����Ӱ��Ĺ�����ID
					uvec4* idBone = new uvec4[currMesh.mNumVertices];
					memset(idBone, 0, sizeof(uvec4) * currMesh.mNumVertices);//��Ϊ��Ч
					//�����Զ����Ӱ��Ȩ��
					vec4* wBone = new vec4[currMesh.mNumVertices];
					memset(wBone, 0, sizeof(vec4) * currMesh.mNumVertices);//��Ϊ��Ч

					for (GLuint j = 0; j < currMesh.mNumBones; j++)
					{
						aiBone& currBone = *currMesh.mBones[j];
						FBone* pMyBone = bones.newOne(currBone.mName.C_Str());
						if (!pMyBone)continue;//�Ѿ����ڵĹ���
						//������������:
						pMyBone->id = _nextBoneID++;
						pMyBone->name = currBone.mName.C_Str();
						pMyBone->matOffset = convert(currBone.mOffsetMatrix);
						//Ȩ������:
						for (GLuint k = 0; k < currBone.mNumWeights; k++)
						{
							GLuint verID = currBone.mWeights[k].mVertexId;//��Ӱ��Ķ����id
							GLfloat verWeight = currBone.mWeights[k].mWeight;//Ӱ��Ȩ��

							for (GLuint t = 0; t < F_MAX_VERTEX_BONE_INDEX; t++)
								if (wBone[verID][t] == 0)//δ��ʹ�õĶ����
								{
									idBone[verID][t] = pMyBone->id;//��0��ʼ
									wBone[verID][t] = verWeight;
									break;
								}
						}
					}
					//��������:
					pMyMesh->addVertexAttr(FObject::aiBoneIndex, 4, GL_UNSIGNED_INT, sizeof(GLuint));
					pMyMesh->setVertices(idBone, sizeof(uvec4) * currMesh.mNumVertices, FObject::aiBoneIndex);
					pMyMesh->addVertexAttr(FObject::aiBoneWeight, 4, GL_FLOAT, sizeof(GLfloat));
					pMyMesh->setVertices(wBone, sizeof(vec4) * currMesh.mNumVertices, FObject::aiBoneWeight);

					delete[] idBone;
					delete[] wBone;
				}
				else
				{
					pMyMesh->addVertexAttr(FObject::aiBoneIndex, 4, GL_UNSIGNED_INT, 0);
					pMyMesh->addVertexAttr(FObject::aiBoneWeight, 4, GL_FLOAT, 0);
				}
			}
		};
		//��������:
		{
			aiMaterial& currMat = *pScene->mMaterials[currMesh.mMaterialIndex];
			FMaterial myMat;

			//��ɫ����:
			{
				aiColor3D clr;

				//����������ɫ:
				clr = aiColor3D(0.f);
				currMat.Get(AI_MATKEY_COLOR_AMBIENT, clr);
				myMat.clr[FMaterial::ctAmbient] = convert(clr);
				//��������ɫ:
				clr = aiColor3D(0.f);
				currMat.Get(AI_MATKEY_COLOR_DIFFUSE, clr);
				myMat.clr[FMaterial::ctDiffuse] = convert(clr);
				//����߹ⷴ����ɫ:
				clr = aiColor3D(0.f);
				currMat.Get(AI_MATKEY_COLOR_SPECULAR, clr);
				myMat.clr[FMaterial::ctSpecular] = convert(clr);
				//�Է�����ɫ:
				clr = aiColor3D(0.f);
				currMat.Get(AI_MATKEY_COLOR_EMISSIVE, clr);
				myMat.clr[FMaterial::ctEmissive] = convert(clr);
			}
			//ֵ����:
			{
				float val;

				//�����:
				val = 32.f;
				currMat.Get(AI_MATKEY_SHININESS, val);
				myMat.shininess = val;
				//�߹�ǿ��:
				val = 1.f;
				currMat.Get(AI_MATKEY_SHININESS_STRENGTH, val);
				myMat.shininessStrength = val;
				//�Է���ǿ��:
				val = 1.f;
				currMat.Get(AI_MATKEY_EMISSIVE_INTENSITY, val);
				myMat.emissiveIntensity = val;
				//��͸����:
				val = 1.f;
				currMat.Get(AI_MATKEY_OPACITY, val);
				myMat.opacity = val;
			}
			//��������:
			{
				using tt = FTexture2D::TextureType;
				aiTextureType aiTexList[myMat._cntTex] = { aiTextureType_AMBIENT,aiTextureType_DIFFUSE,
					aiTextureType_SPECULAR,aiTextureType_EMISSIVE,
					aiTextureType_NORMALS,aiTextureType_SHININESS,
					aiTextureType_OPACITY };
				tt myTexList[myMat._cntTex] = { tt::ttAmbient,tt::ttDiffuse,tt::ttSpecular,
					tt::ttEmissive,tt::ttNormal,tt::ttShininess,tt::ttOpacity };

				if (ext == "obj")aiTexList[4] = aiTextureType_HEIGHT;//obj�ļ�������ͼ����

				const GLuint _cntMode = 4;
				using wt = FTexture2D::WrapType;
				aiTextureMapMode aiModeList[_cntMode] = { aiTextureMapMode_Clamp,aiTextureMapMode_Decal,
					aiTextureMapMode_Mirror,aiTextureMapMode_Wrap };
				wt myModeList[_cntMode] = { wt::wtClampToEdge,wt::wtClampToBorder,
					wt::wtMirror,wt::wtRepeat };

				for (GLuint j = 0; j < myMat._cntTex; j++)
				{
					const aiTexture* pCurrTex;
					FTexture2D* pMyTex = FGlobalManager::get_instance().g_mgrTexture.newOne();
					//��������:
					{
						aiString path;
						currMat.Get(AI_MATKEY_TEXTURE(aiTexList[j], 0), path);//��ȡ����·��
						if (path.length == 0)//û�д�����
						{
							myMat.hasTex[myTexList[j]] = false;
							continue;
						}
						else
							myMat.hasTex[myTexList[j]] = true;


						std::string finalPath = std::string(path.C_Str()).find(':') == std::string::npos ?
							dir + '/' + path.C_Str() : path.C_Str();//����·��

						pCurrTex = pScene->GetEmbeddedTexture(path.C_Str());//����·�������Ƿ�ΪǶ��ʽ����
						if (pCurrTex)//Ƕ��ʽ
						{
							if (pCurrTex->mHeight == 0)//��ѹ��
							{
								FImage* pImg = FGlobalManager::get_instance().g_mgrImage.newOne();
								pImg->load(pCurrTex->pcData, pCurrTex->mWidth, finalPath);
								pMyTex->load(pImg, myTexList[j]);
							}
							else//δѹ��
							{
								pMyTex->load(pCurrTex->pcData,
									pCurrTex->mWidth, pCurrTex->mHeight, finalPath, myTexList[j]);
							}
						}
						else//�ⲿ
						{
							//�����Ƿ��Ѿ����ع���ͼ��:
							bool loaded = false;
							GLuint k = 0;
							for (; k < FGlobalManager::get_instance().g_mgrImage.lib.size(); k++)
							{
								if (FGlobalManager::get_instance().g_mgrImage.lib[k]->path == finalPath)
								{
									loaded = true;
									break;
								}
							}
							if (loaded)//�Ѿ�����
								pMyTex->load(FGlobalManager::get_instance().g_mgrImage.lib[k], myTexList[j]);
							else//δ����
							{
								FImage* pImg = FGlobalManager::get_instance().g_mgrImage.newOne();
								pImg->load(finalPath);
								pMyTex->load(pImg, myTexList[j]);
							}
						}
					}
					//ƽ��ģʽ:
					{
						aiTextureMapMode aiMapMode[2];
						currMat.Get(AI_MATKEY_MAPPINGMODE_U(aiTexList[j], 0), aiMapMode[0]);//U��
						currMat.Get(AI_MATKEY_MAPPINGMODE_V(aiTexList[j], 0), aiMapMode[1]);//V��

						wt x = wt::wtRepeat, y = wt::wtRepeat;
						for (GLuint k = 0; k < _cntMode; k++)
						{
							if (aiModeList[k] == aiMapMode[0])
								x = myModeList[k];
							if (aiModeList[k] == aiMapMode[1])
								y = myModeList[k];
						}
						pMyTex->setWrap(x, y);
					}
					myMat.tex[myTexList[j]] = pMyTex;
				}
			}
			pMyMesh->mtl = myMat;
		}
	}
}

void FModel::_analyseAnimations(MeshType _type)
{
	if (_type == MeshType::meshSkeleton)//���Ǽ����������
	{
		//��������:
		for (GLuint i = 0; i < pScene->mNumAnimations; i++)//ÿ����������
		{
			aiAnimation& currAnimation = *pScene->mAnimations[i];
			FAnimation* pMyAnimation = animations.newOne();

			GLdouble cntFrame = currAnimation.mDuration;//��֡��
			GLdouble fps = currAnimation.mTicksPerSecond ? currAnimation.mTicksPerSecond : 25.;//֡��

			pMyAnimation->_set(currAnimation.mName.C_Str(), pScene->mRootNode, &bones.m, &currAnimation, fps, cntFrame);
		}
	}
}
