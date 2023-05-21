#include <graphics/primitives.h>

/*
void GenerateLines(vector3d<glm::vec3>& data, std::vector<float> &pts)
{
    glm::vec3 grid_size = glm::vec3(1.f);

    glm::vec3 cellSize = glm::vec3(1.f / data.dim().x, 1.f / data.dim().y, 1.f / data.dim().z);

    bool first = true;

    // uniform generation
    for (unsigned int z = 0; z < data.dim().z; z++)
    {
        for (unsigned int y = 0; y < data.dim().y; y++)
        {
            for (unsigned int x = 0; x < data.dim().x; x++)
            {
                glm::vec3 s = (glm::vec3(x, y, z) * cellSize) - 0.5f; //start position
                glm::vec3 e = s + (glm::normalize(glm::vec3(data(x,y,z))) * cellSize); // start position + vec direction normalized
                pts.push_back(s.x);
                pts.push_back(s.y);
                pts.push_back(s.z);
                pts.push_back(e.x);
                pts.push_back(e.y);
                pts.push_back(e.z);
            }
        }
    }
}
*/

void GenerateLinesLayerHorizontal(vector3d<glm::vec3>& data, unsigned int level, std::vector<float> &pts, std::vector<float> &velo)
{
    glm::vec3 grid_size = glm::vec3(1.f);

    glm::vec3 cellSize = glm::vec3(1.f / data.dim().x, 1.f / data.dim().y, 1.f / data.dim().z);

    bool first = true;

    // uniform generation
    for (unsigned int y = level ; y <= level; y++)
    {
        for (unsigned int z = 0; z < data.dim().z; z++)
        {
            for (unsigned int x = 0; x < data.dim().x; x++)
            {
                glm::vec3 s = (glm::vec3(x, y, z) * cellSize) - 0.5f; //start position
                glm::vec3 e = s + (glm::normalize(glm::vec3(data(x,y,z))) * cellSize); // start position + vec direction normalized
                pts.push_back(s.x);
                pts.push_back(s.y);
                pts.push_back(s.z);
                pts.push_back(e.x);
                pts.push_back(e.y);
                pts.push_back(e.z);

                velo.push_back(glm::dot(data(x,y,z), data(x,y,z)));
            }
        }
    }
}