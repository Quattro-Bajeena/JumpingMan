#version 330


uniform mat4 P;
uniform mat4 V;
uniform mat4 M;


in vec4 vertex; 
in vec2 texCoord; 


out vec2 i_tc;

void main(void) {
    gl_Position=P*V*M*vertex;
    i_tc=texCoord;
}
