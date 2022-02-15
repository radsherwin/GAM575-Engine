//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "ProtoBuffMesh.h"

#include "AnimationManager.h"
#include "TextureManager.h"
#include "MeshFileHdr.h"
#include "File.h"

using namespace Azul;

struct Vert_xyz
{
    float x;
    float y;
    float z;
};

struct Vert_nxnynz
{
    float nx;
    float ny;
    float nz;
};

struct Vert_uv
{
    float u;
    float v;
};

struct Vert_color
{
    float r;
    float g;
    float b;
    float a;
};

struct Vert_weight
{
    float wa;
    float wb;
    float wc;
    float wd;
};

struct Vert_joint
{
    float ja;
    float jb;
    float jc;
    float jd;
};

ProtoBuffMesh::ProtoBuffMesh(meshData &mB, const unsigned int totalBones)
    : Mesh()
{
    this->privCreateMesh(mB, totalBones);
}

ProtoBuffMesh::~ProtoBuffMesh()
{
    // remove anything dynamic here
}

void ProtoBuffMesh::privCreateMesh(meshData &mB, const unsigned int &totalBones)
{
    Trace::out("Building mesh: %s\n", mB.pName);

    this->textureIDInt = mB.materialIndex;
    this->meshName = mB.pName;
    this->totalBones = 12;//totalBones;

    if (mB.jointCount > 0)
    {
        this->poJointData = new Mesh::JointData[mB.jointCount];
        this->jointCount = mB.jointCount;
    }

    jointData *pJD = nullptr;
    for (int i = 0; i < mB.jointCount; i++)
    {
        pJD = &mB.pJointData[i];
        memcpy_s(this->poJointData[i].name, 32, pJD->pName, 32);
        this->poJointData[i].jointIndex = pJD->jointIndex;
        this->poJointData[i].parentIndex = pJD->parentIndex;
    }

    // Create a VAO
    glGenVertexArrays(1, &this->vao);
    assert(this->vao != 0);
    glBindVertexArray(this->vao);

    // Create a VBO
    glGenBuffers(1, &this->vbo_verts);
    assert(this->vbo_verts != 0);

    glGenBuffers(1, &this->vbo_norms);
    assert(this->vbo_norms != 0);

    glGenBuffers(1, &this->vbo_texts);
    assert(this->vbo_texts != 0);

    glGenBuffers(1, &this->vbo_color);
    assert(this->vbo_color != 0);

    glGenBuffers(1, &this->vbo_index);
    assert(this->vbo_index != 0);

    glGenBuffers(1, &this->vbo_weight);
    assert(this->vbo_weight != 0);

    glGenBuffers(1, &this->vbo_joint);
    assert(this->vbo_joint != 0);

    // Skin data

    // General Stuff
    assert(mB.mode == meshData::RENDER_MODE::MODE_TRIANGLES);

    this->numTris = (int)mB.triCount;
    this->numVerts = (int)mB.vertCount;
    assert(this->numTris > 0);
    assert(this->numVerts > 0);

    // Load the verts data: ---------------------------------------------------------
    if (mB.vbo_vert.enabled)
    {
        assert(mB.vbo_vert.targetType == vboData::VBO_TARGET::ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_verts);

        // load the data to the GPU
        assert(mB.vbo_vert.poData);
        assert(mB.vbo_vert.dataSize > 0);
        assert(mB.vbo_vert.count * sizeof(Vert_xyz) == mB.vbo_vert.dataSize);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(mB.vbo_vert.dataSize), mB.vbo_vert.poData, GL_STATIC_DRAW);

        // Vert data is location: 0  (used in vertex shader)
        assert(0 == mB.vbo_vert.attribIndex);
        assert(mB.vbo_vert.vboType == vboData::VBO_TYPE::VEC3);
        assert(mB.vbo_vert.componentType == vboData::VBO_COMPONENT::FLOAT);
        glVertexAttribPointer(mB.vbo_vert.attribIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyz), 0);
        glEnableVertexAttribArray(mB.vbo_vert.attribIndex);

        //Get Verts for Bounding Sphere
        Vert_xyz *meshXYZ = (Vert_xyz *)(mB.vbo_vert.poData);
        Vect *pVerts = new Vect[mB.vbo_vert.count];
        for (int i = 0; i < (int)mB.vbo_vert.count; i++)
        {
            pVerts[i].set(meshXYZ[i].x, meshXYZ[i].y, meshXYZ[i].z);
            //Trace::out("%d:  %f %f %f\n", i, pVerts[i][x], pVerts[i][y], pVerts[i][z]);
        }

        assert(this->poRefSphere);
        this->poRefSphere->RitterSphere(pVerts, (int)mB.vbo_vert.count);

        //Cleaning up verts
        delete[] pVerts;
    }
    else
    {
        this->poRefSphere = nullptr;
    }

    // Load the norms data: ---------------------------------------------------------

    if (mB.vbo_norm.enabled)
    {
        assert(mB.vbo_norm.targetType == vboData::VBO_TARGET::ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_norms);

        // load the data to the GPU
        assert(mB.vbo_norm.poData);
        assert(mB.vbo_norm.dataSize > 0);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(mB.vbo_norm.dataSize), mB.vbo_norm.poData, GL_STATIC_DRAW);

        // normals data in location 1 (used in vertex shader)
        assert(1 == mB.vbo_norm.attribIndex);
        assert(mB.vbo_norm.vboType == vboData::VBO_TYPE::VEC3);
        assert(mB.vbo_norm.componentType == vboData::VBO_COMPONENT::FLOAT);
        glVertexAttribPointer(mB.vbo_norm.attribIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_nxnynz), 0);
        glEnableVertexAttribArray(mB.vbo_norm.attribIndex);
    }

    // Load the texts data: ---------------------------------------------------------

    if (mB.vbo_uv.enabled)
    {
        assert(mB.vbo_uv.targetType == vboData::VBO_TARGET::ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_texts);

        // load the data to the GPU
        assert(mB.vbo_uv.poData);
        assert(mB.vbo_uv.dataSize > 0);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(mB.vbo_uv.dataSize), mB.vbo_uv.poData, GL_STATIC_DRAW);

        // Texture data is location: 2  (used in vertex shader)
        assert(2 == mB.vbo_uv.attribIndex);
        assert(mB.vbo_uv.vboType == vboData::VBO_TYPE::VEC2);
        assert(mB.vbo_uv.componentType == vboData::VBO_COMPONENT::FLOAT);
        glVertexAttribPointer(mB.vbo_uv.attribIndex, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_uv), 0);
        glEnableVertexAttribArray(mB.vbo_uv.attribIndex);
    }

    // Load the color data: ---------------------------------------------------------

    if (mB.vbo_color.enabled)
    {
        assert(mB.vbo_color.targetType == vboData::VBO_TARGET::ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_color);

        // load the data to the GPU
        assert(mB.vbo_color.poData);
        assert(mB.vbo_color.dataSize > 0);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(mB.vbo_color.dataSize), mB.vbo_color.poData, GL_STATIC_DRAW);

        // Texture data is location: 4  (used in vertex shader)
        assert(4 == mB.vbo_color.attribIndex);
        assert(mB.vbo_color.vboType == vboData::VBO_TYPE::VEC4);
        assert(mB.vbo_color.componentType == vboData::VBO_COMPONENT::FLOAT);
        glVertexAttribPointer(mB.vbo_color.attribIndex, 4, GL_FLOAT, GL_FALSE, sizeof(Vert_color), 0);
        glEnableVertexAttribArray(mB.vbo_color.attribIndex);
    }

    // Load the index data: ---------------------------------------------------------
    if (mB.vbo_index.enabled)
    {
        // Bind our 2nd VBO as being the active buffer and storing index )
        assert(mB.vbo_index.targetType == vboData::VBO_TARGET::ELEMENT_ARRAY_BUFFER);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_index);

        // Copy the index data to our buffer
        assert(mB.vbo_index.vboType == vboData::VBO_TYPE::SCALAR);
        assert(mB.vbo_index.componentType == vboData::VBO_COMPONENT::UNSIGNED_INT);
        assert(mB.vbo_index.dataSize > 0);
        assert(mB.vbo_index.poData);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(mB.vbo_index.dataSize), mB.vbo_index.poData, GL_STATIC_DRAW);
    }

    // Load the weight data: ---------------------------------------------------------
    if (mB.vbo_weights.enabled)
    {
        assert(mB.vbo_weights.targetType == vboData::VBO_TARGET::ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_weight);

        // load the data to the GPU
        assert(mB.vbo_weights.poData);
        assert(mB.vbo_weights.dataSize > 0);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(mB.vbo_weights.dataSize), mB.vbo_weights.poData, GL_STATIC_DRAW);

        // Weight data is location: 5  (used in vertex shader)
        assert(5 == mB.vbo_weights.attribIndex);
        assert(mB.vbo_weights.vboType == vboData::VBO_TYPE::VEC4);
        assert(mB.vbo_weights.componentType == vboData::VBO_COMPONENT::FLOAT);
        glVertexAttribPointer(mB.vbo_weights.attribIndex, 4, GL_FLOAT, GL_FALSE, sizeof(Vert_weight), 0);
        glEnableVertexAttribArray(mB.vbo_weights.attribIndex);
    }

    // Load the Joint data: ---------------------------------------------------------
    if (mB.vbo_joints.enabled)
    {
        assert(mB.vbo_joints.targetType == vboData::VBO_TARGET::ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_joint);

        // load the data to the GPU
        assert(mB.vbo_joints.poData);
        assert(mB.vbo_joints.dataSize > 0);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(mB.vbo_joints.dataSize), mB.vbo_joints.poData, GL_STATIC_DRAW);

        // Joint data is location: 6  (used in vertex shader)
        assert(6 == mB.vbo_joints.attribIndex);
        assert(mB.vbo_joints.vboType == vboData::VBO_TYPE::VEC4);
        assert(mB.vbo_joints.componentType == vboData::VBO_COMPONENT::UNSIGNED_INT);

        glVertexAttribIPointer(mB.vbo_joints.attribIndex, 4, GL_UNSIGNED_INT, sizeof(Vert_joint), 0);
        glEnableVertexAttribArray(mB.vbo_joints.attribIndex);
    }

    // Load the InverseBind Matrices data: ---------------------------------------------------------
    if (mB.vbo_invBind.enabled)
    {
        assert(mB.vbo_joints.targetType == vboData::VBO_TARGET::ARRAY_BUFFER);
        //glBindBuffer(GL_ARRAY_BUFFER, this->vbo_invBind);

        // load the data to the GPU
        assert(mB.vbo_invBind.poData);
        assert(mB.vbo_invBind.dataSize > 0);
        //glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(mB.vbo_invBind.dataSize), mB.vbo_invBind.poData, GL_STATIC_DRAW);

        // Inverse Bind data is location: 7  (used in vertex shader)
        assert(7 == mB.vbo_invBind.attribIndex);
        assert(mB.vbo_invBind.vboType == vboData::VBO_TYPE::MAT4);
        assert(mB.vbo_invBind.componentType == vboData::VBO_COMPONENT::FLOAT);
        //glVertexAttribPointer(mB.vbo_invBind.attribIndex, 4, GL_FLOAT, GL_FALSE, mB.vbo_invBind.count*sizeof(Matrix), 0);
        //glEnableVertexAttribArray(mB.vbo_invBind.attribIndex);
        //
        // copy data into buffer
        this->numInverseBind = mB.vbo_invBind.count;
        assert(this->numInverseBind > 0);
        this->poInverseBind = new Matrix[this->numInverseBind]();
        assert(this->poInverseBind);
        memcpy_s(this->poInverseBind, this->numInverseBind * sizeof(Matrix), mB.vbo_invBind.poData, mB.vbo_invBind.dataSize);
    }
}

// --- End of File ---