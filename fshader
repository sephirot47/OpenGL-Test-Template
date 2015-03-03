#version 130	

uniform float time;
uniform mat4 transform, projection;

void main()  
{  
    gl_FragColor = vec4(sin(time), cos(time), sin(gl_FragCoord.x / 5.0), 1.0);  
}

