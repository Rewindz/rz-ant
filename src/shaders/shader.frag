#version 430 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D gridTexture;
uniform vec2 antPos;
uniform vec2 gridSize;

void main() {

    ivec2 currentCell = ivec2(TexCoord * gridSize);
    ivec2 antCell = ivec2(antPos);

    if(currentCell == antCell){
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        vec3 color = texture(gridTexture, TexCoord).rgb;
        FragColor = vec4(color, 1.0);
    }
}
