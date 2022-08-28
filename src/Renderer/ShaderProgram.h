#pragma once

#include <glad/glad.h>
#include <string>

namespace Renderer {
	class ShaderProgram {
	public:
		ShaderProgram() = delete;
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		~ShaderProgram();
		bool isCompiled() const {return m_isCompiled;}
		void use() const;

		
		ShaderProgram& operator=(ShaderProgram&& shaderProgram)noexcept;
		ShaderProgram(ShaderProgram&& shaderProgram)noexcept;
	private:
		bool m_isCompiled = false;
		GLuint m_ID=0;

		bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);
	};
}