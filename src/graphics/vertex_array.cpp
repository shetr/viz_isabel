#include "vertex_array.h"

int VertexElement::TypeSize(GLenum type)
{
    switch (type)
    {
    case GL_BYTE:
        return 1;
    case GL_SHORT:
        return 2;
    case GL_INT:
        return 4;
    case GL_UNSIGNED_BYTE:
        return 1;
    case GL_UNSIGNED_SHORT:
        return 2;
    case GL_UNSIGNED_INT:
        return 4;
    case GL_HALF_FLOAT:
        return 2;
    case GL_FLOAT:
        return 4;
    case GL_DOUBLE:
        return 8;
    case GL_FIXED:
        return 1;
    
    default:
        return 0;
    }
}

VertexLayout::VertexLayout(std::initializer_list<VertexElement> init) {
    for(const VertexElement& element: init) {
        AddElement(element);
    }
}
VertexLayout::VertexLayout(const std::vector<VertexElement>& elements) : _elements(elements)
{
    for(VertexElement& element: _elements){
        element.SetOffset(_stride);
        _stride += element.ByteSize();
    }
}

void VertexLayout::AddElement(const VertexElement& element)
{ 
    _elements.push_back(element);
    _elements.back().SetOffset(_stride);
    _stride += element.ByteSize();
}

int VertexLayout::ByteSize() const
{
    int size = 0;
    for(const VertexElement& element : _elements) {
        size += element.ByteSize();
    }
    return size;
}

GraphicsBuffer::GraphicsBuffer(GLenum target, int size, const void* data)
    : _target(target), _size(size)
{
    GL(GenBuffers(1, &_id));
    Bind();
    GL(BufferStorage(_target, size, data, 0));
    UnBind();
}

GraphicsBuffer::~GraphicsBuffer()
{
    GL(DeleteBuffers(1, &_id));
}

void GraphicsBuffer::SetData(int offset, int size, const void* data) {
    Bind();
    GL(BufferSubData(_target, offset, size, data));
    UnBind();
}

void GraphicsBuffer::Bind()
{
    GL(BindBuffer(_target, _id));
}

void GraphicsBuffer::UnBind()
{
    GL(BindBuffer(_target, 0));
}

VertexArray::VertexArray(VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer)
{
    GL(GenVertexArrays(1, &_id));

    Bind();
    indexBuffer.Bind();
    UnBind();
    indexBuffer.UnBind();

    Bind();
    vertexBuffer.Bind();
    const VertexLayout& layout = vertexBuffer.GetLayout();
    for(const VertexElement& element: layout.Elements()) {
        EnableVertexAttribArray(_elementCount);
        VertexAttribPointer(
            _elementCount,
            element.Count(),
            element.Type(),
            false,
            layout.GetStride(),
            (const void*)(intptr_t)element.GetOffset()
        );
        VertexAttribDivisor(_elementCount, 0);
        _elementCount++;
    }
    UnBind();
    vertexBuffer.UnBind();
}

VertexArray::~VertexArray()
{
    GL(DeleteVertexArrays(1, &_id));
}


void VertexArray::Bind()
{
    GL(BindVertexArray(_id));
}

void VertexArray::UnBind()
{
    GL(BindVertexArray(0));
}

void VertexArray::EnableVertexAttribArray(int i)
{
    GL(EnableVertexAttribArray(i));
}
void VertexArray::VertexAttribPointer(int i, int count, int type, bool normalized, int stride, const void* offset)
{
    GL(VertexAttribPointer(
        i,
        count,
        type,
        normalized,
        stride,
        offset
    ));
}
void VertexArray::VertexAttribDivisor(int i, int divisor)
{
    GL(VertexAttribDivisor(i, divisor));
}