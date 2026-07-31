#version 430 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D gridTexture;

void main() {
    vec3 color = texture(gridTexture, TexCoord).rgb;
    FragColor = vec4(color, 1.0);
}
