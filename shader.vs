#version 330 core

layout(location = 0) in vec3 vertexPos_modelspace;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexColor;

out vec3 vertexPos_worldspace;
out vec3 vertexNormal_cameraspace;

//camera position relative to vertex
out vec3 CameraPosRelVertex_cameraspace;

//light position relative to vertex
out vec3 LightPosRelVertex_cameraspace;
out vec3 fragmentColor;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightPos_worldspace;

//basic flow of process
//1. vertex_modelspace modified by ModelView Matrix M, change to worldspace(actual postion in the world) -> M*vertex_modelspace
//2. then the View Matrix is applied (Matrix driven from cameraspace) -> V*M*vertex
//View Matrix changes the vertexes into camera's perspective
//3. lastly project it to the screen using projection matrix P (MVP = P*V*M) -> MVP*vertex = P*V*M*vertex

void main(){
	//gl_Position: built-in variable, position on screen
	//convet to vec4, last entry 1
	gl_Position = MVP * vec4(vertexPos_modelspace,1);

	Pos_worldspace = (M * vec4(vertexPos_modelspace,1)).xyz;
	
	vec3 vertexPos_cameraspace = (V * M * vec4(vertexPos_modelspace,1)).xyz;
	CameraPosRelVertex_cameraspace = vec3(0,0,0) - vertexPos_cameraspace;

	vec3 LightPos_cameraspace = (V * vec4(LightPos_worldspace,1)).xyz;
	LightPosRelVertex_cameraspace = LightPos_cameraspace + CameraPosRelVertex_cameraspace;

	//Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	//vector's starting point is fixed to the origin
	//last entry -> 0 makes the translation component of matrix to not affect the normal vector
	Normal_cameraspace = ( V * M * vec4(vertexNormal,0)).xyz;

	fragmentColor = vertexColor;
}