//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MESH_H
#define MESH_H

#include "Bone.h"
#include "sb7.h"
#include "MathEngine.h"
#include "DLink.h"
#include "BoundingSphere.h"
#include "Texture.h"
#include "Animation.h"

using namespace Azul;

class Mesh
{
public:
	enum class Name
	{
		CUBE,
		DOG_HOUSE,
		AVOCADO,
		ANTIQUE_CAMERA,
		SPRITE,
		BONE_A,
		BONE_B,
		CHICKEN_BOT,
		CRATE,
		CORSET,
		SNOWMAN,
		PYRAMID,
		BONE,
		BONE_R,
		BONE_R_001,
		BONE_R_002,
		BONE_L,
		BONE_L_001,
		BONE_L_002,
		BONE_01,
		SPACE_FRIGATE,
		CAMERA,
		POKE_BALL,
		SPHERE,
		NULL_MESH,
		NOT_INITIALIZED
	};

	struct JointData
	{
		char name[32];
		int jointIndex;
		int parentIndex;
	};

public:
	Mesh();
	Mesh(Mesh& copyMesh) = delete;
	Mesh& operator = (Mesh& copyMesh) = delete;
	virtual ~Mesh();

	// Data
	int numVerts;
	int numTris;
	
	GLuint vao;
	GLuint vbo_verts;	//x,y,z
	GLuint vbo_norms;	//nx, ny, nz
	GLuint vbo_texts;	//u v
	GLuint vbo_color;
	GLuint vbo_index;	//trilist
	GLuint vbo_weight;
	GLuint vbo_joint;

	void Wash();
	bool Compare(DLink* pTarget);
	char* GetName();
	void SetName(Name _name);

	//Reference sphere before any transformations are applied
	Sphere* poRefSphere;

public:
	Name name;
	std::string meshName;
	unsigned int textureIDInt;
	unsigned int totalBones;
	unsigned int jointCount;
	JointData *poJointData;

	//Inv Bind
	Matrix *poInverseBind;
	unsigned int numInverseBind;
};

#endif

// --- End of File ---