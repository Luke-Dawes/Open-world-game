#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {

	//reading from the file
	std::string vertexStringCode, fragmentStringCode; //creates strings which will hold whats in the files

	std::ifstream vertFile(vertexPath), fragFile(fragmentPath); //opens the files with the names vertFile and fragFile

	std::stringstream vertStream, fragStream; //makes it so i can get all the data in one string

	if (!vertFile.is_open() || !fragFile.is_open()) { //checks if the files can open
		throw std::runtime_error("couldnt open shader files");
	}

	vertStream << vertFile.rdbuf();
	fragStream << fragFile.rdbuf(); //(gets all the data as a buffer object)

	vertFile.close(); //closes the file
	fragFile.close();

	vertexStringCode = vertStream.str(); //makes the buffer object a string 
	fragmentStringCode = fragStream.str();

	const char* vertSourceCode = vertexStringCode.c_str(); //makes the string a c style string 
	const char* fragSourceCode = fragmentStringCode.c_str();

	//compile shaders
	unsigned int vertID = compileShader(GL_VERTEX_SHADER, vertSourceCode);
	unsigned int fragID = compileShader(GL_FRAGMENT_SHADER, fragSourceCode);

	//linking the shaders
	ID = glCreateProgram(); //creates a shader program

	glAttachShader(ID, vertID); //attaches the vert and frag shaders into the shader program
	glAttachShader(ID, fragID);

	glLinkProgram(ID); //links the ID together and finishes building the shader.

	glDeleteShader(vertID); //deleting the shaders now that they are linked
	glDeleteShader(fragID);

}

void Shader::use() const {
	glUseProgram(ID);
}

unsigned int Shader::compileShader(unsigned int type, const char* source) {

	unsigned int shader = glCreateShader(type); //creates an unique ID for this shader (which is different from the shader program ID)

	glShaderSource(shader, 1, &source, NULL); //this takes the unique ID for this shader, the amount of strings i want to pass in? (i can do multiple?)
	//the actual string (source) and the format (e.g. what seperates each string)

	glCompileShader(shader); //compiles the shader 

	return shader; //returns the unique ID for this shader which then needs to be linked into the shader program
}

void Shader::setMat4(const std::string& name, glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setBool(const std::string& name, bool value) const 
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), int(value));
}

void Shader::setFloat(const std::string& name, float value) const 
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}