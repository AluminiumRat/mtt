//particlesShadow.frag

#ifdef COLOR_SAMPLER_ENABLED
  layout( set = staticSet,
          binding = colorSamplerBinding)
                                uniform sampler2D colorSampler[TEXTURES_NUMBER];
#endif

layout(location = 0) in vec4 inColor;
layout(location = 1) in flat vec2 inAvgDistances;

#ifdef COLOR_SAMPLER_ENABLED
  layout(location = 3) in vec2 inTexCoords;
  layout(location = 4) in flat uint inTextureIndex;
#endif

layout(location = 0) out vec4 outValue;

void main()
{
  float weight = inColor.a;

  #ifdef COLOR_SAMPLER_ENABLED
    uint textureIndex = clamp(inTextureIndex, 0, TEXTURES_NUMBER - 1);
    weight *= texture(colorSampler[textureIndex], inTexCoords).a;
  #endif

  outValue = vec4(1.f, inAvgDistances.x, inAvgDistances.y, weight);
}