//directLightModificator.glsl

layout( set = volatileSet,
        binding = lightDataBinding$INDEX$) uniform LightData$INDEX$
{
  vec3 illuminance;
  vec3 lightInverseDirection;
  float distance;
  float radius;
  mat4 clipToView;
  mat4 viewToLocal;
} lightData$INDEX$;

void $APPLY_FUNCTION$()
{
  vec4 localCoord = lightData$INDEX$.viewToLocal * vec4(viewCoord, 1.f);
  float toLightDistance = -localCoord.z;
  if(toLightDistance < 0 || toLightDistance > lightData$INDEX$.distance) return;
  if(length(localCoord.xy) > lightData$INDEX$.radius) return;

  #if $LAMBERT_SPECULAR_LUMINANCE_MODEL$
    vec3 lightInverseDirection = lightData$INDEX$.lightInverseDirection;
    float lightDotNorm = max(dot(lightInverseDirection, normal), 0.f);
  #else
    float lightDotNorm = 1.f;
  #endif

  vec3 illuminance = lightData$INDEX$.illuminance;
  #if $SHADOW_MAP_ENABLED$
    vec2 shadowCoords = vec2(localCoord.x, -localCoord.y);
    shadowCoords /= 2.f * lightData$INDEX$.radius;
    shadowCoords += vec2(.5f, .5f);

    float shadowmapSize = 2 * lightData$INDEX$.radius;
    float shadowmapSlope =
                      shadowmapSize / lightDotNorm / lightData$INDEX$.distance;

    illuminance *= getShadowFactor$INDEX$(
                                    shadowCoords,
                                    toLightDistance / lightData$INDEX$.distance,
                                    shadowmapSlope);
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