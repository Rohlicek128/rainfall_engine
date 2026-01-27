#include "TitlebarPanel.h"

#include <engine/rendering/Window.h>
#include "../lib/engine/src/Rendering/EngineArgs.h"
#include "imgui_internal.h"
#include <glfw3.h>


namespace editor
{
    void TitlebarPanel::draw(engine::Window& window)
    {
        const float titlebarHeight = 58.0f;
		const bool isMaximized = (bool)glfwGetWindowAttrib(window.engine_args.window, GLFW_MAXIMIZED);
		float titlebarVerticalOffset = isMaximized ? -6.0f : 0.0f;
		const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;

		ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset));
		const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
		const ImVec2 titlebarMax = { ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - windowPadding.y * 2.0f,
									 ImGui::GetCursorScreenPos().y + titlebarHeight };
		auto* bgDrawList = ImGui::GetBackgroundDrawList();
		auto* fgDrawList = ImGui::GetForegroundDrawList();
		bgDrawList->AddRectFilled(titlebarMin, titlebarMax, ImU32 { 200 });
		// DEBUG TITLEBAR BOUNDS
		// fgDrawList->AddRect(titlebarMin, titlebarMax, UI::Colors::Theme::invalidPrefab);

		// Logo
		{
			const int logoWidth = 48;// m_LogoTex->GetWidth();
			const int logoHeight = 48;// m_LogoTex->GetHeight();
			const ImVec2 logoOffset(16.0f + windowPadding.x, 5.0f + windowPadding.y + titlebarVerticalOffset);
			const ImVec2 logoRectStart = { ImGui::GetItemRectMin().x + logoOffset.x, ImGui::GetItemRectMin().y + logoOffset.y };
			const ImVec2 logoRectMax = { logoRectStart.x + logoWidth, logoRectStart.y + logoHeight };
			//fgDrawList->AddImage(m_AppHeaderIcon->GetDescriptorSet(), logoRectStart, logoRectMax);
		}

		//ImGui::BeginHorizontal("Titlebar", { ImGui::GetWindowWidth() - windowPadding.y * 2.0f, ImGui::GetFrameHeightWithSpacing() });

		static float moveOffsetX;
		static float moveOffsetY;
		const float w = ImGui::GetContentRegionAvail().x;
		const float buttonsAreaWidth = 94;

		// Title bar drag area
		// On Windows we hook into the GLFW win32 window internals
		ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset)); // Reset cursor pos
		// DEBUG DRAG BOUNDS
		// fgDrawList->AddRect(ImGui::GetCursorScreenPos(), ImVec2(ImGui::GetCursorScreenPos().x + w - buttonsAreaWidth, ImGui::GetCursorScreenPos().y + titlebarHeight), UI::Colors::Theme::invalidPrefab);
		ImGui::InvisibleButton("##titleBarDragZone", ImVec2(w - buttonsAreaWidth, titlebarHeight));

		titleBarHovered = ImGui::IsItemHovered();

		if (isMaximized)
		{
			float windowMousePosY = ImGui::GetMousePos().y - ImGui::GetCursorScreenPos().y;
			if (windowMousePosY >= 0.0f && windowMousePosY <= 5.0f)
				titleBarHovered = true; // Account for the top-most pixels which don't register
		}

		/*//  Draw Menubar
		if (m_MenubarCallback)
		{
			ImGui::SuspendLayout();
			{
				ImGui::SetItemAllowOverlap();
				const float logoHorizontalOffset = 16.0f * 2.0f + 48.0f + windowPadding.x;
				ImGui::SetCursorPos(ImVec2(logoHorizontalOffset, 6.0f + titlebarVerticalOffset));
				UI_DrawMenubar();

				if (ImGui::IsItemHovered())
					m_TitleBarHovered = false;
			}

			ImGui::ResumeLayout();
			}*/

		{
			// Centered Window title
			ImVec2 currentCursorPos = ImGui::GetCursorPos();
			ImVec2 textSize = ImGui::CalcTextSize("Rainfall");
			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() * 0.5f - textSize.x * 0.5f, 2.0f + windowPadding.y + 6.0f));
			ImGui::Text("%s", "Rainfall"); // Draw title
			ImGui::SetCursorPos(currentCursorPos);
		}

		// Window buttons
		const ImU32 buttonColN = ImU32 { 90 }; //UI::Colors::ColorWithMultipliedValue(UI::Colors::Theme::text, 0.9f);
		const ImU32 buttonColH = ImU32 { 120 }; //UI::Colors::ColorWithMultipliedValue(UI::Colors::Theme::text, 1.2f);
		const ImU32 buttonColP = ImU32 { 50 };
		const float buttonWidth = 14.0f;
		const float buttonHeight = 14.0f;

		// Minimize Button

		// Maximize Button

		// Close Button
		{
			//const int iconWidth = m_IconClose->GetWidth();
			//const int iconHeight = m_IconClose->GetHeight();
			if (ImGui::InvisibleButton("Close", ImVec2(buttonWidth, buttonHeight)))
				glfwWindowShouldClose(window.engine_args.window);

			//UI::DrawButtonImage(m_IconClose, UI::Colors::Theme::text, UI::Colors::ColorWithMultipliedValue(UI::Colors::Theme::text, 1.4f), buttonColP);
		}

		//ImGui::Spring(-1.0f, 18.0f);
		//ImGui::EndHorizontal();

		//outTitlebarHeight = titlebarHeight;
    }
}
