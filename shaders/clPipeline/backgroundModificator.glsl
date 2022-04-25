//backgroundApplicator.glsl

#if $LUMINANCE_SAMPLER_ENABLED$
  layout( set = staticSet,
          binding = luminanceTextureBinding$INDEX$)
                                    uniform samplerCube luminanceSampler$INDEX$;
#endif

layout( set = volatileSet,
        binding = drawDataBinding$INDEX$) uniform DrawData$INDEX$
{
  vec3 luminance;
  float dissolutionStartDistance;
  float dissolutionLength;
  mat3 viewToLocal;
} drawData$INDEX$;

void $APPLY_FUNCTION$()
{
  float distance = length(viewCoord.xyz);

  float weight = smoothstep(drawData$INDEX$.dissolutionStartDistance,
                            drawData$INDEX$.dissolutionStartDistance +
                                              drawData$INDEX$.dissolutionLength,
                            distance);

  vec3 luminance = drawData$INDEX$.luminance;

  #if $LUMINANCE_SAMPLER_ENABLED$
    vec3 direction = normalize(drawData$INDEX$.viewToLocal * viewCoord.xyz);
    direction.z = -direction.z;
    luminance *= texture(luminanceSampler$INDEX$, direction).rgb;
  #endif

  luminance *= outColor.a;
  outColor.rgb = mix(outColor.rgb, luminance, weight);
}