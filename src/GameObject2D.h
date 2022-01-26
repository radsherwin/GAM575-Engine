#ifndef GAME_OBJECT_2D_H
#define GAME_OBJECT_2D_H

#include "GameObject.h"

#include "AnimTime.h"

namespace Azul
{
class GraphicsObjectHdr_Sprite;
class GameObject2D : public GameObject
{
public:
	GameObject2D(GraphicsObjectHdr_Sprite *pGraphicsObjectHdr); // needs to be sprite

	//Big 4
	GameObject2D() = delete;
	GameObject2D(const GameObject2D &) = delete;
	GameObject2D &operator=(const GameObject2D &) = delete;
	virtual ~GameObject2D();

	virtual void Update(AnimTime currentTime) override;

public:
	float scaleX;
	float scaleY;
	float posX;
	float posY;
	float angle;
};

} //namespace Azul

#endif // !GAME_OBJECT_H
