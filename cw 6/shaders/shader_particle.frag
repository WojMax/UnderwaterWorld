#version 430 core

in float lifetime;

out vec4 fragColor;

void main()
{
   float lifetimeNormalize = lifetime/2;
   fragColor = vec4(0.0f, lifetimeNormalize, 1.0f, lifetimeNormalize);
} 