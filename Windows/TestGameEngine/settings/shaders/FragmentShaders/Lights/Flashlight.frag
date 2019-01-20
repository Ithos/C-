#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Attenuation
{
    float constant;
    float linear;
    float quadratic;
};

struct Light {  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    vec3 direction;
    float maxAngle;

    Attenuation attenuation;
};
  
uniform Material material;
uniform Light light;
uniform vec3 viewPos;

in vec3 fragNormal;
in vec3 fragPos;

out vec4 FragColor;

void main() {

	// Fun with vectors
	vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir); 
    float theta = dot(lightDir, normalize(-light.direction));

    // Attenuation
    float distance  = length(light.position - fragPos);
    float lightAtt = 1.0/( light.attenuation.constant + light.attenuation.linear * distance + light.attenuation.quadratic * distance * distance);
	
	// ambient
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = vec3(0.0, 0.0, 0.0);
    vec3 specular = vec3(0.0, 0.0, 0.0);

  	if(theta > cos(radians(light.maxAngle)))
    {
        // diffuse 
       
        float diff = max(dot(norm, lightDir), 0.0);
        diffuse = light.diffuse * (diff * material.diffuse);
        
        // specular
          
        float spec = pow(max(dot(fragNormal, halfwayDir), 0.0), material.shininess);
        specular = light.specular * (spec * material.specular);  
    }
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result * lightAtt, 1.0);
}