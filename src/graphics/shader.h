#pragma once
#include <core/common.h>
#include <unordered_map>

class Shader
{
private:
    GLuint _programID;

	GLuint createShaderFromFile(GLenum shaderType, const std::string& filename);
	GLuint createShaderFromSource(GLenum shaderType, const std::string& strShaderText);
    bool linkProgram(GLuint program);
    GLuint createProgram(const std::vector<GLuint>& shaderList);
    void deleteProgramAndShaders(GLuint program);

	std::unordered_map<std::string, GLint> _uniformLocations;
	std::unordered_map<std::string, GLint> _attribLocations;
private:
	Shader() : _programID(0) {}
public:
    Shader(const std::string& vertexShader, const std::string& fragmentShader);
    ~Shader();

	static std::unique_ptr<Shader> CreateFromSource(const std::string& vertexShaderText, const std::string& fragmentShaderText);

    void Bind();
    void Unbind();

    void SetUniformInt(const std::string& name, const int val);
	void SetUniformFloat(const std::string& name, const float val);
	void SetUniformFloat2(const std::string& name, float val1, float val2);
	void SetUniformFloat2(const std::string& name, const glm::vec2& val);
	void SetUniformFloat2v(const std::string& name, const glm::vec2& val);
	void SetUniformFloat3(const std::string& name, float val1, float val2, float val3);
	void SetUniformFloat3(const std::string& name, const glm::vec3& val);
	void SetUniformFloat3v(const std::string& name, const glm::vec3& val);
	void SetUniformFloat4(const std::string& name, float val1, float val2, float val3, float val4);
	void SetUniformFloat4(const std::string& name, const glm::vec4& val);
	void SetUniformFloat4v(const std::string& name, const glm::vec4& val);
	void SetUniformMat3(const std::string& name, const glm::mat3& mat);
	void SetUniformMat4(const std::string& name, const glm::mat4& mat);

	int GetAttribLocation(const std::string& name);
	int GetUniformLocation(const std::string& name);
};
