#include <graphics/shader.h>

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
{
    std::vector<GLuint> shaderList;
    shaderList.push_back(createShaderFromFile(GL_VERTEX_SHADER, vertexShader));
    shaderList.push_back(createShaderFromFile(GL_FRAGMENT_SHADER, fragmentShader));

    _programID = createProgram(shaderList);

    shaderList.clear();
}

Shader::~Shader()
{
    GL(UseProgram(0));
    if (_programID != 0) {
        GL(DeleteProgram(_programID));
    }
}

std::unique_ptr<Shader> Shader::CreateFromSource(const std::string& vertexShaderText, const std::string& fragmentShaderText)
{
    std::unique_ptr<Shader> shader = std::unique_ptr<Shader>(new Shader());
    std::vector<GLuint> shaderList;
    shaderList.push_back(shader->createShaderFromSource(GL_VERTEX_SHADER, vertexShaderText));
    shaderList.push_back(shader->createShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderText));

    shader->_programID = shader->createProgram(shaderList);

    shaderList.clear();

    return std::move(shader);
}

void Shader::Bind()
{
    GL(UseProgram(_programID));
}

void Shader::Unbind()
{
    GL(UseProgram(0));
}

GLuint Shader::createShaderFromSource(GLenum shaderType, const std::string& strShaderText) 
{
    GLuint shader = glCreateShader(shaderType);
    const char* strFileData = strShaderText.c_str();
    GL(ShaderSource(shader, 1, &strFileData, NULL));

    GL(CompileShader(shader));

    GLint status;
    GL(GetShaderiv(shader, GL_COMPILE_STATUS, &status));
    if (status == GL_FALSE) {
        GLint infoLogLength;
        GL(GetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength));

        GLchar* strInfoLog = new GLchar[infoLogLength + 1];
        GL(GetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog));

        const char* strShaderType = NULL;
        switch (shaderType) {
        case GL_VERTEX_SHADER: strShaderType = "vertex";   break;
        case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
        case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
        }

        std::cout << "Compile failure in " << strShaderType << " shader:";
        std::cout << strInfoLog;

        delete[] strInfoLog;
        GL(DeleteShader(shader));
        return 0;
    }

    return shader;
}

GLuint Shader::createShaderFromFile(GLenum shaderType, const std::string& filename) 
{
    FILE* f = fopen(filename.c_str(), "rb");
    if (!f) {
        std::cout << "Unable to open file " << filename << " for reading." << std::endl;
        return 0;
    }
    else
        std::cout << "Loading shader: " << filename << std::endl;

    fseek(f, 0, SEEK_END);
    int length = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* buffer = new char[length + 1];
    int length2 = fread(buffer, 1, length, f);
    fclose(f);
    buffer[length] = '\0';

    GLuint sh = createShaderFromSource(shaderType, buffer);
    delete[] buffer;
    return sh;
}

bool Shader::linkProgram(GLuint program) 
{
    GL(LinkProgram(program));

    GLint status = GL_FALSE;
    GL(GetProgramiv(program, GL_LINK_STATUS, &status));

    if (status == GL_FALSE) {
        GLint infoLogLength;
        GL(GetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength));

        GLchar* strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
        return false;
    }
    return true;
}

GLuint Shader::createProgram(const std::vector<GLuint>& shaderList) 
{
    GLuint program = GL(CreateProgram());

    for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
    {
        GL(AttachShader(program, shaderList[iLoop]));
    }

    if (!linkProgram(program)) {
        deleteProgramAndShaders(program);
        return 0;
    }

    return program;
}

void Shader::deleteProgramAndShaders(GLuint program) 
{
    const GLsizei MAX_COUNT = 10;  // hopefully not more shaders
    GLsizei count = 0;
    GLuint shaders[MAX_COUNT];

    if (program != 0) {
        GL(GetAttachedShaders(program, MAX_COUNT, &count, shaders));
        for (int i = 0; i < count; i++)
            GL(DeleteShader(shaders[i]));

        GL(DeleteProgram(program));
    }
}


void Shader::SetUniformInt(const std::string& name, int val)
{
    GL(UseProgram(_programID));
    GL(Uniform1i(GetUniformLocation(name), val));
    Unbind();
}

void Shader::SetUniformFloat(const std::string& name, float val)
{
    GL(UseProgram(_programID));
    GL(Uniform1f(GetUniformLocation(name), val));
    Unbind();
}

void Shader::SetUniformFloat2(const std::string& name, float val1, float val2)
{
    GL(UseProgram(_programID));
    GL(Uniform2f(GetUniformLocation(name), val1, val2));
    Unbind();
}

void Shader::SetUniformFloat2(const std::string& name, const glm::vec2& val)
{
    SetUniformFloat2(name, val.x, val.y);
}

void Shader::SetUniformFloat2v(const std::string& name, const glm::vec2& val)
{
    GL(UseProgram(_programID));
    GL(Uniform2fv(GetUniformLocation(name), 1, glm::value_ptr(val)));
    Unbind();
}

void Shader::SetUniformFloat3(const std::string& name, float val1, float val2, float val3)
{
    GL(UseProgram(_programID));
    GL(Uniform3f(GetUniformLocation(name), val1, val2, val3));
    Unbind();
}

void Shader::SetUniformFloat3(const std::string& name, const glm::vec3& val)
{
    SetUniformFloat3(name, val.x, val.y, val.z);
}

void Shader::SetUniformFloat3v(const std::string& name, const glm::vec3& val)
{
    GL(UseProgram(_programID));
    GL(Uniform3fv(GetUniformLocation(name), 1, glm::value_ptr(val)));
    Unbind();
}

void Shader::SetUniformFloat4(const std::string& name, float val1, float val2, float val3, float val4)
{
    GL(UseProgram(_programID));
    GL(Uniform4f(GetUniformLocation(name), val1, val2, val3, val4));
    Unbind();
}

void Shader::SetUniformFloat4(const std::string& name, const glm::vec4& val)
{
    SetUniformFloat4(name, val.x, val.y, val.z, val.w);
}

void Shader::SetUniformFloat4v(const std::string& name, const glm::vec4& val)
{
    GL(UseProgram(_programID));
    GL(Uniform4fv(GetUniformLocation(name), 1, glm::value_ptr(val)));
    Unbind();
}

void Shader::SetUniformMat3(const std::string& name, const glm::mat3& mat)
{
    GL(UseProgram(_programID));
    GL(UniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat)));
    Unbind();
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& mat)
{
    GL(UseProgram(_programID));
    GL(UniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat)));
    Unbind();
}

int Shader::GetUniformLocation(const std::string& name)
{
    auto it = _uniformLocations.find(name);
    if (it == _uniformLocations.end())
    {
        _uniformLocations[name] = GL(GetUniformLocation(_programID, name.c_str()));
    }

    return _uniformLocations[name];
}

int Shader::GetAttribLocation(const std::string& name)
{
    auto it = _attribLocations.find(name);
    if (it == _attribLocations.end())
    {
        _attribLocations[name] = GL(GetAttribLocation(_programID, name.c_str()));
    }

    return _attribLocations[name];
}