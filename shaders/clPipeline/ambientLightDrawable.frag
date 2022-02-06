//ambientLightDrawable.frag

layout(location = 0) in noperspective vec2 inClipCoord;

layout (set = volatileSet,
        binding = depthMapBinding) uniform sampler2D depthMap;

#ifndef WEIGHT_RENDER
  layout (set = volatileSet,
          binding = weightMapBinding) uniform sampler2D weightMap;

  layout (set = volatileSet,
          binding = normalMapBinding) uniform sampler2D normalMap;

  layout (set = volatileSet,
          binding = albedoMapBinding) uniform sampler2D albedoMap;

  layout (set = volatileSet,
          binding = specularMapBinding) uniform sampler2D specularMap;

  #ifdef DIFFUSE_LUMINANCE_MAP_ENABLED
    layout( set = staticSet,
            binding = diffuseLuminanceMapBinding)
                                        uniform samplerCube diffuseLuminanceMap;
  #endif

  #ifdef AMBIENT_MAP_ENABLED
    layout( set = staticSet,
            binding = ambientMapBinding) uniform samplerCube ambientMap;
  #endif
#endif

layout( binding = lightDataBinding,
        set = volatileSet) uniform LightData
{
  vec3 illuminance;
  float distance;
  float saturationDistance;
  bool infArea;
  vec3 position;
  mat4 clipToView;
  mat4 viewToLocal;
} lightData;

#ifdef WEIGHT_RENDER
  layout(location = 0) out float outWeight;
#else
  layout(location = 0) out vec4 outColor;
#endif


#ifdef AMBIENT_MAP_ENABLED
  vec3 readAmbientMap(vec3 direction, float roughness)
  {
    roughness = max(roughness, .001f);

    int sideSize = textureSize(ambientMap, 0).x;
    float pixelAngle = M_PI / 2.f / sideSize;

    float roughness2 = roughness * roughness;
    float roughness4 = roughness2 * roughness2;
    float blurCos = (1.83f * roughness4 - 1.f) / (roughness4 - 1.0001f);
    float blurAngle = M_PI / 2.f * sqrt(1.f - blurCos);

    float blurRate = 2.f * blurAngle / pixelAngle;
    float lod = max(0, log2(blurRate));
    lod = max(textureQueryLod(ambientMap, direction).x, lod);

    return textureLod(ambientMap, direction, lod).rgb;
  }
#endif

void main()
{
  ivec2 fragCoord = ivec2(gl_FragCoord.xy);
  float depth = texelFetch(depthMap, fragCoord, 0).r;
  if(depth == 0) discard;
  vec4 clipCoord = vec4(inClipCoord, depth, 1);
  vec4 viewCoord = lightData.clipToView * clipCoord;
  viewCoord /= viewCoord.w;

  #ifndef INFINITY_AREA
    vec4 localCoord = lightData.viewToLocal * viewCoord;
    float toLightDistance = length(localCoord.xyz);
    if(toLightDistance > lightData.distance) discard;

    float weight = 1.f - smoothstep(lightData.saturationDistance,
                                    lightData.distance,
                                    toLightDistance);
  #else
    float weight = 1.f;
  #endif

  #ifdef WEIGHT_RENDER
    outWeight = weight;
  #else
    float overallWeight = texelFetch(weightMap, fragCoord, 0).r;

    #ifdef INFINITY_AREA
      weight = max(1.f - overallWeight, 0.f);
    #else
      weight /= max(overallWeight, 1.f);
    #endif

    vec4 albedo = texelFetch(albedoMap, fragCoord, 0);
    vec3 normal = normalize(texelFetch(normalMap, fragCoord, 0).rgb);

    vec3 diffuseLuminance = albedo.rgb * lightData.illuminance / M_PI * weight;
    #ifdef DIFFUSE_LUMINANCE_MAP_ENABLED
      vec3 probeDirection = normalize(mat3(lightData.viewToLocal) * normal);
      probeDirection.z = -probeDirection.z;
      vec3 diffuseLuminanceProbe =
                              texture(diffuseLuminanceMap, probeDirection).rgb;
      diffuseLuminance *= diffuseLuminanceProbe;
    #endif

    vec3 specular = texelFetch(specularMap, fragCoord, 0).xyz;
    float roughness = specular.x;
    float specularStrength = specular.y;
    float metallic = specular.z;

    #ifdef AMBIENT_MAP_ENABLED
      vec3 reflectionDirView = reflect(viewCoord.xyz, normal);
      vec3 reflectionDir =
                    normalize(mat3(lightData.viewToLocal) * reflectionDirView);
      reflectionDir.z = -reflectionDir.z;
      vec3 reflectionLuminance = readAmbientMap(reflectionDir, roughness);
      reflectionLuminance *= lightData.illuminance / M_PI * weight;
    #else
      vec3 reflectionLuminance = lightData.illuminance / M_PI * weight;
    #endif

    float viewDotNorm = dot(normalize(-viewCoord.xyz), normal);

    vec3 plasticLuminance =
                diffuseLuminance +
                reflectionLuminance * specularStrength *
                                                fresnelFactor(.1f, viewDotNorm);

    vec3 metallicLuminance = reflectionLuminance * albedo.rgb *
                                                fresnelFactor(.8f, viewDotNorm);

    vec3 luminance = mix(plasticLuminance, metallicLuminance, metallic);

    outColor = vec4(luminance, 0.f);
  #endif
}