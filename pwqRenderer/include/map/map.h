//////////////////////////////////////////////////////////////////////////////
//map.h - 地图
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "object/object.h"
#include "image/height_image.h"
#include <glm/glm.hpp>
#include "misc/err.hpp"
#include <glm/detail/type_vec.hpp>
#include "misc/global_manager.h"
//////////////////////////////////////////////////////////////////////////////
//地图类
class FMap :protected FObject
{
public:
	FMap() :FObject()
	{

		auto& gm = FGlobalManager::get_instance();
		if (gm.g_isInited)_initThis();
	}
	FMap(FHeightImage* _img, glm::vec2 _rgnX, glm::vec2 _rgnY, glm::vec2 _rgnZ, glm::vec2 _precision) :FObject()
	{
		auto& gm = FGlobalManager::get_instance();
		if (!gm.g_isInited)throw ex(0, F_EXC_DESCRIP_OPENGL_NOT_INITED);
		_initThis();
		load(_img, _rgnX, _rgnY, _rgnZ, _precision);
	}
	virtual ~FMap()
	{
		if (_autorelease)_destroyThis();
	}
	//初始化
	void _init()
	{
		__super::_init();
		_initThis();
	}
	//销毁
	void _destroy()
	{
		_destroyThis();
		__super::_destroy();
	}
private:
	void _initThis();

	void _destroyThis();

public:
	//@brief 加载高度图并生成地图
	//@param _img 高度图
	//@param _rgnX X坐标范围,[minX,maxX]
	//@param _rgnY Y坐标范围,[minY,maxY]
	//@param _rgnZ Z坐标范围,[minZ,maxZ]
	//@param _precision 地形生成精度(XZ平面上相邻顶点间隔)
	void load(FHeightImage* _img, glm::vec2 _rgnX, glm::vec2 _rgnY, glm::vec2 _rgnZ, glm::vec2 _precision);
	
	//绘制
	void draw(FShaderProgram& _sp)
	{
		__super::draw(_sp, dtIndices, GL_TRIANGLES);
	}
	//@brief 碰撞检测
	//@param _p 世界空间坐标
	//@return 0.f表示紧挨地形;<0.f表示在地形下面,值表示距离;>0.f代表在地形上面,值表示距离.
	GLfloat collision(glm::vec3 _p);

	//设置材质
	void setMaterial(FMaterial& _mtl)
	{
		__super::setMaterial(_mtl);
	}
	//获取地图位置
	glm::vec3& getP()
	{
		return p;
	}
	//@brief 获取地形高度Y值
	//@param _xz 模型空间XZ平面坐标
	//@return 高度值对应的模型空间Y坐标,超出范围返回rgnYmin
	GLfloat _getHeight(glm::vec2 _xz);

public:
	//@brief 获取N列_cy行矩阵中第_x列_y行个元素的索引,从0开始,溢出时裁剪处理.
	//@param _cy 行数
	//@param _x 列
	//@param _y 行
	static GLuint _index(GLuint _cx, GLuint _cy, GLint _x, GLint _y);

	//计算顶点_this的法线
	static glm::vec3 _normal(glm::vec3 _this, glm::vec3 _l, glm::vec3 _t, glm::vec3 _r, glm::vec3 _b);

	//计算顶点_this的切线
	static glm::vec3 _tangent(glm::vec3 _this, glm::vec3 _pt1, glm::vec3 _pt2, glm::vec2 _uvThis, glm::vec2 _uv1, glm::vec2 _uv2);
	
public:
	//高度图(只读)
	FHeightImage* img = nullptr;
	//坐标范围(只读)
	glm::vec2 rgnX, rgnY, rgnZ;
	//XYZ尺寸(只读)
	glm::vec3 size = glm::vec3(2.f, 2.f, 2.f);
	//XZ方向纹理平铺次数
	glm::vec2 texTile = glm::vec2(4.f, 4.f);
	//碰撞比较误差范围
	GLfloat range = 0.05f;
};
