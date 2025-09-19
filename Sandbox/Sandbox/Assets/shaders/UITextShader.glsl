    #version 450 core
    layout(location = 0) in vec3 a_Position;
    layout(location = 1) in vec4 a_Color;
    layout(location = 2) in vec2 a_TexCoord;   
    layout(location = 3) in float a_TexIndex;
    layout(location = 4) in int a_EntityID;
    layout(location = 5) in float a_buffer;
    layout(location = 6) in float a_gamma;


    uniform mat4 u_ViewProjection;

    out vec4 v_Color;
    out vec2 v_TexCoord;
    out float v_TexIndex;
    out vec3 v_Position;
    layout(location = 4) out int v_EntityID;
    out float v_buffer;
    out float v_gamma;

    void main()
    {   
        v_Color = a_Color;
        v_Position = a_Position;
        v_TexIndex = a_TexIndex;
        v_TexCoord = a_TexCoord;
        v_EntityID = a_EntityID;
        v_buffer = a_buffer;
        v_gamma = a_gamma;

        gl_Position = u_ViewProjection * vec4(a_Position, 1.0); 
    }

#fragment
    
    
    #version 450 core

    precision highp float; 

    layout (location = 0) out vec4 FragColor;
    layout (location = 1) out int out_entity_ID;

    uniform sampler2D u_Texture[32];

     in vec4 v_Color;
     in vec2 v_TexCoord;
     in float v_TexIndex;
     in vec3 v_Position;
     layout(location = 4) in flat int v_EntityID;
     in float v_buffer;
     in float v_gamma;

void main()
{
     //gl_FragDepth = v_Position.z;

    int index = int(v_TexIndex);
    float redValue = texture(u_Texture[index], v_TexCoord).r;
    float alpha = smoothstep(v_buffer - v_gamma, v_buffer + v_gamma, redValue);

    FragColor = vec4(v_Color.rgb, alpha);
    out_entity_ID = v_EntityID;
}