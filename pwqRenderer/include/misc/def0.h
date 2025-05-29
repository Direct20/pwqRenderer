//////////////////////////////////////////////////////////////////////////////
//def0.h - 定义0
//////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _DEF_0_H_
#define _DEF_0_H_
//////////////////////////////////////////////////////////////////////////////
//着色器代码最大长度
#define F_MAX_SHADER_SRC				16384	/*16 KiB*/
//状态信息最大长度
#define F_MAX_STATUS_INFO				2048	/*1KiB*/
//一个顶点最多被几个骨骼影响
#define F_MAX_VERTEX_BONE_INDEX			4
//////////////////////////////////////////////////////////////////////////////
#define F_TEXTURE_START					GL_TEXTURE4		/*0,1,2,3号纹理槽保留用作创建纹理*/
#define F_TEXTURE_START_INDEX			4
//阴影贴图使用的纹理插槽起始值(从16开始)
#define F_TEXTURE_START_LD_TEX_SHADOW			F_TEXTURE_START+12
#define F_TEXTURE_START_INDEX_LD_TEX_SHADOW		F_TEXTURE_START_INDEX+12
#define F_TEXTURE_START_LP_TEX_SHADOW			F_TEXTURE_START_LD_TEX_SHADOW+10
#define F_TEXTURE_START_INDEX_LP_TEX_SHADOW		F_TEXTURE_START_INDEX_LD_TEX_SHADOW+10
#define F_TEXTURE_START_LS_TEX_SHADOW			F_TEXTURE_START_LP_TEX_SHADOW+10
#define F_TEXTURE_START_INDEX_LS_TEX_SHADOW		F_TEXTURE_START_INDEX_LP_TEX_SHADOW+10
//天空盒贴图使用的纹理插槽
#define F_TEXTURE_SKYBOX						F_TEXTURE_START
#define F_TEXTURE_SKYBOX_INDEX					F_TEXTURE_START_INDEX						
//////////////////////////////////////////////////////////////////////////////
#define F_DIR_SHADER							"shader/"

#define F_DIR_RES						"res/"
#define F_DIR_RES_MODEL					F_DIR_RES"model/"
#define F_DIR_RES_MODEL_CHARACTER		F_DIR_RES_MODEL"character/"
#define F_DIR_RES_TEXTURE				F_DIR_RES"texture/"
#define F_DIR_RES_MODEL_MAP				F_DIR_RES_MODEL"map/"
#define F_DIR_RES_MODEL_OBJECT			F_DIR_RES_MODEL"object/"
#define F_DIR_RES_MODEL_WEAPON			F_DIR_RES_MODEL"weapon/"
#define F_DIR_RES_MODEL_WEAPON_1		F_DIR_RES_MODEL_WEAPON"1/"

#define F_DIR_RES_SKYBOX				F_DIR_RES"skybox/"

#define F_DIR_RES_MAP					F_DIR_RES"map/"
//////////////////////////////////////////////////////////////////////////////
//OpenGL上下文,GLAD库是否已初始化
//bool g_isInited;
//////////////////////////////////////////////////////////////////////////////
#define F_UNAME_MESH_TYPE						"meshType"

#define F_UNAME_TRANSF							"trans"
#define F_UNAME_TRANSF_MODEL					F_UNAME_TRANSF".model"
#define F_UNAME_TRANSF_VIEW						F_UNAME_TRANSF".view"
#define F_UNAME_TRANSF_PROJECTION				F_UNAME_TRANSF".projection"

#define F_UNAME_BONES_TRANSF					"matBones[%d]"

#define F_UNAME_BLOCK_MATERIAL					"Material"
#define F_UNAME_MATERIAL_TEXTURE				"tex[%d]"

#define F_UNAME_LIGHTDOT						"ld"
#define F_UNAME_LIGHTDOT_ENABLED				F_UNAME_LIGHTDOT"[%d].enabled"
#define F_UNAME_LIGHTDOT_POS					F_UNAME_LIGHTDOT"[%d].pos"
#define F_UNAME_LIGHTDOT_CLR					F_UNAME_LIGHTDOT"[%d].clr"
#define F_UNAME_LIGHTDOT_INTENSITY				F_UNAME_LIGHTDOT"[%d].intensity"
#define F_UNAME_LIGHTDOT_KC						F_UNAME_LIGHTDOT"[%d].Kc"
#define F_UNAME_LIGHTDOT_KL						F_UNAME_LIGHTDOT"[%d].Kl"
#define F_UNAME_LIGHTDOT_KQ						F_UNAME_LIGHTDOT"[%d].Kq"
#define F_UNAME_LIGHTDOT_SHADOW					F_UNAME_LIGHTDOT"[%d].shadow"
#define F_UNAME_LIGHTDOT_TEX_SHADOW				F_UNAME_LIGHTDOT"[%d].texShadow"
#define F_UNAME_LIGHTDOT_FAR_MINUS_NEAR			F_UNAME_LIGHTDOT"[%d].farMnear"

#define F_UNAME_LIGHTPARALLEL					"lp"
#define F_UNAME_LIGHTPARALLEL_ENABLED			F_UNAME_LIGHTPARALLEL"[%d].enabled"
#define F_UNAME_LIGHTPARALLEL_DIRECTION			F_UNAME_LIGHTPARALLEL"[%d].direction"
#define F_UNAME_LIGHTPARALLEL_CLR				F_UNAME_LIGHTPARALLEL"[%d].clr"
#define F_UNAME_LIGHTPARALLEL_INTENSITY			F_UNAME_LIGHTPARALLEL"[%d].intensity"
#define F_UNAME_LIGHTPARALLEL_SHADOW			F_UNAME_LIGHTPARALLEL"[%d].shadow"
#define F_UNAME_LIGHTPARALLEL_TEX_SHADOW		F_UNAME_LIGHTPARALLEL"[%d].texShadow"
#define F_UNAME_LIGHTPARALLEL_MAT_VP			F_UNAME_LIGHTPARALLEL"[%d].matVP"

#define F_UNAME_LIGHTSPOT						"ls"
#define F_UNAME_LIGHTSPOT_ENABLED				F_UNAME_LIGHTSPOT"[%d].enabled"
#define F_UNAME_LIGHTSPOT_POS					F_UNAME_LIGHTSPOT"[%d].pos"
#define F_UNAME_LIGHTSPOT_CLR					F_UNAME_LIGHTSPOT"[%d].clr"
#define F_UNAME_LIGHTSPOT_INTENSITY				F_UNAME_LIGHTSPOT"[%d].intensity"
#define F_UNAME_LIGHTSPOT_DIRECTION				F_UNAME_LIGHTSPOT"[%d].direction"
#define F_UNAME_LIGHTSPOT_INNERCUTOFF			F_UNAME_LIGHTSPOT"[%d].innerCutOff"
#define F_UNAME_LIGHTSPOT_OUTTERCUTOFF			F_UNAME_LIGHTSPOT"[%d].outterCutOff"
#define F_UNAME_LIGHTSPOT_KC					F_UNAME_LIGHTSPOT"[%d].Kc"
#define F_UNAME_LIGHTSPOT_KL					F_UNAME_LIGHTSPOT"[%d].Kl"
#define F_UNAME_LIGHTSPOT_KQ					F_UNAME_LIGHTSPOT"[%d].Kq"
#define F_UNAME_LIGHTSPOT_TEX_SHADOW			F_UNAME_LIGHTSPOT"[%d].texShadow"
#define F_UNAME_LIGHTSPOT_SHADOW				F_UNAME_LIGHTSPOT"[%d].shadow"
#define F_UNAME_LIGHTSPOT_MAT_VP				F_UNAME_LIGHTSPOT"[%d].matVP"

#define F_UNAME_CAMERA							"cmr"
#define F_UNAME_CAMERA_E						F_UNAME_CAMERA".e"
#define F_UNAME_CAMERA_G						F_UNAME_CAMERA".g"
#define F_UNAME_CAMERA_T						F_UNAME_CAMERA".t"

#define F_UNAME_SKYBOX_TEX						"tex"

#define F_UNAME_SHDWLD_TRANSF_LIGHT			"light[%d]"
#define F_UNAME_SHDWLD_LIGHT_POS				"lightPos"
#define F_UNAME_SHDWLD_FAR_MINUS_NEAR			"farMnear"
//////////////////////////////////////////////////////////////////////////////
//可复制类
#define F_REPLICABLE
//内部拷贝并存储
#define F_COPY_STORE
//////////////////////////////////////////////////////////////////////////////
//异常提示
#define F_EXC_DESCRIP_OPENGL_NOT_INITED			"The OpenGL hasn't been inited."
#endif
