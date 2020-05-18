#version 330 core

uniform sampler2D texturePlanet;
in vec3 interpolatedNormal;
in vec2 st;
out vec4 FragColor;

void main () {
    //FragColor = vec4(1,0,0,1);
    //const vec3 AmbientColor = vec3(0.1, 0.0, 0.0);
    //const vec3 DiffuseColor = vec3(1.0, 0.0, 0.0);
    //vec3 normalized_normal = normalize(interpolatedNormal);
    //vec3 normalized_lightDir = normalize(vec3(1.0,0.0,1.0));
    //float DiffuseTerm = clamp(dot(normalized_normal, normalized_lightDir), 0.0, 1.0);
    FragColor = texture(texturePlanet, st);
}
