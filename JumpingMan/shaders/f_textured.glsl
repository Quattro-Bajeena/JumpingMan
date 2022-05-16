#version 330


uniform sampler2D textureMap;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

//Zmienne interpolowane
in vec2 i_tc;

void main(void) {
	pixelColor=texture(textureMap,i_tc);
}
