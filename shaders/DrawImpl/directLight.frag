//directLight.frag

layout(location = 0) in noperspective vec2 inClipCoord;

layout (set = volatileSet, binding = depthMapBinding) uniform sampler2D depthMap;
layout (set = volatileSet, binding = normalMapBinding) uniform sampler2D normalMap;
layout (set = volatileSet, binding = albedoMapBinding) uniform sampler2D albedoMap;
layout (set = volatileSet, binding = specularMapBinding) uniform sampler2D specularMap;

layout(binding = lightDataBinding, set = volatileSet) uniform LightData
{
  vec3 illuminance;
  vec3 lightInverseDirection;
  float distance;
  float radius;
  mat4 clipToView;
  mat4 viewToLocal;
} lightData;

#ifdef SHADOW_MAP_ENABLED
  layout(binding = shadowMapBinding, set = volatileSet)
                              uniform sampler2D shadowMap[SHADOW_CASCADE_SIZE];

  layout(binding = shadowCoordsCorrectionBinding, set = volatileSet)
                                                  uniform ShadowCoordsCorrection
  {
    vec4 values[SHADOW_CASCADE_SIZE]; // x is multiplicator
                                      // yz if uv shift
                                      // w is dead zone near the border of map
  } shadowCoordsCorrection;
#endif

layout(location = 0) out vec4 outColor;

#ifdef SHADOW_MAP_ENABLED
  int getLayer(vec2 shadowCoords)
  {
    int result = 0;
    for(int layer = 1; layer < SHADOW_CASCADE_SIZE; layer++)
    {
      vec4 coordsCorrection = shadowCoordsCorrection.values[layer];
      vec2 correctedCoords = shadowCoords * coordsCorrection.x +
                                                            coordsCorrection.yz;
      vec2 sign = step(vec2(coordsCorrection.w),correctedCoords);
      sign *= step(correctedCoords, vec2(1.f - coordsCorrection.w));

      if(sign.x * sign.y != 0) result = layer;
    }
    return result;
  }

  float getShadowFactor(int layer,
                        vec2 shadowCoords,
                        float normalizedDistanceToLight)
  {
    vec4 coordsCorrection = shadowCoordsCorrection.values[layer];
    shadowCoords = shadowCoords * coordsCorrection.x + coordsCorrection.yz;
    float shadowDepth = texture(shadowMap[layer], shadowCoords).x;
    return step(normalizedDistanceToLight, shadowDepth);
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

  vec4 localCoord = lightData.viewToLocal * viewCoord;
  float toLightDistance = -localCoord.z;
  if(toLightDistance < 0 || toLightDistance > lightData.distance) discard;
  if(length(localCoord.xy) > lightData.radius) discard;

  vec4 albedo = texelFetch(albedoMap, fragCoord, 0);
  vec3 normal = normalize(texelFetch(normalMap, fragCoord, 0).rgb);
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
    float angleCorrection = 2 *
                    (lightData.radius / textureSize(shadowMap[layer], 0).x) /
                                                                  lightDotNorm;
    angleCorrection /= shadowCoordsCorrection.values[layer].x;

    float correctedDistanceToLight = toLightDistance - angleCorrection;
    correctedDistanceToLight /= lightData.distance;

    luminance *= getShadowFactor(layer, shadowCoords, correctedDistanceToLight);
  #endif

  #ifdef ENABLE_DEPTH_PEELING
    outColor = vec4(luminance * (1.f - albedo.a), albedo.a);
  #else
    outColor = vec4(luminance, 0.f);
  #endif
}