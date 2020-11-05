#include "Object.h"
#include <vector>

Object::Object() {
	translate = glm::vec3(0.0, 0.0, 0.0);
	scale = glm::vec3(1.0, 1.0, 1.0);
	//orientation = glm::vec4(1.0, 1.0, 1.0, 0.0);

	//initialize all ids/ptrs to 0/nullptr
	vertex_array_id = 0;
	vertex_buffer_id = 0;
	normal_buffer_id = 0;
	color_buffer_id = 0;
	index_buffer_id = 0;

	g_vertex_buffer_data = nullptr;
	g_normal_buffer_data = nullptr;
	g_color_buffer_data = nullptr;
	g_index_buffer_data = nullptr;

	num_triangle = 0;
	num_vertex = 0;
}

Object::~Object() {
	//delete all buffer objects and vertex arrays generated
	if (!vertex_buffer_id) glDeleteBuffers(1, &vertex_buffer_id);
	if (!normal_buffer_id) glDeleteBuffers(1, &normal_buffer_id);
	if (!color_buffer_id) glDeleteBuffers(1, &color_buffer_id);
	if (!index_buffer_id) glDeleteBuffers(1, &index_buffer_id);
	if (!vertex_array_id) glDeleteVertexArrays(1, &vertex_array_id);
}

void Object::genVAO(GLfloat* v, GLfloat* n, GLfloat* c, GLint* ind, size_t n_t, size_t n_v) {
	//generate VAO/VBO (VAO<-VBO)

	num_triangle = n_t;
	num_vertex = n_v;
	g_vertex_buffer_data = v;
	g_normal_buffer_data = n;
	g_color_buffer_data = c;
	g_index_buffer_data = ind;

	glGenVertexArrays(1, &vertex_array_id);
	//activate VAO
	glBindVertexArray(vertex_array_id);

	//Create VBO for GPU
	//each vertex has 3 coordinates 
	glGenBuffers(1, &vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * num_vertex, g_vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &normal_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * num_vertex, g_normal_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &color_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * num_vertex, g_color_buffer_data, GL_STATIC_DRAW);

	//triangles containing index of vertices?
	glGenBuffers(1, &index_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * num_triangle * 3, g_index_buffer_data, GL_STATIC_DRAW);
}

void Object::updateVBO() {
	//update vertex poisition&normal

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	//glMapBuffer returns address of activated buffer of given type
	float* ptr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (ptr) {
		//overwrite memory
		memcpy(ptr, g_vertex_buffer_data, sizeof(float) * 3 * num_vertex);
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id);
	ptr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (ptr) {
		memcpy(ptr, g_normal_buffer_data, sizeof(float) * 3 * num_vertex);
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	//deactivate buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Object::draw() {
	//Shader input layout
	//layout(location = 0) in vec3 vertexPos_modelspace;
	//layout(location = 1) in vec3 vertexNormal;
	//layout(location = 2) in vec3 vertexColor;

	//vertex position
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//vertex normal
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);
	//vertex color
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer_id);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//indexes of vertices in triangle

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
	glDrawElements(
		GL_TRIANGLES,				//mode
		(GLsizei)num_triangle * 3,  //count - 3 vertex per triangle
		GL_UNSIGNED_INT,			//element array type
		(void*)0					//element array buffer offset
	);

	//deactivate buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//deactivate atrributes
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

bool Object::loadAssImp(const char* path) {
	//read data from file and convert to float arrays
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_GenNormals | aiProcess_Triangulate);
	if (!scene) {
		//error while reading file
		fprintf(stderr, importer.GetErrorString());
		return false;
	}

	//for OBJ containing only one mesh (usually there's only one)
	const aiMesh* mesh = scene->mMeshes[0];

	//v, n, c, ind used for genVAO()
	GLfloat* v = (GLfloat*)malloc(sizeof(GLfloat) * 3 * mesh->mNumVertices);
	GLfloat* n = (GLfloat*)malloc(sizeof(GLfloat) * 3 * mesh->mNumVertices);
	GLfloat* c = nullptr;
	GLint* ind = (GLint*)malloc(sizeof(GLint) * 3 * mesh->mNumFaces);

	//vertex position
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D pos = mesh->mVertices[i];
		v[i * 3] = pos.x;
		v[i * 3 + 1] = pos.y;
		v[i * 3 + 2] = pos.z;
	}

	//texture coordinates
	if (mesh->mTextureCoords[0]){
		//Fill vertices texture coordinates
		GLfloat* t = (GLfloat*)malloc(sizeof(GLfloat) * 2 * mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			// Assume only 1 set of UV coords; AssImp supports 8 UV sets.
			aiVector3D UVW = mesh->mTextureCoords[0][i]; 
			t[i * 2] = UVW.x;
			t[i * 2 + 1] = UVW.y;
		}
	}

	//vertex normals
	if (mesh->mNormals){
		//Fill vertices normals
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			aiVector3D normal = mesh->mNormals[i];
			n[i * 3] = normal.x;
			n[i * 3 + 1] = normal.y;
			n[i * 3 + 2] = normal.z;
		}
	}

	//vertex colors
	if (mesh->mColors[0]){
		//if color exists
		c = (GLfloat*)malloc(sizeof(GLfloat) * 4 * mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			aiColor4D color = mesh->mColors[0][i];
			c[i * 3] = color.r;
			c[i * 3 + 1] = color.g;
			c[i * 3 + 2] = color.b;
			c[i * 3 + 3] = color.a;
		}
	}
	else{
		glm::vec3 defaultColor(0.7, 0.7, 0.0);
		//default color
		c = (GLfloat*)malloc(sizeof(GLfloat) * 3 * mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			c[i * 3] = defaultColor.x;
			c[i * 3 + 1] = defaultColor.y;
			c[i * 3 + 2] = defaultColor.z;
		}
	}

	//face indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		// Assume the model has only triangles.
		ind[i * 3] = mesh->mFaces[i].mIndices[0];
		ind[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
		ind[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
	}

	num_triangle = mesh->mNumFaces;
	num_vertex = mesh->mNumVertices;

	genVAO(v, n, c, ind, num_triangle, num_vertex);

	return true;
}

void Object::transform() {
	//transform vertex positions
	for (size_t i = 0; i < num_vertex; ++i)
	{
		g_vertex_buffer_data[i * 3] = (scale.x * g_vertex_buffer_data[i * 3]) + translate.x;
		g_vertex_buffer_data[i * 3 + 1] = (scale.y * g_vertex_buffer_data[i * 3 + 1]) + translate.y;
		g_vertex_buffer_data[i * 3 + 2] = (scale.z * g_vertex_buffer_data[i * 3 + 2]) + translate.z;
	}

	updateVBO();
}

void Object::updateNormal()
{
	//vector of bundle(vector) of normal vectors
	//if a vertex is in more than one triangle
	//there will be more than one normal vector corresponding to the vertex
	//if normal vectors are calculated for each triangle
	std::vector<std::vector<glm::vec3>> vNormal(num_vertex);

	for (size_t i = 0; i < num_triangle; ++i){
		//calculate triangle based normal vector
		int v0 = g_index_buffer_data[i * 3];
		int v1 = g_index_buffer_data[i * 3 + 1];
		int v2 = g_index_buffer_data[i * 3 + 2];
		glm::vec3 x0(g_vertex_buffer_data[v0 * 3], g_vertex_buffer_data[v0 * 3 + 1], g_vertex_buffer_data[v0 * 3 + 2]);
		glm::vec3 x1(g_vertex_buffer_data[v1 * 3], g_vertex_buffer_data[v1 * 3 + 1], g_vertex_buffer_data[v1 * 3 + 2]);
		glm::vec3 x2(g_vertex_buffer_data[v2 * 3], g_vertex_buffer_data[v2 * 3 + 1], g_vertex_buffer_data[v2 * 3 + 2]);

		glm::vec3 t1 = x1 - x0;
		glm::vec3 t2 = x2 - x0;
		glm::vec3 f_norm = normalize(cross(t1, t2));

		vNormal[v0].push_back(f_norm);
		vNormal[v1].push_back(f_norm);
		vNormal[v2].push_back(f_norm);
	}

	for (size_t i = 0; i < num_vertex; ++i)
	{
		//add all normal vector in the normal vector bundle for each vertex
		glm::vec3 norm(0.0, 0.0, 0.0);
		for (auto& itr : vNormal[i])
		{
			norm += itr;
		}
		//normalize it
		normalize(norm);

		//update normal vector data buffer
		g_normal_buffer_data[i * 3] = norm.x;
		g_normal_buffer_data[i * 3 + 1] = norm.y;
		g_normal_buffer_data[i * 3 + 2] = norm.z;
	}

	updateVBO();
}