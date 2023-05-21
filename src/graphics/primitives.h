#pragma once
#include <core/common.h>
#include <core/arrays.h>

class Primitive
{
public:
    Primitive();
};

/*
struct VertexPosUV
{
    glm::vec3 pos;
    glm::vec2 uv;
};
*/
//void GenerateLines(vector3d<glm::vec3>& data, std::vector<float> &pts); //Ambiguous
void GenerateLinesLayerHorizontal(vector3d<glm::vec3>& data, unsigned int level, std::vector<float> &pts, std::vector<float> &velo);
