#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include "glm/gtx/string_cast.hpp"

namespace GameEngine
{

	/**
	 * @brief The shaderProgram is used to load, compile and store shaders, in case of an Compile Error the error is printed in the terminal
	 * 
	 */
	class ShaderProgram
	{
	public:
		int getShaderProgram();
		ShaderProgram(const char *vs, const char *fs);
		ShaderProgram() {}
		int createShader(const char *shaderCode, int shaderType);
		void useShader();

		// uniform setters

		void setBool(const std::string &name, bool value) const
		{
			glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int) value);
		}
		void setInt(const std::string &name, int value) const
		{
			glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
		}
		void setFloat(const std::string &name, float value) const
		{
			glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
		}
		void setVec2(const std::string &name, const glm::vec2 &value) const
		{
			glUniform2fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]);
		}
		void setVec2(const std::string &name, float x, float y) const
		{
			glUniform2f(glGetUniformLocation(shaderProgram, name.c_str()), x, y);
		}
		// ------------------------------------------------------------------------
		void setVec3(const std::string &name, const glm::vec3 &value) const
		{
			glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]);
		}
		void setVec3(const std::string &name, float x, float y, float z) const
		{
			glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z);
		}
		void setVec4(const std::string &name, const glm::vec4 &value) const
		{
			glUniform4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]);
		}
		void setVec4(const std::string &name, float x, float y, float z, float w)
		{
			glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z, w);
		}
		void setMat2(const std::string &name, const glm::mat2 &mat) const
		{
			glUniformMatrix2fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		}
		void setMat3(const std::string &name, const glm::mat3 &mat) const
		{
			glUniformMatrix3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		}
		void setMat4(const std::string &name, const glm::mat4 &mat) const
		{
			glUniformMatrix4fv(
				glGetUniformLocation(shaderProgram, name.c_str()), 
				1, GL_FALSE, &mat[0][0]);
		}

	private:
		void checkCompileErrors(GLuint shader, std::string type)
		{
			GLint success;
			GLchar infoLog[1024];
			if (type != "PROGRAM") {
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				if (!success) {
					glGetShaderInfoLog(shader, 1024, NULL, infoLog);
					std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
							  << infoLog << "\n -- --------------------------------------------------- -- "
							  << std::endl;
				}
			} else {
				glGetProgramiv(shader, GL_LINK_STATUS, &success);
				if (!success) {
					glGetProgramInfoLog(shader, 1024, NULL, infoLog);
					std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
							  << infoLog << "\n -- --------------------------------------------------- -- "
							  << std::endl;
				}
			}
		}

		unsigned int shaderProgram = -1;
	};
} // namespace RenderEngine
