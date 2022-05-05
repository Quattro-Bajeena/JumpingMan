#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;


uniform vec4 color=vec4(1,0,1,1);
uniform vec4 lightDir=vec4(0,0,1,0);

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec2 texCoord;

out vec4 iC;


void main(void) {
	vec4 a = vertex;
	gl_Position=P*V*M*a;

	float d = distance(V * M * vertex, vec4(0, 0, 0, 1));
	float d_norm = 1 - ((d - 3.3) /1.7);
	iC = vec4(color.rgb * d_norm, color.a);

	


}
