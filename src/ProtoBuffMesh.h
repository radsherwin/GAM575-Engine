//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef PROTO_BUFF_MESH_H
#define PROTO_BUFF_MESH_H

#include "Mesh.h"
#include "meshData.h"

class ProtoBuffMesh : public Mesh
{
public:
	ProtoBuffMesh(meshData &pB, const unsigned int totalBones);

	ProtoBuffMesh() = delete;
	ProtoBuffMesh(const ProtoBuffMesh&) = delete;
	ProtoBuffMesh& operator = (ProtoBuffMesh&) = delete;
	virtual ~ProtoBuffMesh();

private:
	void privCreateMesh(meshData &pB, const unsigned int &totalBones);

};

#endif

// --- End of File ---
