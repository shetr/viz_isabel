#include <graphics/primitives.h>

const int g_numSegments = 6;
const float g_epsilon = 1e-9;

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

glm::vec3 GeometryGenerator::GenRandomVec(glm::vec3 min, glm::vec3 max)
{
    glm::vec3 r = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
    return (r * (max - min)) + min;
}

void GeometryGenerator::GenTriangle(unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices, float vel, glm::vec3 x1, glm::vec3 x2, glm::vec3 x3, glm::vec3 n1, glm::vec3 n2, glm::vec3 n3)
{
    pts.push_back({x1, vel, n1});
    pts.push_back({x2, vel, n2});
    pts.push_back({x3, vel, n3});
    indices.push_back(index + 0);
    indices.push_back(index + 1);
    indices.push_back(index + 2);
    index += 3;
}

void GeometryGenerator::GenQuad(unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices, float vel, 
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

void GeometryGenerator::GenLineSegment(const vector3d<glm::vec3>& data, glm::vec3 pos, unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices)
{
    glm::vec3 datapos = TrilinearInterpolation(data, pos);

    glm::vec3 s = (pos * _cellSize) - 0.5f; //start position
    glm::vec3 e = s + (glm::normalize(datapos) * _lineLength); // start position + vec direction normalized
    float vel = glm::pow(glm::length(datapos), _speedFactor);
    pts.push_back({s, vel});
    pts.push_back({e, vel});
    indices.push_back(index);
    indices.push_back(index + 1);
    index += 2;
}

glm::vec3 GetPerpendicualrTo(glm::vec3 dir)
{
    double length = sqrt(dir.x * dir.x + dir.y * dir.y);
    glm::vec3 v;
    if (abs(dir.x) > g_epsilon && abs(dir.y) > g_epsilon) {
        v = glm::vec3(dir.y / length, -dir.x / length, 0);
    } else if (abs(dir.y) > g_epsilon) {
        length = sqrt(dir.y * dir.y + dir.z * dir.z);
        v = glm::vec3(0, -dir.z / length, dir.y / length);
    } else {
        length = sqrt(dir.x * dir.x + dir.z * dir.z);
        v = glm::vec3(-dir.z / length, 0, dir.x / length);
    }
    return v;
}

void GeometryGenerator::GenArrow(const vector3d<glm::vec3>& data, glm::vec3 pos, unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices)
{
    float tubeWidth = _cellSize;
    const float bodyRatio = 0.7f;
    const float tipSize = 2.5f;

    glm::vec3 velDir = TrilinearInterpolation(data, pos);

    float vel = glm::pow(glm::length(velDir), _speedFactor);
    float arrowLength = _scaleArrows ? _lineLength * vel : _lineLength;
    glm::vec3 s = pos - 0.5f;
    glm::vec3 dir = glm::normalize(velDir);
    glm::vec3 uDir = GetPerpendicualrTo(dir);
    glm::vec3 vDir = glm::cross(dir, uDir);
    glm::vec3 uAxis = uDir * tubeWidth;
    glm::vec3 vAxis = vDir * tubeWidth;
    for (int i = 0; i < g_numSegments; ++i) {
        float phi1 = (float)i * 2.f * glm::pi<float>() / (float)g_numSegments;
        float phi2 = ((float)i + 1.f) * 2.f * glm::pi<float>() / (float)g_numSegments;
        float u1 = glm::cos(phi1);
        float v1 = glm::sin(phi1);
        float u2 = glm::cos(phi2);
        float v2 = glm::sin(phi2);

        glm::vec3 x11 = s + u1 * uAxis + v1 * vAxis;
        glm::vec3 x12 = x11 + dir * arrowLength * bodyRatio;
        glm::vec3 x21 = s + u2 * uAxis + v2 * vAxis;
        glm::vec3 x22 = x21 + dir * arrowLength * bodyRatio;

        glm::vec3 n1 = u1 * uDir + v1 * vDir;
        glm::vec3 n2 = u2 * uDir + v2 * vDir;

        GenTriangle(index, pts, indices, vel, x11, x21, s, -dir, -dir, -dir);
        GenQuad(index, pts, indices, vel, x11, x12, x21, x22, n1, n1, n2, n2);
    }
    s = s + dir * arrowLength * bodyRatio;
    uAxis *= tipSize;
    vAxis *= tipSize;
    for (int i = 0; i < g_numSegments; ++i) {
        float phi1 = (float)i * 2.f * glm::pi<float>() / (float)g_numSegments;
        float phi2 = ((float)i + 1.f) * 2.f * glm::pi<float>() / (float)g_numSegments;
        float u1 = glm::cos(phi1);
        float v1 = glm::sin(phi1);
        float u2 = glm::cos(phi2);
        float v2 = glm::sin(phi2);

        glm::vec3 x1 = s + u1 * uAxis + v1 * vAxis;
        glm::vec3 x2 = s + u2 * uAxis + v2 * vAxis;
        glm::vec3 x3 = s + dir * arrowLength * (1 - bodyRatio);

        glm::vec3 n1 = u1 * uDir + v1 * vDir;
        glm::vec3 n2 = u2 * uDir + v2 * vDir;
        glm::vec3 n3 = dir;

        GenTriangle(index, pts, indices, vel, x1, x2, s, -n3, -n3, -n3);
        GenTriangle(index, pts, indices, vel, x1, x2, x3, n1, n1, n3);
    }
}

void GeometryGenerator::GenTubeSegment(const vector3d<glm::vec3>& data, glm::vec3 pos, unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices)
{
    float tubeWidth = _cellSize;

    glm::vec3 velDir = TrilinearInterpolation(data, pos);

    float vel = glm::pow(glm::length(velDir), _speedFactor);
    glm::vec3 s = pos - 0.5f;
    glm::vec3 dir = glm::normalize(velDir);
    glm::vec3 uDir = GetPerpendicualrTo(dir);
    glm::vec3 vDir = glm::cross(dir, uDir);
    glm::vec3 uAxis = uDir * tubeWidth;
    glm::vec3 vAxis = vDir * tubeWidth;
    for (int i = 0; i < g_numSegments; ++i) {
        float phi1 = (float)i * 2.f * glm::pi<float>() / (float)g_numSegments;
        float phi2 = ((float)i + 1.f) * 2.f * glm::pi<float>() / (float)g_numSegments;
        float u1 = glm::cos(phi1);
        float v1 = glm::sin(phi1);
        float u2 = glm::cos(phi2);
        float v2 = glm::sin(phi2);

        glm::vec3 x11 = s + u1 * uAxis + v1 * vAxis;
        glm::vec3 x12 = x11 + dir * _lineLength;
        glm::vec3 x21 = s + u2 * uAxis + v2 * vAxis;
        glm::vec3 x22 = x21 + dir * _lineLength;

        glm::vec3 n1 = u1 * uDir + v1 * vDir;
        glm::vec3 n2 = u2 * uDir + v2 * vDir;

        GenQuad(index, pts, indices, vel, x11, x12, x21, x22, n1, n1, n2, n2);
    }
}

void GeometryGenerator::GenStreamline(const vector3d<glm::vec3>& data, glm::vec3 pos, unsigned int& index, std::vector<VertexPosVel> &pts, std::vector<int> &indices)
{
    const float tubeWidth = _cellSize;

    glm::vec3 velDir = TrilinearInterpolation(data, pos);
    
    unsigned int currSegment = 0;
    glm::vec3 currPos = pos - 0.5f;
    glm::vec3 currVelDir = velDir;

    float totalVel = 0;
    
    while(currSegment < _streamlineSegments)
    {

        float vel = glm::pow(glm::length(currVelDir), _speedFactor);
        float segmentLength = vel * _streamlineScale * _lineLength / _streamlineSegments;
        totalVel += vel;
        float velViz = totalVel / _streamlineSegments;
        
        glm::vec3 s = currPos;
        glm::vec3 dir = glm::normalize(currVelDir);
        
        glm::vec3 uDir = GetPerpendicualrTo(dir);
        glm::vec3 vDir = glm::cross(dir, uDir);
        glm::vec3 uAxis = uDir * tubeWidth;
        glm::vec3 vAxis = vDir * tubeWidth;
        for (int i = 0; i < g_numSegments; ++i) {
            float phi1 = (float)i * 2.f * glm::pi<float>() / (float)g_numSegments;
            float phi2 = ((float)i + 1.f) * 2.f * glm::pi<float>() / (float)g_numSegments;
            float u1 = glm::cos(phi1);
            float v1 = glm::sin(phi1);
            float u2 = glm::cos(phi2);
            float v2 = glm::sin(phi2);

            glm::vec3 x11 = s + u1 * uAxis + v1 * vAxis;
            glm::vec3 x12 = x11 + dir * segmentLength;
            glm::vec3 x21 = s + u2 * uAxis + v2 * vAxis;
            glm::vec3 x22 = x21 + dir * segmentLength;

            glm::vec3 n1 = u1 * uDir + v1 * vDir;
            glm::vec3 n2 = u2 * uDir + v2 * vDir;

            GenQuad(index, pts, indices, velViz, x11, x12, x21, x22, n1, n1, n2, n2);
        }
        
        currPos = s + dir * segmentLength;
        if (!data.is_in_range(glm::vec3(data.dim()) * (currPos + 0.5f))) {
            currPos = s;
        }
        currVelDir = TrilinearInterpolation(data, currPos + 0.5f);
        currSegment++;
    }
}

void GeometryGenerator::GenerateGeometry(const vector3d<glm::vec3>& data, Axis axis, float cutPos, std::vector<VertexPosVel> &pts, std::vector<int> &indices, GenGeomFunc genGeomFunc)
{
    srand(_randomSeed);

    unsigned int totalSamples = _axisNumSamples * _axisNumSamples;
    if (axis == Axis::NONE) {
        totalSamples *= _axisNumSamples;
    }


    glm::vec3 min = glm::vec3(0.f);
    glm::vec3 max = glm::vec3(1.0f);
    if (_vizType == VizType::SELECTION) {
        min = glm::max(min, _selectionPos - glm::vec3(_selectionSize * 0.5f));
        max = glm::min(max, _selectionPos + glm::vec3(_selectionSize * 0.5f));
        _spacing = _selectionSize / _axisNumSamples;
    } else {
        _spacing = 1.0f / _axisNumSamples;
    }
    
    _cellSize = 1.0f / data.dim().x;
    _lineLength = _spacing;

    if (axis != Axis::NONE) {
        float level = cutPos * max[static_cast<int>(axis)];
        min[static_cast<int>(axis)] = level;
        max[static_cast<int>(axis)] = level;
    }

    unsigned int i = 0;
    // uniform generation

    if (_useRandomSamples) {
        for (int s = 0; s < totalSamples; ++s) {
            (this->*genGeomFunc)(data, GenRandomVec(min, max), i, pts, indices);
        }
    } else {
        for (float y = min.y ; y <= max.y; y += _spacing)
        {
            for (float z = min.z; z <= max.z; z += _spacing)
            {
                for (float x = min.x; x <= max.x; x += _spacing)
                {
                    (this->*genGeomFunc)(data, glm::vec3(x, y, z), i, pts, indices);
                }
            }
        }
    }
}

void GeometryGenerator::HgtGeometry(const vector3d<float>& data, std::vector<VertexPosVel> &pts, std::vector<int> &indices)
{
    unsigned int index = 0;
    glm::vec3 n = glm::vec3(0, 1, 0);
    
    float maxHeight = 19835;
    glm::vec3 scale = data.dim() - glm::uvec3(1);
    scale.y = maxHeight;
    
    for (unsigned int x = 0; x < data.dim().x - 1; ++x) {
        for (unsigned int z = 0; z < data.dim().z - 1; ++z) {
            glm::vec3 v11 = glm::vec3((float)x + 0, data(x + 0, 0, z + 0), (float)z + 0) / scale - glm::vec3(0.5f);
            glm::vec3 v12 = glm::vec3((float)x + 0, data(x + 0, 0, z + 1), (float)z + 1) / scale - glm::vec3(0.5f);
            glm::vec3 v21 = glm::vec3((float)x + 1, data(x + 1, 0, z + 0), (float)z + 0) / scale - glm::vec3(0.5f);
            glm::vec3 v22 = glm::vec3((float)x + 1, data(x + 1, 0, z + 1), (float)z + 1) / scale - glm::vec3(0.5f);
            n = glm::normalize(glm::cross(v12 - v11, v21 - v11));
            GenQuad(index, pts, indices, 0, v11, v12, v21, v22, n, n, n, n);
        }
    }
}