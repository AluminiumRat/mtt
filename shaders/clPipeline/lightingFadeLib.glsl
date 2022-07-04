//lightingFadeLib.glsl

//NO_FADE
#if $FADE_TYPE$ == 0
  #define fade$INDEX$(distance, maxDistance) (1.f)
#endif

//LINEAR_FADE
#if $FADE_TYPE$ == 1
  #define fade$INDEX$(distance, maxDistance) (-2.f / maxDistance * distance + 2.f)
#endif

//INVERSE_LINEAR_FADE
#if $FADE_TYPE$ == 2
  #define fade$INDEX$(distance, maxDistance) (maxDistance / distance - 1.f)
#endif

//INVERSE_SQUARE_FADE
#if $FADE_TYPE$ == 3
  #define fade$INDEX$(distance, maxDistance) (maxDistance * maxDistance / 3.f / distance / distance - 1.f / 3.f)
#endif
