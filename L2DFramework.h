#pragma once

namespace l2d
{
	class Framework
	{
	public:
		static bool lazyInit();
		static bool end();

	private:
		static bool _init();
	};
}
