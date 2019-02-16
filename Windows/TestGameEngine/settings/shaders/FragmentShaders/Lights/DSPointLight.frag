#version 330 core

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

    Attenuation attenuation;
};
  
uniform Light mLight;
uniform vec3 mViewPos;

uniform sampler2D mPositionMap;
uniform sampler2D mColorMap;
uniform sampler2D mNormalMap;

uniform vec2 mTextureSize;

layout (location = 0) out vec4 FragColor;

void main() {
    
    vec2 TexCoord = gl_FragCoord.xy / mTextureSize; // Calculate position in coordinate textures for the texture we are processing

    vec3 WorldPos = texture(mPositionMap, TexCoord).xyz;
    vec3 Color = texture(mColorMap, TexCoord).xyz;
    vec3 Normal = texture(mNormalMap, TexCoord).xyz; // Get data from the textures 

    // Fun with vectors
    Normal = normalize(Normal);
    vec3 lightDir = normalize(mLight.position - WorldPos);
    vec3 viewDir = normalize(mViewPos - WorldPos);
    vec3 halfwayDir = normalize(lightDir + viewDir); 

    // Attenuation
    float distance  = length(mLight.position - WorldPos);
    float lightAtt = 1.0/( mLight.attenuation.constant + mLight.attenuation.linear * distance + mLight.attenuation.quadratic * distance * distance);

    // ambient
    vec3 ambient = mLight.ambient;
    
    // diffuse 
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = mLight.diffuse * diff;

    // specular
    vec3 reflectDir = reflect(-lightDir, Normal);  
    
    //float spec = pow(max(dot(Normal, halfwayDir), 0.0), mMaterial.shininess);
    float spec = max(dot(Normal, halfwayDir), 0.0);
    vec3 specular = mLight.specular * spec; 
    
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(Color, 1.0) * vec4(result * lightAtt, 1.0);
}