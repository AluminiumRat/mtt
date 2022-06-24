//pointLightModificator.glsl

#if $FILTER_SAMPLER_ENABLED$
  layout (set = staticSet,
          binding = filterSamplerBinding$INDEX$)
                                      uniform samplerCube filterSampler$INDEX$;
#endif

layout( set = volatileSet,
        binding = lightDataBinding$INDEX$) uniform LightData$INDEX$
{
  vec3 illuminance;
  vec3 lightPosition;
  float distance;
  mat4 clipToView;
  mat4 viewToLocal;
  float blurRadius;
  int startSample;
  int endSample;
} lightData$INDEX$;

void $APPLY_FUNCTION$()
{
  vec4 localCoord = lightData$INDEX$.viewToLocal * vec4(viewCoord, 1.f);

  float toLightDistance = length(localCoord.xyz);
  if(toLightDistance > lightData$INDEX$.distance) return;

  #if $LAMBERT_SPECULAR_LUMINANCE_MODEL$
    vec3 lightInverseDirection = normalize(
                                    lightData$INDEX$.lightPosition - viewCoord);
    float lightDotNorm = max(dot(lightInverseDirection, normal), 0.f);
  #else
    float lightDotNorm = 1.f;
  #endif

  vec3 illuminance = lightData$INDEX$.illuminance;
  vec3 cubeCoord = normalize(localCoord.xyz);
  cubeCoord.z = -cubeCoord.z;

  #if $FILTER_SAMPLER_ENABLED$
    illuminance *= texture(filterSampler$INDEX$, cubeCoord).rgb;
  #endif

  #if $SHADOW_MAP_ENABLED$
    vec3 tangent = normalize(cross(cubeCoord, cubeCoord.zxy));
    vec3 cotangent = normalize(cross(cubeCoord, tangent));

    vec3 orthoDistances = abs(localCoord.xyz);
    float maxDistance = max(orthoDistances.x,
                            max(orthoDistances.y, orthoDistances.z));
    float shadowmapSize = 2.f * maxDistance;
    float slopeFactor = min(1.f / lightDotNorm, 10.f);
    float shadowmapSlope = shadowmapSize * slopeFactor /
                                                      lightData$INDEX$.distance;

    illuminance *= getShadowFactor$INDEX$(
                                        cubeCoord,
                                        maxDistance / lightData$INDEX$.distance,
                                        shadowmapSlope,
                                        lightData$INDEX$.blurRadius,
                                        lightData$INDEX$.startSample,
                                        lightData$INDEX$.endSample,
                                        tangent,
                                        cotangent);
  #endif

  #if $LAMBERT_SPECULAR_LUMINANCE_MODEL$
    vec3 lambertLuminance = illuminance * lightDotNorm / M_PI;

    vec3 halfDir = normalize(toView + lightInverseDirection);
    float normDotHalf = max(dot(normal, halfDir), 0.f);

    float distributionFactor = GGXDistribution(normDotHalf, roughness);
    float geometryFactor = neumannGeometryFactor( lightDotNorm,
                                                  normDotHalf,
                                                  viewDotNorm);
    float specularFactor =  distributionFactor * geometryFactor /
                                                            (4.f * viewDotNorm);
    vec3 specularLuminance = illuminance * specularFactor;

    applyLight(lambertLuminance, specularLuminance);
  #else
    applyLight(illuminance);
  #endif
}