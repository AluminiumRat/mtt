//meshUnlighted.frag

#ifdef ENABLE_TEX_COORDS
  layout(location = 1) in vec2 inTexCoord;
#endif
#ifdef ENABLE_REFLECTION
  layout(location = 4) in vec3 inReflectDirection;
#endif

#ifdef ENABLE_DIFFUSE_TEXTURE
  layout( set = staticSet,
          binding = albedoTextureBinding) uniform sampler2D albedoSampler;
#endif

#ifdef ENABLE_OPAQUE_TEXTURE
  layout( set = staticSet,
          binding = opaqueTextureBinding) uniform sampler2D opaqueSampler;
#endif

#ifdef ENABLE_EMISSION_TEXTURE
  layout( set = staticSet,
          binding = emissionTextureBinding) uniform sampler2D emissionSampler;
#endif
#ifdef ENABLE_REFLECTION
  layout( set = staticSet,
          binding = reflectionTextureBinding) uniform sampler2D reflectionSampler;
#endif

#ifdef MATERIAL_ENABLED
  layout( set = staticSet,
          binding = surfaceMaterialUniformBinding) uniform MaterialBuffer
  {
    vec3 albedo;
    float roughness;
    float specularStrength;
    float metallic;
    float reflectionFactor;
    vec3 emission;
    float opaque;
  } material;
#endif

layout(location = 0) out vec4 outColor;

vec4 getSurfaceColor()
{
  #ifdef MATERIAL_ENABLED
    vec4 surfaceColor = vec4(material.albedo, material.opaque);
  #else
    vec4 surfaceColor = vec4(1, 1, 1, 1);
  #endif

  #ifdef ENABLE_OPAQUE_TEXTURE
    surfaceColor.a *= texture(opaqueSampler, inTexCoord).r;
  #endif

  #ifdef ENABLE_DIFFUSE_TEXTURE
    #ifdef USE_ALPHA_FROM_DIFFUSE_TEXTURE
      surfaceColor *= texture(albedoSampler, inTexCoord);
    #else
      surfaceColor.rgb *= texture(albedoSampler, inTexCoord).rgb;
    #endif
  #endif

  return surfaceColor;
}

vec3 getEmission()
{
  #ifdef MATERIAL_ENABLED
    vec3 emission = material.emission;
  #else
    vec3 emission = vec3(0);
  #endif

  #ifdef ENABLE_EMISSION_TEXTURE
    emission *= texture(emissionSampler, inTexCoord).rgb;
  #endif

  #ifdef ENABLE_REFLECTION
    vec3 reflectDirection = normalize(inReflectDirection);

    float lon = atan(reflectDirection.y, reflectDirection.x);
    float lat = asin(reflectDirection.z);

    vec2 reflectTexCoords = vec2(mod(-lon / 2.f / M_PI, 1.f), 0.5f - lat / M_PI);
    vec3 reflectLuminance = texture(reflectionSampler, reflectTexCoords).rgb;
    #ifdef MATERIAL_ENABLED
      reflectLuminance *= material.reflectionFactor;
    #endif

    emission += reflectLuminance;
  #endif

  return emission;
}

void main()
{
  outColor = getSurfaceColor();
  outColor.rgb *= outColor.a;
  outColor.rgb += getEmission();

  #ifdef ENABLE_ALPHA_TEST
    if(outColor == vec4(0)) discard;
  #endif

  outColor.a = 1.f - outColor.a;
}