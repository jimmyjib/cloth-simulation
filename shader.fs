#version 330 core

//5 variables outputed from vs;
in vec3 vertexPos_worldspace;
in vec3 vertexNormal_cameraspace;
in vec3 CameraPosRelVertex_cameraspace;
in vec3 LightPosRelVertex_cameraspace;
in vec3 fragmentColor;

out vec3 color;

uniform mat4 MV;
uniform vec3 LightPos_worldspace;

void main(){
	
	//initializing

	//sets light color to white
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 50.0f;

	//Material properties - ambient, diffuse, specular 
	vec3 MaterialDiffuseColor = fragmentColor;
	vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.1,0.1,0.1);

	//distance to light
	float distance = length(LightPos_worldspace-vertexPos_worldspace);

	//////////////////////////////////////////////////////////////////////////////////
	//variables below are for calculating two cos values
	//cosTheta & cosAlpha

	//normal vector -> used for reflection 
	vec3 n = normalize(vertexNormal_cameraspace);

	//change both front and back face to front face
	//if cloth doens't have thickness 
	//making both sides front faced is more natural
	if(!gl_FrontFacing){
		n = -n;
	}

	//light position relative to vertex 
	vec3 l = normalize(LightPosRelVertex_cameraspace);

	//clamp -> restirct cosine to 0~1
	//if light is behind face 0, vertical 1
	float cosTheta = clamp(dot(n,l),0,1);

	//Camera/Eye position relative to vertex
	vec3 E = normalize(CameraPosRelVertex_cameraspace);

	//reflected vector 
	vec3 R = reflect(-l,n);

	float cosAlpha = clamp(dot(E,R),0,1);
	//////////////////////////////////////////////////////////////////////////////////
	//final calculation of color

	color = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
}