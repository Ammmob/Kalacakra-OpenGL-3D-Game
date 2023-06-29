#version 330 core
struct DirLight{
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	vec3 direction;
};

struct PointLight{
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	vec3 position;

	float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	vec3 position;
	vec3 direction;

	float innerCutOff;
	float outerCutOff;

	float constant;
    float linear;
    float quadratic;
};
out vec4 fragColor;

in vec3 v_normal;
in vec3 v_fragPos;
in vec2 v_texCoord;

uniform sampler2D texture_diffuse0;

uniform vec3 u_view_position;
uniform DirLight u_dirLight;
uniform PointLight u_pointLight;
uniform SpotLight u_spotLight;

uniform bool isBlinn = false;
uniform bool isSpotLight = false;
uniform float shininess;

vec4 GetDirLight(DirLight light, vec4 object_color, vec4 specular_color) {
	vec3 view_direction = normalize(u_view_position - v_fragPos);
	vec3 light_direction = normalize(-light.direction);

	float diff = max(0.0f, dot(v_normal, light_direction));
	float spec;
	if(isBlinn){
		vec3 halfwayDir = normalize(light_direction + view_direction);  
        spec = pow(max(dot(v_normal, halfwayDir), 0.0), 2* shininess);
	}
	else{
		vec3 reflect_direction = reflect(-light_direction, v_normal);
		spec = pow(max(0.0f,dot(view_direction, reflect_direction)),shininess);
	}

	vec4 ambient = vec4(light.ambient,1.0f) * object_color;
	vec4 diffuse = vec4(light.diffuse,1.0f) * diff * object_color;
	vec4 specular = vec4(light.specular, 1.0f) * spec * specular_color;
	return (ambient + diffuse + specular);
	
}
vec4 GetPointLight(PointLight light, vec4 object_color, vec4 specular_color) {
	vec3 view_direction = normalize(u_view_position - v_fragPos);
	vec3 light_direction = normalize(light.position-v_fragPos);
	
	float diff = max(0.0f, dot(v_normal, light_direction));
	float spec;
	if(isBlinn){
		vec3 halfwayDir = normalize(light_direction + view_direction);  
        spec = pow(max(dot(v_normal, halfwayDir), 0.0), 2 * shininess);
	}
	else{
		vec3 reflect_direction = reflect(-light_direction, v_normal);
		spec = pow(max(0.0f,dot(view_direction, reflect_direction)),shininess);
	}

	float d = length(v_fragPos - light.position);
	float attenuation = 1.0f/(light.constant + light.linear * d + light.quadratic * d * d);

	vec4 ambient = vec4(light.ambient,1.0f) * object_color;
	vec4 diffuse = vec4(light.diffuse,1.0f) * diff * object_color;
	vec4 specular = vec4(light.specular, 1.0f) * spec * specular_color;
	return attenuation * (ambient + diffuse + specular);
	
}
vec4 GetSpotLight(SpotLight light, vec4 object_color, vec4 specular_color) {
	vec3 view_direction = normalize(u_view_position - v_fragPos);
	vec3 light_direction = normalize(light.position-v_fragPos);

	float diff = max(0.0f, dot(v_normal, light_direction));
	float spec;
	if(isBlinn){
		vec3 halfwayDir = normalize(light_direction + view_direction);  
        spec = pow(max(dot(v_normal, halfwayDir), 0.0), 2 * shininess);
	}
	else{
		vec3 reflect_direction = reflect(-light_direction, v_normal);
		spec = pow(max(0.0f,dot(view_direction, reflect_direction)), shininess);
	}

	float d = length(v_fragPos - light.position);
	float attenuation = 1.0f/(light.constant + light.linear * d + light.quadratic * d * d);
	float theta = dot(normalize(-light.direction), light_direction);
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity  = clamp((theta - light.outerCutOff)/epsilon, 0.0f, 1.0f);

	vec4 ambient = vec4(light.ambient,1.0f) * object_color;
	vec4 diffuse = vec4(light.diffuse,1.0f) * diff * object_color;
	vec4 specular = vec4(light.specular, 1.0f) * spec * specular_color;
	return intensity * attenuation * (ambient + diffuse + specular);
}
void main() {
	vec4 object_color = texture(texture_diffuse0, v_texCoord);  
	vec4 specular_color = vec4(0.0f);

	fragColor = GetDirLight(u_dirLight, object_color, specular_color);
	if(isSpotLight){
		fragColor += GetSpotLight(u_spotLight, object_color, specular_color);
	}
	// fragColor = texture(texture_diffuse0, v_texCoord);
}