//meshUid.frag

layout(binding = uidBinding, set = volatileSet) uniform Uid
{
  uvec2 value;
} uid;

layout(location = 0) out uvec2 outColor;

void main()
{
  outColor = uid.value;
}