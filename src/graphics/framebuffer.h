#pragma once
#include <core/common.h>

class Framebuffer
{    
private:
    GLuint _fbo;
    GLuint _textureID;
    GLuint _renderbufferID;

    glm::ivec2 _size;
	glm::vec4 _clearColor;

public:
    Framebuffer(unsigned int width, unsigned int height);
    ~Framebuffer();

    inline GLuint GetFBO() const
    {
        return _fbo;
    }

    inline GLuint GetTextureID() const
    {
        return _textureID;
    }
    
    inline GLuint GetRenderBufferID() const
    {
        return _renderbufferID;
    }

	inline void SetClearColor(float r, float g, float b, float a) 
	{ 
		_clearColor = glm::vec4(r, g, b, a); 
	}

	inline void SetClearColor(glm::vec4 newClearColor) 
	{ 
		_clearColor = newClearColor; 
	}

};