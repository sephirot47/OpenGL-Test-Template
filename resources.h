const char* vertexShaderSource = " \
#version 130 \n\r\
 \
uniform float time;\
uniform mat4 transform, projection;\
in vec4 pos; \
out vec2 uv; \
 \
void main() \
{ \
    \
    gl_Position = projection * transform * vec4(vec3(pos), 1.0); \
} \
";

const char* fragmentShaderSource = " \
#version 130 \n\r\
 uniform float time;\
 uniform mat4 transform;\
void main() \
{ \
    gl_FragColor = vec4(sin(time), cos(time), 0.0, 1.0); \
} \
";

const float triMesh[9] = {-0.4f, 0.1f, 0.0f,
                       0.4f, 0.1f, 0.0f,
                       0.0f, 0.7f, 0.0f};
