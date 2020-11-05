#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure(scene)
#include <assimp/postprocess.h>     // Post processing flags - aiProcess

class Object{
public:
	Object();
	~Object();

	//loadAssImp() -> genVAO();
	//get data array and make VAO
	bool loadAssImp(const char* path);
	void genVAO(GLfloat* v, GLfloat* n, GLfloat* c, GLint* ind, size_t n_t, size_t n_v);
	void updateVBO();
	void draw();

	//transform(), updateNormal() -> updateVBO()
	//uses translate, scale, orientation
	void setTranslate(glm::vec3 t) { translate = t; }
	void setScale(glm::vec3 s) { scale = s; }
	void transform();
	void updateNormal();

private:
	//used to modify objects directly
	glm::vec3 translate;
	glm::vec3 scale;
	//didn't implement orientation
	//glm::vec4 orientation; 

	//VAO id
	GLuint vertex_array_id;

	//id of buffer objects
	GLuint vertex_buffer_id;
	GLuint normal_buffer_id;
	GLuint color_buffer_id;
	GLuint index_buffer_id;

	//data buffers
	GLfloat* g_vertex_buffer_data;
	GLfloat* g_normal_buffer_data;
	GLfloat* g_color_buffer_data;
	GLint* g_index_buffer_data;
	
	//number of trangles & vertexes
	size_t num_triangle;
	size_t num_vertex;
};

