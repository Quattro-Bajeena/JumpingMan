#version 330

//Zmienne jednorodne
uniform mat4 P; //projection
uniform mat4 V; //view
uniform mat4 M; //model
uniform vec3 viewPos;
uniform vec3 lightPos1;
uniform vec3 lightPos2;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec3 normal; //wektor normalny w przestrzeni modelu
in vec2 texCoord;

in vec3 tangent;
in vec3 bitangent;


out vec2 TexCoord;
out vec3 FragPosTS; //TS = tangent space
out vec3 ViewPosTS;
out vec3 LightPosTS1;
out vec3 LightPosTS2;



void main(void){
    vec3 FragPos = vec3(M * vertex);
    TexCoord = texCoord;

    mat3 modelVector = transpose(inverse(mat3(M)));
    vec3 T = normalize(modelVector * tangent);
    vec3 B = normalize(modelVector * bitangent);
    vec3 N = normalize(modelVector * normal);
    mat3 TBN = transpose(mat3(T, B, N));

    FragPosTS = TBN * FragPos;
    LightPosTS1 = TBN * lightPos1;
    LightPosTS2 = TBN * lightPos2;
    ViewPosTS = TBN * viewPos;

    gl_Position = P*V*M*vertex;


}
