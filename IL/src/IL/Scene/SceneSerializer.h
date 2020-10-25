#pragma once

#include "Scene.h"

namespace IL
{

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void SerializeYaml(const std::string& filepath);
		void SerializeBinary(const std::string& filepath);

		bool DeserializeYaml(const std::string& filepath);
		bool DeserializeBinary(const std::string& filepath);
	private:
		Ref<Scene> m_Scene;
	};

}