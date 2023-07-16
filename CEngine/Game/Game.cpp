#include "Game.h"

#include "CEngine/Imgui/imgui.h"
#include "CEngine/Imgui/imgui_impl_win32.h"
#include "CEngine/Imgui/imgui_impl_dx11.h"

Game::Game() : m_wnd(L"D3D11_main", kRenderWidth, kRenderHeight) {
}

int Game::Start() {
    m_wnd.getGraphics()->ClearBuffer(0.0f, 0.0f, 0.0f);
    try {
        bool running = true;
        MSG msg = { 0 };
        while (running) {
            // main message loop
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                if (m_wnd.m_keyboard.IsKeyPressed(VK_TAB) || m_wnd.m_keyboard.IsKeyPressed(VK_MENU)) {
                    unsigned int result = MessageBox(nullptr, L"HIHIHI", L"TMEPY", MB_OK);
                    if (result == IDOK)
                        m_wnd.CaptureWindow();
                }

                while (!m_wnd.m_mouse.IsEventQueueEmpty()) {
                    const Mouse::MouseEvent* mouseEvent = m_wnd.m_mouse.ReadFirstEvent();

                    static int test = 0;
                    if (mouseEvent) {
                        switch (mouseEvent->getEventType()) {
                        case Mouse::MouseEvent::Type::Leave:
                            m_wnd.setTitle(L"outside");
                            break;
                        case Mouse::MouseEvent::Type::WheelDown: {
                            --test;
                            const std::wstring title = std::to_wstring(test);
                            m_wnd.setTitle(title);
                            break;
                        }

                        case Mouse::MouseEvent::Type::WheelUp: {
                            ++test;
                            const std::wstring title = std::to_wstring(test);
                            m_wnd.setTitle(title);
                            break;
                        }

                        case Mouse::MouseEvent::Type::Move: {
                            Vector3Int pos = mouseEvent->getPosition();
                            const std::wstring title = L"X: " + std::to_wstring(pos.m_x) + L", Y: " + std::to_wstring(pos.m_y);
                            m_wnd.setTitle(title);
                            break;
                        }
                        }
                    }

                    delete mouseEvent;
                }

                if (msg.message == WM_QUIT) {
                    running = false;
                }
            }
            DoFrameStart();
            m_wnd.getGraphics()->GraphicsEnd(); // necessary for render!! 
        }

        return msg.wParam;
    }
    catch (const BaseException& except){
        std::wstring whatString;
        CharToWChar(except.what(), whatString);
        MessageBox(nullptr, whatString.c_str(), except.getWType(), MB_OK);
    }
    catch (const std::exception& except) {
        std::wstring whatString;
        CharToWChar(except.what(), whatString);
        MessageBox(nullptr, whatString.c_str(), L"std::exception", MB_OK);
    }
    catch (...) {
        MessageBox(nullptr, L"No details", L"Unknown error", MB_OK);
    }
    return -1;
}

void Game::DoFrameStart() {
    const float c = sin(m_timer.Peek()) / 2.0f + 0.5f;
    m_wnd.getGraphics()->ClearBuffer(c, 0.5f, 0.0f);
    //m_wnd.getGraphics()->DrawTriangle();
    m_wnd.getGraphics()->DrawIndex(-m_timer.Peek(), 0.0f);
    m_wnd.getGraphics()->DrawIndex(m_timer.Peek(), 0.0f);

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    static bool show_demo_window = true;
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    m_wnd.getGraphics()->GraphicsEnd();
}
