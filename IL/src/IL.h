#pragma once

// For use by application

#include <stdio.h>

#include "IL/Core/Application.h"
#include "IL/Layer/Layer.h"
#include "IL/Core/Log.h"

#include "IL/Core/TimeStep.h"
#include "IL/Debug/Profiler.h"

#include "IL/Input/Input.h"
#include "IL/Input/KeyCodes.h"
#include "IL/Input/MouseButtonCodes.h"

#include "IL/ImGui/ImGuiLayer.h"

#include "IL/Input/OrthographicCameraController.h"

#include "IL/Scene/Components.h"
#include "IL/Scene/Scene.h"
#include "IL/Scene/Entity.h"
#include "IL/Scene/ScriptableEntity.h"

#include "IL/Events/Event.h"
#include "IL/Events/MouseEvent.h"
#include "IL/Events/KeyEvent.h"
#include "IL/Events/ApplicationEvent.h"

// -------------Renderer-----------------

#include "IL/Renderer/Buffer.h"
#include "Il/Renderer/VertexArray.h"
#include "IL/Renderer/Renderer.h"
#include "IL/Renderer/Renderer2D.h"
#include "IL/Renderer/RenderCommand.h"
#include "IL/Renderer/Camera/Camera.h"
#include "IL/Renderer/Shader.h"
#include "IL/Renderer/FrameBuffer.h"
#include "IL/Renderer/Texture.h"
#include "IL/Renderer/SubTexture2D.h"

