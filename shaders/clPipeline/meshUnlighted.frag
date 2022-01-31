//meshUnlighted.frag

#ifdef ENABLE_TEX_COORDS
  layout(location = 1) in vec2 inTexCoord;
#endif
#ifdef ENABLE_DIFFUSE_TEXTURE
  layout(binding = albedoTextureBinding, set = staticSet)
                                                  uniform sampler2D albedoSampler;
#endif

#ifdef ENABLE_OPAQUE_TEXTURE
  layout( set = staticSet,
          binding = opaqueTextureBinding) uniform sampler2D opaqueSampler;
#endif

#ifdef MATERIAL_ENABLED
  layout(binding = surfaceMaterialUniformBinding, set = staticSet)
                                                            uniform MaterialBuffer
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

void main()
{
  #ifdef MATERIAL_ENABLED
    outColor = vec4(material.albedo, material.opaque);
  #else
    outColor = vec4(1, 1, 1, 1);
  #endif

  #ifdef ENABLE_OPAQUE_TEXTURE
    outColor.a *= texture(opaqueSampler, inTexCoord).r;
  #endif

  #ifdef ENABLE_DIFFUSE_TEXTURE
    #ifdef USE_ALPHA_FROM_DIFFUSE_TEXTURE
      outColor *= texture(albedoSampler, inTexCoord);
    #else
      outColor.rgb *= texture(albedoSampler, inTexCoord).rgb;
    #endif
  #endif

  #ifdef ENABLE_ALPHA_TEST
    if(outColor.a == 0) discard;
  #endif

  outColor.rgb *= outColor.a;
  outColor.a = 1.f - outColor.a;
}