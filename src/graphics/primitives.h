#pragma once
#include <core/common.h>
#include <core/arrays.h>

enum class Axis
{
    X = 0,
    Y,
    Z
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

void GenLineSegment(const vector3d<glm::vec3>& data, glm::vec3 pos, unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices, glm::vec3 cellSize, glm::vec3 lineLength);

void GenTubeSegment(const vector3d<glm::vec3>& data, glm::vec3 pos, unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices, glm::vec3 cellSize, glm::vec3 lineLength);

void GenStreamline(const vector3d<glm::vec3>& data, glm::vec3 pos, unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices, glm::vec3 cellSize, glm::vec3 lineLength);

void GenQuad(unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices, float vel, glm::vec3 x11, glm::vec3 x12, glm::vec3 x21, glm::vec3 x22, glm::vec3 n11, glm::vec3 n12, glm::vec3 n21, glm::vec3 n22);

//void GenerateLines(vector3d<glm::vec3>& data);VertexPosVel
void GenerateLinesLayerHorizontal(const vector3d<glm::vec3>& data, Axis axis, float cutPos, std::vector<VertexPosVel> &pts, std::vector<int> &indices, unsigned int numSamples);
