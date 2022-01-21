//meshInstrument.frag

#ifdef ENABLE_TEX_COORDS
  layout(location = 1) in vec2 inTexCoord;
#endif
#ifdef ENABLE_DIFFUSE_TEXTURE
  layout(binding = albedoTextureBinding, set = staticSet)
                                                  uniform sampler2D albedoSampler;
#endif

layout(location = 0) out vec4 outColor;

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

void main()
{
  #ifdef MATERIAL_ENABLED
    outColor = vec4(material.albedo * material.opaque, material.opaque);
  #else
    outColor = vec4(1, 1, 1, 1);
  #endif

  #ifdef ENABLE_DIFFUSE_TEXTURE
    outColor *= texture(albedoSampler, inTexCoord);
  #endif

  outColor.a = 1.f - outColor.a;
}