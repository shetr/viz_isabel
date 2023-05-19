#pragma once

#include <core/common.h>

class Texture
{
private:
    GLuint _id;
    GLenum _type;
    GLenum _sizedFormat;
    GLenum _format;
    GLenum _dataType;
public:

    Texture(GLenum type, GLenum sizedFormat, GLenum format, GLenum dataType);
    ~Texture();

    
    void SetData2D(int width, int height, const void* data);
    void SetData3D(int width, int height, int depth, const void* data);

    void BindToUnit(GLenum unit);
    static void UnbindUnit(GLenum unit);
};