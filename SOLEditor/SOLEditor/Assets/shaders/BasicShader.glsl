    #version 450 core
    layout(location = 0) in vec3 a_Position;
    layout(location = 1) in vec4 a_Color;
    layout(location = 2) in  int a_EntityID;

    uniform mat4 u_ViewProjection;

    out vec4 v_Color;
    out vec3 v_Position;
    layout (location = 2) out flat int v_EntityID;


    void main()
    {   
        v_Color = a_Color;
        v_Position = a_Position;
        gl_Position =  u_ViewProjection * vec4(a_Position, 1.0); 
        v_EntityID = a_EntityID;
    }

#fragment

    #version 450 core
    layout (location = 0) out vec4 FragColor;
    layout (location = 1) out  int out_entity_ID;

     in vec4 v_Color;
     in vec3 v_Position;
     layout (location = 2) in flat int v_EntityID;


    void main()
    {

       // gl_FragDepth = v_Position.z;

        FragColor = v_Color;

        out_entity_ID = v_EntityID;

    }