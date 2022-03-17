//particles.vert

layout(location = positionLocation) in vec3 inPosition;
layout(location = sizeRotationLocation) in vec2 inSizeRotation;
layout(location = colorLocation) in vec4 inColor;
layout(location = textureIndexLocation) in uint inTextureIndex;
layout(location = tileIndexLocation) in uint inTileIndex;

layout( set = volatileSet,
        binding = drawMatricesBinding) uniform DrawMatrices
{
  mat4 localToViewMatrix;
  mat3 normalsLocalToViewMatrix;
  mat4 projectionMatrix;
} drawMatrices;

layout(location = 0) out vec2 outSizeRotation;
layout(location = 1) out vec4 outColor;
layout(location = 2) out flat uint outTextureIndex;
layout(location = 3) out flat uint outTileIndex;

void main()
{
  gl_Position = drawMatrices.localToViewMatrix * vec4(inPosition, 1.f);
  outSizeRotation = inSizeRotation;
  outColor = inColor;
  outTextureIndex = inTextureIndex;
  outTileIndex = inTileIndex;
}
