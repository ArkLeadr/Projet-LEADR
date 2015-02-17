#version 400 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;
layout(location = 3) in vec3 tangent;

out Data {
    vec3 pos;
    vec3 normal;
    vec2 texcoord;
    vec3 tangent;
} outData;

uniform mat4 world;

void main( void )
{
    outData.pos =  vertex;
    outData.normal = normalize(normal);
    outData.texcoord = texcoord;
    outData.tangent = tangent;
}
