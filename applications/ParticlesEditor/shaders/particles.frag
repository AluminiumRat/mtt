//particles.frag

#ifdef DEPTH_SAMPLER_ENABLED
  layout (set = volatileSet,
          binding = depthSamplerBinding) uniform sampler2D depthMap;
#endif

#ifdef COLOR_SAMPLER_ENABLED
  layout( set = staticSet,
          binding = colorSamplerBinding)
                                uniform sampler2D colorSampler[TEXTURES_NUMBER];
#endif

layout(location = 0) in vec4 inColor;

#ifdef SHADOWMAP_OUTPUT
  layout(location = 1) in flat vec2 inAvgDistances;
#else
  #ifdef DEPTH_SAMPLER_ENABLED
    layout(location = 1) in flat float inNearDepth;
    layout(location = 2) in flat float inFarDepth;
  #endif
#endif

#ifdef COLOR_SAMPLER_ENABLED
  layout(location = 3) in vec2 inTexCoords;
  layout(location = 4) in flat uint inTextureIndex;
#endif

layout(location = 0) out vec4 outValue;

#ifdef SHADOWMAP_OUTPUT
  void main()
  {
    float weight = inColor.a;

    #ifdef COLOR_SAMPLER_ENABLED
      uint textureIndex = clamp(inTextureIndex, 0, TEXTURES_NUMBER - 1);
      weight *= texture(colorSampler[textureIndex], inTexCoords).a;
    #endif

    outValue = vec4(1.f, inAvgDistances.x, inAvgDistances.y, weight);
  }
#else
  void main()
  {
    outValue = inColor;

    #ifdef COLOR_SAMPLER_ENABLED
      uint textureIndex = clamp(inTextureIndex, 0, TEXTURES_NUMBER - 1);
      vec4 textureColor = texture(colorSampler[textureIndex], inTexCoords);
      outValue.rgb *= textureColor.a;
      outValue *= textureColor;
    #endif

    #ifdef DEPTH_SAMPLER_ENABLED
      ivec2 fragCoord = ivec2(gl_FragCoord.xy);
      float depth = texelFetch(depthMap, fragCoord, 0).r;
      float fading = (depth - inFarDepth) / (inNearDepth - inFarDepth);
      fading = 1.f - clamp(fading, 0.f, 1.f);
      outValue *= fading;
    #endif

    outValue.a = 1.f - outValue.a;
  }
#endif