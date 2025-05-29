//////////////////////////////////////////////////////////////////////////////
//main.cpp - Blinn-Phong,几何着色器,万向阴影贴图
//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <cmath>
using namespace std;
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#define __STDC_LIB_EXT1__
#include "stb_image.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include <stb_image_write.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace glm;
#include "misc/def0.h"
#include "misc/err.hpp"
#include "base/base.h"
#include "misc/convert.hpp"
#include "camera/camera.h"
#include "material/material.h"
#include "light/light_dot.h"
#include "light/light_spot.h"
#include "light/light_parallel.h"
#include "object/skybox.h"
#include "shader/shader.h"
#include "misc/def1.h"
#include "object/object.h"
#include "collision/box_base.h"
#include "map/map.h"
#include "animation/animation.h"
#include "object/model.h"
#include "particle/particle.h"
#include "texture/texture_cube.h"
#include "buffer/framebuffer.h"
#include "main.h"
//////////////////////////////////////////////////////////////////////////////
int main()
{
	//system("chcp 65001");
	try
	{
		glfwSetErrorCallback(err_excep);//设置glfw库函数错误回调
		if (!glfwInit())//初始化glfw
			throw ex(0, "Failed to init OpenGL library.");
		infWnd.p = glfwCreateWindow(infWnd.size.cx, infWnd.size.cy, infWnd.title.c_str(), nullptr, nullptr);//创建窗口
		if (!infWnd.p) { glfwTerminate(); throw ex(0, "Failed to create window or OpenGL context."); }
		glfwMakeContextCurrent(infWnd.p);//选中当前的OpenGL上下文
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { glfwTerminate(); throw ex(0, "Failed to load OpenGL process loader."); }//函数加载器
		glfwSetFramebufferSizeCallback(infWnd.p, callback_framebufferSize);
		glfwSetKeyCallback(infWnd.p, callback_key);
		glfwSetCursorPosCallback(infWnd.p, callback_mouseMove);
		glfwSetMouseButtonCallback(infWnd.p, callback_mouseButton);
		auto& gm = FGlobalManager::get_instance();
		gm.g_isInited = true;
		//启用属性:
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//////////////////////////////////////////////////////////////////////
		//场景着色器:
		FShader vert_sc(FShader::ShaderType::stVertex, FShader::_load(F_DIR_SHADER"scene.vert"));
		FShader frag_sc(FShader::ShaderType::stFragment, FShader::_load(F_DIR_SHADER"scene.frag"));
		FShaderProgram spScene(vert_sc, frag_sc);
		spScene.setTransfMatrices(FShaderProgram::mtProjection, cmr._genProjectionMatrix(infWnd.size.cx, infWnd.size.cy));
		//点光源阴影着色器:
		FShader vert_sh_d(FShader::stVertex, FShader::_load(F_DIR_SHADER"shadow-dot.vert"));
		FShader geom_sh_d(FShader::stGeometry, FShader::_load(F_DIR_SHADER"shadow-dot.geom"));
		FShader frag_sh_d(FShader::stFragment, FShader::_load(F_DIR_SHADER"shadow-dot.frag"));
		FShaderProgram spShadowD(vert_sh_d, geom_sh_d, frag_sh_d, true);
		//平行光阴影着色器:
		FShader vert_sh_p(FShader::ShaderType::stVertex, FShader::_load(F_DIR_SHADER"shadow-parallel.vert"));
		FShader frag_sh_p(FShader::ShaderType::stFragment, FShader::_load(F_DIR_SHADER"shadow-parallel.frag"));
		FShaderProgram spShadowP(vert_sh_p, frag_sh_p, true);
		//天空盒着色器:
		FShader vert_sky(FShader::ShaderType::stVertex, FShader::_load(F_DIR_SHADER"skybox.vert"));
		FShader frag_sky(FShader::ShaderType::stFragment, FShader::_load(F_DIR_SHADER"skybox.frag"));
		FShaderProgram spSkybox(vert_sky, frag_sky, true);
		spSkybox.setTransfMatrices(FShaderProgram::mtProjection, cmr._genProjectionMatrix(infWnd.size.cx, infWnd.size.cy));
		//粒子着色器:
		//FShader vert_particle(FShader::stVertex, FShader::_load(F_DIR_SHADER"particle.vert"));
		//FShader geom_particle(FShader::stGeometry, FShader::_load(F_DIR_SHADER"particle.geom"));
		//FShader frag_particle(FShader::stFragment, FShader::_load(F_DIR_SHADER"particle.frag"));
		//FShaderProgram spParticle(vert_particle, geom_particle, frag_particle, true);
		//spParticle.setTransfMatrices(FShaderProgram::mtProjection,cmr._genProjectionMatrix(infWnd.size.cx,infWnd.size.cy));
		//阴影贴图:
		FTextureCube texswLD[10];
		texswLD[0].attachment(GL_DEPTH_COMPONENT, infFrame.rswd);
		texswLD[1].attachment(GL_DEPTH_COMPONENT, infFrame.rswd);
		texswLD[2].attachment(GL_DEPTH_COMPONENT, infFrame.rswd);
		FTexture2D texswLP[10];
		texswLP[0].attachment(GL_DEPTH_COMPONENT, infFrame.rswp.x, infFrame.rswp.y);
		texswLP[1].attachment(GL_DEPTH_COMPONENT, infFrame.rswp.x, infFrame.rswp.y);
		//阴影帧缓冲:
		FFramebuffer fboswLD[10];
		fboswLD[0].rect = ivec4(0, 0, infFrame.rswd, infFrame.rswd);
		fboswLD[0].attach(texswLD[0], GL_DEPTH_ATTACHMENT, true);
		fboswLD[1].rect = ivec4(0, 0, infFrame.rswd, infFrame.rswd);
		fboswLD[1].attach(texswLD[1], GL_DEPTH_ATTACHMENT, true);
		fboswLD[2].rect = ivec4(0, 0, infFrame.rswd, infFrame.rswd);
		fboswLD[2].attach(texswLD[2], GL_DEPTH_ATTACHMENT, true);
		FFramebuffer fboswLP[10];
		fboswLP[0].rect = ivec4(0, 0, infFrame.rswp.x, infFrame.rswp.y);
		fboswLP[0].attach(texswLP[0], GL_DEPTH_ATTACHMENT, true);
		fboswLP[1].rect = ivec4(0, 0, infFrame.rswp.x, infFrame.rswp.y);
		fboswLP[1].attach(texswLP[1], GL_DEPTH_ATTACHMENT, true);
		//天空盒
		FTextureCube texSkybox(names_skybox);
		FSkybox skybox;
		skybox.ptex = &texSkybox;
		skybox.setUniformData(spSkybox);
		//灯光:
		FLightDot ld[10];
		ld[0].intensity = vec3(0.7f, 0.7f, 0.3f);
		ld[0].pos = vec3(0., 3., 0.);
		ld[0].shadow = true;
		ld[0].ptexShadow = &texswLD[0];
		ld[0].enabled = false;
		ld[0].setUniformData(spScene.h);
		ld[1].intensity.z = 0.5f;
		ld[1].pos = vec3(5., 3., 5.);
		ld[1].shadow = true;
		ld[1].ptexShadow = &texswLD[1];
		ld[1].enabled = false;
		ld[1].setUniformData(spScene.h);
		ld[2].intensity.z = 0.5f;
		ld[2].pos = vec3(10., 3., 10.);
		ld[2].shadow = true;
		ld[2].ptexShadow = &texswLD[2];
		ld[2].enabled = false;
		ld[2].setUniformData(spScene.h);
		FLightParallel lp[10];
		lp[0].direction = vec3(1, -1, 1);
		lp[0].intensity = vec3(0.7, 0.8, 0.5);
		lp[0].autoPos(12);
		lp[0].shadow = true;
		lp[0].ptexShadow = &texswLP[0];
		lp[0].enabled = true;
		lp[0].setUniformData(spScene.h);
		lp[1].direction = vec3(0, -1, -1);
		lp[1].intensity.z = 0.7f;
		lp[1].enabled = false;
		lp[1].color = vec3(1., .5, .3);
		lp[1].autoPos(6);
		lp[1].shadow = true;
		lp[1].ptexShadow = &texswLP[1];
		lp[1].setUniformData(spScene.h);
		//摄像机:
		cmr.translateBy(vec3(0., 0., 5.));
		//模型:
		FModel model(F_DIR_RES"wave hip hop dance/wave hip hop dance.dae");
		model.analyse(FModel::MeshType::meshSkeleton);
		model.p = vec3(-3.f, -2.f, -3.f);
		model.s = vec3(.1, .1, .1);
		//纹理:
		FTexture2D tex(F_DIR_RES"gnd.jpg");
		FTexture2D texMap(F_DIR_RES_MAP"desert/desert.png");
		FTexture2D texMapNormal(F_DIR_RES_MAP"desert/desert_normal.png");
		FTexture2D texMapSpecular(F_DIR_RES_MAP"desert/desert_specular.png");
		FHeightImage himg(F_DIR_RES_MAP"desert/desert_height.png");
		//材质:
		FMaterial mat;
		mat.hasTex[FTexture2D::ttDiffuse] = true;
		mat.tex[FTexture2D::ttDiffuse] = &tex;
		mat.clr[FMaterial::ctSpecular] = vec3(1., 1., 1.);
		mat.shininessStrength = 1.f;
		FMaterial matMap;
		matMap.hasTex[FTexture2D::ttDiffuse] = true;
		matMap.tex[FTexture2D::ttDiffuse] = &texMap;
		matMap.hasTex[FTexture2D::ttSpecular] = true;
		matMap.tex[FTexture2D::ttSpecular] = &texMapSpecular;
		matMap.hasTex[FTexture2D::ttNormal] = true;
		matMap.tex[FTexture2D::ttNormal] = &texMapNormal;
		matMap.clr[FMaterial::ctSpecular] = vec3(1., 1., 1.);
		matMap.shininessStrength = 0.1f;
		//可绘制物体:
		FObject axes;//坐标轴
		axes.addVertexAttr(FObject::aiPosition, 3, GL_FLOAT, sizeof(GLfloat));
		axes.addVertexAttr(FObject::aiColor, 4, GL_FLOAT, sizeof(GLfloat));
		axes.setVertices(data_axes, sizeof(data_axes));
		FObject::VertexAttrib attrs[16];
		attrs[0] = { FObject::aiPosition, 3, GL_FLOAT, sizeof(GLfloat) };
		attrs[1] = { FObject::aiColor, 0, GL_FLOAT, sizeof(GLfloat) };
		attrs[2] = { FObject::aiTexCoord, 2, GL_FLOAT, sizeof(GLfloat) };
		attrs[3] = { FObject::aiNormal, 3, GL_FLOAT, sizeof(GLfloat) };
		attrs[4] = { FObject::aiTangent, 0, GL_FLOAT, sizeof(GLfloat) };
		FObject gnd;//地板
		gnd.addVertexAttrs(attrs, 5);
		gnd.setVertices(data_plane, sizeof(data_plane));
		gnd.setIndices(indices_plane, sizeof(indices_plane));
		gnd.setMaterial(mat);
		gnd.s = vec3(2., 1., 2.);
		FObject objs[10];//物体
		objs[0].addVertexAttrs(attrs, 5);
		objs[0].setVertices(data_cube1, sizeof(data_cube1));
		objs[0].setMaterial(mat);
		objs[0].s = vec3(2, 2, 2);
		objs[0].p = vec3(0., -1., 0.);
		objs[1].addVertexAttrs(attrs, 5);
		objs[1].setVertices(data_cube1, sizeof(data_cube1));
		objs[1].setMaterial(mat);
		objs[1].p = vec3(2, 0, 2);
		objs[2].addVertexAttrs(attrs, 5);
		objs[2].setVertices(data_cube1, sizeof(data_cube1));
		objs[2].setMaterial(mat);
		objs[2].p = vec3(5, 1, 2);
		objs[3].addVertexAttrs(attrs, 5);
		objs[3].setVertices(data_cube1, sizeof(data_cube1));
		objs[3].setMaterial(mat);
		objs[3].p = vec3(2, 1, 5);
		objs[4].addVertexAttrs(attrs, 5);
		objs[4].setVertices(data_cube1, sizeof(data_cube1));
		objs[4].setMaterial(mat);
		objs[4].p = vec3(-2, 1, -5);
		srand(0);
		for (int i = 2; i < 10; i++)
		{
			int random = rand() % 90;
			objs[i].r = vec3(random, random / 2., random);
		}
		FObject points;
		points.addVertexAttr(FObject::aiPosition, 3, GL_FLOAT, sizeof(GLfloat));
		points.setVertices(data_points, sizeof(data_points));
		//地图:
		FMap map(&himg, vec2(-40., 40.), vec2(-5., 0.), vec2(-40., 40.), vec2(1.));
		map.setMaterial(matMap);
		//////////////////////////////////////////////////////////////////////
		//渲染点光源阴影贴图:
		for (int i = 0; i < 3; i++)
		{
			ld[i].setShadowUniformData(spShadowD.h);
			fboswLD[i].begin();
			glClear(GL_DEPTH_BUFFER_BIT);
			spShadowD.setTransfMatrices(FShaderProgram::mtProjection, ld[i]._genProjectionMatrix());
			//----------------------------------------------------------------
			map.draw(spShadowD);

			//gnd.draw(spShadowD, FObject::dtIndices, GL_TRIANGLES);
			for (int j = 0; j < 5; j++)
				objs[j].draw(spShadowD, FObject::dtVertices, GL_TRIANGLES);
			//model.draw(spShadowD);
			//----------------------------------------------------------------
			fboswLD[i].end();
		}
		//////////////////////////////////////////////////////////////////////
		GLdouble t0 = glfwGetTime();//上一次循环取得的时间
		while (!glfwWindowShouldClose(infWnd.p))
		{
			//限制FPS:
			GLdouble t2 = glfwGetTime();//这一次循环的时间
			GLdouble dt = t2 - t0;
			if (dt < (1. / infFrame.fpsLimit))
				continue;
			t0 = t2;
			infFrame.fps = 1.f / dt;
			//渲染平行光源阴影贴图:
			for (int i = 0; i < 2; i++)
			{
				fboswLP[i].begin();
				glClear(GL_DEPTH_BUFFER_BIT);
				spShadowP.setTransfMatrices(FShaderProgram::mtView, lp[i]._genLightMatrix());
				spShadowP.setTransfMatrices(FShaderProgram::mtProjection, lp[i]._genProjectionMatrix());
				//----------------------------------------------------------------
				map.draw(spShadowP);

				axes.draw(spShadowP, FObject::dtVertices, GL_LINES);
				//gnd.draw(spShadowP, FObject::dtIndices, GL_TRIANGLES);
				for (int j = 0; j < 5; j++)
					objs[j].draw(spShadowP, FObject::dtVertices, GL_TRIANGLES);
				model.play(spShadowP,0, glfwGetTime());
				//----------------------------------------------------------------
				fboswLP[i].end();
			}
			//////////////////////////////////////////////////////////////////////
			//渲染场景:
			glViewport(0, 0, infWnd.size.cx, infWnd.size.cy);
			glClearColor(infFrame.clrBk.r, infFrame.clrBk.g, infFrame.clrBk.b, infFrame.clrBk.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//----------------------------------------------------------------
			//摄像机,视图变换矩阵:
			GLfloat col = map.collision(cmr.getPosition());
			if (col > 0.f)
				cmr.e.y -= 0.1f;
			else if (col < 0.f)
				cmr.e.y += 0.1f;

			cmr.setUniformData(spScene.h);
			spScene.setTransfMatrices(FShaderProgram::mtView, cmr._genViewMatrix());
			spSkybox.setTransfMatrices(FShaderProgram::mtView, cmr._genViewMatrix());
			//spParticle.setTransfMatrices(FShaderProgram::mtView, cmr._genViewMatrix());
			skybox.getp() = cmr.e;
			//----------------------------------------------------------------

			map.draw(spScene);

			axes.draw(spScene, FObject::dtVertices, GL_LINES);
			//	gnd.draw(spScene, FObject::dtIndices, GL_TRIANGLES);
			for (int i = 0; i < 5; i++)
				objs[i].draw(spScene, FObject::dtVertices, GL_TRIANGLES);

			model.play(spScene, 0, glfwGetTime());

			//points.draw(spParticle, FObject::dtVertices, GL_POINTS);

			skybox.draw(spSkybox);
			//////////////////////////////////////////////////////////////////////
			glfwSwapBuffers(infWnd.p);//交换缓冲
			glfwPollEvents();//处理事件
		}

		glfwDestroyWindow(infWnd.p);//销毁窗口
		glfwTerminate();//终止glfw
	}
	catch (ex _e)
	{
		err_excep(_e);
		glfwDestroyWindow(infWnd.p);//销毁窗口
		glfwTerminate();//终止glfw
	}
	catch (...)
	{
		err_excep(0, "Unexpected error.");
		glfwDestroyWindow(infWnd.p);//销毁窗口
		glfwTerminate();//终止glfw
	}
	system("pause");
}

void callback_framebufferSize(GLFWwindow* _w, int _wt, int _ht)
{
	//glViewport(0, 0, _wt, _ht);
	if (_w = infWnd.p)
	{
		infWnd.size.cx = _wt;
		infWnd.size.cy = _ht;
	}
}
void callback_mouseMove(GLFWwindow* _w, double _xpos, double _ypos)
{
	if (_w == infWnd.p)
	{
		if (!infCursor.focus)return;
		if (infCursor.first)
		{
			infCursor.prevX = _xpos, infCursor.prevY = _ypos;
			infCursor.first = false;
			return;
		}
		cmr.rotateBy((GLfloat)(-(_xpos - infCursor.prevX) * infCursor.sensitivity),
			(GLfloat)(-(_ypos - infCursor.prevY) * infCursor.sensitivity));

		infCursor.prevX = _xpos, infCursor.prevY = _ypos;
	}
}
void callback_mouseButton(GLFWwindow* _w, int _button, int _action, int _mods)
{
	if (_w == infWnd.p)
	{
		if (_action == GLFW_PRESS)
		{
			switch (_button)
			{
			case GLFW_MOUSE_BUTTON_LEFT:
				glfwSetInputMode(_w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				infCursor.focus = true;
				break;
			}
		}
	}
}
void callback_key(GLFWwindow* _w, int _key, int _scancode, int _action, int _mods)
{
	/*action: press release repeat*/
	/*mods: shift alt ctrl*/
	/*scancode:?*/
	if (_w == infWnd.p)
	{
		if (_action == GLFW_PRESS)
		{
			switch (_key)
			{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(_w, GL_FALSE);

				glfwSetInputMode(_w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				infCursor.focus = false;
				infCursor.first = true;
				break;
			case GLFW_KEY_1:
				static bool _enable1;
				//sp.setBool("enableLightDot", _enable1);
				_enable1 = !_enable1;
				break;
			case GLFW_KEY_2:
				static bool _enable2;
				//sp.setBool("enableLightParallel", _enable2);
				_enable2 = !_enable2;
				break;
			case GLFW_KEY_3:
				static bool _enable3;
				spot = !spot;
				//sp.setBool("enableLightSpot", _enable3);
				_enable3 = !_enable3;
				break;

			}

		}

		if (_action == GLFW_RELEASE)
		{
			switch (_key)
			{
			case GLFW_KEY_Q:
				break;
			case GLFW_KEY_E:
				break;
			}
		}

		if (_action == GLFW_PRESS || _action == GLFW_REPEAT)
		{
			const GLfloat step = .5;
			switch (_key)
			{
			case GLFW_KEY_W:
				cmr.translateBy(glm::normalize(cmr.g) * step);
				break;
			case GLFW_KEY_S:
				cmr.translateBy(-glm::normalize(cmr.g) * step);
				break;
			case GLFW_KEY_A:
				cmr.translateBy(-glm::normalize(cmr.getRight()) * step);
				break;
			case GLFW_KEY_D:
				cmr.translateBy(glm::normalize(cmr.getRight()) * step);
				break;
			}

		}
	}
}


