#ifndef SPRITE_MESH_H
#define SPRITE_MESH_H

#include "Mesh.h"

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
	void privCreateVAO(const char *const modelFileName);
};

} //namespace Azul

#endif //SPRITE_MESH_H 