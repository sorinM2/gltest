#version 330 core
out vec4 FragColor;

uniform sampler2D _texture;
uniform bool isLight;
uniform vec3 lightPos;
uniform vec3 eyePos;


in vec2 texCoord;
in vec3 normal;
in vec3 position;

vec3 light_color = vec3(1.f, 1.f, 1.f);
float ambient_strength = 0.1f;
float specular_strength = 0.5;


void main()
{
	if ( isLight )
	{
		FragColor = vec4(light_color, 1.f);
		return;
	}

	vec3 ambient = light_color * ambient_strength;
	
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - position);

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * light_color; 
	
	vec3 eyeDir = normalize(eyePos - position);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(reflectDir, eyeDir), 0.f), 132);
	vec3 specular = spec * light_color * specular_strength;

	vec3 result = texture(_texture, texCoord).xyz * (ambient + diffuse + specular);

	FragColor = vec4(result, 1.f);
}
