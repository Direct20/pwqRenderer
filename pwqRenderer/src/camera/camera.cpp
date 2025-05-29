#include "camera/camera.h"
#include "misc/err.hpp"
#include "misc/global_manager.h"
void FCamera::rotateBy(GLfloat _dx, GLfloat _dy, GLfloat _dz)
{
	using namespace glm;
	vec3 dd(_dx, _dy, _dz);//变换量
	static vec3 tt(0.f);//总量
	//约束角度:
	tt += dd;
	for (int i = 0; i < 3; i++)
	{
		if (constraintRmin[i] && tt[i] < constraintRmin[i])
			tt[i] = constraintRmin[i], dd[i] = 0.;
		if (constraintRmax[i] && tt[i] > constraintRmax[i])
			tt[i] = constraintRmax[i], dd[i] = 0.;
	}

	vec3 u = getRight();
	//生成变换矩阵:
	mat4 m(1.f);
	m = glm::rotate(m, glm::radians(dd.x), t);//x
	m = glm::rotate(m, glm::radians(dd.y), u);//y
	m = glm::rotate(m, glm::radians(dd.z), g);//z
	//变换:
	g = normalize(m * vec4(g, 0.f));
}
void FCamera::translateBy(glm::vec3 _d)
{
	//mat4 m(1.f);
	//m = ::translate(m, _d);
	//e = m * vec4(e, 1.f);
	e += _d;
}
glm::vec3 FCamera::getRight()
{
	return glm::cross(g, t);
}
void FCamera::setUniformData(GLuint _hProgram)
{
	auto& gm = FGlobalManager::get_instance();
	if (!gm.g_isInited)throw ex("OpenGL hasn't been inited.");
	glUseProgram(_hProgram);
	glUniform3fv(glGetUniformLocation(_hProgram, F_UNAME_CAMERA_E), 1, &e.x);
	glUniform3fv(glGetUniformLocation(_hProgram, F_UNAME_CAMERA_G), 1, &g.x);
	glUniform3fv(glGetUniformLocation(_hProgram, F_UNAME_CAMERA_T), 1, &t.x);
}

glm::mat4 FCamera::_genViewMatrix()
{
	//vec3 u = cross(g, t);//右向量
	glm::mat4 m(1.f);

	//m[0] = vec4(u.x, u.y, u.z, 0.f);//m[0]对应m的第一列
	//m[1] = vec4(t.x, t.y, t.z, 0.f);
	//m[2] = vec4(-g.x, -g.y, -g.z, 0.f);
	//m[3] = vec4(0.f, 0.f, 0.f, 1.f);

	//m = ::transpose(m);//转置正交矩阵得到其逆矩阵

	//m = ::translate(m, -e);//平移到原点
	m = glm::lookAt(e, e + g, t);
	return m;
}

glm::mat4 FCamera::_genProjectionMatrix(GLuint _cx, GLuint _cy)
{
	return glm::perspective(glm::radians(60.f), (GLfloat)_cx / _cy, 0.1f, 100.f);
}
