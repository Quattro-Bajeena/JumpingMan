#version 330


uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;

out vec4 FragColor; 

in vec2 TexCoord;
in vec3 LightPosTS;
in vec3 FragPosTS;
in vec3 ViewPosTS;


void main(void){
	vec3 normal = vec3(0,0,1.0);
	normal = texture(normalMap, TexCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	
	vec3 color = texture(diffuseMap, TexCoord).rgb;
	vec3 ambient = 0.1 * color;

	vec3 lightDir = normalize(LightPosTS - FragPosTS);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * color;

	vec3 viewDir = normalize(ViewPosTS - FragPosTS);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 25.0);
	vec3 specular = vec3(1.0) * spec;

	FragColor = vec4(ambient + diffuse + specular, 1.0);
}
