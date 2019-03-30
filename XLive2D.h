#pragma once
#include "cocos2d.h"
#include <CubismFramework.hpp>
#include "LAppAllocator.hpp"

namespace l2d
{
	class XLive2D
	{
	public:
		static bool lazyInit();
		static bool end();

	private:
		static bool _init();
		static cocos2d::EventListenerCustom* _recreatedEventlistener;
		static LAppAllocator                 _cubismAllocator;
		static Csm::CubismFramework::Option  _cubismOption;
		static bool inited;
		static bool initTried;
	};
}
