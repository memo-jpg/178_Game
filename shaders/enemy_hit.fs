#version 120

uniform sampler2D u_texture;
uniform vec3 u_shadowColor;
uniform vec3 u_highlightColor;

varying vec2 v_texcoord;

void main()
{
    vec4 texColor = texture2D(u_texture, v_texcoord);
    float luminance = dot(texColor.rgb, vec3(0.299, 0.587, 0.114));
    vec3 paletteColor = mix(u_shadowColor, u_highlightColor, luminance);

    gl_FragColor = vec4(paletteColor, texColor.a);
}
