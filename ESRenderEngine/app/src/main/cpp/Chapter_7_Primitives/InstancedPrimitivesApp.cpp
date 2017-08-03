//
// Created by we.kim on 2017-07-24.
// http://www.opengl-tutorial.org/kr/intermediate-tutorials/billboards-particles/particles-instancing/

#include "InstancedPrimitivesApp.h"

bool InstancedPrimitivesApp::init()
{
    ALOGD("InstancedPrimitivesApp::init");

    mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if(!mProgram) return false;

    // Position VBO; uses same color vbo per Instance.
    glGenBuffers(1, &vboPosId);
    glBindBuffer(GL_ARRAY_BUFFER, vboPosId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(VPOS_ATTR_LOC, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(VPOS_ATTR_LOC);

    // Color VBO; uses same color vbo per Instance.
    glGenBuffers(1, &vboColorId);
    glBindBuffer(GL_ARRAY_BUFFER, vboColorId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(VCLR_ATTR_LOC, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(VCLR_ATTR_LOC);

#if INSTANCED_DRAW
    // TransformMatrix VBO; uses Difference transform vbo per Instance, because of divisor 1
    GLfloat tMat[16*2] = {0,};
    Matrix4::SetIdentity(transformMatrix.data, 12);
    transformMatrix.fillGLArray(tMat);
    for(int i = 0; i < 4; i++)  // transformMatrix data of First Triangle.
        ALOGD("%f %f %f %f", tMat[4*i], tMat[4*i+1], tMat[4*i+2], tMat[4*i+3]  );

    transformMatrix.data[3] = 1.0f; // translation of x->x+1 for a second triangle
    transformMatrix.fillGLArray(&tMat[16]);
    for(int i = 0; i < 4; i++) // transformMatrix data of Second Triangle.
        ALOGD("%f %f %f %f", tMat[16+4*i], tMat[16+4*i+1], tMat[16+4*i+2], tMat[16+4*i+3]  );
    glGenBuffers(1, &vboMatId);
    glBindBuffer(GL_ARRAY_BUFFER, vboMatId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tMat), tMat, GL_DYNAMIC_DRAW);

    for(int i = 0; i < 4; i++)
    {
        // Mat4 is to be assigned 4 location, which should be uploaded by vec4 type actually
        GLint location = VMAT_ATTR_LOC + i;
        GLint offset = 4*i*sizeof(GLfloat);
        glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(tMat)/2, (void*)offset);
        glEnableVertexAttribArray(location);

        // finally, looping four times with a vec4 will result in a Mat4
        glVertexAttribDivisor(location, 1);
    }
#else
    GLfloat tMat[16] = {0,};
    Matrix4::SetIdentity(transformMatrix.data, 12);
    transformMatrix.fillGLArray(tMat);
    glGenBuffers(1, &vboMatId);
    glBindBuffer(GL_ARRAY_BUFFER, vboMatId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tMat), tMat, GL_DYNAMIC_DRAW);
    for(int i = 0; i < 4; i++)
    {
        // Mat4 is to be assigned 4 location, which should be uploaded by vec4 type actually
        GLint location = VMAT_ATTR_LOC + i;
        GLint offset = 4*i*sizeof(GLfloat);
        glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(tMat), (void*)offset);
        glEnableVertexAttribArray(location);

        // finally, looping four times with a vec4 will result in a Mat4
        glVertexAttribDivisor(location, 1);
    }
#endif
    // Index VBO
    glGenBuffers(1, &vboIndexId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndexId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Init CamMatrix
    io = InputHandlerTouchImpl::GetInstance();
    InputHandlerTouchImpl::GetInstance()->init(mProgram);

    checkGLError("InstancedPrimitivesApp::init() init error");
    ALOGD("InstancedPrimitivesApp::init finished");
    return true;
}

void InstancedPrimitivesApp::render()
{
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT);
    glUseProgram(mProgram);

#if INSTANCED_DRAW
    // Drawing triangles within a draw call by 'glDraw*Instanced()' but
    // divisor with transform matrix will move one of it to x->x+1
    glDrawElementsInstanced(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0, NUM_INSTANCES);
#else
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_SHORT, 0);
#endif
    checkGLError("InstancedPrimitivesApp::render()");
}