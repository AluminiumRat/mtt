//particles.vert

layout( set = volatileSet,
        binding = drawMatricesBinding) uniform DrawMatrices
{
  mat4 localToViewMatrix;
  mat3 normalsLocalToViewMatrix;
  mat4 projectionMatrix;
} drawMatrices;

layout( set = staticSet,
        binding = positionsBufferBinding) uniform Positions
{
  vec4 data[4096];
} positions;

layout( set = staticSet,
        binding = sizeRotationBufferBinding) uniform SizeRotation
{
  vec4 data[4096];
} sizeRotation;

layout( set = staticSet,
        binding = colorsBufferBinding) uniform Colors
{
  vec4 data[4096];
} colors;

layout( set = staticSet,
        binding = textureIndicesBinding) uniform TextureIndices
{
  uvec4 data[4096];
} textureIndices;


layout(location = 0) out vec2 outSizeRotation;
layout(location = 1) out vec4 outColor;
layout(location = 2) out flat uint outTextureIndex;

void main()
{
  vec4 position = positions.data[gl_VertexIndex];
  gl_Position = drawMatrices.localToViewMatrix * position;

  outSizeRotation = sizeRotation.data[gl_VertexIndex].xy;
  outColor = colors.data[gl_VertexIndex];
  outTextureIndex = textureIndices.data[gl_VertexIndex / 4][gl_VertexIndex % 4];
}
