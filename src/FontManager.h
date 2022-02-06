#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include "ManBase.h"
#include "Font.h"

class protoData;

class FontManager : public ManBase
{
private:
    FontManager(int reserveNum = 3, int reserveGrow = 1);
    FontManager() = delete;
    FontManager(const FontManager &) = delete;
    FontManager &operator=(const FontManager &) = delete;
    virtual ~FontManager();

public:
    static void Create(int reserveNum = 3, int reserveGrow = 1);
    static void Destroy();

    static Font *Add(const char *const pText, Glyph::Name glyphName, Font::Name fontName, const float &xStart, const float &yStart);
    static void UpdateMessage(Font::Name fontName, std::string pText);
    static void Draw();
    static Font *Find(Font::Name fontName);
    static void Remove(Font *pNode);
    static void Dump();

private:
    static FontManager *privGetInstance();

protected:
    DLink *derivedCreateNode() override;

private:
    Font *poNodeCompare;
    static FontManager *posInstance;
};

#endif // !FONT_MANAGER_H
