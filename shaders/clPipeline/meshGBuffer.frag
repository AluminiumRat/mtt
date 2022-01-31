//meshGBuffer.frag

layout(location = 0) in vec3 inNormal;
#ifdef ENABLE_TEX_COORDS
  layout(location = 1) in vec2 inTexCoord;
#endif
#ifdef ENABLE_NORMAL_MAPPING
  layout(location = 2) in vec3 inTangent;
  layout(location = 3) in vec3 inBinormal;
#endif

#ifdef ENABLE_DIFFUSE_TEXTURE
  layout( set = staticSet,
          binding = albedoTextureBinding) uniform sampler2D albedoSampler;
#endif

#ifdef ENABLE_OPAQUE_TEXTURE
  layout( set = staticSet,
          binding = opaqueTextureBinding) uniform sampler2D opaqueSampler;
#endif

#ifdef ENABLE_SPECULAR_TEXTURE
  layout( set = staticSet,
          binding = specularTextureBinding) uniform sampler2D specularSampler;
#endif

#ifdef ENABLE_NORMAL_MAPPING
  layout( set = staticSet,
          binding = normalTextureBinding) uniform sampler2D normalSampler;
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

layout(location = 0) out vec4 outAlbedo;
layout(location = 1) out vec4 outSpecular;
layout(location = 2) out vec4 outNormal;

void main()
{
  #ifdef MATERIAL_ENABLED
    outAlbedo = vec4(material.albedo, 1.f);
  #else
    outAlbedo = vec4(1.f);
  #endif

  #ifdef ENABLE_DIFFUSE_TEXTURE
    outAlbedo.rgb *= texture(albedoSampler, inTexCoord).rgb;
  #endif

  #ifdef MATERIAL_ENABLED
    outSpecular = vec4( material.roughness,
                        material.specularStrength,
                        material.metallic,
                        0);
  #else
    outSpecular = vec4(1.f, 0.f, 0.f, 1.f);
  #endif

  #ifdef ENABLE_SPECULAR_TEXTURE
    outSpecular.y *= texture(specularSampler, inTexCoord).r;
  #endif

  #ifdef ENABLE_NORMAL_MAPPING
    vec3 localNorm = texture(normalSampler, inTexCoord).rgb;
    localNorm = (localNorm - vec3(.5f)) * 2.f;
    vec3 viewNormal = normalize(inTangent) * localNorm.x +
                        normalize(inBinormal) * localNorm.y +
                        normalize(inNormal) * localNorm.z;
    outNormal = vec4(normalize(viewNormal), 0);
  #else
    outNormal = vec4(normalize(inNormal), 0);
  #endif
}