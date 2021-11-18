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

layout(location = 0) out vec2 outColor;

void main()
{
  float distance = -inViewCoord.z;
  distance -= nearFar.near;
  distance /= nearFar.nearFar;
  outColor = vec2(distance, distance * distance);
}