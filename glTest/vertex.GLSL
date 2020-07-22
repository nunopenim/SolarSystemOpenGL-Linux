#version 330 core
layout(location=0) in vec3 Position;
layout(location=1) in vec3 Normal;
layout(location=2) in vec2 TexCoord;

   out vec3 FragPos;
   out vec3 interpolatedNormal;
   out vec2 st;
   out vec3 lightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 P;


void main()
{
    gl_Position = projection * view * model * vec4(Position, 1.0);
    P = vec3(view * model * vec4(Position, 1.0));
    interpolatedNormal = mat3(transpose(inverse(view * model))) * Normal;
    st = TexCoord; // Will also be interpolated across the triangle
    vec3 lightLocation = vec3(1.0,1.0,1.0);

    lightPos = vec3(view * vec4(lightLocation, 1.0));
}

