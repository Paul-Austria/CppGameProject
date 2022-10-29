#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D texture1;
uniform vec3 spriteColor;

uniform vec4 inColor;
uniform bool useColor;

void main()
{   if(!useColor)
    {
        vec4 texture = texture(texture1, TexCoords);
        if(texture.a <= 0)
        {
            discard;
        }
        color = texture;
    }else{
        color = inColor;
    }
}  
