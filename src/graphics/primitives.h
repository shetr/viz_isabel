#pragma once
#include <core/common.h>
#include <core/arrays.h>

enum class Axis
{
    X = 0,
    Y,
    Z,
    NONE
};

class Primitive
{
public:
    Primitive();
};

struct VertexPosUV
{
    glm::vec3 pos;
    glm::vec2 uv;
};

struct VertexPosVel
{
    glm::vec3 pos;
    float vel;
    glm::vec3 normal;
};

class Line
{

};

enum class GeometryType
{
    STREAMSLINES = 0,
    ARROWS
};

class GeometryGenerator
{
private:
    unsigned int _axisNumSamples = 30;
    float _spacing;
    float _cellSize;
    float _lineLength;
public:
    typedef void (GeometryGenerator::*GenGeomFunc)(const vector3d<glm::vec3>&, glm::vec3, unsigned int&, std::vector<VertexPosVel> &, std::vector<int>&);

    unsigned int& GetAxisNumSamples() { return _axisNumSamples; }

    void GenTriangle(unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices, float vel, glm::vec3 x1, glm::vec3 x2, glm::vec3 x3, glm::vec3 n1, glm::vec3 n2, glm::vec3 n3);

    void GenQuad(unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices, float vel, glm::vec3 x11, glm::vec3 x12, glm::vec3 x21, glm::vec3 x22, glm::vec3 n11, glm::vec3 n12, glm::vec3 n21, glm::vec3 n22);

    void GenLineSegment(const vector3d<glm::vec3>& data, glm::vec3 pos, unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices);

    void GenTubeSegment(const vector3d<glm::vec3>& data, glm::vec3 pos, unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices);

    void GenArrow(const vector3d<glm::vec3>& data, glm::vec3 pos, unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices);

    void GenStreamline(const vector3d<glm::vec3>& data, glm::vec3 pos, unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices);

    //void GenerateLines(vector3d<glm::vec3>& data);VertexPosVel
    void GenerateGeometry(const vector3d<glm::vec3>& data, Axis axis, float cutPos, std::vector<VertexPosVel> &pts, std::vector<int> &indices, GenGeomFunc genGeomFunc);


    void HgtGeometry(const vector3d<float>& data, std::vector<VertexPosVel> &pts, std::vector<int> &indices);

};