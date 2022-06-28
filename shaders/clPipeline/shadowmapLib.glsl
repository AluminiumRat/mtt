//shadowmapLib.glsl

layout( set = volatileSet,
        binding = shadowMapBinding$INDEX$) uniform sampler2D shadowMap$INDEX$;

float getOpaqueShadowFactor$INDEX$( vec2 shadowCoords,
                                    float normalizedDistanceToLight,
                                    float shadowmapSlope,
                                    float blurRadius)
{
  float slopeCorrection = 2.f * blurRadius * shadowmapSlope;
  normalizedDistanceToLight -= slopeCorrection;

  float mapSize = textureSize(shadowMap$INDEX$, 0).x;

  vec2 startTexel = mapSize * (shadowCoords - vec2(blurRadius));
  ivec2 iStartTexel = ivec2(startTexel);
  vec2 startWeights = vec2(1.f) - fract(startTexel);

  vec2 finishTexel = mapSize * (shadowCoords + vec2(blurRadius));
  ivec2 iFinishTexel = ivec2(finishTexel);
  vec2 finishWeights = fract(finishTexel);

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

  float blurSize = blurRadius * mapSize;
  return opaqueFactor / (4 * blurSize * blurSize);
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