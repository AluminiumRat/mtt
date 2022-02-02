//backgroundDrawable.frag

layout(location = 0) in noperspective vec2 inClipCoord;

#ifdef LUMINANCE_SAMPLER_ENABLED
  layout( set = staticSet,
          binding = luminanceTextureBinding)
                                          uniform samplerCube luminanceSampler;
#endif

layout (input_attachment_index = 0,
        set = volatileSet,
        binding = depthMapBinding) uniform subpassInput depthMap;

layout( set = volatileSet,
        binding = drawDataBinding) uniform DrawData
{
  vec3 luminance;
  float dissolutionStartDistance;
  float dissolutionLength;
  mat3 viewToLocal;
} drawData;

layout( set = volatileSet,
        binding = drawMatricesBinding) uniform DrawMatrices
{
  mat4 localToViewMatrix;
  mat3 normalsLocalToViewMatrix;
  mat4 projectionMatrix;
  mat4 clipToViewMatrix;
  mat4 viewToReflection;
} drawMatrices;

layout(location = 0) out vec4 outColor;

void main()
{
  float depth = subpassLoad(depthMap).x;

  vec4 clipCoord = vec4(inClipCoord, depth, 1);
  vec4 viewCoord = drawMatrices.clipToViewMatrix * clipCoord;
  viewCoord.xyz /= viewCoord.w;

  float distance = length(viewCoord.xyz);

  float weight = smoothstep(drawData.dissolutionStartDistance,
                            drawData.dissolutionStartDistance +
                                                    drawData.dissolutionLength,
                            distance);
  
  vec3 luminance = drawData.luminance;

  #ifdef LUMINANCE_SAMPLER_ENABLED
    vec3 direction = normalize(drawData.viewToLocal * viewCoord.xyz);
    direction.z = -direction.z;
    luminance *= texture(luminanceSampler, direction).rgb;
  #endif

  outColor = vec4(luminance * weight, 1.f - weight);
}