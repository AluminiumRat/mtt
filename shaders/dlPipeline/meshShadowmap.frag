//meshShadowmap.frag

#ifndef ENABLE_VIEW_COORDS_IN_FRAGMENT_SHADER
  You should set VIEW_COORDS_IN_FRAGMENT_SHADER_FEATURE
#endif

layout(binding = nearfarBindong, set = volatileSet) uniform NearFar
{
  float near;
  float nearFar;
} nearFar;

layout(location = 5) in vec3 inViewCoord;

layout(location = 0) out float outDistance;

void main()
{
  outDistance = -inViewCoord.z;
  outDistance -= nearFar.near;
  outDistance /= nearFar.nearFar;
}