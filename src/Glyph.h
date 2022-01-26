#ifndef GLYPH_H
#define GLYPH_H

#include "DLink.h"
#include "Image.h"
using namespace Azul;
class Glyph : public DLink
{
public:
    enum class Name
    {
        FONT_30PT,
        NOT_INITIALIZED
    };

public:
    Glyph();
    Glyph(const Glyph &) = delete;
    Glyph &operator=(const Glyph &) = delete;
    virtual ~Glyph() override;

    //void Set();

	virtual char* GetName() override;
	virtual bool Compare(DLink* pTarget) override;
	virtual void Dump() override;
	virtual void Wash() override;

public:
    Name          glyphName;
    Texture       *pTexture;
    unsigned int  fontSize;
    unsigned int  startingOffset;
    Image         *pImage;

};

#endif // !GLYPH_H

