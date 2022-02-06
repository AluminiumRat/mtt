//meshFL.frag

layout(location = 0) in vec3 inNormal;

#ifdef ENABLE_TEX_COORDS
  layout(location = 1) in vec2 inTexCoord;
#endif

#ifdef ENABLE_NORMAL_MAPPING
  layout(location = 2) in vec3 inTangent;
  layout(location = 3) in vec3 inBinormal;
#endif

#ifdef ENABLE_REFLECTION
  layout(location = 4) in vec3 inReflectDirection;
#endif

#ifndef ENABLE_VIEW_COORDS_IN_FRAGMENT_SHADER
  You should set VIEW_COORDS_IN_FRAGMENT_SHADER_FEATURE
#endif
layout(location = 5) in vec3 viewCoord;

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

vec3 getNormal()
{
  #ifdef ENABLE_NORMAL_MAPPING
    vec3 localNorm = texture(normalSampler, inTexCoord).rgb;
    localNorm = (localNorm - vec3(.5f)) * 2.f;
    vec3 viewNormal = normalize(inTangent) * localNorm.x +
                        normalize(inBinormal) * localNorm.y +
                        normalize(inNormal) * localNorm.z;
    return normalize(viewNormal);
  #else
    return normalize(inNormal);
  #endif
}

// x is roughness
// y is specular strength
// z is metallic
vec3 getSpecular()
{
  #ifdef MATERIAL_ENABLED
    vec3 specular = vec3( material.roughness,
                          material.specularStrength,
                          material.metallic);
  #else
    vec3 specular = vec3(1.f, 0.f, 0.f);
  #endif

  #ifdef ENABLE_SPECULAR_TEXTURE
    specular.y *= texture(specularSampler, inTexCoord).r;
  #endif

  return specular;
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

MODIFICATOR_DECLARATION

float overallAmbientWeight = 0;
vec4 surfaceColor = vec4(0);
vec3 normal = vec3(1.f, 0.f, 0.f);
vec3 specular = vec3(0.f, 0.f, 0.f); //roughness, strength, metallic
float roughness = 0.f;

void applyAmbient(vec3 diffuseIlluminance, vec3 reflectionIlluminance)
{
  vec3 diffuseLuminance = diffuseIlluminance * surfaceColor.rgb / M_PI;
  vec3 reflectionLuminance = reflectionIlluminance / M_PI;

  float viewDotNorm = dot(normalize(-viewCoord), normal);

  vec3 plasticLuminance =
            diffuseLuminance +
            reflectionLuminance * specular.y * fresnelFactor(.1f, viewDotNorm);

  vec3 metallicLuminance = reflectionLuminance * surfaceColor.rgb *
                                                fresnelFactor(.8f, viewDotNorm);

  outColor.rgb += mix(plasticLuminance, metallicLuminance, specular.z);
}

void main()
{
  surfaceColor = getSurfaceColor();
  normal = getNormal();

  specular = getSpecular();
  roughness = specular.x;

  outColor = vec4(0.f, 0.f, 0.f, surfaceColor.a);

  APPLY_AMBIENT_WEIGHT
  APPLY_LIGHT

  outColor.rgb *= outColor.a;
  outColor.rgb += getEmission();

  APPLY_POSTEFFECT

  #ifdef ENABLE_ALPHA_TEST
    if(outColor == vec4(0)) discard;
  #endif

  outColor.a = 1.f - outColor.a;
}