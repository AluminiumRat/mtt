//peelingMerge.frag

layout(input_attachment_index = 0, set = volatileSet, binding = layerInputBinding) uniform subpassInput layer;

layout(location = 0) out vec4 outColor;

void main()
{
  outColor = subpassLoad(layer);
}