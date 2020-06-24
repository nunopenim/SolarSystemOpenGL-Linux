#version 330 core


uniform sampler2D texturePlanet;
in vec3 interpolatedNormal;
in vec2 st;
in vec3 FragPos;
in vec3 lightPos;
out vec4 FragColor;

uniform vec3 rotationS;
in vec3 P;

void main () {
    FragColor = vec4(1,0,0,1);
    
    //ambient
    const vec3 AmbientColor = vec3(0.0, 0.0, 0.0);

    //diffuse
    const vec3 DiffuseColor = vec3(1.0, 1.0, 1.0);
    vec3 normalized_normal = normalize(interpolatedNormal);
    vec3 normalized_lightDir = normalize(lightPos - P);
    float DiffuseTerm = clamp(dot(normalized_normal, normalized_lightDir), 0.0, 1.0);

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(-FragPos); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
    vec3 reflectDir = reflect(-normalized_lightDir, normalized_normal);  
    float spec = pow(max(dot(vec3(0.0,0.0,0.0), reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);
    FragColor = texture(texturePlanet, st) * vec4(AmbientColor + DiffuseColor * DiffuseTerm, 1.0);
}
