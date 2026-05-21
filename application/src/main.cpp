#include "VE_InputUpdateLayer.h"
#include "VE_UIManager.h"
#include "VE_Core.h"
#include "VE_Logger.h"
#include "VE_LoggerMacros.h"
#include "VE_Time.h"
#include "VE_RenderTypes.h"
#include "VE_WindowManager.h"

using namespace velopraEngine;

int main(int argc, char *argv[]) {
  // Initialize Core and other systems
  auto &core = core::Core::Instance();
  core::Logger::Init();
  auto inputUpdateLayer = std::make_shared<input::InputUpdateLayer>();
  core.PushLayer(inputUpdateLayer.get());
  VELOPRA_CORE_INFO("Starting Velopra Engine");

  auto windowManager = std::make_shared<ui::WindowManager>();

  // Initialize and run the UI
  ui::UIManager uiManager(argc, argv, ui::RenderType::OpenGL, windowManager);
  return uiManager.Run();
}
