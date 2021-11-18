//filter2D.vert

vec4 points[] = { vec4(-1.f,  1.f, .0f, 1.f),
                  vec4(-1.f, -1.f, .0f, 1.f),
                  vec4( 1.f,  1.f, .0f, 1.f),
                  vec4( 1.f, -1.f, .0f, 1.f)};

void main()
{
  gl_Position = points[gl_VertexIndex];
}
