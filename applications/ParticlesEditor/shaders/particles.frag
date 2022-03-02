//particles.frag

#ifdef COLOR_SAMPLER_ENABLED
  layout( set = staticSet,
          binding = colorSamplerBinding)
                                uniform sampler2D colorSampler[TEXTURES_NUMBER];
#endif

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 inTexCoords;
layout(location = 2) in flat uint inTextureIndex;

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

  outColor.a = 1.f - outColor.a;
}