#include "SpriteMesh.h"
#include "sb7.h"
#include "SpriteVBO.h"
#include "ProtoMeshFactory.h"

namespace Azul
{

void SpriteMesh::privCreateVAO(protoData &pD)
{
	assert(pD.meshCount != 0);
	meshData *mD = &pD.pMeshData[0];
	this->numVerts = (int)mD->vertCount;
	this->numTris = (int)mD->triCount;

	// Create a VAO
	glGenVertexArrays(1, &this->vao);
	assert(this->vao != 0);
	glBindVertexArray(this->vao);

	// Create a VBO
	glGenBuffers(1, &this->vbo_verts);
	assert(this->vbo_verts != 0);

	//	glGenBuffers(1, &this->vbo_norms);
	//	assert(this->vbo_norms != 0);

	glGenBuffers(1, &this->vbo_texts);
	assert(this->vbo_texts != 0);

	//	glGenBuffers(1, &this->vbo_color);
	//	assert(this->vbo_color != 0);

	glGenBuffers(1, &this->vbo_index);
	assert(this->vbo_index != 0);

	assert(this->numTris > 0);
	assert(this->numVerts > 0);

	// Load the verts data: ---------------------------------------------------------

	if (mD->vbo_vert.enabled)
	{
		//assert(mB.vbo_vert.targetType == vboData::VBO_TARGET::ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_verts);

		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(mD->vbo_vert.dataSize), mD->vbo_vert.poData, GL_STATIC_DRAW);

		glVertexAttribPointer(mD->vbo_vert.attribIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyz), 0);
		glEnableVertexAttribArray(mD->vbo_vert.attribIndex);
	}

	// Load the texts data: ---------------------------------------------------------

	if (mD->vbo_uv.enabled)
	{
		//assert(mB.vbo_uv.targetType == vboData::VBO_TARGET::ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_texts);

		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(mD->vbo_uv.dataSize), mD->vbo_uv.poData, GL_STATIC_DRAW);

		glVertexAttribPointer(mD->vbo_uv.attribIndex, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_uv), 0);
		glEnableVertexAttribArray(mD->vbo_uv.attribIndex);
	}

	// Load the index data: ---------------------------------------------------------

	if (mD->vbo_index.enabled)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_index);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(mD->vbo_index.dataSize), mD->vbo_index.poData, GL_STATIC_DRAW);
	}
}// privCreateVAO()

SpriteMesh::SpriteMesh(const char *const modelFileName)
	: Mesh()
{
	protoData pD;
	ProtoMeshFactory::LoadProto(modelFileName, pD);
	this->privCreateVAO(pD);
}

SpriteMesh::~SpriteMesh()
{
}
} //namespace Azul