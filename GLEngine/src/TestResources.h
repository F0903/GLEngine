#pragma once

const char* testRainbowShader = R"(
@vertex
#version 460 core

layout(location=0) in vec4 position;
layout(location = 0) in vec2 texCoord;

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

layout(location=0) out vec4 color;
in vec4 gl_FragCoord;

in vec2 localTexCoord;

in vec3 vertPosition;

void main()
{
	vec2 uv = gl_FragCoord.xy / res;

	vec3 col = 0.5 + 0.5 * cos(time + uv.xyx + vec3(0, 2, 4));

	vec4 texColor = texture(tex, localTexCoord);

	color = vec4(col + texColor.rgb, 1);
}
)";

const char* testShader = R"(
@vertex
#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec3 vertPosition;

out vec2 localTexCoord;

void main()
{
	vertPosition = position.xyz;
	localTexCoord = texCoord;
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

layout(location = 0) out vec4 color;

void main()
{
	vec2 norm = gl_FragCoord.xy / res;

	vec4 texColor = texture(tex, localTexCoord);

	color = texColor + vec4(col, 1);
}
)";