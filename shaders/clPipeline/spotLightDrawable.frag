//directLightDrawable.frag

layout(location = 0) in noperspective vec2 inClipCoord;

layout (set = volatileSet,
        binding = depthMapBinding) uniform sampler2D depthMap;
layout (set = volatileSet,
        binding = normalMapBinding) uniform sampler2D normalMap;
layout (set = volatileSet,
        binding = albedoMapBinding) uniform sampler2D albedoMap;
layout (set = volatileSet,
        binding = specularMapBinding) uniform sampler2D specularMap;

#ifdef FILTER_SAMPLER_ENABLED
  layout (set = staticSet,
          binding = filterSamplerBinding) uniform sampler2D filterSampler;
#endif

layout(set = volatileSet, binding = lightDataBinding) uniform LightData
{
  vec3 illuminance;
  vec3 lightPosition;
  float distance;
  float halfangleTan;
  mat4 clipToView;
  mat4 viewToLocal;
  float blurRadius;
} lightData;

layout(location = 0) out vec4 outColor;

void main()
{
  ivec2 fragCoord = ivec2(gl_FragCoord.xy);
  float depth = texelFetch(depthMap, fragCoord, 0).r;
  if(depth == 0) discard;
  vec4 clipCoord = vec4(inClipCoord, depth, 1);
  vec4 viewCoord = lightData.clipToView * clipCoord;
  viewCoord /= viewCoord.w;

  vec4 localCoord = lightData.viewToLocal * viewCoord;
  float toLightDistance = length(localCoord.xyz);
  if(localCoord.z > 0.f || toLightDistance > lightData.distance) discard;

  float areaHalfsize = -localCoord.z * lightData.halfangleTan;
  if(abs(localCoord.x) > areaHalfsize || abs(localCoord.y) > areaHalfsize)
  {
    discard;
  }

  vec4 albedo = texelFetch(albedoMap, fragCoord, 0);
  vec3 normal = normalize(texelFetch(normalMap, fragCoord, 0).xyz);
  vec3 specular = texelFetch(specularMap, fragCoord, 0).xyz;
  float roughness = max(specular.x, .001f);
  float specularStrength = specular.y;
  float metallic = specular.z;

  vec3 lightInverseDirection =
                            normalize(lightData.lightPosition - viewCoord.xyz);

  vec3 luminance = getLuminance(albedo.rgb,
                                roughness,
                                specularStrength,
                                metallic,
                                normal,
                                normalize(-viewCoord.xyz),
                                lightInverseDirection,
                                lightData.illuminance);

  vec2 textureCoord = vec2(localCoord.x, -localCoord.y);
  textureCoord /= 2.f * areaHalfsize;
  textureCoord += vec2(.5f, .5f);

  #ifdef FILTER_SAMPLER_ENABLED
    luminance *= texture(filterSampler, textureCoord).rgb;
  #endif

  luminance *= fade(toLightDistance, lightData.distance);

  #ifdef SHADOW_MAP_ENABLED
    float lightDotNorm = abs(dot(lightInverseDirection, normal));
    float slopeFactor = min(1.f / lightDotNorm, 20.f);
    float shadowmapSlope =
                          2.f * areaHalfsize * slopeFactor / lightData.distance;

    luminance *= getShadowFactor( textureCoord,
                                  -localCoord.z / lightData.distance,
                                  shadowmapSlope,
                                  lightData.blurRadius);
  #endif

  outColor = vec4(luminance, 0.f);
}