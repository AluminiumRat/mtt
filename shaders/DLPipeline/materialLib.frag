//materialLib.frag

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
  float normDotView = max(dot(normal, toViewDirection), 0.f);
  float viewDotHalf = max(dot(toViewDirection, halfDir), 0.f);

  float distributionFactor = GGXDistribution(normDotHalf, roughness);
  float geometryFactor = neumannGeometryFactor( lightDotNorm,
                                                normDotHalf,
                                                normDotView);
  float specularFactor =  distributionFactor * geometryFactor /
                                                            (4.f * normDotView);

  vec3 lambertFactor = albedo * (lightDotNorm / M_PI);
  vec3 plasticReflection = lambertFactor + vec3(specularFactor *
                            specularStrength * fresnelFactor(.1f, viewDotHalf));
  vec3 plasticLuminance = illuminance * plasticReflection;
  
  vec3 metallicReflection = albedo *
                            (specularFactor * fresnelFactor(.8f, viewDotHalf));
  vec3 metallicLuminance = illuminance * metallicReflection;

  return mix(plasticLuminance, metallicLuminance, metallic);
}