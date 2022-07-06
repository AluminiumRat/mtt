//cubeShadowmapLib.glsl

layout( set = volatileSet,
        binding = opaqueShadowMapBinding$INDEX$)
                                    uniform samplerCube opaqueShadowMap$INDEX$;
layout( set = volatileSet,
        binding = transparentShadowMapBinding$INDEX$)
                                uniform samplerCube transparentShadowMap$INDEX$;

layout( set = staticSet,
        binding = blurShiftsBinding$INDEX$) uniform BlurShift$INDEX$
{
  vec2 values[285];
} blurShift$INDEX$;

float getOpaqueShadowFactor$INDEX$( vec3 cubeCoord,
                                    float normalizedDistanceToLight,
                                    float shadowmapSlope,
                                    float blurRadius,
                                    int startSample,
                                    int endSample,
                                    vec3 tangent,
                                    vec3 cotangent)
{
  float mapSize = textureSize(opaqueShadowMap$INDEX$, 0).x;
  float texelSlope = shadowmapSlope / mapSize;
  float slope = blurRadius * shadowmapSlope + texelSlope;
  normalizedDistanceToLight -= slope;

  vec4 shadows = vec4(0);

  for(int i = startSample; i < endSample; i++)
  {
    vec2 planeShift = blurShift$INDEX$.values[i];
    vec3 shift = planeShift.x * tangent + planeShift.y * cotangent;
    shift *= blurRadius;

    vec4 shadowDepth = textureGather( opaqueShadowMap$INDEX$,
                                      cubeCoord + shift,
                                      0);
    shadows += step(normalizedDistanceToLight, shadowDepth);
  }

  return (shadows.x + shadows.y + shadows.z + shadows.w) / 4.f /
                                                      (endSample - startSample);
}

float getTransparentShadowFactor$INDEX$(vec3 cubeCoord,
                                        float normalizedDistanceToLight)
{
  vec3 variadicValues = textureLod( transparentShadowMap$INDEX$,
                                    cubeCoord,
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

float getShadowFactor$INDEX$( vec3 cubeCoord,
                              float normalizedDistanceToLight,
                              float shadowmapSlope,
                              float blurRadius,
                              int startSample,
                              int endSample,
                              vec3 tangent,
                              vec3 cotangent)
{
  float opaqueFactor = getOpaqueShadowFactor$INDEX$(cubeCoord,
                                                    normalizedDistanceToLight,
                                                    shadowmapSlope,
                                                    blurRadius,
                                                    startSample,
                                                    endSample,
                                                    tangent,
                                                    cotangent);

  float transparentFactor = getTransparentShadowFactor$INDEX$(
                                                    cubeCoord,
                                                    normalizedDistanceToLight);
  return opaqueFactor * transparentFactor;
}