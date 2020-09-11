#include "ilpch.h"
#include "OpenGlShader.h"

#include <fstream>

#include <glad/glad.h>
#include "glm/gtc/type_ptr.hpp"

namespace IL
{

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;
		if (type == "geometry")
			return GL_GEOMETRY_SHADER;
		if (type == "compute")
			return GL_COMPUTE_SHADER;

		IL_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}


	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		:m_Program(0), m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sourceMap;
		sourceMap[GL_VERTEX_SHADER] = vertexSrc;
		sourceMap[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sourceMap);
	}

	OpenGLShader::OpenGLShader(const std::string& path)
	{
		std::string shaderSource = ReadFile(path);
		auto sourceMap = PreProcess(shaderSource);
		Compile(sourceMap);

		// Extract name from the filepath
		size_t lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		size_t lastDot = path.rfind('.');
		size_t count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
		m_Name = path.substr(lastSlash, count);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_Program);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_Program);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, const int& value)
	{
		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		UploadUniformMat4(name, value);
	}

	void OpenGLShader::SetFloat(const std::string& name, const float& value)
	{
		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_Program, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, const int& values)
	{
		GLint location = glGetUniformLocation(m_Program, name.c_str());
		glUniform1i(location, values);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, const float& values)
	{
		GLint location = glGetUniformLocation(m_Program, name.c_str());
		glUniform1f(location, values);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint location = glGetUniformLocation(m_Program, name.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(m_Program, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		GLint location = glGetUniformLocation(m_Program, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::ifstream in(filepath, std::ios::in, std::ios::binary);
		std::string data;

		if (in.good())
		{
			in.seekg(0, std::ios::end);
			data.resize(in.tellg());  // set the length of the string to the length of the shader
			in.seekg(0, std::ios::beg);
			in.read(&data[0], data.size());
			in.close();
		}
		else
		{
			IL_CORE_ERROR("Open shader file failure! (shader: {0})", filepath);
		}

		return data;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> sourceMap;

		const char* typeToken = "#type";
		size_t typeTokenLen = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		// find type until the end
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			IL_CORE_ASSERT(eol != std::string::npos, "Syntax error!"); // make sure we find the right place

			size_t begin = pos + typeTokenLen + 1;
			std::string type = source.substr(begin, eol - begin);
			IL_CORE_ASSERT(ShaderTypeFromString(type), "Unknown shader type!");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			sourceMap[ShaderTypeFromString(type)] = source.substr(nextLinePos, 
				pos - (nextLinePos == std::string::npos ? source.size() : nextLinePos));
		}

		return sourceMap;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& sourceMap)
	{
		GLuint program = glCreateProgram();
		IL_CORE_ASSERT(sourceMap.size() <= 2, "Exceed maximum size of shaders!");
		std::array<GLenum, 2> glShaderIDs;
		uint32_t shaderIDIndex = 0;

		for (const auto& shaderCource : sourceMap)
		{
			GLenum type = shaderCource.first;
			const std::string& source = shaderCource.second;

			GLuint shader = glCreateShader(type);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceCStr = (const GLchar*)source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				IL_CORE_ERROR("{0}", infoLog.data());
				IL_CORE_ASSERT(false, "Shader compilation failure!");
				return;

			}
			glAttachShader(program, shader);
			glShaderIDs[shaderIDIndex++] = shader;
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			for (auto shader : glShaderIDs)
			{
				glDeleteShader(shader);

			}

			IL_CORE_ERROR("{0}", infoLog.data());
			IL_CORE_ASSERT(false, "program linking failure!");
			return;
		}

		// Always detach shaders after a successful link.
		for (auto shader : glShaderIDs)
		{
			glDetachShader(program, shader);
		}

		m_Program = program;
	}

}