#ifndef _TESTS_H_
#define _TESTS_H_

enum
{
	TEST_FULLDEMO = 0,
	TEST_PERFORMANCE,
    TEST_MATERIALS,
    TEST_COLOR,
    TEST_SHADOW,
    TEST_ANIMLIGHT,
    TEST_TUNNEL,
    TEST_POSTEFFECT,
    TEST_PARTICLE,
    TEST_CHANGECLOTHES,
#ifdef WIN32
    TEST_PHYSICS,
#else
    TEST_CarDemo,
#endif
    TEST_MATERIALS10,
	
    TESTS_COUNT,
};

const std::string g_aTestNames[TESTS_COUNT] = {
	"FullDemoTest",
	"PerformanceTest",
    "MaterialTest",
    "ColorTest",
    "ShadowTest",
    "AnimLightTest",
    "TunnelTest",
    "PostEffectTest",
    "ParticleTest",
    "ChangeClothesTest",
#ifdef WIN32
    "Physics Test",
#else
    "CarDemo",
#endif
    "Todo:New test",
};

#endif
