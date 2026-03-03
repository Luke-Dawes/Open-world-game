#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {

public:

	unsigned int ID; //the id which is used to call the shader program

	Shader(const char* vertexPath, const char* fragmentPath);

	void use() const; //uses the ID

	unsigned int compileShader(unsigned int type, const char* source);

	//needed for uniforms
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4& value) const;
	void setMat4(const std::string& name, const glm::mat4& value) const;

};