#pragma once

#include <unordered_map>
#include <set>
#include <pumex/Window.h>
#include <windows.h>
#include <windowsx.h>

namespace pumex
{
  
// Class implementing a pumex::Window for MS Windows system
class PUMEX_EXPORT WindowWin32 : public Window, public std::enable_shared_from_this<WindowWin32>
{
public:
  explicit WindowWin32(const WindowTraits& windowTraits);
  WindowWin32(const WindowWin32&)            = delete;
  WindowWin32& operator=(const WindowWin32&) = delete;
  virtual ~WindowWin32();

  static void registerWindow(HWND hwnd, WindowWin32* window);
  static void unregisterWindow(HWND hwnd);
  static WindowWin32* getWindow(HWND hwnd);

  std::shared_ptr<Surface> createSurface(std::shared_ptr<Viewer> viewer, std::shared_ptr<Device> device, const SurfaceTraits& surfaceTraits) override;
  
  static bool checkWindowMessages();

  LRESULT handleWin32Messages(UINT msg, WPARAM wParam, LPARAM lParam);

  void normalizeMouseCoordinates( float& x, float& y);
  
  InputEvent::Key win32KeyCodeToPumex(WPARAM keycode) const;
  
protected:
  HWND _hwnd              = nullptr;
  bool swapChainResizable = false;
  bool sizeMaximized      = false;
  
  static std::unordered_map<WPARAM, InputEvent::Key> win32Keycodes;
  
  static void fillWin32Keycodes();
  
  
  static std::unordered_map<HWND, WindowWin32*> registeredWindows;

  std::set<InputEvent::MouseButton> pressedMouseButtons;
};

}