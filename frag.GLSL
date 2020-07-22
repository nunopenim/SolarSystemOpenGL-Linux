#version 330 core
uniform sampler2D texturePlanet;
in vec3 interpolatedNormal;
in vec2 st;
out vec4 FragColor;

void main () {
    FragColor = texture(texturePlanet, st);
}

