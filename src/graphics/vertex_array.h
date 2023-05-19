#pragma once

#include <core/common.h>

class VertexElement
{
private:
    GLenum _type;
    int _count;
    int _byteSize;
    int _offset = 0;
public:
    VertexElement(GLenum type, int count)
        : _type(type), _count(count), _byteSize(_count*TypeSize(type))
    {}

    inline int Count() const { return _count; }
    inline int ByteSize() const { return _byteSize; }
    inline void SetOffset(int offset) { _offset = offset; }
    inline int GetOffset() const { return _offset; }
    int Type() const { return _type; }

    static int TypeSize(GLenum type);
};

class VertexLayout
{
private:
    std::vector<VertexElement> _elements;
    int _stride = 0;
public:
    VertexLayout() : _elements() {}
    VertexLayout(std::initializer_list<VertexElement> init);
    VertexLayout(const std::vector<VertexElement>& elements);

    void AddElement(const VertexElement& element);
    inline int ElementsCount() const { return _elements.size(); }
    inline const std::vector<VertexElement>& Elements() const { return _elements; }
    int ByteSize() const;
    int GetStride() const { return _stride; }
};

class GraphicsBuffer
{
protected:
    GLuint _id = 0;
    GLenum _target;
    int _size;
protected:
    GraphicsBuffer(GLenum target, int size) : GraphicsBuffer(target, size, nullptr) {}
    GraphicsBuffer(GLenum target, int size, const void* data);
public:
    void SetData(int offset, int size, const void* data);

    void Bind();
    void UnBind();
};

class VertexBuffer : public GraphicsBuffer
{
private:
    VertexLayout _layout;
public:
    VertexBuffer(int size, const VertexLayout& layout) : GraphicsBuffer(GL_ARRAY_BUFFER, size), _layout(layout) {}
    VertexBuffer(int size, const void* data, const VertexLayout& layout) : GraphicsBuffer(GL_ARRAY_BUFFER, size, data), _layout(layout) {}

    inline const VertexLayout& GetLayout() const { return _layout; }
};

// uses 32bit integers
class IndexBuffer : public GraphicsBuffer
{
public:
    IndexBuffer(int size) : GraphicsBuffer(GL_ELEMENT_ARRAY_BUFFER, size) {}
    IndexBuffer(int size, const int* data) : GraphicsBuffer(GL_ELEMENT_ARRAY_BUFFER, size, data) {}
};

class VertexArray
{
private:
    GLuint _id;
    unsigned int _elementCount = 0;
public:
    VertexArray(VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer);
    ~VertexArray();

    void Bind();
    void UnBind();
private:
    void EnableVertexAttribArray(int i);
    void VertexAttribPointer(int i, int count, int type, bool normalized, int stride, const void* offset);
    void VertexAttribDivisor(int i, int divisor);
};