#version 330 core

out vec4 fragColor;
in vec3 oColors;

void main()
{
	fragColor = vec4(oColors,1.0);
}
