#pragma once

namespace l2d
{
	class Framework
	{
		static bool _init();
	public:
		static bool lazyInit();
		static bool end();

		static unsigned int getVersion();
		static unsigned int getLatestMocVersion();
	};
}
