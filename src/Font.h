#ifndef FONT_H
#define FONT_H

class Font : public DLink
{
public:
    enum class Name
    {
        FONT_30PT
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
    Name name;
    unsigned int  fontSize;
    unsigned int  *x;
    unsigned int  *y;
    unsigned int  *width;
    unsigned int  *height;
};

#endif