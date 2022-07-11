//particlesFragmentLighting.frag

layout (set = volatileSet,
        binding = depthSamplerBinding) uniform sampler2D depthMap;

#ifdef COLOR_SAMPLER_ENABLED
  layout( set = staticSet,
          binding = colorSamplerBinding)
                                uniform sampler2D colorSampler[TEXTURES_NUMBER];
#endif

layout(location = 0) in flat vec4 inAlbedo;
layout(location = 1) in flat float inNearDepth;
layout(location = 2) in flat float inNearFarDepth;

#ifdef COLOR_SAMPLER_ENABLED
  layout(location = 3) in vec2 inTexCoords;
  layout(location = 4) in flat uint inTextureIndex;
#endif

layout(location = 5) in vec3 inViewCoord;
layout(location = 6) in vec3 inEmission;

layout(location = 0) out vec4 outColor;

MODIFICATOR_DECLARATION

vec3 viewCoord;
float overallAmbientWeight = 0.f;

vec3 totalIlluminance = vec3(0.f, 0.f, 0.f);
void applyLight(vec3 illuminance)
{
  totalIlluminance += illuminance;
}

void main()
{
  #ifdef COLOR_SAMPLER_ENABLED
    uint textureIndex = clamp(inTextureIndex, 0, TEXTURES_NUMBER - 1);
    vec4 textureColor = texture(colorSampler[textureIndex], inTexCoords);
    if(textureColor.a == .0f) discard;
  #endif

  ivec2 fragCoord = ivec2(gl_FragCoord.xy);
  float depth = texelFetch(depthMap, fragCoord, 0).r;
  float fading = (inNearDepth - depth) / inNearFarDepth;
  if(fading == 0.f) discard;

  viewCoord = inViewCoord;

  APPLY_AMBIENT_WEIGHT
  APPLY_LIGHT

  outColor = inAlbedo;
  outColor.rgb *= totalIlluminance / M_PI;
  outColor.rgb += inEmission;

  #ifdef COLOR_SAMPLER_ENABLED
    outColor.rgb *= textureColor.a;
    outColor *= textureColor;
  #endif

  outColor *= clamp(fading, 0.f, 1.f);

  APPLY_POSTEFFECT
}