//particlesVertexLighting.vert

layout(location = positionLocation) in vec3 inPosition;
layout(location = sizeRotationLocation) in vec2 inSizeRotation;
layout(location = albedoLocation) in vec4 inAlbedo;
layout(location = emissionLocation) in vec4 inEmission;
layout(location = textureIndexLocation) in uint inTextureIndex;
layout(location = tileIndexLocation) in uint inTileIndex;
layout(location = falloffFactorLocation) in float inFalloffFactor;

layout( set = volatileSet,
        binding = drawMatricesBinding) uniform DrawMatrices
{
  mat4 localToViewMatrix;
  mat3 normalsLocalToViewMatrix;
  mat4 projectionMatrix;
} drawMatrices;

layout( set = volatileSet,
        binding = mppxFunctionBinding) uniform MppxFunction
{
  float intercept;
  float slope;
} mppxFunction;

layout( set = volatileSet,
        binding = falloffBinding) uniform FalloffValues
{
  vec2 value;       // x value is base mppx of falloff start
                    // y value is base mppx to falloff finish
} falloffBase;

layout(location = 0) out vec2 outSizeRotation;
layout(location = 1) out vec4 outColor;
layout(location = 2) out flat uint outTextureIndex;
layout(location = 3) out flat uint outTileIndex;

MODIFICATOR_DECLARATION

vec3 viewCoord;
float overallAmbientWeight = 0.f;

vec3 totalIlluminance = vec3(0.f, 0.f, 0.f);
void applyLight(vec3 illuminance)
{
  totalIlluminance += illuminance;
}

void main()
{
  gl_Position = drawMatrices.localToViewMatrix * vec4(inPosition, 1.f);
  viewCoord = gl_Position.xyz;

  outColor = inAlbedo;

  APPLY_AMBIENT_WEIGHT
  APPLY_LIGHT

  outColor.rgb *= totalIlluminance / M_PI;
  outColor.rgb += inEmission.rgb;

  vec2 falloffMppx = falloffBase.value * inFalloffFactor;
  float distanceToParticle = length(gl_Position.xyz);
  float particleMppx = distanceToParticle * mppxFunction.slope +
                                                        mppxFunction.intercept;
  outColor *= 1.f - smoothstep( falloffMppx.x,
                                falloffMppx.y,
                                particleMppx);

  float sizeFalloffFactor = sqrt(particleMppx / falloffBase.value.x);
  outSizeRotation = inSizeRotation;
  outSizeRotation.x *= max(sizeFalloffFactor, 1.f);

  outTextureIndex = inTextureIndex;
  outTileIndex = inTileIndex;

  APPLY_POSTEFFECT
}
