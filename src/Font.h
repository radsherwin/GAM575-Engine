#ifndef FONT_H
#define FONT_H

#include "DLink.h"
#include "FontSprite.h"
#include "Glyph.h"
#include "Rect.h"

class Font : public DLink
{
public:
    enum class Name
    {
        FONT_30PT,
        NOT_INITIALIZED
    };

    Font();
    Font(const Font &) = delete;
    Font &operator=(const Font &) = delete;
    virtual ~Font() override;

    void Set(Name _name,
             const char *const pMessage,
             const unsigned int &_textLength,
             Glyph::Name _glyphName,
             const float &xStart,
             const float &yStart);

    void Render();

    virtual char *GetName() override;
    virtual bool Compare(DLink *pTarget) override;
    virtual void Dump() override;
    virtual void Wash() override;

public:
    Name fontName;
    Rect rect;
    Glyph::Name glyphName;
    unsigned int textLength;
    char *pText;
    float startX;
    float startY;
};

#endif