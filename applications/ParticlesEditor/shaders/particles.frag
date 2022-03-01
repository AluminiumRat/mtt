//particles.frag

//layout( set = staticSet,
//        binding = colorSamplerBinding) uniform sampler2D colorSampler;

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 inTexCoords;

layout(location = 0) out vec4 outColor;

void main()
{
  outColor = inColor;
  //outColor *= texture(colorSampler, inTexCoords);
}