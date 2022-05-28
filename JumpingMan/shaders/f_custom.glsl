#version 330


uniform sampler2D textureMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;

out vec4 pixelColor; 

in vec4 lightDir;
in vec4 normalDir;
in vec4 viewDir;
in vec2 iTexCoord;

void main(void) {

	//Znormalizowane interpolowane wektory
	vec4 normLightDir = normalize(lightDir);
	vec4 normViewDir = normalize(viewDir);
	vec2 texCoord = iTexCoord;

	vec4 normNormalDir = normalize(normalDir);
	//vec4 normNormalDir = normalize(vec4(texture(normalMap, iTexCoord).rgb*2-1, 0));
	
	//Wektor odbity
	vec4 mr = reflect(-normLightDir, normNormalDir);

	//Parametry powierzchni
	vec4 kd = texture(textureMap, texCoord); 
	vec4 ks = vec4(1, 1, 1, 1);

	//Obliczenie modelu oświetlenia
	float nl = clamp(dot(normNormalDir, normLightDir), 0, 1);
	float rv = pow(clamp(dot(mr, normViewDir), 0, 1),25);

	pixelColor= vec4(kd.rgb * nl, kd.a) + vec4(ks.rgb*rv, 0);


	//pixelColor=texture(textureMap,iTexCoord);
}
