#ifndef PROTO_MESH_FACTORY_H
#define PROTO_MESH_FACTORY_H

#include <vector>

#include "Mesh.h"
#include "Texture.h"
#include "AnimationManager.h"

class protoData;
class textureData;

class ProtoMeshFactory
{
public:
	ProtoMeshFactory();
	ProtoMeshFactory(const ProtoMeshFactory*) = delete;
	ProtoMeshFactory& operator=(const ProtoMeshFactory*) = delete;
	~ProtoMeshFactory() = default;

	static void LoadProto(const char *const pMeshFileName, protoData &mB);
	static void CreateMeshArray(const char *const pFileName, Mesh** meshArray, Texture::Name texName = Texture::Name::NOT_INITIALIZED);
	static void CreateMeshSingle(const char *const pFileName, Mesh *&mesh, Texture::Name texName = Texture::Name::NOT_INITIALIZED);
	static void GetAnimation(const char* const pMeshFileName, Animation** AnimationArray);
};

#endif
