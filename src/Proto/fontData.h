#ifndef FONT_DATA_H
#define FONT_DATA_H

#include "fontData.pb.h"

class fontData
{
public:
    static const unsigned int FILE_NAME_SIZE = 64;

    fontData();
    fontData(const fontData &);
    fontData &operator=(const fontData &);
    ~fontData() = default;

    //fontData(   )

    void Serialize(fontData_proto &out) const;
    void Deserialize(const fontData_proto &in);

    void Print(const char *const pName) const;

public:
    bool            enabled;
    unsigned int    key;
    unsigned int    x;
    unsigned int    y;
    unsigned int    width;
    unsigned int    height;
};

#endif
