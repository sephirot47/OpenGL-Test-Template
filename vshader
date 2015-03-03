#version 130

uniform float time;
uniform mat4 transform, projection;

in vec3 position;

void main()  
{  
    gl_Position = projection * transform * vec4(position.xyz, 1.0);
}

