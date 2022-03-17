//particles.geom

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

layout( set = volatileSet,
        binding = drawMatricesBinding) uniform DrawMatrices
{
  mat4 localToViewMatrix;
  mat3 normalsLocalToViewMatrix;
  mat4 projectionMatrix;
} drawMatrices;

layout(location = 0) in vec2 inSizeRotation[];
layout(location = 1) in vec4 inColor[];
layout(location = 2) in uint inTextureIndex[];
layout(location = 3) in uint inTileIndex[];

layout(location = 0) out vec4 outColor;

#ifdef COLOR_SAMPLER_ENABLED
  layout(location = 1) out vec2 outTexCoords;
  layout(location = 2) out flat uint outTextureIndex;
#endif

const vec2 shifts[4] = {vec2(-.5f,  .5f),
                        vec2( .5f,  .5f),
                        vec2(-.5f, -.5f),
                        vec2( .5f, -.5f)};

#ifdef COLOR_SAMPLER_ENABLED
  const vec2 texCoords[4] = { vec2( 0.f, 0.f),
                              vec2( 1.f, 0.f),
                              vec2( 0.f, 1.f),
                              vec2( 1.f, 1.f)};

  const uint textureExtent[TEXTURES_NUMBER] = {EXTENT_DEFINE};
#endif

void main()
{
  float size = inSizeRotation[0].x;
  float rotation = inSizeRotation[0].y;
  float angleSin = sin(rotation);
  float angleCos = cos(rotation);

  #ifdef COLOR_SAMPLER_ENABLED
    uint textureExtent = textureExtent[inTextureIndex[0]];
    float texCoordWidth = 1.f / textureExtent;
    vec2 texCoordStart = vec2(inTileIndex[0] % textureExtent,
                              inTileIndex[0] / textureExtent);
    texCoordStart *= texCoordWidth;
  #endif

  for(int pointIndex = 3; pointIndex >= 0; pointIndex--)
  {
    vec2 shift = shifts[pointIndex];
    vec2 rotatedShift = vec2( shift.x * angleCos - shift.y * angleSin,
                              shift.x * angleSin + shift.y * angleCos);
    vec4 viewPosition = gl_in[0].gl_Position;
    viewPosition.xy += rotatedShift * size;
    gl_Position = drawMatrices.projectionMatrix * viewPosition;
    outColor = inColor[0];

    #ifdef COLOR_SAMPLER_ENABLED
      outTexCoords = texCoordStart + texCoords[pointIndex] * texCoordWidth;
      outTextureIndex = inTextureIndex[0];
    #endif

    EmitVertex();
  }
}