//meshCommon.vert

layout(location = positionLocation) in vec3 inPosition;
#if defined(ENABLE_NORMAL) || defined(ENABLE_REFLECTION)
  layout(location = normalLocation) in vec3 inNormal;
#endif
#ifdef ENABLE_TEX_COORDS
  layout(location = texCoordLocation) in vec2 inTexCoord;
#endif
#ifdef ENABLE_NORMAL_MAPPING
  layout(location = tangentLocation) in vec3 inTangent;
  layout(location = binormalLocation) in vec3 inBinormal;
#endif
#ifdef SKELETON
  layout(location = boneIndicesLocation) in uvec4 inBoneIndices;
  layout(location = boneWeightsLocation) in vec4 inBoneWeights;
#endif

#ifdef ENABLE_NORMAL
  layout(location = 0) out vec3 outNormal;
#endif
#ifdef ENABLE_TEX_COORDS
  layout(location = 1) out vec2 outTexCoord;
#endif
#ifdef ENABLE_NORMAL_MAPPING
  layout(location = 2) out vec3 outTangent;
  layout(location = 3) out vec3 outBinormal;
#endif
#ifdef ENABLE_REFLECTION
  layout(location = 4) out vec3 outReflectDirection;
#endif
#ifdef ENABLE_VIEW_COORDS_IN_FRAGMENT_SHADER
  layout(location = 5) out vec3 outViewCoord;
#endif

#ifdef SKELETON
  layout(binding = boneInverseMatricesBinding, set = staticSet)
                                                    uniform BoneInverseMatrices
  {
    mat4 value[BONE_NUMBER];
  } boneInverseMatrices;

  layout(binding = boneMatricesBinding, set = volatileSet) uniform BoneMatrices
  {
    mat4 value[BONE_NUMBER];
  } boneMatrices;
#endif

layout(binding = drawMatricesBinding, set = volatileSet) uniform DrawMatrices
{
  mat4 localToViewMatrix;
  mat3 normalsLocalToViewMatrix;
  mat4 projectionMatrix;
  mat4 clipToViewMatrix;
  mat4 viewToReflection;
} drawMatrices;

void main()
{
  #ifdef SKELETON
    vec4 position = vec4(0);
    vec4 sourcePosition = vec4(inPosition, 1);

    #if defined(ENABLE_NORMAL) || defined(ENABLE_REFLECTION)
      vec3 normal = vec3(0);
      vec4 sourceNormal = vec4(inNormal, 0);
    #endif

    #ifdef ENABLE_NORMAL_MAPPING
      vec3 tangent = vec3(0);
      vec4 sourceTangent = vec4(inTangent, 0);

      vec3 binormal = vec3(0);
      vec4 sourceBinormal = vec4(inBinormal, 0);
    #endif

    for(int i = 0; i < 4; i++)
    {
      uint boneIndex = inBoneIndices[i];
      float weight = inBoneWeights[i];
      mat4 boneInverseTransform = boneInverseMatrices.value[boneIndex];
      mat4 boneTransform = boneMatrices.value[boneIndex];

      vec4 deformedPosition = boneTransform *
                                        (boneInverseTransform * sourcePosition);
      position += weight * deformedPosition;
      
      #if defined(ENABLE_NORMAL) || defined(ENABLE_REFLECTION)
        vec4 deformedNormal = boneTransform *
                                          (boneInverseTransform * sourceNormal);
        normal += weight * deformedNormal.xyz;
      #endif

      #ifdef ENABLE_NORMAL_MAPPING
        vec4 deformedTangent = boneTransform *
                                          (boneInverseTransform * sourceTangent);
        tangent += weight * deformedTangent.xyz;
        vec4 deformedBinormal = boneTransform *
                                          (boneInverseTransform * sourceBinormal);
        binormal += weight * deformedBinormal.xyz;
      #endif
    }

    #if defined(ENABLE_NORMAL) || defined(ENABLE_REFLECTION)
      normal = normalize(normal);
    #endif
    #ifdef ENABLE_NORMAL_MAPPING
      tangent = normalize(tangent);
      binormal = normalize(binormal);
    #endif

  #else
    vec4 position = vec4(inPosition, 1);
    #if defined(ENABLE_NORMAL) || defined(ENABLE_REFLECTION)
      vec3 normal = inNormal;
    #endif
    #ifdef ENABLE_NORMAL_MAPPING
      vec3 tangent = inTangent;
      vec3 binormal = inBinormal;
    #endif
  #endif

  vec4 viewPosition = drawMatrices.localToViewMatrix * position;
  #ifdef ENABLE_VIEW_COORDS_IN_FRAGMENT_SHADER
    outViewCoord = viewPosition.xyz;
  #endif

  gl_Position = drawMatrices.projectionMatrix * viewPosition;

  #if defined(ENABLE_NORMAL) || defined(ENABLE_REFLECTION)
    vec3 normalView = drawMatrices.normalsLocalToViewMatrix * normal;

    #ifdef ENABLE_NORMAL
      outNormal = normalView;
    #endif

    #ifdef ENABLE_REFLECTION
      vec3 reflectionViewSpace = reflect(viewPosition.xyz, normalize(normalView));
      outReflectDirection =
            (drawMatrices.viewToReflection * vec4(reflectionViewSpace, 0.0)).xyz;
    #endif
  #endif

  #ifdef ENABLE_TEX_COORDS
    outTexCoord = inTexCoord;
  #endif

  #ifdef ENABLE_NORMAL_MAPPING
    outTangent = drawMatrices.normalsLocalToViewMatrix * tangent;
    outBinormal = drawMatrices.normalsLocalToViewMatrix * binormal;
  #endif
}
