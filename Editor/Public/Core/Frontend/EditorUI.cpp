// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "EditorUI.hpp"
#include <iostream>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Editor
{
	void EditorUI::ShowMainWindow()
	{

		ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration;
		WindowFlags					|= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ConstPtr<ImGuiViewport> Viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(Viewport->Pos);
		ImGui::SetNextWindowSize(Viewport->Size);
		ImGui::SetNextWindowViewport(Viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("Dockspace", nullptr, WindowFlags);

		ImGui::PopStyleVar(3);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New File"))
				{
					/*new file*/
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Open File..."))
				{
					/*Load file*/
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Save"))
				{
					/*save file*/
				}

				if (ImGui::MenuItem("Save As..."))
				{
					/*Load file*/
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit"))
				{
					/*close app*/
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools"))
			{

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Windows"))
			{
				if (ImGui::MenuItem("Reset Layout"))
				{
					ResetDocking();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGuiID DockspaceID = ImGui::GetID("Dockspace");
		ImGui::DockSpace(DockspaceID, ImVec2(0.0f, 0.0f));

		if (mResetDock)
		{
			mResetDock = false;

			//Clear existing layout
			ImGui::DockBuilderRemoveNode(DockspaceID);
			ImGui::DockBuilderAddNode(DockspaceID, ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(DockspaceID, Viewport->Size);

			//Main regions of the main dockspace
			ImGuiID LeftDock	= ImGui::DockBuilderSplitNode(DockspaceID, ImGuiDir::ImGuiDir_Left, 0.2f, nullptr, &DockspaceID);
			ImGuiID RightDock	= ImGui::DockBuilderSplitNode(DockspaceID, ImGuiDir::ImGuiDir_Right, 0.2f, nullptr, &DockspaceID);
			ImGuiID BottomDock	= ImGui::DockBuilderSplitNode(DockspaceID, ImGuiDir::ImGuiDir_Down, 0.2f, nullptr, &DockspaceID);
			
			//Regions in the left dockspace
			ImGuiID LeftBotDock = ImGui::DockBuilderSplitNode(LeftDock, ImGuiDir::ImGuiDir_Down, 0.2f, nullptr, &LeftDock);
			ImGuiID LeftTopDock = LeftDock; //No need to split again, as it uses the remaining (used just for clarify)
			

			ImGui::DockBuilderDockWindow("Tools", LeftTopDock);
			ImGui::DockBuilderDockWindow("Test", LeftBotDock);
			ImGui::DockBuilderDockWindow("Main Viewport", DockspaceID);
			ImGui::DockBuilderDockWindow("Details", RightDock);
			ImGui::DockBuilderDockWindow("Console", BottomDock);

			ImGui::DockBuilderFinish(DockspaceID);
		}
		ImGui::End();

		ImGui::Begin("Tools");
		{
			if (ImGui::Button("Click me", ImVec2(50.0f, 100.0f)))
			{

			}

		}
		ImGui::End();

		ImGui::Begin("Main Viewport");
		{

		}
		ImGui::End();

		ImGui::Begin("Details");
		{

		}
		ImGui::End();
		ImGui::Begin("Console");
		{

		}
		ImGui::End();
		ImGui::Begin("Test");
		{

		}
		ImGui::End();
	}

	void EditorUI::ResetDocking()
	{
		mResetDock = true;
	}
}
