#include <graphics/primitives.h>

const int g_numSegments = 6;

glm::vec3 TrilinearInterpolation(const vector3d<glm::vec3>& data, glm::vec3 coords)
{
    coords *= glm::vec3(data.dim());
    coords = glm::clamp(coords, glm::vec3(0), glm::vec3(data.dim() - glm::uvec3(1)));
    glm::vec3 coords000 = glm::uvec3((unsigned int)std::floor(coords.x), (unsigned int)std::floor(coords.y), (unsigned int)std::floor(coords.z));
    glm::vec3 c000 = data[coords000];
    glm::vec3 c001 = data[glm::uvec3((unsigned int)std::floor(coords.x), (unsigned int)std::floor(coords.y), (unsigned int)std::ceil(coords.z))];
    glm::vec3 c010 = data[glm::uvec3((unsigned int)std::floor(coords.x), (unsigned int)std::ceil(coords.y), (unsigned int)std::floor(coords.z))];
    glm::vec3 c011 = data[glm::uvec3((unsigned int)std::floor(coords.x), (unsigned int)std::ceil(coords.y), (unsigned int)std::ceil(coords.z))];
    
    glm::vec3 c100 = data[glm::uvec3((unsigned int)std::ceil(coords.x), (unsigned int)std::floor(coords.y), (unsigned int)std::floor(coords.z))];
    glm::vec3 c110 = data[glm::uvec3((unsigned int)std::ceil(coords.x), (unsigned int)std::ceil(coords.y), (unsigned int)std::floor(coords.z))];
    glm::vec3 c101 = data[glm::uvec3((unsigned int)std::ceil(coords.x), (unsigned int)std::floor(coords.y), (unsigned int)std::ceil(coords.z))];
    glm::vec3 c111 = data[glm::uvec3((unsigned int)std::ceil(coords.x), (unsigned int)std::ceil(coords.y), (unsigned int)std::ceil(coords.z))];
    
    glm::vec3 f = coords - coords000;

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

void GenLineSegment(const vector3d<glm::vec3>& data, glm::vec3 pos, unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices, glm::vec3 cellSize, glm::vec3 lineLength)
{
    glm::vec3 datapos = TrilinearInterpolation(data, pos);

    glm::vec3 s = (pos * cellSize) - 0.5f; //start position
    glm::vec3 e = s + (glm::normalize(datapos) * lineLength); // start position + vec direction normalized
    float vel = std::sqrt(glm::dot(datapos, datapos));
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

void GenTubeSegment(const vector3d<glm::vec3>& data, glm::vec3 pos, unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices, glm::vec3 cellSize, glm::vec3 lineLength)
{
    float tubeWidth = cellSize.x;

    glm::vec3 velDir = TrilinearInterpolation(data, pos);

    float vel = std::sqrt(glm::dot(velDir, velDir));
    glm::vec3 s = pos - 0.5f;
    glm::vec3 dir = glm::normalize(velDir);
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
    uDir *= tubeWidth;
    vDir *= tubeWidth;
    for (int i = 0; i < g_numSegments; ++i) {
        float phi1 = (float)i * 2.f * glm::pi<float>() / (float)g_numSegments;
        float phi2 = ((float)i + 1.f) * 2.f * glm::pi<float>() / (float)g_numSegments;
        float u1 = glm::cos(phi1);
        float v1 = glm::sin(phi1);
        float u2 = glm::cos(phi2);
        float v2 = glm::sin(phi2);

        glm::vec3 x11 = s + u1 * uDir + v1 * vDir;
        glm::vec3 x12 = x11 + dir * lineLength;
        glm::vec3 x21 = s + u2 * uDir + v2 * vDir;
        glm::vec3 x22 = x21 + dir * lineLength;

        glm::vec3 n1 = u1 * uDir + v1 * vDir;
        glm::vec3 n2 = u2 * uDir + v2 * vDir;

        GenQuad(index, pts, indices, vel, x11, x12, x21, x22, n1, n1, n2, n2);
    }
}

void GenStreamline(const vector3d<glm::vec3>& data, glm::vec3 pos, unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices, glm::vec3 cellSize, glm::vec3 lineLength)
{
    const unsigned int slineSegments = 5;
    const float tubeWidth = cellSize.x;

    glm::vec3 velDir = TrilinearInterpolation(data, pos);
    
    unsigned int currSegment = 0;
    glm::vec3 currPos = pos - 0.5f;
    glm::vec3 currVelDir = velDir;
    
    while(currSegment < slineSegments)
    {

        float vel = std::sqrt(glm::dot(currVelDir, currVelDir));
        
        glm::vec3 s = currPos;
        glm::vec3 dir = glm::normalize(currVelDir);
        
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
        uDir *= tubeWidth;
        vDir *= tubeWidth;
        for (int i = 0; i < g_numSegments; ++i) {
            float phi1 = (float)i * 2.f * glm::pi<float>() / (float)g_numSegments;
            float phi2 = ((float)i + 1.f) * 2.f * glm::pi<float>() / (float)g_numSegments;
            float u1 = glm::cos(phi1);
            float v1 = glm::sin(phi1);
            float u2 = glm::cos(phi2);
            float v2 = glm::sin(phi2);

            glm::vec3 x11 = s + u1 * uDir + v1 * vDir;
            glm::vec3 x12 = x11 + dir * lineLength;
            glm::vec3 x21 = s + u2 * uDir + v2 * vDir;
            glm::vec3 x22 = x21 + dir * lineLength;

            glm::vec3 n1 = u1 * uDir + v1 * vDir;
            glm::vec3 n2 = u2 * uDir + v2 * vDir;

            GenQuad(index, pts, indices, vel, x11, x12, x21, x22, n1, n1, n2, n2);
        }
        
        currPos = s + dir * lineLength;
        currVelDir = TrilinearInterpolation(data, currPos + 0.5f);
        currSegment++;
    }
}

void GenerateLinesLayerHorizontal(const vector3d<glm::vec3>& data, Axis axis, float cutPos, std::vector<VertexPosVel> &pts, std::vector<int> &indices, unsigned int numSamples)
{
    float spacing = 1.0f / numSamples;
    glm::vec3 cellSize = glm::vec3(1.0f / data.dim().x);
    glm::vec3 lineLength = glm::vec3(spacing);

    glm::vec3 min = glm::vec3(0.f);
    glm::vec3 max = glm::vec3(1.0f);
    float level = cutPos * max[static_cast<int>(axis)];
    min[static_cast<int>(axis)] = level;
    max[static_cast<int>(axis)] = level;

    unsigned int i = 0;
    // uniform generation

    for (float y = min.y ; y <= max.y; y += spacing)
    {
        for (float z = min.z; z <= max.z; z += spacing)
        {
            for (float x = min.x; x <= max.x; x += spacing)
            {
                GenStreamline(data, glm::vec3(x, y, z), i, pts, indices, cellSize, lineLength);
            }
        }
    }
}