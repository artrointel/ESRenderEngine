//
// Created by we.kim on 2017-08-14.
//

#include "Texture2DApp.h"

bool Texture2DApp::init()
{
    mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if(!mProgram)
        return false;

    // Axis VAO
    glGenVertexArrays(1, &mAxisVaoId);
    glBindVertexArray(mAxisVaoId);
    {
        glGenBuffers(1, &mAxisVboId);
        glBindBuffer(GL_ARRAY_BUFFER, mAxisVboId);
        GLfloat axis[42] = {0, 0, 0, // X
                            1, 0, 0, 1,

                           100, 0, 0,
                            1, 0, 0, 1,

                           0, 0, 0, // Y
                            0, 1, 0, 1,

                           0, 100, 0,
                            0, 1, 0, 1,

                           0, 0, 0, // Z
                            0, 0, 1, 1,

                           0, 0, 100,
                            0, 0, 1, 1
        };
        GLuint stride = sizeof(GLfloat) * 7;
        GLuint color_offset = sizeof(GLfloat) * 3;
        glBufferData(GL_ARRAY_BUFFER, sizeof(axis), axis, GL_STATIC_DRAW);
        glVertexAttribPointer(VPOS_ATTR_LOC, 3, GL_FLOAT, GL_FALSE, stride, 0);
        glVertexAttribPointer(VCLR_ATTR_LOC, 4, GL_FLOAT, GL_FALSE, stride, (void *)color_offset);
        glEnableVertexAttribArray(VPOS_ATTR_LOC);
        glEnableVertexAttribArray(VCLR_ATTR_LOC);
    }

    glBindVertexArray(0);

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
    glVertexAttribPointer(VCLR_ATTR_LOC, 4, GL_FLOAT, GL_FALSE, Cube3D::ByteStride, (void*)Cube3D::ByteOffsetPos);
    glEnableVertexAttribArray(VPOS_ATTR_LOC);
    glEnableVertexAttribArray(VCLR_ATTR_LOC);

    // Upload Textures
    // set the pixel row of alignment, default value is 4 which means that word-sized-memory.
    // but we're setting it to 1 which means that tightly-packed-memory.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // 1. make Texture object
    glGenTextures(1, &mCubeToId);
    glBindTexture(GL_TEXTURE_2D, mCubeToId);
    // 2. Upload texture data by using glTexImageXX.
    // Now we're just uploading a simple texture 2x2 like this. one of the 0 value in ...2,2,0...is dummy for consistency of API.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
    // 'texture' which is last parameter of glTexImage2D will be replaced by offset of the Pixel Buffer Object.
    // as the glVertexAttribPointer() does.

    // 3. Generate mipmap example but actually we don't have to use it.
    _generate_box_filtering_mipmapEven(texture, 2, 2, 0);
    // or, Generate mipmap from GL API.
    glGenerateMipmap(GL_TEXTURE_2D);
    // 4. Set Filtering Modes when MIN/MAX. when mipmapping gets fault, gpu should refer these filter options.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // (nearest sampling but it causes bad artifacts like aliasing.)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // but actually, we'll not using the glTexParameter() to set the same settings for every textures (that is sampler)

    // 5. Set Texture Wrapping Modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // recursive width  texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // recursive height texture


    glActiveTexture(GL_TEXTURE0);
    GLuint samplerLoc = glGetUniformLocation(mProgram, "s_texture");
    // actually gles uses sampler = 0 as default sampler.
    // it means that the current of texture object settings will be used, not the sampler object.
    glUniform1i(samplerLoc, 0);

    // Init CamMatrix
    io = InputHandlerTouchImpl::GetInstance();
    InputHandlerTouchImpl::GetInstance()->init(mProgram);
    //io = InputHandlerDebug::GetInstance();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    checkGLError("Texture2DApp::init");
    return true;
}

// Suppose that width and height is even number. or, mipmap data will ignore the edge texture pixels.
// But this mechanism is already implemented on GPU-side, that is glGenerateMipmap()
// 1. since drawing with texture data requires cache memory from GPU side, mipmapping gets better performance in general.
// 2. also, since texture filtering mechanism requires some hw cost, using by mipmapped-texture gets better performance in general.
void Texture2DApp::_generate_box_filtering_mipmapEven(GLubyte *texture, GLuint width, GLuint height, GLuint level)
{
    if(width < 2 || height < 2)
        return;

    GLubyte *prevImage = texture;

    int newWidth = width/2;
    int newHeight = height/2;
    GLubyte *newTexture = new GLubyte[newHeight*newWidth];
    for(int h = 0; h < newHeight; h++)
    {
        int row = width*2*h;
        int next_row = width*(2*h+1);
        for(int w = 0; w < newWidth; w++)
        {
            // 2x2 to a pixel.
            GLubyte pixel = ((int)texture[2*w + row] + (int)texture[2*w+1 + row] +
                            (int)texture[2*w + next_row] + (int)texture[2*w+1 + next_row])*0.25f;

            newTexture[w + width*h] = pixel;
        }
    }

    // Finally Upload generated new texture as mipmap recursively.
    glTexImage2D(GL_TEXTURE_2D, level, GL_RGB,
                newWidth,newHeight, 0, GL_RGB,
                GL_UNSIGNED_BYTE, newTexture);

    _generate_box_filtering_mipmapEven(newTexture, newWidth, newHeight, ++level);

    // Delete all data of mipmap texture which has been already copied to the GPU.
    delete[] newTexture;
    newTexture = nullptr;
}

void Texture2DApp::draw()
{
    glBindVertexArray(mAxisVaoId);
    glDrawArrays(GL_LINES,0,6);
    glBindVertexArray(0);
    glDrawElements(GL_TRIANGLE_STRIP, sizeof(mIndices)/sizeof(GLfloat), GL_UNSIGNED_INT, 0);
}

void Texture2DApp::render()
{
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(mProgram);
    draw();
    checkGLError("Texture2DApp::render");
}