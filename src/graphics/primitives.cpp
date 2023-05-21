#include <graphics/primitives.h>

glm::vec3 TrilinearInterpolation(vector3d<glm::vec3>& data, glm::vec3 coords)
{
    glm::vec3 c000 = glm::vec3(std::floor(coords.x), std::floor(coords.y), std::floor(coords.z));
    glm::vec3 c001 = glm::vec3(std::floor(coords.x), std::floor(coords.y), std::ceil(coords.z));
    glm::vec3 c010 = glm::vec3(std::floor(coords.x), std::ceil(coords.y), std::floor(coords.z));
    glm::vec3 c011 = glm::vec3(std::floor(coords.x), std::ceil(coords.y), std::ceil(coords.z));
    
    glm::vec3 c100 = glm::vec3(std::ceil(coords.x), std::floor(coords.y), std::floor(coords.z));
    glm::vec3 c110 = glm::vec3(std::ceil(coords.x), std::ceil(coords.y), std::floor(coords.z));
    glm::vec3 c101 = glm::vec3(std::ceil(coords.x), std::floor(coords.y), std::ceil(coords.z));
    glm::vec3 c111 = glm::vec3(std::ceil(coords.x), std::ceil(coords.y), std::ceil(coords.z));
    
    glm::vec3 f = coords - c000;

    glm::vec3 val = 
        c000 * (1.0f - f.x) * (1.0f - f.y) * (1.0f - f.z) + 
        c001 * (1.0f - f.x) * (1.0f - f.y) * (f.z) + 
        c010 * (1.0f - f.x) * (f.y) * (1.0f - f.z) +
        c011 * (1.0f - f.x) * (f.y) * (f.z) +
        c100 * (f.x) * (1.0f - f.y) * (1.0f - f.z) +
        c110 * (f.x) * (f.y) * (1.0f - f.z) +
        c101 * (f.x) * (1.0f - f.y) * f.z +
        c111 * (f.x) * (f.y) * (f.z);

    return val;
}

void GenLineSegment(const vector3d<glm::vec3>& data, glm::vec3 pos, unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices, glm::vec3 cellSize, glm::vec3 lineSize)
{
    glm::vec3 s = (pos * cellSize) - 0.5f; //start position
    glm::vec3 e = s + (glm::normalize(data[pos]) * lineSize); // start position + vec direction normalized
    float vel = std::sqrt(glm::dot(data[pos], data[pos]));
    pts.push_back({s, vel});
    pts.push_back({e, vel});
    indices.push_back(index);
    indices.push_back(index + 1);
    index += 2;
}

void GenQuad(unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices, float vel, 
            glm::vec3 x11, glm::vec3 x12, glm::vec3 x21, glm::vec3 x22, glm::vec3 n11, glm::vec3 n12, glm::vec3 n21, glm::vec3 n22)
{
    pts.push_back({x11, vel, n11});
    pts.push_back({x12, vel, n12});
    pts.push_back({x21, vel, n21});
    pts.push_back({x22, vel, n22});
    indices.push_back(index + 0);
    indices.push_back(index + 1);
    indices.push_back(index + 2);
    indices.push_back(index + 2);
    indices.push_back(index + 1);
    indices.push_back(index + 3);
    index += 4;
}

void GenTubeSegment(const vector3d<glm::vec3>& data, glm::vec3 pos, unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices, glm::vec3 cellSize, glm::vec3 lineSize)
{
    const int numSegments = 6;
    float tubeSize = cellSize.x;
    float vel = std::sqrt(glm::dot(data[pos], data[pos]));
    glm::vec3 s = (pos * cellSize) - 0.5f;
    glm::vec3 dir = glm::normalize(data[pos]);
    const float epsilon = 1e-9;
    double length = sqrt(dir.x * dir.x + dir.y * dir.y);
    glm::vec3 uDir;
    if (abs(dir.x) > epsilon && abs(dir.y) > epsilon) {
        uDir = glm::vec3(dir.y / length, -dir.x / length, 0);
    } else if (abs(dir.y) > epsilon) {
        length = sqrt(dir.y * dir.y + dir.z * dir.z);
        uDir = glm::vec3(0, -dir.z / length, dir.y / length);
    } else {
        length = sqrt(dir.x * dir.x + dir.z * dir.z);
        uDir = glm::vec3(-dir.z / length, 0, dir.x / length);
    }
    glm::vec3 vDir = glm::cross(dir, uDir);
    uDir *= tubeSize;
    vDir *= tubeSize;
    for (int i = 0; i < numSegments; ++i) {
        float phi1 = (float)i * 2.f * glm::pi<float>() / (float)numSegments;
        float phi2 = ((float)i + 1.f) * 2.f * glm::pi<float>() / (float)numSegments;
        float u1 = glm::cos(phi1);
        float v1 = glm::sin(phi1);
        float u2 = glm::cos(phi2);
        float v2 = glm::sin(phi2);

        glm::vec3 x11 = s + u1 * uDir + v1 * vDir;
        glm::vec3 x12 = x11 + dir * lineSize;
        glm::vec3 x21 = s + u2 * uDir + v2 * vDir;
        glm::vec3 x22 = x21 + dir * lineSize;

        glm::vec3 n1 = u1 * uDir + v1 * vDir;
        glm::vec3 n2 = u2 * uDir + v2 * vDir;

        GenQuad(index, pts, indices, vel, x11, x12, x21, x22, n1, n1, n2, n2);
    }
}

void GenerateLinesLayerHorizontal(const vector3d<glm::vec3>& data, unsigned int level, std::vector<VertexPosVel> &pts, std::vector<int> &indices, unsigned int spacing)
{
    glm::vec3 cellSize = glm::vec3(1.f / data.dim().x, 1.f / data.dim().y, 1.f / data.dim().z);
    glm::vec3 lineSize = glm::vec3(1.f/data.dim().x) * glm::vec3(spacing);

    bool first = true;
    unsigned int i = 0;
    // uniform generation
    for (unsigned int y = level ; y <= level; y++)
    {
        for (unsigned int z = 0; z < data.dim().z; z += spacing)
        {
            for (unsigned int x = 0; x < data.dim().x; x += spacing)
            {
                GenTubeSegment(data, glm::vec3(x, y, z), i, pts, indices, cellSize, lineSize);
            }
        }
    }
}