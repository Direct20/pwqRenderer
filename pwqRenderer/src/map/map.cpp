#include "map/map.h"
using namespace glm;
void FMap::_initThis()
{
	addVertexAttr(aiPosition, 3, GL_FLOAT, sizeof(GLfloat));
	addVertexAttr(aiColor, 0, GL_FLOAT, sizeof(GLfloat));
	addVertexAttr(aiTexCoord, 3, GL_FLOAT, sizeof(GLfloat));
	addVertexAttr(aiNormal, 3, GL_FLOAT, sizeof(GLfloat));
	addVertexAttr(aiTangent, 3, GL_FLOAT, sizeof(GLfloat));
	rgnX = rgnY = rgnZ = vec2(-1., 1.);
}

void FMap::_destroyThis()
{
	rgnX = rgnY = rgnZ = vec2(-1., 1.);
	size = vec3(2., 2., 2.);
}

void FMap::load(FHeightImage* _img, vec2 _rgnX, vec2 _rgnY, vec2 _rgnZ, vec2 _precision)
{
	if (!_img)return;

	img = _img;
	rgnX = _rgnX, rgnY = _rgnY, rgnZ = _rgnZ;

	vec3 sz = vec3(_rgnX.y - _rgnX.x, _rgnY.y - _rgnY.x, _rgnZ.y - _rgnZ.x);//XYZ尺寸
	size = sz;
	uvec2 ec = uvec2(floor(sz.x / _precision.x) + 1, floor(sz.z / _precision.y) + 1);//估计XZ方向顶点数量
	_precision /= vec2(sz.x, sz.z);//转换为标准值
	//生成顶点数据:
	GLuint expectedCnt = ec.x * ec.y + ec.x + ec.y;//估计顶点总数量
	vec3* pPositions = new vec3[expectedCnt];//顶点位置,预留部分内存
	vec3* pTexCoords = new vec3[expectedCnt];//纹理坐标
	vec3* pNormals = new vec3[expectedCnt];//顶点法线
	vec3* pTangents = new vec3[expectedCnt];//顶点切线

	uvec2 rc = uvec2(0, 0);//实际XZ方向顶点数量
	uvec2 cl = uvec2(0, 0);//顶点的XZ索引
	GLuint realCnt = 0;//实际顶点总数量
	for (GLfloat x = 0.f; x <= 1.f; x += _precision.x, cl.x++)
	{
		cl.y = 0;
		rc.y = 0;
		for (GLfloat z = 0.f; z <= 1.f; z += _precision.y, cl.y++)
		{
			GLfloat val = _img->get(x, z);//高度值
			pPositions[realCnt] = vec3(_rgnX.x + sz.x * x, _rgnY.x + sz.y * val, _rgnZ.x + sz.z * z);
			pTexCoords[realCnt] = vec3(x * texTile.x, z * texTile.y, 0.f);
			pNormals[realCnt] = _normal(pPositions[_index(ec.x, ec.y, cl.x, cl.y)],
				pPositions[_index(ec.x, ec.y, cl.x - 1, cl.y)],
				pPositions[_index(ec.x, ec.y, cl.x, cl.y - 1)],
				pPositions[_index(ec.x, ec.y, cl.x + 1, cl.y)],
				pPositions[_index(ec.x, ec.y, cl.x, cl.y + 1)]);
			pTangents[realCnt] = _tangent(pPositions[_index(ec.x, ec.y, cl.x, cl.y)],
				pPositions[_index(ec.x, ec.y, cl.x - 1, cl.y)],
				pPositions[_index(ec.x, ec.y, cl.x, cl.y - 1)],
				pTexCoords[_index(ec.x, ec.y, cl.x, cl.y)],
				pTexCoords[_index(ec.x, ec.y, cl.x - 1, cl.y)],
				pTexCoords[_index(ec.x, ec.y, cl.x, cl.y - 1)]);

			realCnt++;
			rc.y++;
		}
		rc.x++;
	}
	//设置顶点数据:
	setVertices(pPositions, realCnt * sizeof(vec3), aiPosition);
	setVertices(pTexCoords, realCnt * sizeof(vec3), aiTexCoord);
	setVertices(pNormals, realCnt * sizeof(vec3), aiNormal);
	setVertices(pTangents, realCnt * sizeof(vec3), aiTangent);
	//生成索引:
	uvec3* pIndices = new uvec3[(rc.x - 1) * (rc.y - 1) * 2 + rc.x + rc.y];
	GLuint arrpos = 0;
	for (GLuint x = 0; x < rc.x - 1; x++)//X
		for (GLuint y = 0; y < rc.y - 1; y++, arrpos += 2)//Z
		{
			pIndices[arrpos] = uvec3(_index(rc.x, rc.y, x, y),
				_index(rc.x, rc.y, x + 1, y), _index(rc.x, rc.y, x + 1, y + 1));
			pIndices[arrpos + 1] = uvec3(_index(rc.x, rc.y, x, y),
				_index(rc.x, rc.y, x, y + 1), _index(rc.x, rc.y, x + 1, y + 1));
		}
	//设置顶点索引:
	setIndices(pIndices, sizeof(uvec3) * arrpos);

	delete[]pPositions;
	delete[]pTexCoords;
	delete[]pNormals;
	delete[]pTangents;
	delete[]pIndices;
}
GLfloat FMap::collision(vec3 _p)
{
	mat4 m = inverse(_genModelMatrix());
	_p = m * vec4(_p, 1.);//从世界空间变换到模型空间
	GLfloat mapY = _getHeight(vec2(_p.x, _p.z));

	GLfloat delta = _p.y - mapY;
	if (delta > range)//上
		return delta;
	else if (delta < -range)//下
		return delta;
	else //紧挨
		return 0.f;
}

GLfloat FMap::_getHeight(vec2 _xz)
{
	_xz -= vec2(rgnX.x, rgnZ.x);
	if (_xz.x<0 || _xz.y<0 || _xz.x >size.x || _xz.y > size.z)return rgnY.x;//返回Y范围内最低高度
	_xz /= vec2(size.x, size.z);//标准化
	GLfloat height = img->get(_xz);
	return height * size.y + rgnY.x;
}

GLuint FMap::_index(GLuint _cx, GLuint _cy, GLint _x, GLint _y)
{
	_x = _x < 0 ? 0 : _x;
	_x = _x > (GLint)_cx - 1 ? _cx - 1 : _x;
	_y = _y < 0 ? 0 : _y;
	_y = _y > (GLint)_cy - 1 ? _cy - 1 : _y;
	return _x * _cy + _y;
}

vec3 FMap::_normal(vec3 _this, vec3 _l, vec3 _t, vec3 _r, vec3 _b)
{
	vec3 ll = _l - _this,
		tt = _t - _this,
		rr = _r - _this,
		bb = _b - _this;
	vec3 nl = cross(ll, bb),
		nt = cross(tt, ll),
		nr = cross(rr, tt),
		nb = cross(bb, rr);
	if (nl.y < 0)nl.y = -nl.y;
	if (nt.y < 0)nt.y = -nt.y;
	if (nr.y < 0)nr.y = -nr.y;
	if (nb.y < 0)nb.y = -nb.y;
	vec4 len = vec4(length(nl), length(nt), length(nr), length(nb));
	if (len.x)nl /= len.x;
	if (len.y)nt /= len.y;
	if (len.z)nr /= len.z;
	if (len.w)nb /= len.w;
	return nl + nt + nr + nb;
}

vec3 FMap::_tangent(vec3 _this, vec3 _pt1, vec3 _pt2, vec2 _uvThis, vec2 _uv1, vec2 _uv2)
{
	vec3 tangent(0.f);

	vec3 e1 = _pt1 - _this;
	vec3 e2 = _pt2 - _this;
	vec2 dUV1 = _uv1 - _uvThis;
	vec2 dUV2 = _uv2 - _uvThis;

	GLfloat f = 1.0f / (dUV1.x * dUV2.y - dUV2.x * dUV1.y);

	tangent.x = f * (dUV2.y * e1.x - dUV1.y * e2.x);
	tangent.y = f * (dUV2.y * e1.y - dUV1.y * e2.y);
	tangent.z = f * (dUV2.y * e1.z - dUV1.y * e2.z);

	tangent = normalize(tangent);
	return tangent;
}