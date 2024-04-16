#version 330 core
uniform highp vec4 u_tint_color;
uniform sampler2D u_tux;

varying mediump vec2 tux_coord;

void main()
{
  gl_FragColor = vec4(texture2D(u_tux, tux_coord) * u_tint_color);
}
