#version 330 core

in vec3 TexCoords;
in vec3 color;

uniform samplerCube skybox;
uniform float mode;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    if(mode == 0.0f){
        fragColor = texture(skybox, TexCoords);
    }
    else if(mode == 2.0f){
        fragColor = vec4(1.0, 1.0, 1.0, 0.0);
    }
    else{
        vec3 myColor = color;
//        myColor.x = 0.5f * myColor.x + 0.5f;
//        myColor.y = 0.5f * myColor.y + 0.5f;
//        myColor.z = 0.5f * myColor.z + 0.5f;
        myColor.x = 0.5f * myColor.x;
        myColor.y = 0.5f * myColor.y + 0.5f;
        myColor.z = 0.5f * myColor.z;
        fragColor = vec4(myColor, 1.0f);
    }
}
