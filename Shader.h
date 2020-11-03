#pragma once
class Shader{
	//loads shader: program to integrate camera, vertex, color etc.
	//vs is for calculating position, fs is for color
public:
	Shader() {};
	~Shader() {};

	//reads the fs, vs code and compile it
	void LoadShaders(const char* vs_path, const char* fs_path);
	unsigned int id();
private:
	unsigned int programID;
};

