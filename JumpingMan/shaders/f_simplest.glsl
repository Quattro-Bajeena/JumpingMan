#version 330

uniform sampler2D textureMap0; //globalnie
uniform sampler2D textureMap1; //globalnie

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 ic; 
in vec4 n;
in vec4 l;
in vec4 v;
in vec2 iTexCoord0; //globalnie
in vec2 iTexCoord1;

void main(void) {

	//Znormalizowane interpolowane wektory
	vec4 ml = normalize(l);
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);
	//Wektor odbity
	vec4 mr = reflect(-ml, mn);

	//Parametry powierzchni
	vec4 metal = texture(textureMap0, iTexCoord0);//main;
	vec4 sky = texture(textureMap1, iTexCoord1);//main;
	vec4 kd = mix(metal, sky, 0.3);
	vec4 ks = vec4(1, 1, 1, 1);

	//Obliczenie modelu oświetlenia
	float nl = clamp(dot(mn, ml), 0, 1);
	float rv = pow(clamp(dot(mr, mv), 0, 1), 50);
	pixelColor= vec4(kd.rgb * nl, kd.a) + vec4(ks.rgb*rv, 0);
}
