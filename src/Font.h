#ifndef FONT_H
#define FONT_H

#include "DLink.h"
#include "Glyph.h"
#include "Rect.h"
#include "Matrix.h"

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
             Glyph::Name _glyphName,
             const float &xStart,
             const float &yStart);

    virtual char *GetName() override;
    virtual bool Compare(DLink *pTarget) override;
    virtual void Dump() override;
    virtual void Wash() override;

public:
    Name fontName;
    Rect rect;
    Texture *pTexture;
    Glyph::Name glyphName;
    std::string pText;
    float startX;
    float startY;
};

#endif