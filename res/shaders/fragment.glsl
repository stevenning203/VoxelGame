#version 330 core

in vec2 texture_coordinate;
in float color_ambient_occlusion_offset;

out vec4 FragColor;

uniform sampler2D texture_sampler;

void main() {
    vec4 texture_color = texture(texture_sampler, texture_coordinate);
    float alpha = texture_color.a;
    FragColor = vec4(texture_color.rgb - color_ambient_occlusion_offset, alpha);
}