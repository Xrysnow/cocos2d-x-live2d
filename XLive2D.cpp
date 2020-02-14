#include "XLive2D.h"
#include "LAppDefine.hpp"
#include "LAppPal.hpp"
#include <Rendering/CubismRenderer.hpp>
#ifdef CSM_TARGET_ANDROID_ES2
#include <Rendering/OpenGL/CubismRenderer_OpenGLES2.hpp>
#endif

using namespace l2d;
using namespace cocos2d;
using namespace Live2D::Cubism::Framework;

bool XLive2D::inited = false;
bool XLive2D::initTried = false;
EventListenerCustom* XLive2D::_recreatedEventlistener = nullptr;
LAppAllocator                 XLive2D::_cubismAllocator;
Csm::CubismFramework::Option  XLive2D::_cubismOption;

bool XLive2D::lazyInit()
{
	if (inited) return true;
	if (initTried)return false;
	return _init();
}

bool XLive2D::_init()
{
	initTried = true;
	// prepare for Cubism Framework API.
	_cubismOption.LogFunction = LAppPal::PrintMessage;
	_cubismOption.LoggingLevel = LAppDefine::CubismLoggingLevel;
	if (!Csm::CubismFramework::StartUp(&_cubismAllocator, &_cubismOption))
	{
		cocos2d::log("[L2D] can't start framework");
		return false;
	}
	CubismFramework::Initialize();
#ifdef CSM_TARGET_ANDROID_ES2
	char *exts = (char*)glGetString(GL_EXTENSIONS);
	if (strstr(exts, "GL_NV_shader_framebuffer_fetch ")) {
		Rendering::CubismRenderer_OpenGLES2::SetExtShaderMode(true, true);
	}
#endif

	_recreatedEventlistener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [](EventCustom*)
	{
		Rendering::CubismRenderer::StaticRelease();
	});
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_recreatedEventlistener, -1);

	LINFO("CubismFramework started successfully");
	inited = true;
	return true;
}

bool XLive2D::end()
{
	if (!inited) return true;
	Director::getInstance()->getEventDispatcher()->removeEventListener(_recreatedEventlistener);
	_recreatedEventlistener = nullptr;
	CubismFramework::Dispose();
	cocos2d::log("[L2D] framework end successfully");
	inited = false;
	initTried = false;
	return true;
}
