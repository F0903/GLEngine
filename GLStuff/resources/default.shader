//#VERTEX
#version 400 core
layout(location = 0) in vec3 _vertPos;
layout(location = 1) in vec2 _texCoord;

out vec2 texCoord;

void main()
{
	gl_Position = vec4(_vertPos.x, _vertPos.y, _vertPos.z, 1.0);
	texCoord = _texCoord;
}

//#FRAGMENT
#version 400 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D tex;

uniform double time;

const double pi = 3.1415;
const double full = 2 * pi;
const double third = full / 3;

void main()
{
	double ta = (sin(float(time + third + 1)) + 1) / 2;
	double tb = (sin(float(time + third * 2 + 2)) + 1) / 2;
	double tc = (sin(float(time + third * 3 + 3)) + 1) / 2;
	fragColor = texture(tex, texCoord) * vec4(ta, tb, ta, 1.0f);
}
