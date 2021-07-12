//#VERTEX
#version 330 core
layout(location = 0) in vec3 _vertPos;
layout(location = 1) in vec2 _texCoord;

out vec2 texCoord;

void main()
{
	gl_Position = vec4(_vertPos.x, _vertPos.y, _vertPos.z, 1.0);
	texCoord = _texCoord;
}

//#FRAGMENT
#version 330 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D tex;

void main()
{
	fragColor = texture(tex, texCoord);
}
