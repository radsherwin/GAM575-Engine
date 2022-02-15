//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Mesh.h"
#include "StringThis.h"

Mesh::Mesh()
	: numVerts(0),
	numTris(0),
	vao(0),
	vbo_verts(0),
	vbo_norms(0),
	vbo_texts(0),
	vbo_color(0),
	vbo_index(0),
	poRefSphere(new Sphere()),
	textureIDInt(0),
	jointCount(0),
	totalBones(0),
	poJointData{nullptr},
	poInverseBind{nullptr},
	numInverseBind(0)
	
{
}

Mesh::~Mesh()
{
	delete this->poRefSphere;
	delete[] this->poJointData;
	delete[] this->poInverseBind;
}

void Mesh::Wash()
{
	this->numVerts = 0;
	this->numTris = 0;
	vao = 0;
	vbo_verts = 0;
	vbo_norms = 0;
	vbo_texts = 0;
	vbo_color = 0;
	vbo_index = 0;
	this->jointCount = 0;
	this->totalBones = 0;
	this->numInverseBind = 0;
	
}

bool Mesh::Compare(DLink* pTarget)
{
	// This is used in ManBase.Find() 
	assert(pTarget != nullptr);

	Mesh* pDataB = (Mesh*)pTarget;

	bool status = false;

	if (this->name == pDataB->name)
	{
		status = true;
	}

	return status;
}

void Mesh::SetName(Name _name)
{
	this->name = _name;
}

char* Mesh::GetName()
{
	// todo - Hack understand why is this needed for print and fix...
	static char pTmp[128];
	strcpy_s(pTmp, 128, StringMe(this->name));
	return pTmp;
}



// --- End of File ---