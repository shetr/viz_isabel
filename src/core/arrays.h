#pragma once

#include "common.h"

template<typename T>
class vector2d
{
private:
    glm::uvec2 m_dim;
    std::vector<T> m_data;
public:
    vector2d() : m_dim(glm::uvec2(0,0)) {}
    vector2d(glm::uvec2 a_dim, const T& v) : m_dim(a_dim), m_data(a_dim.x*a_dim.y, v) {}
    vector2d(glm::uvec2 a_dim) : m_dim(a_dim), m_data(a_dim.x*a_dim.y) {}
    template<class InputIt>
    vector2d(glm::uvec2 a_dim, InputIt first, InputIt last) : m_dim(a_dim), m_data(first, last) {
        assert(a_dim.x*a_dim.y == m_data.size());
    }
    vector2d(glm::uvec2 a_dim, std::initializer_list<T> init) : m_dim(a_dim), m_data(init) {
        assert(a_dim.x*a_dim.y == m_data.size());
    }
    void assign(glm::uvec2 a_dim, const T& v) {
        m_dim = a_dim;
        m_data.assign(m_dim.x*m_dim.y, v);
    }
    template<class InputIt>
    void assign(glm::uvec2 a_dim, InputIt first, InputIt last ) {
        m_dim = a_dim;
        m_data.assign(first, last);
        assert(a_dim.x*a_dim.y == m_data.size());
    }
    void assign(glm::uvec2 a_dim, std::initializer_list<T> init ) {
        m_dim = a_dim;
        m_data.assign(init);
        assert(a_dim.x*a_dim.y == m_data.size());
    }

    inline size_t index(glm::uvec2 i) const { return i.x + i.y*m_dim.x; }
    inline size_t index(size_t x, size_t y) const { return x + y*m_dim.x; }
    inline T& operator[](size_t i) { return m_data[i]; }
    inline const T& operator[](size_t i) const { return m_data[i]; }
    inline T& operator[](glm::uvec2 i) { return m_data[index(i)]; }
    inline const T& operator[](glm::uvec2 i) const { return m_data[index(i)]; }
    inline T& operator()(size_t x, size_t y) { return m_data[index(x, y)]; }
    inline const T& operator()(size_t x, size_t y) const { return m_data[index(x, y)]; }

    inline const T* data() const { return m_data.data(); }
    inline T* data() { return m_data.data(); }
    inline const std::vector<T>& flattend() const { return m_data; }
    inline std::vector<T>& flattend() { return m_data; }

    inline typename std::vector<T>::iterator begin() { return m_data.begin(); }
    inline typename std::vector<T>::iterator end() { return m_data.end(); }
    inline const typename std::vector<T>::iterator begin() const { return m_data.begin(); }
    inline const typename std::vector<T>::iterator end() const { return m_data.end(); }

    inline bool empty() const { return m_data.empty(); }
    inline size_t size() const { return m_data.size(); }
    inline glm::uvec2 dim() const { return m_dim; }
    inline void reserve(size_t cap) { m_data.reserve(cap); }
    inline void reserve(glm::uvec2 a_dim) { m_data.reserve(a_dim.x*a_dim.y); }
    inline size_t capacity() const { return m_data.capacity(); }
    inline void shrink_to_fit() { m_data.shrink_to_fit(); }

    inline void clear() {
        m_data.clear();
        m_dim = glm::uvec2(0, 0);
    }
    inline void swap(vector2d<T>& other) { 
        m_data.swap(other.m_data);
        std::swap(m_dim, other.m_dim);
    }
};

template<typename T>
class vector3d
{
private:
    glm::uvec3 m_dim;
    std::vector<T> m_data;
public:
    vector3d() : m_dim(glm::uvec3(0,0,0)) {}
    vector3d(glm::uvec3 a_dim, const T& v) : m_dim(a_dim), m_data(a_dim.x*a_dim.y*a_dim.z, v) {}
    vector3d(glm::uvec3 a_dim) : m_dim(a_dim), m_data(a_dim.x*a_dim.y*a_dim.z) {}
    template<class InputIt>
    vector3d(glm::uvec3 a_dim, InputIt first, InputIt last) : m_dim(a_dim), m_data(first, last) {
        assert(a_dim.x*a_dim.y*a_dim.z == m_data.size());
    }
    vector3d(glm::uvec3 a_dim, std::initializer_list<T> init) : m_dim(a_dim), m_data(init) {
        assert(a_dim.x*a_dim.y*a_dim.z == m_data.size());
    }
    void assign(glm::uvec3 a_dim, const T& v) {
        m_dim = a_dim;
        m_data.assign(m_dim.x*m_dim.y*m_dim.z, v);
    }
    template<class InputIt>
    void assign(glm::uvec3 a_dim, InputIt first, InputIt last ) {
        m_dim = a_dim;
        m_data.assign(first, last);
        assert(a_dim.x*a_dim.y*m_dim.z == m_data.size());
    }
    void assign(glm::uvec3 a_dim, std::initializer_list<T> init ) {
        m_dim = a_dim;
        m_data.assign(init);
        assert(a_dim.x*a_dim.y*m_dim.z == m_data.size());
    }

    inline bool is_in_range(glm::vec3 v) const { return v.x >= 0 && v.y >= 0 && v.z >= 0 && v.x <= m_dim.x - 1 && v.y <= m_dim.y - 1 && v.z <= m_dim.z - 1; }

    inline size_t index(glm::uvec3 i) const { return i.x + i.y*m_dim.x + i.z*m_dim.x*m_dim.y; }
    inline size_t index(size_t x, size_t y, size_t z) const { return x + y*m_dim.x + z*m_dim.x*m_dim.y; }
    inline T& operator[](size_t i) { return m_data[i]; }
    inline const T& operator[](size_t i) const { return m_data[i]; }
    inline T& operator[](glm::uvec3 i) { return m_data[index(i)]; }
    inline const T& operator[](glm::uvec3 i) const { return m_data[index(i)]; }
    inline T& operator()(size_t x, size_t y, size_t z) { return m_data[index(x, y, z)]; }
    inline const T& operator()(size_t x, size_t y, size_t z) const { return m_data[index(x, y, z)]; }

    inline const T* data() const { return m_data.data(); }
    inline T* data() { return m_data.data(); }
    inline const std::vector<T>& flattend() const { return m_data; }
    inline std::vector<T>& flattend() { return m_data; }

    inline typename std::vector<T>::iterator begin() { return m_data.begin(); }
    inline typename std::vector<T>::iterator end() { return m_data.end(); }
    inline const typename std::vector<T>::iterator begin() const { return m_data.begin(); }
    inline const typename std::vector<T>::iterator end() const { return m_data.end(); }

    inline bool empty() const { return m_data.empty(); }
    inline size_t size() const { return m_data.size(); }
    inline glm::uvec3 dim() const { return m_dim; }
    inline void reserve(size_t cap) { m_data.reserve(cap); }
    inline void reserve(glm::uvec3 a_dim) { m_data.reserve(a_dim.x*a_dim.y*a_dim.z); }
    inline size_t capacity() const { return m_data.capacity(); }
    inline void shrink_to_fit() { m_data.shrink_to_fit(); }

    inline void clear() {
        m_data.clear();
        m_dim = glm::uvec3(0, 0, 0);
    }
    inline void swap(vector3d<T>& other) { 
        m_data.swap(other.m_data);
        std::swap(m_dim, other.m_dim);
    }
};