#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
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
    vec3 lightDir = normalize(-light.direction);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir); 
	
	// ambient
    vec3 ambient = light.ambient * material.ambient;
  	
    // diffuse 
   
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // specular
     
    float spec = pow(max(dot(fragNormal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}