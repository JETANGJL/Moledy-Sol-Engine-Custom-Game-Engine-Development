#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform float u_Offset; 

out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;
    
    vec3 newPosition = a_Position;
    newPosition.y -= u_Offset * 2.f;

    gl_Position = vec4(newPosition, 1.0);
}


#fragment
    
#version 450 core

layout(location = 0) out vec4 FragColor;

uniform float u_Offset; 

in vec2 v_TexCoord;

void main()
{
    vec3 color = vec3(0.0, 0.0, 0.0);  
    FragColor = vec4(color, 1.0);
}