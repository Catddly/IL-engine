#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform bool u_BatchRender;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexIndex;
out float v_TilingFactor;

void main()
{
	v_TexCoord = a_TexCoord;
	if (!u_BatchRender)
	{
		gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0f);
	}
	else 
	{
		v_Color = a_Color;
		v_TexIndex = a_TexIndex;
		v_TilingFactor = a_TilingFactor;
		gl_Position = u_ViewProjection * vec4(a_Position, 1.0f);
	}
}

#type fragment
#version 460 core

layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;
uniform sampler2D[32] u_Textures;
uniform vec4 u_Color;
uniform float u_UVScaling;
uniform bool u_BatchRender;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;
in float v_TilingFactor;

void main()
{
	if (!u_BatchRender)
	{
		color = texture(u_Texture, v_TexCoord * u_UVScaling) * u_Color;
	}
	else
	{
		color = texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_TilingFactor) * v_Color;
	}
}	