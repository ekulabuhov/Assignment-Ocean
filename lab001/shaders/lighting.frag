#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;   // Extra in variable, since we need the light position in view space we calculate this in the vertex shader
in vec2 TexCoords;

out vec4 color;

uniform vec3 objectColor;
uniform sampler2D diffuseTexture;

// viewPos is the same as cameraPos
uniform vec3 viewPos;

bool useTexture = true;

void main()
{
	//vec3 myColor = objectColor;
	//vec3 lightColor = vec3(0.3);
	//if (useTexture) {
		vec3 myColor = texture(diffuseTexture, TexCoords).rgb;
	//}
 //   // Ambient
 //   float ambientStrength = 0.1f;
 //   vec3 ambient = ambientStrength * lightColor;    
    
 //    // Diffuse 
 //   vec3 norm = normalize(Normal);
 //   vec3 lightDir = normalize(LightPos - FragPos);
 //   float diff = max(dot(norm, lightDir), 0.0);
 //   vec3 diffuse = diff * lightColor;
    
 //   // Specular
 //   float specularStrength = 1.5f;
 //   vec3 viewDir = normalize(-FragPos); // The viewer is at (0,0,0) so viewDir is (0,0,0) - Position => -Position
	
	//// Phong
 //   //vec3 reflectDir = reflect(-lightDir, norm);  
 //   //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	
	//// Blinn-Phong
	//vec3 halfwayDir = normalize(lightDir + viewDir);  
	//float spec = pow(max(dot(norm, halfwayDir), 0.0), 128.0);
	
 //   vec3 specular = specularStrength * spec * lightColor; 
    
    //vec3 result = (ambient + diffuse + specular) * myColor;
    color = vec4(myColor, 1.0f);
}