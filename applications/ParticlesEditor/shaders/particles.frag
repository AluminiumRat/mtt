//particles.frag

layout (set = volatileSet,
        binding = depthSamplerBinding) uniform sampler2D depthMap;

#ifdef COLOR_SAMPLER_ENABLED
  layout( set = staticSet,
          binding = colorSamplerBinding)
                                uniform sampler2D colorSampler[TEXTURES_NUMBER];
#endif

layout(location = 0) in vec4 inColor;
layout(location = 1) in flat float inNearDepth;
layout(location = 2) in flat float inFarDepth;

#ifdef COLOR_SAMPLER_ENABLED
  layout(location = 3) in vec2 inTexCoords;
  layout(location = 4) in flat uint inTextureIndex;
#endif

layout(location = 0) out vec4 outColor;

void main()
{
  outColor = inColor;

  #ifdef COLOR_SAMPLER_ENABLED
    uint textureIndex = clamp(inTextureIndex, 0, TEXTURES_NUMBER - 1);
    vec4 textureColor = texture(colorSampler[textureIndex], inTexCoords);
    outColor.rgb *= textureColor.a;
    outColor *= textureColor;
  #endif

  ivec2 fragCoord = ivec2(gl_FragCoord.xy);
  float depth = texelFetch(depthMap, fragCoord, 0).r;
  float fading = (depth - inFarDepth) / (inNearDepth - inFarDepth);
  fading = 1.f - clamp(fading, 0.f, 1.f);

  outColor *= fading;

  outColor.a = 1.f - outColor.a;
}