//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#ifndef MESH_DATA_H
#define MESH_DATA_H

#include "meshData.pb.h"
#include "vboData.h"
#include "textureData.h"
#include "animData.h"

class meshData
{
public:
    static const unsigned int FILE_NAME_SIZE = 64;

    enum class RENDER_MODE
    {
        MODE_POINTS = 0,
        MODE_LINE,
        MODE_LINE_LOOP,
        MODE_LINE_STRIP,
        MODE_TRIANGLES,
        MODE_TRIANGLE_STRIP,
        MODE_TRIANGLE_FAN,
        DEFAULT = MODE_TRIANGLES
    };

public:
    meshData();
    meshData(const meshData &);
    meshData &operator = (const meshData &);
    ~meshData();

    /*meshData( char *_pName,
              RENDER_MODE _mode,
              )*/

    // Serialization
    void Serialize(meshData_proto &out) const;
    void Deserialize(const meshData_proto &in);

    void Print(const char *const pName, const int i) const;

public:
    bool                enabled;
    char                pName[FILE_NAME_SIZE]; //gltfModel->meshes[i]->name
    RENDER_MODE         mode;

    vboData             vbo_vert;
    vboData             vbo_norm;
    vboData             vbo_uv;
    vboData             vbo_index;
    vboData             vbo_color;

    vboData             vbo_weights;
    vboData             vbo_joints;
    vboData             vbo_invBind;

    unsigned int        triCount;
    unsigned int        vertCount;
    unsigned int        materialIndex;
    signed int          jointIndex;
    signed int          parentJointIndex;
   
};

#endif

// --- End of File ---
