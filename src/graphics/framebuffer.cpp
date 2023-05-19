#include <graphics/framebuffer.h>

Framebuffer::Framebuffer(unsigned int width, unsigned int height) : _fbo(0), _textureID(0), _renderbufferID(0), _clearColor(0.f,0.f,0.f,1.0f), _size({width, height})
{
    GL(GenFramebuffers(1, &_fbo));
    GL(BindFramebuffer(GL_FRAMEBUFFER, _fbo));
    
    //GL(glGenTextures(1, &_textureID));
    //GL(glBindTexture(GL_TEXTURE_2D, _textureID));

    //TODO: framebuffer texture and filtering
    //...
    
    GL(BindTexture(GL_TEXTURE_2D, 0));
    GL(FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureID, 0));

    GL(GenRenderbuffers(1, &_renderbufferID));
    GL(BindRenderbuffer(GL_RENDERBUFFER, _renderbufferID));
    GL(RenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _size.x, _size.y));
    GL(BindRenderbuffer(GL_RENDERBUFFER, 0));
    GL(FramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL, GL_RENDERBUFFER, _renderbufferID));

    int status = glad_glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Failed to create a framebuffer. Status " << status << std::endl;
    }

    GL(BindFramebuffer(GL_FRAMEBUFFER, 0));
}

Framebuffer::~Framebuffer()
{
    glad_glDeleteFramebuffers(1, &_fbo);
    _fbo = 0;
    _renderbufferID = 0;
    //_textureID = 0;
}