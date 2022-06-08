//cascadeShadowmapLib.glsl

layout( set = volatileSet,
        binding = shadowMapBinding$INDEX$)
                      uniform sampler2D shadowMap$INDEX$[$SHADOW_CASCADE_SIZE$];

layout( set = volatileSet,
        binding = shadowCoordsCorrectionBinding$INDEX$)
                                          uniform ShadowCoordsCorrection$INDEX$
{
  vec4 values[$SHADOW_CASCADE_SIZE$]; // x is multiplicator
                                      // yz if uv shift
                                      // w is blur radius in texture
                                      // coordinates
} shadowCoordsCorrection$INDEX$;

int getLayer$INDEX$(vec2 shadowCoords)
{
  int result = 0;
  for(int layer = 1; layer < $SHADOW_CASCADE_SIZE$; layer++)
  {
    vec4 coordsCorrection = shadowCoordsCorrection$INDEX$.values[layer];
    vec2 correctedCoords = shadowCoords * coordsCorrection.x +
                                                            coordsCorrection.yz;
    float deadZone = coordsCorrection.w +
                                .5f / textureSize(shadowMap$INDEX$[layer], 0).x;
    vec2 sign = step(vec2(deadZone),correctedCoords);
    sign *= step(correctedCoords, vec2(1.f - deadZone));

    if(sign.x * sign.y != 0) result = layer;
  }
  return result;
}

float getOpaqueShadowFactor$INDEX$( int layer,
                                    vec2 centerCoords,
                                    float normalizedDistanceToLight,
                                    float shadowmapSlope)
{
  float mapSize = textureSize(shadowMap$INDEX$[layer], 0).x;
  vec2 centerTexelCoords = mapSize * centerCoords;
  vec2 blurSize =
            vec2(shadowCoordsCorrection$INDEX$.values[layer].w * mapSize + .5f);

  vec2 startTexel = centerTexelCoords - blurSize;
  ivec2 iStartTexel = ivec2(startTexel);
  vec2 startWeights = vec2(1.f) - fract(startTexel);
  startTexel = floor(startTexel) + vec2(.5f);

  vec2 finishTexel = centerTexelCoords + blurSize;
  ivec2 iFinishTexel = ivec2(finishTexel);
  vec2 finishWeights = fract(finishTexel);
  finishTexel = floor(finishTexel) + vec2(.5f);

  float texelSlope = shadowmapSlope /
                      shadowCoordsCorrection$INDEX$.values[layer].x /
                      mapSize;
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
      float shadowDepth = texelFetch( shadowMap$INDEX$[layer],
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

float getTransparentShadowFactor$INDEX$(int layer,
                                        vec2 centerCoords,
                                        float normalizedDistanceToLight)
{
  vec3 variadicValues = textureLod( shadowMap$INDEX$[layer],
                                    centerCoords,
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

float getShadowFactor$INDEX$( int layer,
                              vec2 shadowCoords,
                              float normalizedDistanceToLight,
                              float shadowmapSlope)
{
  vec4 coordsCorrection = shadowCoordsCorrection$INDEX$.values[layer];
  vec2 centerCoords = shadowCoords * coordsCorrection.x + coordsCorrection.yz;

  float opaqueFactor = getOpaqueShadowFactor$INDEX$(layer,
                                                    centerCoords,
                                                    normalizedDistanceToLight,
                                                    shadowmapSlope);

  float transparentFactor = getTransparentShadowFactor$INDEX$(
                                                  layer,
                                                  centerCoords,
                                                  normalizedDistanceToLight);
  return min(opaqueFactor, transparentFactor);
}