#version 330


uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D heightMap;

out vec4 FragColor; 

in vec2 TexCoord;
in vec3 LightPosTS1;
in vec3 LightPosTS2;
in vec3 FragPosTS;
in vec3 ViewPosTS;

vec2 parallaxTexCoords(vec3 v, vec2 t, float h, float s ) {
	vec2 ti = -v.xy / s;
	float hi = -v.z / s;

	vec2 tc = t;
	float hc = h;

	float ht = h * texture(heightMap, tc).r;

	if (v.z <= 0) discard;
	while (hc > ht) {
		tc = tc + ti;
		if (tc.x < 0 || tc.x > 1 || tc.y < 0 || tc.y > 1) discard;
		hc = hc + hi;
		ht = h * texture(heightMap, tc).r;
	}
	return tc;

	
}



vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
	const float height_scale = 1;
    // number of depth layers
    const float numLayers = 10;
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy * height_scale; 
    vec2 deltaTexCoords = P / numLayers;  

	vec2  currentTexCoords     = texCoords;
	float currentDepthMapValue = texture(heightMap, currentTexCoords).r;
  
	while(currentLayerDepth < currentDepthMapValue)
	{
		// shift texture coordinates along direction of P
		currentTexCoords -= deltaTexCoords;
		// get depthmap value at current texture coordinates
		currentDepthMapValue = texture(heightMap, currentTexCoords).r;  
		// get depth of next layer
		currentLayerDepth += layerDepth;  
	}

	return currentTexCoords;
} 

void main(void){
	vec3 viewDir = normalize(ViewPosTS - FragPosTS);



	vec3 normal = texture(normalMap, TexCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	
	vec3 color = texture(diffuseMap, TexCoord).rgb;
	vec3 ambient = 0.1 * color;

	vec3 lightDir1 = normalize(LightPosTS1 - FragPosTS);
	vec3 lightDir2 = normalize(LightPosTS2 - FragPosTS);
	float diff1 = 3 * max(dot(lightDir1, normal), 0.0);
	float diff2 = 0.5 * max(dot(lightDir2, normal), 0.0);
	vec3 diffuse = (diff1 + diff2)/2 * color;

	
	vec3 reflectDir1 = reflect(-lightDir1, normal);
	vec3 halfwayDir1 = normalize(lightDir1 + viewDir);

	vec3 reflectDir2 = reflect(-lightDir2, normal);
	vec3 halfwayDir2 = normalize(lightDir2 + viewDir);

	float spec1 = 3 * pow(max(dot(normal, halfwayDir1), 0.0), 25.0);
	float spec2 = 0.5 * pow(max(dot(normal, halfwayDir2), 0.0), 25.0);
	vec3 specular = vec3(1.0) * (spec1+spec2);

	FragColor = vec4(ambient + diffuse + specular, 1.0);
}
