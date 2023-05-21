#include "texture.h"

Texture::Texture(GLenum type, GLenum sizedFormat, GLenum format, GLenum dataType)
    : _type(type), _sizedFormat(sizedFormat), _format(format), _dataType(dataType)
{
    GL(CreateTextures(_type, 1, &_id));

    GL(TextureParameteri(_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL(TextureParameteri(_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL(TextureParameteri(_id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    GL(TextureParameteri(_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL(TextureParameteri(_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
}

Texture::~Texture()
{
    if(_id != 0) {
        GL(DeleteTextures(1, &_id));
    }
}

void Texture::SetData2D(int width, int height, const void* data)
{
    GL(BindTexture(_type, _id));
    GL(TexImage2D(
        _type,
        0,
        _sizedFormat,
        width,
        height,
        0,
        _format,
        _dataType,
        data
    ));
    GL(BindTexture(_type, 0));
}

void Texture::SetData3D(int width, int height, int depth, const void* data)
{
    GL(BindTexture(_type, _id));
    GL(TexImage3D(
        _type,
        0,
        _sizedFormat,
        width,
        height,
        depth,
        0,
        _format,
        _dataType,
        data
    ));
    GL(BindTexture(_type, 0));
}


void Texture::BindToUnit(GLenum unit)
{
    GL(BindTextureUnit(unit, _id));
}
void Texture::UnbindUnit(GLenum unit)
{
    GL(BindTextureUnit(unit, 0));
}