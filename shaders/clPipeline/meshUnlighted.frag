//meshUnlighted.frag

layout(location = 0) out vec4 outColor;

void main()
{
  outColor = getSurfaceColor();
  outColor.rgb *= outColor.a;
  outColor.rgb += getEmission();

  #ifdef ENABLE_ALPHA_TEST
    if(outColor == vec4(0)) discard;
  #endif

  outColor.a = 1.f - outColor.a;
}