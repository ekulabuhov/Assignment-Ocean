#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;
out vec2 TexCoords;
out vec4 clipSpace;

uniform vec3 lightPos; // We now define the uniform in the vertex shader and pass the 'view space' lightpos to the fragment shader. lightPos is currently in world space.

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

// gravitational constant (m/sec)
float g = 9.8f;

float rfactor(float V)
{
	// H = 7.065 * 10e-3 * V^2.5
	// H = 2r => r = H / 2
    return 7.065 * 0.001 * pow(V, 2.5) * 2;
}


float wfactor(float V)
{
	// 9.8 / V * 0.81 = 8 / V
    return (g / V) * sqrt(2.0/3.0);
}

float kfactor(float V)
{
    float w = wfactor(V);
    return (w*w)/g;
}

// kr ~= 0.9
float windspeed = 5;
float kludgeCoef = 8.5;

void main()
{
	vec3 dpos = position;
    
    float r = rfactor(windspeed);
    float k = kfactor(windspeed);
    float w = wfactor(windspeed);

	dpos.x += r * sin(k*dpos.x - w*time);
    dpos.z = -r * cos(k*dpos.x - w*time); 
	
	float phase = k * position.x - w  * time -  kludgeCoef * dpos.z * 0.1;
	dpos.z = -r * cos(phase);

	clipSpace = projection * view * model * vec4(dpos, 1.0f);
    gl_Position = clipSpace;
    FragPos = vec3(view * model * vec4(dpos, 1.0f));
    Normal = mat3(transpose(inverse(view * model))) * normal;
    LightPos = vec3(view * vec4(lightPos, 1.0)); // Transform world-space light position to view-space light position
	TexCoords = texCoords;
}