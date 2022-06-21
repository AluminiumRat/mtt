//cubeShadowmapLib.glsl

layout( set = volatileSet,
        binding = shadowMapBinding$INDEX$) uniform samplerCube shadowMap$INDEX$;

layout( set = staticSet,
        binding = blurShiftsBinding$INDEX$) uniform BlurShift$INDEX$
{
  vec2 values[1024];
} blurShift$INDEX$;

float getOpaqueShadowFactor$INDEX$( vec3 cubeCoord,
                                    float normalizedDistanceToLight,
                                    float shadowmapSlope,
                                    float blurRadius,
                                    int sampleNumber,
                                    vec3 tangent,
                                    vec3 cotangent)
{
  float mapSize = textureSize(shadowMap$INDEX$, 0).x;
  float texelSlope = shadowmapSlope / mapSize;
  float slope = blurRadius * shadowmapSlope + texelSlope;

  vec4 shadows = vec4(0);

  for(int i = 0; i < sampleNumber; i++)
  {
    vec2 planeShift = blurShift$INDEX$.values[i];
    vec3 shift = planeShift.x * tangent + planeShift.y * cotangent;
    shift *= blurRadius;

    vec4 shadowDepth = textureGather(shadowMap$INDEX$, cubeCoord + shift, 0);
    shadows += step(normalizedDistanceToLight - slope, shadowDepth);
  }

  return (shadows.x + shadows.y + shadows.z + shadows.w) / 4.f / sampleNumber;
}

float getTransparentShadowFactor$INDEX$(vec3 cubeCoord,
                                        float normalizedDistanceToLight)
{
  vec3 variadicValues = textureLod(shadowMap$INDEX$, cubeCoord, 0).gba;
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
                              int sampleNumber,
                              vec3 tangent,
                              vec3 cotangent)
{
  float opaqueFactor = getOpaqueShadowFactor$INDEX$(cubeCoord,
                                                    normalizedDistanceToLight,
                                                    shadowmapSlope,
                                                    blurRadius,
                                                    sampleNumber,
                                                    tangent,
                                                    cotangent);

  float transparentFactor = getTransparentShadowFactor$INDEX$(
                                                    cubeCoord,
                                                    normalizedDistanceToLight);
  return min(opaqueFactor, transparentFactor);
}