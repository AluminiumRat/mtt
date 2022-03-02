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

const vec2 shifts[6] = {vec2(-.5f,  .5f),
                        vec2( .5f,  .5f),
                        vec2( .5f, -.5f),
                        vec2(-.5f,  .5f),
                        vec2( .5f, -.5f),
                        vec2(-.5f, -.5f)};

const vec2 texCoords[6] = { vec2( 0.f, 0.f),
                            vec2( 1.f, 0.f),
                            vec2( 1.f, 1.f),
                            vec2( 0.f, 0.f),
                            vec2( 1.f, 1.f),
                            vec2( 0.f, 1.f)};

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 outTexCoords;
layout(location = 2) out flat uint textureIndex;

void main()
{
  int dataIndex = gl_VertexIndex / 6;
  int cornerIndex = gl_VertexIndex % 6;

  vec4 position = positions.data[dataIndex];
  float size = sizeRotation.data[dataIndex].x;
  float rotation = sizeRotation.data[dataIndex].y;
  float angleSin = sin(rotation);
  float angleCos = cos(rotation);

  vec4 viewPosition = drawMatrices.localToViewMatrix * position;
  vec2 shift = shifts[cornerIndex];
  vec2 rotatedShift = vec2( shift.x * angleCos - shift.y * angleSin,
                            shift.x * angleSin + shift.y * angleCos);
  viewPosition.xy += rotatedShift * size;

  gl_Position = drawMatrices.projectionMatrix * viewPosition;

  outColor = colors.data[dataIndex];
  outTexCoords = texCoords[cornerIndex];

  textureIndex = textureIndices.data[dataIndex / 4][dataIndex % 4];
}
