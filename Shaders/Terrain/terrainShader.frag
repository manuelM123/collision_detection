// fragment shader
#version 410 core

in float Height;
out vec4 FragColor;

void main()
{
    // convert height to a color value
    float h = (Height + 16)/64.0f;
    // assing the color to the fragment
    FragColor = vec4(h, h, h, 1.0);
}