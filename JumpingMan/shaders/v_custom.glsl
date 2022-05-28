#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 viewPos;
uniform vec4 lightPos;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 normal; //wektor normalny w przestrzeni modelu
in vec2 texCoord;

in vec4 tangent;
in vec4 bitangent;

//Zmienne interpolowane
//out vec4 ic;
out vec4 lightDir;
out vec4 normalDir;
out vec4 viewDir;
out vec2 iTexCoord;



void main(void) {
    vec4 normal_camera = normalize(V * M * normal);
    vec4 tangent_cameraspace = normalize(V * M * tangent);
    vec4 bitangent_cameraspace = normalize(V * M* bitangent);
	mat4 invTBM = transpose(mat4(normal, tangent, bitangent, vec4(0,0,0,1)));

    //lightDir = normalize(invTBM * (inverse(M)* lightPos - vertex)); 
    //viewDir = normalize( invTBM * (inverse(V*M)* viewPos - vertex)); 

    lightDir = normalize(V * lightPos - V*M*vertex); //wektor do światła w przestrzeni oka
    viewDir = normalize(viewPos - V * M * vertex); //wektor do obserwatora w przestrzeni oka
    normalDir = normalize(V * M * normal); //wektor normalny w przestrzeni oka

    iTexCoord = texCoord;
    gl_Position=P*V*M*vertex;
}
