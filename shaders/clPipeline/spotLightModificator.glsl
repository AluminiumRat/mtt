//spotLightModificator.glsl

#if $FILTER_SAMPLER_ENABLED$
  layout (set = staticSet,
          binding = filterSamplerBinding$INDEX$)
                                        uniform sampler2D filterSampler$INDEX$;
#endif

layout( set = volatileSet,
        binding = lightDataBinding$INDEX$) uniform LightData$INDEX$
{
  vec3 illuminance;
  vec3 lightPosition;
  float distance;
  float halfangleTan;
  mat4 clipToView;
  mat4 viewToLocal;
  float blurRadius;
} lightData$INDEX$;

void $APPLY_FUNCTION$()
{
  vec4 localCoord = lightData$INDEX$.viewToLocal * vec4(viewCoord, 1.f);

  float toLightDistance = length(localCoord.xyz);
  if(localCoord.z > 0.f || toLightDistance > lightData$INDEX$.distance) return;

  float areaRadius = -localCoord.z * lightData$INDEX$.halfangleTan;
  if(length(localCoord.xy) > areaRadius) return;

  #if $LAMBERT_SPECULAR_LUMINANCE_MODEL$
    vec3 lightInverseDirection = normalize(
                                    lightData$INDEX$.lightPosition - viewCoord);
    float lightDotNorm = max(dot(lightInverseDirection, normal), 0.f);
  #else
    float lightDotNorm = 1.f;
  #endif

  vec3 illuminance = lightData$INDEX$.illuminance;
  vec2 textureCoord = vec2(localCoord.x, -localCoord.y);
  textureCoord /= 2.f * areaRadius;
  textureCoord += vec2(.5f, .5f);

  #if $FILTER_SAMPLER_ENABLED$
    illuminance *= texture(filterSampler$INDEX$, textureCoord).rgb;
  #endif

  illuminance *= fade$INDEX$(toLightDistance, lightData$INDEX$.distance);

  #if $SHADOW_MAP_ENABLED$
    float slopeFactor = min(1.f / lightDotNorm, 10.f);
    float shadowmapSlope =
                    2.f * areaRadius * slopeFactor / lightData$INDEX$.distance;

    illuminance *= getShadowFactor$INDEX$(
                                      textureCoord,
                                      -localCoord.z / lightData$INDEX$.distance,
                                      shadowmapSlope,
                                      lightData$INDEX$.blurRadius);
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