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

layout(set = volatileSet, binding = lightDataBinding) uniform LightData
{
  vec3 illuminance;
  vec3 lightInverseDirection;
  float distance;
  float radius;
  mat4 clipToView;
  mat4 viewToLocal;
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
  float toLightDistance = -localCoord.z;
  if(toLightDistance < 0 || toLightDistance > lightData.distance) discard;
  if(length(localCoord.xy) > lightData.radius) discard;

  vec4 albedo = texelFetch(albedoMap, fragCoord, 0);
  vec3 normal = normalize(texelFetch(normalMap, fragCoord, 0).xyz);
  vec3 specular = texelFetch(specularMap, fragCoord, 0).xyz;
  float roughness = max(specular.x, .001f);
  float specularStrength = specular.y;
  float metallic = specular.z;

  vec3 luminance = getLuminance(albedo.rgb,
                                roughness,
                                specularStrength,
                                metallic,
                                normal,
                                normalize(-viewCoord.xyz),
                                lightData.lightInverseDirection,
                                lightData.illuminance);

  #ifdef SHADOW_MAP_ENABLED
    vec2 shadowCoords = vec2(localCoord.x, -localCoord.y);
    shadowCoords /= 2.f * lightData.radius;
    shadowCoords += vec2(.5f, .5f);

    int layer = getLayer(shadowCoords);

    float lightDotNorm = max(dot(lightData.lightInverseDirection, normal), 0.f);

    float shadowmapSize = 2 * lightData.radius;
    float shadowmapSlope = shadowmapSize / lightDotNorm / lightData.distance;

    luminance *= getShadowFactor( layer,
                                  shadowCoords,
                                  toLightDistance / lightData.distance,
                                  shadowmapSlope);
  #endif

  outColor = vec4(luminance, 0.f);
}