//shadowmapBlur.frag

layout(binding = srcSamplerBinding, set = volatileSet)
                                                  uniform sampler2D srcSampler;
layout(binding = blurParamUniformBinding, set = volatileSet)
                                                        uniform BlurParamUniform
{
  ivec2 shift;
  float weight;
} blurParam;

layout(location = 0) out vec2 outValues;

void main()
{
  ivec2 texelCoords = ivec2(gl_FragCoord.xy);
  outValues = texelFetch(srcSampler, texelCoords, 0).xy;

  ivec2 minBorder = ivec2(0, 0);
  ivec2 maxBorder = textureSize(srcSampler, 0) - ivec2(1, 1);

  float weight = .5f * blurParam.weight;

  outValues += weight * texelFetch( srcSampler,
                                    clamp(texelCoords + blurParam.shift,
                                          minBorder,
                                          maxBorder),
                                    0).xy;

  outValues += weight * texelFetch( srcSampler,
                                    clamp(texelCoords - blurParam.shift,
                                          minBorder,
                                          maxBorder),
                                    0).xy;

  outValues /= 1.f + blurParam.weight;
}