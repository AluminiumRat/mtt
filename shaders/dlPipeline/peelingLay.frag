//peelingLay.frag

layout(input_attachment_index = 0, set = volatileSet, binding = accumulatorInputBinding) uniform subpassInput accumulatorMap;

layout(location = 0) out vec4 outColor;

void main()
{
  outColor = subpassLoad(accumulatorMap);
}