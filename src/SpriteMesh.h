#ifndef SPRITE_MESH_H
#define SPRITE_MESH_H

#include "Mesh.h"
#include "protoData.h"

namespace Azul
{

class SpriteMesh : public Mesh
{
public:
	SpriteMesh(const char *const modelFileName);
	~SpriteMesh();

	SpriteMesh(const SpriteMesh &) = delete;
	SpriteMesh &operator=(const SpriteMesh &) = delete;
private:
	void privCreateVAO(protoData &pD);
};

} //namespace Azul

#endif //SPRITE_MESH_H 