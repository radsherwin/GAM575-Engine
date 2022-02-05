#ifndef GAME_OBJECT_FONT_H
#define GAME_OBJECT_FONT_H

#include "GameObject.h"
#include "GameObject2D.h"
#include "Font.h"

#include "AnimTime.h"

namespace Azul
{
class GraphicsObjectHdr_Sprite;
class GameObjectFont : public GameObject
{
public:
    GameObjectFont(GraphicsObjectHdr_Sprite *pGraphicsObjectHdr, Font::Name fontName); // needs to be sprite

    //Big 4
    GameObjectFont() = delete;
    GameObjectFont(const GameObjectFont &) = delete;
    GameObjectFont &operator=(const GameObjectFont &) = delete;
    virtual ~GameObjectFont();

    virtual void Update(AnimTime currentTime) override;
    virtual void Draw() override;

public:
    Font *pFont;
    float scaleX;
    float scaleY;
    float posX;
    float posY;
    float angle;
};
} //namespace Azul

#endif // !GAME_OBJECT_H
