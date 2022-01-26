#ifndef GLYPH_MANAGER_H
#define GLYPH_MANAGER_H

#include "ManBase.h"
#include "Glyph.h"
#include "Texture.h"
class protoData;
namespace Azul
{
class GlyphManager : public ManBase
{
private:
    GlyphManager(int reserveNum = 3, int reserveGrow = 1);
    GlyphManager() = delete;
    GlyphManager(const GlyphManager &) = delete;
    GlyphManager &operator=(const GlyphManager &) = delete;
    virtual ~GlyphManager();

public:
    static void Create(int reserveNum = 3, int reserveGrow = 1);
    static void Destroy();

    static Glyph *Add(const char *const pFilePath, Glyph::Name glyphName, Texture::Name textureName);

    static Glyph *Find(Glyph::Name glyphName);
    static Glyph *Find(const char *const pText, Glyph::Name glyphName);
    static void Remove(Glyph *pNode);
    static void Dump();

private:
    static GlyphManager *privGetInstance();

    void privLoadGlyph(Glyph *pGlyph, protoData &pD, Texture::Name textureName);

protected:
    DLink *derivedCreateNode() override;

private:
    Glyph *poNodeCompare;
    static GlyphManager *posInstance;

};
}
#endif // !Glyph_MANAGER_H

