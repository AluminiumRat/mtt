//directLightDrawable.vert

layout(location = 0) out noperspective vec2 outClipCoord;

const vec4 points[] = { vec4(-1.f,  1.f, .0f, 1.f),
                        vec4(-1.f, -1.f, .0f, 1.f),
                        vec4( 1.f,  1.f, .0f, 1.f),
                        vec4( 1.f, -1.f, .0f, 1.f)};

void main()
{
  gl_Position = points[gl_VertexIndex];
  outClipCoord = gl_Position.xy;
}