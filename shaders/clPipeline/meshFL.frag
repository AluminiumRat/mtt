//meshFL.frag

#ifndef ENABLE_VIEW_COORDS_IN_FRAGMENT_SHADER
  You should set VIEW_COORDS_IN_FRAGMENT_SHADER_FEATURE
#endif
layout(location = 5) in vec3 viewCoord;

layout(location = 0) out vec4 outColor;

MODIFICATOR_DECLARATION

float overallAmbientWeight = 0;
vec4 surfaceColor = vec4(0);
vec3 normal = vec3(1.f, 0.f, 0.f);
vec3 toView = vec3(0.f, 0.f, 1.f);
float viewDotNorm = 0;
vec3 specular = vec3(0.f, 0.f, 0.f); //roughness, strength, metallic
float roughness = 0.f;

void applyLight(vec3 lambertLuminance, vec3 specularLuminance)
{
  outColor.rgb += getLuminance( lambertLuminance,
                                specularLuminance,
                                surfaceColor.rgb,
                                specular.y,
                                specular.z,
                                viewDotNorm);
}

void main()
{
  surfaceColor = getSurfaceColor();
  normal = getNormal();

  toView = normalize(-viewCoord.xyz);
  viewDotNorm = dot(toView, normal);

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