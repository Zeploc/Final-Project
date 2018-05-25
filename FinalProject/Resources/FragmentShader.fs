#version 450 core

out vec4 color;

in vec4 fragcolor;
in vec2 fragTexCoord;

uniform sampler2D tex;

void main(void)
{
	//color = fragcolor;	
	
	color = texture(tex, fragTexCoord) * fragcolor;
	//if (color.a < 1) color = vec4(1,0,0,1); else color = vec4(0,1,0,1);
} 

