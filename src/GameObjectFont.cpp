#include "MathEngine.h"
#include "FontManager.h"
#include "GlyphManager.h"
#include "GameObjectFont.h"
#include "GraphicsObjectHdr_Sprite.h"
#include "GraphicsObject_Font.h"

namespace Azul
{
GameObjectFont::GameObjectFont(GraphicsObjectHdr_Sprite *pGraphicsObjHdr, Font::Name fontName)
    : GameObject(pGraphicsObjHdr)
{
    assert(pGraphicsObjHdr != 0);
    this->pFont = FontManager::Find(fontName);
    this->angle = 0.f;

    this->posX = pFont->startX;
    this->posY = pFont->startY;
    this->scaleX = 1.f;
    this->scaleY = 1.f;

    GraphicsObject_Font *pGPF = (GraphicsObject_Font *)(pGraphicsObjHdr->poGraphicsObject);
    pGPF->pTexture = GlyphManager::Find(pFont->glyphName)->pTexture;
}

GameObjectFont::~GameObjectFont()
{
}

void GameObjectFont::Update(AnimTime currentTime)
{
    AZUL_UNUSED_VAR(currentTime);

    Matrix Scale(Matrix::Scale::XYZ, this->scaleX, this->scaleY, 1.0f);
    Matrix Rot(Matrix::Rot1::Z, this->angle);
    Matrix Trans(Matrix::Trans::XYZ, this->posX, this->posY, 0.0f);
    *this->poWorld = Scale * Rot * Trans;

    //*this->poWorld = Scale * Rot * Trans;

    // push to graphics object
    GraphicsObjectHdr *tmp = this->poGraphicsObjHdr;
    if (!this->IsHidden())
    {
        tmp->poGraphicsObject->SetWorld(*this->poWorld);
    }
}

void GameObjectFont::Draw()
{
    GraphicsObject_Font *pGPF = (GraphicsObject_Font *)(this->poGraphicsObjHdr->poGraphicsObject);
    assert(pGPF);
    // Update font data
    //for(textLength)

    float xTmp = this->pFont->startX;
    float yTmp = this->pFont->startY;
    float xEnd = this->pFont->startX;
    for (int i = 0; i < this->pFont->pText.length(); i++)
    {
        // Update world
        int key(pFont->pText[i]); // gets ascii

        Glyph *pGlyph = GlyphManager::Find(pFont->glyphName); // Hack

        key -= pGlyph->startingOffset; // set the array offset

        Rect *pRect = &pGlyph->pImage[key].imageRect;
        Image *pImage = &pGlyph->pImage[key];

        xTmp = xEnd + ((float)pRect->width / 2.0f);
        float w = pRect->width / (float)pImage->pText->width[0];
        float h = pRect->height / (float)pImage->pText->height[0];
        float u = pRect->x / (float)pImage->pText->width[0];
        float v = pRect->y / (float)pImage->pText->height[0];
        Matrix ScaleUV(Matrix::Scale::XYZ, w, h, 1.0f);
        Matrix TransUV(Matrix::Trans::XYZ, u, v, 0.0f);
        *pGPF->poMatrix_uv = ScaleUV * TransUV;

        Matrix Scale2(Matrix::Scale::XYZ, pRect->width, pRect->height, 1.0f);
        *pGPF->poMatrix_orig = Scale2;

        //this->posX = xTmp;
        //this->posY = yTmp;
        Matrix Scale(Matrix::Scale::XYZ, this->scaleX, this->scaleY, 1.0f);
        Matrix Rot(Matrix::Rot1::Z, this->angle);
        Matrix Trans(Matrix::Trans::XYZ, xTmp, yTmp, 0.0f);
        *this->poWorld = Scale * Rot * Trans;
        pGPF->SetWorld(*this->poWorld);

        pGPF->Render();

        // Move the start to the next character
        xEnd = ((float)pRect->width / 2.0f) + xTmp;
    }
}
} //namespace Azul