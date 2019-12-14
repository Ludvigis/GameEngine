#pragma once
#include "spdlog/spdlog.h"
#include <memory>
#include "Log.h"

namespace Engine {
	
	std::shared_ptr<spdlog::logger> Log::logger;
	
	void Log::init()
	{
		spdlog::set_pattern("%^[%T]: [%l] %v%$");
		logger = spdlog::stdout_color_mt("Engine");
	}
}
