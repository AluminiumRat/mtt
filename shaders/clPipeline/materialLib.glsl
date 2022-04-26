//materialLib.glsl
#ifndef MATERIAL_LIB
#define MATERIAL_LIB

float GGXDistribution(float normDotHalf, float roughness)
{
  float roughness2 = roughness * roughness;
  float roughness4 = roughness2 * roughness2;
  float factor = roughness4 / M_PI;
  float denom = normDotHalf * normDotHalf * (roughness4 - 1.f) + 1.f;
  denom = denom * denom;
  factor /= denom;
  return factor;
}

float neumannGeometryFactor(float lightDotNorm,
                            float normDotHalf,
                            float normDotView)
{
  float factor = lightDotNorm * normDotView;
  factor /= max(normDotHalf, normDotView);
  return factor;
}

float fresnelFactor(float basis, float viewDotHalf)
{
  float compliment = min(1.f - viewDotHalf, 1.f);
  compliment = compliment * compliment * compliment * compliment * compliment;
  return basis + (1.f - basis) * compliment;
}

vec3 getPlasticSpecular(  vec3 specularLuminance,
                          float specularStrength,
                          float viewDotHalf)
{
  float frensel = fresnelFactor(.1f, viewDotHalf);
  return specularLuminance * specularStrength * frensel;
}

vec3 getPlasticLuminance( vec3 lambertLuminance,
                          vec3 specularLuminance,
                          vec3 albedo,
                          float specularStrength,
                          float viewDotHalf)
{
  vec3 diffuseLuminance = lambertLuminance * albedo;
  float frensel = fresnelFactor(.1f, viewDotHalf);
  return diffuseLuminance + specularLuminance * specularStrength * frensel;
}

vec3 getMetallicLuminance(vec3 specularLuminance,
                          vec3 albedo,
                          float viewDotHalf)
{
  float frensel = fresnelFactor(.8f, viewDotHalf);
  return albedo * specularLuminance * frensel;
}

vec3 getLuminance(vec3 lambertLuminance,
                  vec3 specularLuminance,
                  vec3 albedo,
                  float specularStrength,
                  float metallic,
                  float viewDotHalf)
{
  vec3 plasticLuminance = getPlasticLuminance(lambertLuminance,
                                              specularLuminance,
                                              albedo,
                                              specularStrength,
                                              viewDotHalf);

  vec3 metallicLuminance = getMetallicLuminance(specularLuminance,
                                                albedo,
                                                viewDotHalf);

  return mix(plasticLuminance, metallicLuminance, metallic);
}

vec3 getLuminance(vec3 albedo,
                  float roughness,
                  float specularStrength,
                  float metallic,
                  vec3 normal,
                  vec3 toViewDirection,
                  vec3 lightInverseDirection,
                  vec3 illuminance)
{
  float lightDotNorm = max(dot(lightInverseDirection, normal), 0.f);

  vec3 halfDir = normalize(toViewDirection + lightInverseDirection);
  float normDotHalf = max(dot(normal, halfDir), 0.f);
  float normDotView = max(dot(normal, toViewDirection), 0.01f);
  float viewDotHalf = max(dot(toViewDirection, halfDir), 0.f);

  float distributionFactor = GGXDistribution(normDotHalf, roughness);
  float geometryFactor = neumannGeometryFactor( lightDotNorm,
                                                normDotHalf,
                                                normDotView);
  float specularFactor =  distributionFactor * geometryFactor /
                                                            (4.f * normDotView);
  vec3 specularLuminance = illuminance * specularFactor;

  vec3 lambertLuminance = illuminance * (lightDotNorm / M_PI);

  return getLuminance(lambertLuminance,
                      specularLuminance,
                      albedo,
                      specularStrength,
                      metallic,
                      viewDotHalf);
}

#endif