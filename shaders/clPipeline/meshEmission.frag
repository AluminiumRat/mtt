//meshEmission.frag

layout(location = 0) out vec4 outColor;

void main()
{
  outColor = vec4(getEmission(), 0);
}