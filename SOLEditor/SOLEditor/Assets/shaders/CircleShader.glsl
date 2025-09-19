 #version 450 core
    layout(location = 0) in vec3 a_Position;
    layout(location = 1) in vec3 a_LocalPosition;
    layout(location = 2) in vec4 a_Color;
    layout(location = 3) in float a_borderThickness;   
    layout(location = 4) in  int a_EntityID;

    uniform mat4 u_ViewProjection;

    out vec3  v_Position;
    out vec3  v_LocalPosition;
    out vec4  v_Color;
    out float v_borderThickness;
    flat out int v_EntityID;  // Changed layout to flat

    void main()
    {   
	    v_LocalPosition = a_LocalPosition;
	    v_Color = a_Color;
	    v_borderThickness = a_borderThickness;
        v_Position = a_Position;
        v_EntityID = a_EntityID;

        gl_Position = u_ViewProjection * vec4(a_Position, 1.0); 
    }

#fragment
    
#version 450 core

    layout(location = 0) out vec4 FragColor;
    layout (location = 1) out  int out_entity_ID;

    in vec3 v_Position;
    in vec3 v_LocalPosition;
    in vec4 v_Color;
    in float v_borderThickness;
    flat in int v_EntityID;  // Changed layout to flat

    void main()
    {   
        out_entity_ID = v_EntityID;

        float distanceFromCenter = length(v_LocalPosition); 
    
        if (distanceFromCenter >= 1.0 || distanceFromCenter <= v_borderThickness) 
        {
            discard;
        }
    
        float outerCircle = smoothstep(1.0, 1.0 - 0.0005, distanceFromCenter);
        float innerCircle  = smoothstep(v_borderThickness, v_borderThickness + 0.0005, distanceFromCenter);
    
        float alpha = outerCircle * innerCircle;
    
        FragColor = vec4(v_Color.rgb, alpha);

      //  gl_FragDepth = v_Position.z;


    }