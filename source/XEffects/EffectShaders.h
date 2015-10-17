#ifndef H_XEFFECTS_SHADERS
#define H_XEFFECTS_SHADERS

enum E_SHADER_EXTENSION
{
	ESE_GLSL,
	ESE_HLSL,

	ESE_COUNT
};

extern const char* LIGHT_MODULATE_P[ESE_COUNT];
extern const char* SHADOW_PASS_1P[ESE_COUNT];
extern const char* SHADOW_PASS_1PT[ESE_COUNT];
extern const char* SHADOW_PASS_1V[ESE_COUNT];
extern const char* SHADOW_PASS_2P[ESE_COUNT];
extern const char* SHADOW_PASS_2V[ESE_COUNT];
extern const char* SIMPLE_P[ESE_COUNT];
extern const char* WHITE_WASH_P[ESE_COUNT];
extern const char* WHITE_WASH_P_ADD[ESE_COUNT];
extern const char* SCREEN_QUAD_V[ESE_COUNT];
extern const char* VSM_BLUR_P[ESE_COUNT];

#endif