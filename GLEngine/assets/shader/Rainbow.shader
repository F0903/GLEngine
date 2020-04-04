@vertex
#version 460 core

in vec4 position;
in vec2 texCoord;

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

in vec4 gl_FragCoord;
in vec2 localTexCoord;
in vec3 vertPosition;

out vec4 color;

void main()
{
	vec2 uv = gl_FragCoord.xy / res;

	vec3 col = 0.5 + 0.5 * cos(time + uv.xyx + vec3(0, 2, 4));

	vec4 texColor = texture(tex, localTexCoord);

	color = vec4(col * texColor.rgb, texColor.a);
}