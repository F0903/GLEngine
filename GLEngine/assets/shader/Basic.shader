@vertex
#version 460 core

in layout(location = 0) vec4 position;
in layout(location = 1) vec2 texCoord;

out vec3 vertPosition;
out vec2 localTexCoord;

void main()
{
	localTexCoord = texCoord;
	vertPosition = position.xyz;
	gl_Position = position;
}

@fragment
#version 460 core

uniform float time;
uniform vec2 res;
uniform sampler2D tex;
uniform vec3 col;

in vec2 localTexCoord;
in vec3 vertPosition;
in vec4 gl_FragCoord;

out vec4 color;

void main()
{
	vec2 norm = gl_FragCoord.xy / res;

	vec4 texColor = texture(tex, localTexCoord);

	color = vec4(texColor.rgb * col, texColor.a);
}