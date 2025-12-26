#version 400 core

out vec4 FragColor;

in vec2 texPos;

in float lightVal;
vec4 light;

uniform sampler2D sampler;

void main()
{
    light = vec4(lightVal, lightVal, lightVal, 1.0f);
    FragColor = texture(sampler, texPos);
   //FragColor = light * vec4(1.0f, 0.0f, 0.0f, 1.0f);
}