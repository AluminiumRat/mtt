//shadowmapLib.glsl

layout( set = volatileSet,
        binding = shadowMapBinding$INDEX$) uniform sampler2D shadowMap$INDEX$;

float getOpaqueShadowFactor$INDEX$( vec2 shadowCoords,
                                    float normalizedDistanceToLight,
                                    float shadowmapSlope,
                                    float blurRadius)
{
  float mapSize = textureSize(shadowMap$INDEX$, 0).x;
  vec2 centerTexelCoords = mapSize * shadowCoords;
  vec2 blurSize = vec2(blurRadius * mapSize + .5f);

  vec2 startTexel = centerTexelCoords - blurSize;
  ivec2 iStartTexel = ivec2(startTexel);
  vec2 startWeights = vec2(1.f) - fract(startTexel);
  startTexel = floor(startTexel) + vec2(.5f);

  vec2 finishTexel = centerTexelCoords + blurSize;
  ivec2 iFinishTexel = ivec2(finishTexel);
  vec2 finishWeights = fract(finishTexel);
  finishTexel = floor(finishTexel) + vec2(.5f);

  float texelSlope = shadowmapSlope / mapSize;
  float maxTexelShift = max(length(centerTexelCoords - startTexel),
                            length(centerTexelCoords - finishTexel));
  float slopeCorrection = maxTexelShift * texelSlope;
  normalizedDistanceToLight -= slopeCorrection;

  float opaqueFactor = 0;

  ivec2 iTexelCoord = iStartTexel;
  float xWeight = startWeights.x;
  for(; iTexelCoord.x <= iFinishTexel.x;)
  {
    iTexelCoord.y = iStartTexel.y;
    float yWeight = startWeights.y;
    for(; iTexelCoord.y <= iFinishTexel.y;)
    {
      float shadowDepth = texelFetch( shadowMap$INDEX$,
                                      iTexelCoord,
                                      0).x;
      float currentWeight = xWeight * yWeight;
      opaqueFactor += currentWeight *
                                  step(normalizedDistanceToLight, shadowDepth);

      iTexelCoord.y++;
      yWeight = iTexelCoord.y == iFinishTexel.y ? finishWeights.y : 1.f;
    }

    iTexelCoord.x++;
    xWeight = iTexelCoord.x == iFinishTexel.x ? finishWeights.x : 1.f;
  }

  return opaqueFactor / (4 * blurSize.x * blurSize.y);
}

float getTransparentShadowFactor$INDEX$(vec2 shadowCoords,
                                        float normalizedDistanceToLight)
{
  vec3 variadicValues = textureLod( shadowMap$INDEX$,
                                    shadowCoords,
                                    0).gba;
  if(variadicValues.z == 0.f) return 1.f;

  float avgDistance = variadicValues.x / variadicValues.z;
  float avgSqDistance = variadicValues.y / variadicValues.z;
  float variance2 = max(avgSqDistance - avgDistance * avgDistance, 0.f);

  float deviation = normalizedDistanceToLight - avgDistance;
  float limit = 3.f * sqrt(variance2);
  float blackout = smoothstep(-limit, limit, deviation);

  blackout *= variadicValues.z;

  return 1.f - blackout;
}

float getShadowFactor$INDEX$( vec2 shadowCoords,
                              float normalizedDistanceToLight,
                              float shadowmapSlope,
                              float blurRadius)
{
  float opaqueFactor = getOpaqueShadowFactor$INDEX$(shadowCoords,
                                                    normalizedDistanceToLight,
                                                    shadowmapSlope,
                                                    blurRadius);

  float transparentFactor = getTransparentShadowFactor$INDEX$(
                                                    shadowCoords,
                                                    normalizedDistanceToLight);
  return min(opaqueFactor, transparentFactor);
}