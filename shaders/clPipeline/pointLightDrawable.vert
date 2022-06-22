//pointLightDrawable.vert

layout(location = 0) out noperspective vec2 outClipCoord;

#ifdef FULLSCREEN_ENABLED
  const vec4 points[] = { vec4(-1.f,  1.f, .0f, 1.f),
                          vec4(-1.f, -1.f, .0f, 1.f),
                          vec4( 1.f,  1.f, .0f, 1.f),
                          vec4( 1.f, -1.f, .0f, 1.f)};

  void main()
  {
    gl_Position = points[gl_VertexIndex];
    outClipCoord = gl_Position.xy;
  }

#else
  const vec4 points[] = { vec4(-1.f,  1.f,  1.f, 1.f), //Up1
                          vec4(-1.f,  1.f, -1.f, 1.f),
                          vec4( 1.f,  1.f, -1.f, 1.f),

                          vec4(-1.f,  1.f,  1.f, 1.f), //Up2
                          vec4( 1.f,  1.f, -1.f, 1.f),
                          vec4( 1.f,  1.f,  1.f, 1.f),

                          vec4(-1.f, -1.f,  1.f, 1.f), //Down1
                          vec4( 1.f, -1.f, -1.f, 1.f),
                          vec4(-1.f, -1.f, -1.f, 1.f),

                          vec4(-1.f, -1.f,  1.f, 1.f), //Down2
                          vec4( 1.f, -1.f,  1.f, 1.f),
                          vec4( 1.f, -1.f, -1.f, 1.f),

                          vec4( 1.f, -1.f,  1.f, 1.f), //Right1
                          vec4( 1.f,  1.f,  1.f, 1.f),
                          vec4( 1.f,  1.f, -1.f, 1.f),

                          vec4( 1.f, -1.f,  1.f, 1.f), //Right2
                          vec4( 1.f,  1.f, -1.f, 1.f),
                          vec4( 1.f, -1.f, -1.f, 1.f),

                          vec4(-1.f, -1.f,  1.f, 1.f), //Left1
                          vec4(-1.f,  1.f, -1.f, 1.f),
                          vec4(-1.f,  1.f,  1.f, 1.f),

                          vec4(-1.f, -1.f,  1.f, 1.f), //Left2
                          vec4(-1.f, -1.f, -1.f, 1.f),
                          vec4(-1.f,  1.f, -1.f, 1.f),

                          vec4(-1.f, -1.f,  1.f, 1.f), //Near1
                          vec4(-1.f,  1.f,  1.f, 1.f),
                          vec4( 1.f,  1.f,  1.f, 1.f),

                          vec4(-1.f, -1.f,  1.f, 1.f), //Near2
                          vec4( 1.f,  1.f,  1.f, 1.f),
                          vec4( 1.f, -1.f,  1.f, 1.f),

                          vec4(-1.f, -1.f, -1.f, 1.f), //Far1
                          vec4( 1.f,  1.f, -1.f, 1.f),
                          vec4(-1.f,  1.f, -1.f, 1.f),

                          vec4(-1.f, -1.f, -1.f, 1.f), //Far2
                          vec4( 1.f, -1.f, -1.f, 1.f),
                          vec4( 1.f,  1.f, -1.f, 1.f)};

  layout(binding = lightDataBinding, set = volatileSet) uniform LightData
  {
    vec3 illuminance;
    vec3 lightPosition;
    float distance;
    mat4 clipToView;
    mat4 viewToLocal;
    float blurRadius;
    int startSample;
    int endSample;
  } lightData;

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
    vec4 localPoint = points[gl_VertexIndex];
    localPoint.xyz *= lightData.distance;
    vec4 viewPosition = drawMatrices.localToViewMatrix * localPoint;
    gl_Position = drawMatrices.projectionMatrix * viewPosition;
    outClipCoord = gl_Position.xy / gl_Position.w;
  }
#endif
