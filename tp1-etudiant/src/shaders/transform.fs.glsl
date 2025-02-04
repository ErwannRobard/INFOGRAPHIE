#version 330 core

// TODO

in vec3 laCouleurVertex;
out vec4 fragColor;

void main() {
    fragColor = vec4(laCouleurVertex, 1);
}