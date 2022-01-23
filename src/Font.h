#ifndef FONT_H
#define FONT_H

#include "DLink.h"
#include "Glyph.h"

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

    void Set();

    virtual char *GetName() override;
    virtual bool Compare(DLink *pTarget) override;
    virtual void Dump() override;
    virtual void Wash() override;

public:
    Name fontName;
    Glyph glyph;
    unsigned int textLength;
    char *pText;
};

#endif