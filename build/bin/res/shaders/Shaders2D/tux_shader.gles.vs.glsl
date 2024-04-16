#version 330 core

attribute highp vec3 i_pos;
attribute highp vec2 i_tux_coord;

uniform highp mat4 u_proj_view;
uniform highp mat4 u_trans;

varying vec2 tux_coord;

void main()
{
  gl_Position = u_proj_view * u_trans * vec4(i_pos, 1.0);
  tux_coord = i_tux_coord;
}
