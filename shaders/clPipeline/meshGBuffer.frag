//meshGBuffer.frag

layout(location = 0) out vec4 outAlbedo;
layout(location = 1) out vec4 outSpecular;
layout(location = 2) out vec4 outNormal;

void main()
{
  outAlbedo = vec4(getSurfaceColor().rgb, 0.f);
  outSpecular = vec4(getSpecular(), 0.f);
  outNormal = vec4(getNormal(), 0.f);
}