#ifndef PROTO_DATA_H
#define PROTO_DATA_H

#include "protoData.pb.h"
#include "meshData.h"
#include "textureData.h"
#include "animData.h"
#include "fontData.h"

class protoData
{
public:
    static const unsigned int FILE_NAME_SIZE = 64;

    static constexpr const char *PROTO_VERSION = "1.3.1"; // Major.Version.Rev
    static const unsigned int VERSION_NUM_BYTES = 8;

public:
    protoData();
    protoData(const protoData &) = delete;
    protoData &operator = (const protoData &) = delete;
    ~protoData();

    // Serialization 
    void Serialize(protoData_proto &out) const;
    void Deserialize(const protoData_proto &in);

    void Print(const char *const _pName) const;

public:
    char				pVersion[VERSION_NUM_BYTES];

    char                pName[FILE_NAME_SIZE]; // Original file name
    unsigned int        meshCount;
    unsigned int        textureCount;
    unsigned int        animCount;
    unsigned int        fontCount;
    meshData            *pMeshData;
    animData            *pAnimData;
    textureData         *pTextureData;
    fontData            *pFontData;

};

#endif

// --- End of File ---
