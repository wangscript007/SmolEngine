#include "Editor.h"
#include "Core/EntryPoint.h"

#include "EditorLayer.h"
#include "VulkanTestLayer.h"
#include "RaytracingTestLayer.h"
#include "PBRTestLayer.h"


namespace SmolEngine
{
	Editor::Editor()
	{

	}

	Editor::~Editor()
	{

	}

	void Editor::ClientInit()
	{
		auto& app = Application::GetApplication();

#ifdef SMOLENGINE_OPENGL_IMPL
		//app.PushLayer(new EditorLayer);
#else
		app.PushLayer(new PBRTestLayer);
#endif
		//app.PushLayer(new EditorLayer);
		EDITOR_INFO("Initialized successfully");
	}


	Application* CreateApp()
	{
		return new Editor;
	}
}
