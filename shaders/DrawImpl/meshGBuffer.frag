//meshGBuffer.frag

#ifdef ENABLE_DEPTH_PEELING
  layout (input_attachment_index = 0, set = volatileSet, binding = frontBufferBinding) uniform subpassInput frontBuffer;
  layout (input_attachment_index = 1, set = volatileSet, binding = backBufferBinding) uniform subpassInput backBuffer;
#endif

layout(location = 0) in vec3 inNormal;
#ifdef ENABLE_TEX_COORDS
  layout(location = 1) in vec2 inTexCoord;
#endif
#ifdef ENABLE_NORMAL_MAPPING
  layout(location = 2) in vec3 inTangent;
  layout(location = 3) in vec3 inBinormal;
#endif

#ifdef ENABLE_DIFFUSE_TEXTURE
  layout(binding = albedoTextureBinding, set = staticSet)
                                                uniform sampler2D albedoSampler;
#endif

#ifdef ENABLE_OPAQUE_TEXTURE
  layout(binding = opaqueTextureBinding, set = staticSet)
                                                uniform sampler2D opaqueSampler;
#endif

#ifdef ENABLE_SPECULAR_TEXTURE
  layout(binding = specularTextureBinding, set = staticSet)
                                              uniform sampler2D specularSampler;
#endif

#ifdef ENABLE_NORMAL_MAPPING
  layout(binding = normalTextureBinding, set = staticSet)
                                                uniform sampler2D normalSampler;
#endif

layout(location = 0) out vec4 outAlbedo;
layout(location = 1) out vec4 outSpecular;
layout(location = 2) out vec4 outNormal;

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

void main()
{
  #ifdef ENABLE_DEPTH_PEELING
    float fragDepth = gl_FragCoord.z;
    if( fragDepth >= subpassLoad(frontBuffer).x ||
        fragDepth <= subpassLoad(backBuffer).x) discard;
  #endif

  outAlbedo = vec4(material.albedo, 1);
  outAlbedo.a = material.opaque;
  #ifdef ENABLE_OPAQUE_TEXTURE
    outAlbedo.a *= texture(opaqueSampler, inTexCoord).r;
  #endif
  #ifdef ENABLE_ALPHA_TEST
    if(outAlbedo.a == 0) discard;
  #endif

  #ifdef ENABLE_DIFFUSE_TEXTURE
    #ifdef USE_ALPHA_FROM_DIFFUSE_TEXTURE
      outAlbedo *= texture(albedoSampler, inTexCoord);
    #else
      outAlbedo.rgb *= texture(albedoSampler, inTexCoord).rgb;
    #endif
  #endif

  outSpecular = vec4( material.roughness,
                      material.specularStrength,
                      material.metallic,
                      0);
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

  outAlbedo.a = 1.f - outAlbedo.a;
}