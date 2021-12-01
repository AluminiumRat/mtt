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
                                      // w is blur radius in texture coordinates
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
      float deadZone = coordsCorrection.w +
                                      .5f / textureSize(shadowMap[layer], 0).x;
      vec2 sign = step(vec2(deadZone),correctedCoords);
      sign *= step(correctedCoords, vec2(1.f - deadZone));

      if(sign.x * sign.y != 0) result = layer;
    }
    return result;
  }

  float rand(vec2 co)
  {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
  }

  float getShadowFactor(int layer,
                        vec2 shadowCoords,
                        float normalizedDistanceToLight,
                        float slope)
  {
    vec4 coordsCorrection = shadowCoordsCorrection.values[layer];
    vec2 centerCoords = shadowCoords * coordsCorrection.x + coordsCorrection.yz;

    float mapSize = textureSize(shadowMap[layer], 0).x;
    vec2 centerTexelCoords = mapSize * centerCoords;
    vec2 blurSize =
                  vec2(shadowCoordsCorrection.values[layer].w * mapSize + .5f);

    vec2 startTexel = centerTexelCoords - blurSize;
    ivec2 iStartTexel = ivec2(startTexel);
    vec2 startWeights = vec2(1.f) - fract(startTexel);
    startTexel = floor(startTexel) + vec2(.5f);

    vec2 finishTexel = centerTexelCoords + blurSize;
    ivec2 iFinishTexel = ivec2(finishTexel);
    vec2 finishWeights = fract(finishTexel);
    finishTexel = floor(finishTexel) + vec2(.5f);

    float shadowFactor = 0;
    float weight = 0;

    ivec2 iTexelCoord = iStartTexel;
    vec2 fTexelCoord = startTexel;
    float xWeight = startWeights.x;
    for(; iTexelCoord.x <= iFinishTexel.x;)
    {
      iTexelCoord.y = iStartTexel.y;
      fTexelCoord.y = startTexel.y;
      float yWeight = startWeights.y;
      for(; iTexelCoord.y <= iFinishTexel.y;)
      {
        float shadowDepth = texelFetch( shadowMap[layer],
                                        iTexelCoord,
                                        0).x;
        float currentWeight = xWeight * yWeight;
        float texelDistance = length(fTexelCoord - centerTexelCoords);
        float slopeCorrection = texelDistance * slope;
        shadowFactor += currentWeight *
                              step( normalizedDistanceToLight - slopeCorrection,
                                    shadowDepth);
        weight += currentWeight;

        fTexelCoord.y++;
        iTexelCoord.y++;
        yWeight = iTexelCoord.y == iFinishTexel.y ? finishWeights.y : 1.f;
      }

      iTexelCoord.x++;
      fTexelCoord.x++;
      xWeight = iTexelCoord.x == iFinishTexel.x ? finishWeights.x : 1.f;
    }

    return shadowFactor / weight;
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

    float texelSize = 2 * lightData.radius / textureSize(shadowMap[layer], 0).x;
    texelSize /= shadowCoordsCorrection.values[layer].x;
    
    float lightDotNorm = max(dot(lightData.lightInverseDirection, normal), 0.f);
    float slope = texelSize / lightDotNorm;
    slope /= lightData.distance;

    luminance *= getShadowFactor( layer,
                                  shadowCoords,
                                  toLightDistance / lightData.distance,
                                  slope);
  #endif

  #ifdef ENABLE_DEPTH_PEELING
    outColor = vec4(luminance * (1.f - albedo.a), albedo.a);
  #else
    outColor = vec4(luminance, 0.f);
  #endif
}