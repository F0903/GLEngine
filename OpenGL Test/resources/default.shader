//#VERTEX
#version 330 core
layout(location = 0) in vec3 vertPos;

void main()
{
	gl_Position = vec4(vertPos.x, vertPos.y, vertPos.z, 1.0);
}

//#FRAGMENT
#version 330 core
out vec4 fragColor;

void main()
{
	fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
