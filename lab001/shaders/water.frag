#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;   // Extra in variable, since we need the light position in view space we calculate this in the vertex shader
in vec2 TexCoords;
in vec4 clipSpace;

out vec4 color;

uniform vec3 objectColor;
uniform samplerCube cubeMap;
uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D normalMap;

// viewPos is the same as cameraPos
uniform vec3 viewPos;

bool useTexture = false;

void main()
{
	vec2 ndc = clipSpace.xy / clipSpace.w / 2.0 + 0.5;

	// Bump Map
	vec4 normalMapColor = texture(normalMap, vec2(ndc.x, ndc.y));
	vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b, normalMapColor.g * 2.0 - 1.0);
	normal = normalize(normal);

	vec3 myColor = objectColor;
	vec3 lightColor = vec3(0.3);
	if (useTexture) {
		myColor = texture(reflectionTexture, TexCoords).rgb;
	}
    // Ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;    
    
    // Diffuse 
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos); // The viewer is at (0,0,0) so viewDir is (0,0,0) - Position => -Position
	
	// Phong
    //vec3 reflectDir = reflect(-lightDir, normal);  
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	
	// Blinn-Phong
	vec3 halfwayDir = normalize(lightDir + viewDir);  
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 128.0);
	
    vec3 specular = specularStrength * spec * lightColor; 

	vec4 reflectionColor = texture(reflectionTexture, vec2(ndc.x, -ndc.y));
	vec4 refractionColor = texture(refractionTexture, vec2(ndc.x, ndc.y));

	if (TexCoords.x == -15 && TexCoords.y == -15) {
		color = vec4(1.0f, 0.0f, 0.0f, 1.0f); // paint it red
	} else {
		color = mix(reflectionColor, refractionColor, 0.5);
		color = color + vec4(specular, 0.0f);
	}
    
    //vec3 result = (ambient + diffuse + specular) * myColor;
    //color = vec4(result, 1.0f);
}