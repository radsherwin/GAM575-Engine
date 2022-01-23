#ifndef GLYPH_H
#define GLYPH_H

#include "DLink.h"

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
    unsigned int  fontSize;
    unsigned int  *x;
    unsigned int  *y;
    unsigned int  *width;
    unsigned int  *height;

};

#endif // !GLYPH_H

