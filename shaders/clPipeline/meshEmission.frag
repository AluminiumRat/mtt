//meshEmission.frag

#ifdef ENABLE_TEX_COORDS
  layout(location = 1) in vec2 inTexCoord;
#endif
#ifdef ENABLE_REFLECTION
  layout(location = 4) in vec3 inReflectDirection;
#endif

#ifdef ENABLE_EMISSION_TEXTURE
  layout( set = staticSet,
          binding = emissionTextureBinding) uniform sampler2D emissionSampler;
#endif
#ifdef ENABLE_REFLECTION
  layout( set = staticSet,
          binding = reflectionTextureBinding) uniform sampler2D reflectionSampler;
#endif

layout(location = 0) out vec4 outColor;

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

void main()
{
  outColor = vec4(material.emission, 0);
  #ifdef ENABLE_EMISSION_TEXTURE
    outColor.rgb *= texture(emissionSampler, inTexCoord).rgb;
  #endif

  #ifdef ENABLE_REFLECTION
    vec3 reflectDirection = normalize(inReflectDirection);

    float lon = atan(reflectDirection.y, reflectDirection.x);
    float lat = asin(reflectDirection.z);

    vec2 reflectTexCoords = vec2(mod(-lon / 2.f / M_PI, 1.f), 0.5f - lat / M_PI);
    vec3 reflectLuminance = texture(reflectionSampler, reflectTexCoords).rgb;
    reflectLuminance *= material.reflectionFactor;

    outColor.rgb += reflectLuminance;
  #endif
}