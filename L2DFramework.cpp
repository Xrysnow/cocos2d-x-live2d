#include "L2DFramework.h"
#include "LAppDefine.hpp"
#include "LAppPal.hpp"
#include "LAppAllocator.hpp"
#include "CubismRenderer_CC.h"
#include "L2DModel.h"
#ifdef CSM_TARGET_ANDROID_ES2
#include <Rendering/OpenGL/CubismRenderer_OpenGLES2.hpp>
#endif
#include "cocos2d.h"

using namespace l2d;
using namespace cocos2d;
using namespace Csm;

static bool L2DFrameworkInited = false;
static bool L2DFrameworkInitTried = false;
static EventListenerCustom* L2DRecreatedListener = nullptr;
static LAppAllocator L2DAllocator;
static CubismFramework::Option L2DOption;

bool Framework::lazyInit()
{
	if (L2DFrameworkInited) return true;
	if (L2DFrameworkInitTried)return false;
	return _init();
}

bool Framework::_init()
{
	L2DFrameworkInitTried = true;
	// prepare for Cubism Framework API.
	L2DOption.LogFunction = LAppPal::PrintMessage;
	L2DOption.LoggingLevel = LAppDefine::CubismLoggingLevel;
	if (!CubismFramework::StartUp(&L2DAllocator, &L2DOption))
	{
		cocos2d::log("[L2D] can't start framework");
		return false;
	}
	CubismFramework::Initialize();
#ifdef CSM_TARGET_ANDROID_ES2
	char *exts = (char*)glGetString(GL_EXTENSIONS);
	if (strstr(exts, "GL_NV_shader_framebuffer_fetch ")) {
		Rendering::CubismRenderer_CC::SetExtShaderMode(true, true);
	}
#endif

	L2DRecreatedListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [](EventCustom*)
	{
		Rendering::CubismRenderer::StaticRelease();
	});
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(
		L2DRecreatedListener, -1);

	L2DFrameworkInited = true;
	return true;
}

bool Framework::end()
{
	if (!L2DFrameworkInited) return true;
	Director::getInstance()->getEventDispatcher()->removeEventListener(L2DRecreatedListener);
	L2DRecreatedListener = nullptr;
	// make a copy to avoid erase in iter
	const auto instances = Model::instances;
	for (auto& m : instances)
		m->removeFromParentAndCleanup(true);
	CC_ASSERT(Model::instances.empty());
	Model::instances.clear();
	// note: all models should be destructed before this
	CubismFramework::Dispose();
	cocos2d::log("[L2D] framework end successfully");
	L2DFrameworkInited = false;
	L2DFrameworkInitTried = false;
	return true;
}
