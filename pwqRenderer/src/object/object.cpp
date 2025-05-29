#include "object/object.h"
#include "misc/err.hpp"
#include "misc/global_manager.h"
void FObject::_init()
{
	auto& gm = FGlobalManager::get_instance();
	if (!gm.g_isInited)throw ex("OpenGL hasn't been initialized.");

	if (!hVAO)glGenVertexArrays(1, &hVAO);
	else war_excep(0, "The object has been initialized.");
	mtl.valid = false;//默认不启用材质
}


void FObject::_destroy()
{
	if (hVAO)glDeleteVertexArrays(1, &hVAO);
	hVAO = 0;
	attribs.clear();
	cntVertex = 0;
	cntIndex = 0;
}

void FObject::setVertices(const void* const _pdata, GLsizeiptr _size)
{
	glBindVertexArray(hVAO);

	GLuint h;
	glGenBuffers(1, &h);
	glBindBuffer(GL_ARRAY_BUFFER, h);
	glBufferData(GL_ARRAY_BUFFER, _size, _pdata, GL_STATIC_DRAW);

	//设置数据解析格式:
	GLsizei stride = 0;
	for (GLuint i = 0; i < attribs.size(); i++)
		stride += attribs[i].size * attribs[i].num;
	GLuint offset = 0;
	for (GLuint i = 0; i < attribs.size(); i++)
	{
		if (attribs[i].num == 0 || attribs[i].size == 0)continue;
		if (attribs[i].type == GL_UNSIGNED_INT || attribs[i].type == GL_INT)//区别使用
			glVertexAttribIPointer(attribs[i].index, attribs[i].num, attribs[i].type, stride, (void*)offset);
		else
			glVertexAttribPointer(attribs[i].index, attribs[i].num, attribs[i].type, GL_FALSE, stride, (void*)offset);

		glEnableVertexAttribArray(attribs[i].index);
		offset += attribs[i].size * attribs[i].num;
	}
	//计算顶点数目:
	cntVertex = _size / stride;

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &h);
}

void FObject::setVertices(const void* const _pdata, GLsizeiptr _size, AttribIndex _index)
{
	glBindVertexArray(hVAO);

	GLuint h;
	glGenBuffers(1, &h);
	glBindBuffer(GL_ARRAY_BUFFER, h);
	glBufferData(GL_ARRAY_BUFFER, _size, _pdata, GL_STATIC_DRAW);

	VertexAttrib* which = nullptr;//哪一条属性
	for (GLuint i = 0; i < attribs.size(); i++)
		if (attribs[i].index == _index)
			which = &attribs[i];
	if (!which)throw ex("Invalid vertex attribute index.");
	GLuint stride = which->size * which->num;
	if (stride != 0)
	{
		if (which->type == GL_UNSIGNED_INT || which->type == GL_INT)//区别使用
			glVertexAttribIPointer(_index, which->num, which->type, stride, (void*)0);
		else
			glVertexAttribPointer(_index, which->num, which->type, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(_index);
		//计算顶点数目:
		cntVertex = _size / stride;
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &h);
}

void FObject::setIndices(const void* const _pdata, GLsizeiptr _size)
{
	//VBO:VertexAttribPointer时VAO存入数据
	//EBO:BindBuffer时VAO存入数据

	glBindVertexArray(hVAO);

	GLuint h;
	glGenBuffers(1, &h);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, h);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size, _pdata, GL_STATIC_DRAW);
	//计算索引数目:
	cntIndex = _size / sizeof(GLuint);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &h);
}

void FObject::setMaterial(FMaterial& _mtl)
{
	mtl = _mtl;
}

void FObject::draw(FShaderProgram& _sp, DrawType _dt, GLenum _mode)
{
	draw(_sp, _genModelMatrix(), _dt, _mode);
}

void FObject::draw(FShaderProgram& _sp, glm::mat4 _modelMatrix, DrawType _dt, GLenum _mode)
{
	//配置着色器:
	_sp.setTransfMatrices(FShaderProgram::mtModel, _modelMatrix);//设置模型变换矩阵
	_sp.setMaterial(mtl);//设置物体材质
	_sp.setMeshType(meshType);//默认为静态网格体

	//绘图:
	glBindVertexArray(hVAO);
	switch (_dt)
	{
	case dtIndices:
		glDrawElements(_mode, cntIndex, GL_UNSIGNED_INT, nullptr);
		break;
	case dtVertices:
		glDrawArrays(_mode, 0, cntVertex);
		break;
	}
	glBindVertexArray(0);
}

void FObject::draw_static(FShaderProgram& _sp, glm::mat4 _modelMatrix, DrawType _dt, GLenum _mode)
{
	MeshType tmp = meshType;
	meshType = MeshType::meshStatic;
	draw(_sp, _modelMatrix, _dt, _mode);
	meshType = tmp;
}

void FObject::draw_static(FShaderProgram& _sp, DrawType _dt, GLenum _mode)
{
	draw_static(_sp, _genModelMatrix(), _dt, _mode);
}
