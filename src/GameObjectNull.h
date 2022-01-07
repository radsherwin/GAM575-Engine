#ifndef GAME_OBJECT_NULL_H
#define GAME_OBJECT_NULL_H

#include "GraphicsObjectHdr.h"
#include "AnimTime.h"
#include "GameObject.h"

namespace Azul
{
class GameObjectNull : public GameObject
{
public:
	GameObjectNull(GraphicsObjectHdr *pGraphicsObjectHdr); //Should be null

	GameObjectNull() = delete;
	GameObjectNull(const GameObjectNull &) = delete;
	GameObjectNull &operator=(GameObjectNull &) = delete;
	virtual ~GameObjectNull() override;

	virtual void Update(AnimTime currentTime) override;
};
}

#endif // GAME_OBJECT_NULL_H
