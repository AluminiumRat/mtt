//ambientLightModificator.glsl

#if $DIFFUSE_LUMINANCE_MAP_ENABLED$
  layout( set = staticSet,
          binding = diffuseLuminanceMapBinding$INDEX$)
                              uniform samplerCube diffuseLuminanceMap$INDEX$;
#endif

#if $AMBIENT_MAP_ENABLED$
  layout( set = staticSet,
          binding = ambientMapBinding$INDEX$)
                                        uniform samplerCube ambientMap$INDEX$;
#endif

layout( binding = lightDataBinding$INDEX$,
        set = volatileSet) uniform LightData$INDEX$
{
  vec3 illuminance;
  float distance;
  float saturationDistance;
  bool infArea;
  vec3 position;
  mat4 clipToView;
  mat4 viewToLocal;
} lightData$INDEX$;

#if $AMBIENT_MAP_ENABLED$
  vec3 readAmbientMap$INDEX$(vec3 direction, float roughnessValue)
  {
    roughnessValue = max(roughnessValue, .001f);

    int sideSize = textureSize(ambientMap$INDEX$, 0).x;
    float pixelAngle = M_PI / 2.f / sideSize;

    float roughness2 = roughness * roughness;
    float roughness4 = roughness2 * roughness2;
    float blurCos = (1.83f * roughness4 - 1.f) / (roughness4 - 1.0001f);
    float blurAngle = M_PI / 2.f * sqrt(1.f - blurCos);

    float blurRate = 2.f * blurAngle / pixelAngle;
    float lod = max(0, log2(blurRate));
    lod = max(textureQueryLod(ambientMap$INDEX$, direction).x, lod);

    return textureLod(ambientMap$INDEX$, direction, lod).rgb;
  }
#endif

float getWeight$INDEX$()
{
  #if $INFINITY_AREA$
    return 0.f;
  #else
    vec4 localCoord = lightData$INDEX$.viewToLocal * vec4(viewCoord, 1.f);
    float toLightDistance = length(localCoord.xyz);
    if(toLightDistance > lightData$INDEX$.distance) return 0.f;

    return 1.f - smoothstep(lightData$INDEX$.saturationDistance,
                            lightData$INDEX$.distance,
                            toLightDistance);
  #endif
}

void $WEIGHT_FUNCTION$()
{
  overallAmbientWeight += getWeight$INDEX$();
}

void $APPLY_FUNCTION$()
{
  #if $INFINITY_AREA$
    float weight = max(1.f - overallAmbientWeight, 0.f);
  #else
    float weight = getWeight$INDEX$() / max(overallAmbientWeight, 1.f);
  #endif

  vec3 lambertLuminance = lightData$INDEX$.illuminance * weight / M_PI;
  #if $DIFFUSE_LUMINANCE_MAP_ENABLED$
    vec3 probeDirection =
                      normalize(mat3(lightData$INDEX$.viewToLocal) * normal);
    probeDirection.z = -probeDirection.z;
    vec3 diffuseLuminanceProbe =
                        texture(diffuseLuminanceMap$INDEX$, probeDirection).rgb;
    lambertLuminance *= diffuseLuminanceProbe;
  #endif

  #if $AMBIENT_MAP_ENABLED$
    vec3 reflectionDirView = reflect(viewCoord, normal);
    vec3 reflectionDir =
            normalize(mat3(lightData$INDEX$.viewToLocal) * reflectionDirView);
    reflectionDir.z = -reflectionDir.z;
    vec3 reflectionLuminance = readAmbientMap$INDEX$(reflectionDir, roughness);
    reflectionLuminance *= lightData$INDEX$.illuminance * weight / M_PI;
  #else
    vec3 reflectionLuminance = lightData$INDEX$.illuminance * weight / M_PI;
  #endif

  applyLight(lambertLuminance, reflectionLuminance);
}