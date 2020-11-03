#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>

void Shader::LoadShaders(const char* vs_path, const char* fs_path) {
	//create 2 shader IDs
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//reading vertex shader file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vs_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		//read buffer -> change to string -> close stream
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Can't read file %s. Is the path valid? \n", vs_path);
		return;
	}

	//reading fragment shader file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fs_path, std::ios::in);
	if (FragmentShaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}
	else{
		printf("Can't read file %s. Is the path valid? \n", fs_path);
		return;
	}

	/////////////////////////////////////////////////////////////////////////////////
	//compile codes
	//checks result of each status
	//GLint Result = GL_FALSE;
	int ErrorLogLength;

	printf("Compiling shader : %s\n", vs_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);

	//compile vs
	glCompileShader(VertexShaderID);

	//glGetShader iv returns variable with info to reference
	//glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &ErrorLogLength);

	//if error log exist print it
	if (ErrorLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(ErrorLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, ErrorLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	printf("Compiling shader : %s\n", fs_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);

	//compile fs
	glCompileShader(FragmentShaderID);

	//glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &ErrorLogLength);

	//if error log exist print it
	if (ErrorLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(ErrorLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, ErrorLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	printf("Linking program\n");
	//create program ID
	programID = glCreateProgram();
	glAttachShader(programID, VertexShaderID);
	glAttachShader(programID, FragmentShaderID);

	//link vs and fs to program
	glLinkProgram(programID);

	//glGetProgramiv(programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &ErrorLogLength);

	//if error log exist print it
	if (ErrorLogLength > 0) {
		std::vector<char> ProgramErrorMessage(ErrorLogLength + 1);
		glGetProgramInfoLog(programID, ErrorLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	/////////////////////////////////////////////////////////////////////////////////
	//finished making program, detatch vs, fs and delete
	glDetachShader(programID, VertexShaderID);
	glDetachShader(programID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
}

unsigned int Shader::id() {
	return programID;
}