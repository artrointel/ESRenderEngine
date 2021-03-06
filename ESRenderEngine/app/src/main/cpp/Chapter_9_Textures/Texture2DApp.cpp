//
// Created by we.kim on 2017-08-14.
//

#include "Texture2DApp.h"

bool Texture2DApp::init()
{
    mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if(!mProgram)
        return false;

    // Upload Indices
    Cube3D::GetIndices(GL_TRIANGLE_FAN, mIndices);
    glGenBuffers(1, &mCubeIboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mCubeIboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mIndices), mIndices, GL_STATIC_DRAW);

    // Upload Positions
    glGenBuffers(1, &mCubeVboId);
    glBindBuffer(GL_ARRAY_BUFFER, mCubeVboId);
    glBufferData(GL_ARRAY_BUFFER, cube3D.ByteSize, cube3D.attrib, GL_STATIC_DRAW);
    glVertexAttribPointer(VPOS_ATTR_LOC, 3, GL_FLOAT, GL_FALSE, Cube3D::ByteStride, 0);
    glEnableVertexAttribArray(VPOS_ATTR_LOC);

    // Upload Textures
/*
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &mCubeToId);
    glBindTexture(GL_TEXTURE_2D, mCubeToId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
*/
    // Init CamMatrix
    io = InputHandlerTouchImpl::GetInstance();
    InputHandlerTouchImpl::GetInstance()->init(mProgram);

    checkGLError("Texture2DApp::init");
    return true;
}

void Texture2DApp::draw()
{
    glDrawElements(GL_TRIANGLE_FAN, 30, GL_UNSIGNED_INT, 0);
}

void Texture2DApp::render()
{
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(mProgram);
    draw();
    checkGLError("Texture2DApp::render");
}