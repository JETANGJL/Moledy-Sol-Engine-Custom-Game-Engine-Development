/******************************************************************************/
/*!
\file		Serializer.cpp
\author		Ang Jie Le Jet
\co-author	Xavier Lye Pin Liang
\date       25 September 2023

\brief  This file consists of functions definitions for the Serializer class

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include "SOLpch.h"
#include "Serializer.h"

namespace SOL
{
	/*!***********************************************************************
	\brief		Serializes a component using registered serializer function
	*************************************************************************/
	void SOL::Serializer::Serialize(Writer& writer, const std::string& typeName, const Components& component)
	{
		if (serializeFunctions.find(typeName) != serializeFunctions.end())
		{
			serializeFunctions[typeName](writer, component);
		}
	}

	/*!***********************************************************************
	\brief		Deserialize a component from a json file using registered 
				deserializer
	*************************************************************************/
	void SOL::Serializer::Deserialize(const Reader& reader, Components& component, const std::string& typeName)
	{
		if (deserializeFunctions.find(typeName) != deserializeFunctions.end())
		{
			deserializeFunctions[typeName](reader, component);
		}
	}

	/*!***********************************************************************
	\brief		Register serializer function for specific components
	*************************************************************************/
	void SOL::Serializer::RegisterSerializeFunction(const std::string& typeName, std::function<void(Writer&, const Components&)> func)
	{
		serializeFunctions[typeName] = func;
	}

	/*!***********************************************************************
	\brief		Register deserializer function for specific components
	*************************************************************************/
	void SOL::Serializer::RegisterDeserializeFunction(const std::string& typeName, std::function<void(const Reader&, Components&)> func)
	{
		deserializeFunctions[typeName] = func;
	}

	/*!***********************************************************************
	\brief		Converts json file into buffer string
	*************************************************************************/
	std::string Serializer::readJsonFile(const std::string& filePath)
	{
		std::ifstream file(filePath);
		if (!file.is_open()) {
			std::cerr << "Could not open the file: " << filePath << std::endl;
			return "";
		}

		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	//____________________________EXTEND WHEN NEW COMPONENTS IMPLEMENTED(ADD MORE)_______________________________

	/*!***********************************************************************
	\brief		Serializes function for transform component
	*************************************************************************/
	void SOL::Serializer::SerializeTransformComponent(Writer& writer, const Components& component)
	{
		const SOL::TransformComponent& transformComponent = static_cast<const SOL::TransformComponent&>(component);

		writer.StartObject();

		writer.String("Identity");
		writer.Int(transformComponent.m_EntityIdentity);

		writer.String("Transform");
		writer.StartArray();
		writer.Double(transformComponent.m_Transform.x);
		writer.Double(transformComponent.m_Transform.y);
		writer.Double(transformComponent.m_TransformZ);

		writer.EndArray();

		writer.String("Scale");
		writer.StartArray();
		writer.Double(transformComponent.m_Scale.x);
		writer.Double(transformComponent.m_Scale.y);
		writer.EndArray();

		writer.String("Rotation");
		writer.Double(transformComponent.m_Rotation);
		writer.EndObject();
	}

	/*!***********************************************************************
	\brief		deserializes function for transform component
	*************************************************************************/
	void SOL::Serializer::DeserializeTransformComponent(const Reader& reader, Components& component)
	{
		SOL::TransformComponent& transformComponent = static_cast<SOL::TransformComponent&>(component);

		if (reader.HasMember("Identity"))
		{
			transformComponent.m_EntityIdentity = reader["Identity"].GetInt();
		}

		if (reader.HasMember("Transform") && reader["Transform"].IsArray())
		{
			const auto& array = reader["Transform"].GetArray();
			transformComponent.m_Transform.x = array[0].GetFloat();
			transformComponent.m_Transform.y = array[1].GetFloat();
			transformComponent.m_TransformZ = array[2].GetFloat();

		}

		if (reader.HasMember("Scale") && reader["Scale"].IsArray())
		{
			const auto& array = reader["Scale"].GetArray();
			transformComponent.m_Scale.x = array[0].GetFloat();
			transformComponent.m_Scale.y = array[1].GetFloat();
		}
		if (reader.HasMember("Rotation") && reader["Rotation"].IsNumber())
		{
			transformComponent.m_Rotation = reader["Rotation"].GetFloat();
		}
		//if (reader.HasMember("EntityID") && reader["EntityID"].IsArray());// TO CHANGE
	}

	/*!***********************************************************************
	\brief		Serializes function for movement component
	*************************************************************************/
	void SOL::Serializer::SerializeMovementComponent(Writer& writer, const Components& component)
	{
		const SOL::MovementComponent& MovementComponent = static_cast<const SOL::MovementComponent&>(component);

		writer.StartObject();

		writer.String("Identity");
		writer.Int(MovementComponent.m_EntityIdentity);

		writer.String("Direction");
		writer.StartArray();
		writer.Double(MovementComponent.m_Direction.x);
		writer.Double(MovementComponent.m_Direction.y);
		writer.EndArray();

		writer.String("Speed");
		writer.Double(MovementComponent.m_Speed);

		writer.EndObject();
	}

	/*!***********************************************************************
	\brief		deserializes function for movement component
	*************************************************************************/
	void SOL::Serializer::DeserializeMovementComponent(const Reader& reader, Components& component)
	{
		SOL::MovementComponent& MovementComponent = static_cast<SOL::MovementComponent&>(component);

		if (reader.HasMember("Identity"))
		{
			MovementComponent.m_EntityIdentity = reader["Identity"].GetInt();
		}

		if (reader.HasMember("Direction") && reader["Direction"].IsArray())
		{
			const auto& array = reader["Direction"].GetArray();

			MovementComponent.m_Direction.x = array[0].GetFloat();
			MovementComponent.m_Direction.y = array[1].GetFloat();
		}
		if (reader.HasMember("Speed") && reader["Speed"].IsNumber())
		{
			MovementComponent.m_Speed = reader["Speed"].GetFloat();
		}
	}

	/*!***********************************************************************
	\brief		Serializes function for primitive component
	*************************************************************************/
	void SOL::Serializer::SerializePrimitiveComponent(Writer& writer, const Components& component)
	{
		const SOL::PrimitiveComponent& primitiveComponent = static_cast<const SOL::PrimitiveComponent&>(component);

		writer.StartObject();

		writer.String("Identity");
		writer.Int(primitiveComponent.m_EntityIdentity);

		// Serialize PrimitiveID
		writer.String("PrimitiveID");
		writer.Int(primitiveComponent.m_PrimitiveID);

		// Serialize Offset
		writer.String("Offset");
		writer.Double(primitiveComponent.m_Offset);

		// Serialize Color
		writer.String("Color");
		writer.StartArray();
		writer.Double(primitiveComponent.m_Color.x);
		writer.Double(primitiveComponent.m_Color.y);
		writer.Double(primitiveComponent.m_Color.z);
		writer.EndArray();

		// Serialize Alpha
		writer.String("Alpha");
		writer.Double(primitiveComponent.m_Alpha);
		writer.EndObject();
	}

	/*!***********************************************************************
	\brief		deserializes function for Primitive component
	*************************************************************************/
	void SOL::Serializer::DeserializePrimitiveComponent(const Reader& reader, Components& component)
	{
		SOL::PrimitiveComponent& primitiveComponent = static_cast<SOL::PrimitiveComponent&>(component);

		if (reader.HasMember("Identity"))
		{
			primitiveComponent.m_EntityIdentity = reader["Identity"].GetInt();
		}

		// Deserialize PrimitiveID
		if (reader.HasMember("PrimitiveID"))
		{
			primitiveComponent.m_PrimitiveID = reader["PrimitiveID"].GetInt();
		}

		// Deserialize Offset
		if (reader.HasMember("Offset"))
		{
			primitiveComponent.m_Offset = reader["Offset"].GetFloat();
		}

		// Deserialize Color
		if (reader.HasMember("Color") && reader["Color"].IsArray())
		{
			const auto& colorArray = reader["Color"].GetArray();
			if (colorArray.Size() == 3)
			{
				primitiveComponent.m_Color.x = colorArray[0].GetFloat();
				primitiveComponent.m_Color.y = colorArray[1].GetFloat();
				primitiveComponent.m_Color.z = colorArray[2].GetFloat();
			}
		}

		// Deserialize Alpha
		if (reader.HasMember("Alpha"))
		{
			primitiveComponent.m_Alpha = reader["Alpha"].GetFloat();
		}
	}

	/*!***********************************************************************
	\brief		Serializes function for sprite component
	*************************************************************************/
	void SOL::Serializer::SerializeSpriteComponent(Writer& writer, const Components& component)
	{
		const SOL::SpriteComponent& spriteComponent = static_cast<const SOL::SpriteComponent&>(component);

		writer.StartObject();

		writer.String("Identity");
		writer.Int(spriteComponent.m_EntityIdentity);

		// Serialize TexKey
		writer.String("TexKey");
		writer.String(spriteComponent.m_TexKey.c_str());

		writer.String("UUID");
		writer.Uint64(spriteComponent.UUID);

		writer.String("Width");
		writer.Double(spriteComponent.m_SpriteWidth);

		writer.String("Height");
		writer.Double(spriteComponent.m_SpriteHeight);

		writer.String("Alpha");
		writer.Double(spriteComponent.m_Alpha);

		// Serialize Color
		writer.String("Color");
		writer.StartArray();
		writer.Double(spriteComponent.m_Color.x);
		writer.Double(spriteComponent.m_Color.y);
		writer.Double(spriteComponent.m_Color.z);
		writer.EndArray();

		writer.EndObject();
	}

	/*!***********************************************************************
	\brief		Serializes function for Name component
	*************************************************************************/
	void Serializer::SerializeNameComponent(Writer& writer, const Components& component)
	{
		const SOL::NameComponent& NameComponent = static_cast<const SOL::NameComponent&>(component);

		writer.StartObject();

		writer.String("Identity");
		writer.Int(NameComponent.m_EntityIdentity);

		// Serialize Name
		writer.String("Name");
		writer.String(NameComponent.m_name.c_str());

		writer.EndObject();
	}

	/*!***********************************************************************
	\brief		Serializes function for Player component
	*************************************************************************/
	void Serializer::SerializePlayerComponent(Writer& writer, const Components& component)
	{
		const SOL::PlayerComponent& PlayerComponent = static_cast<const SOL::PlayerComponent&>(component);

		writer.StartObject();

		writer.String("Identity");
		writer.Int(PlayerComponent.m_EntityIdentity);

		writer.String("TransformAmount");
		writer.Int(PlayerComponent.transformAmount);

		writer.String("MoveSpeed");
		writer.Int(PlayerComponent.moveSpeed);

		//writer.String("testing");
		//writer.Double(1);
		writer.EndObject();
	}

	/*!***********************************************************************
	\brief		Serializes function for Rigidbody component
	*************************************************************************/
	void Serializer::SerializeRigidBody2DComponent(Writer& writer, const Components& component)
	{
		const SOL::RigidBody2DComponent& RigidComponent = static_cast<const SOL::RigidBody2DComponent&>(component);
		writer.StartObject();

		writer.String("Identity");
		writer.Int(RigidComponent.m_EntityIdentity);

		writer.String("Position");
		writer.StartArray();
		writer.Double(RigidComponent.m_body.position.x);
		writer.Double(RigidComponent.m_body.position.y);
		writer.EndArray();

		writer.String("Width");
		writer.StartArray();
		writer.Double(RigidComponent.m_body.width.x);
		writer.Double(RigidComponent.m_body.width.y);
		writer.EndArray();

		writer.String("Mass");
		writer.Double(RigidComponent.m_body.mass);

		writer.String("BodyType");
		writer.Int(RigidComponent.m_body.bodytype);

		writer.String("Offset");
		writer.Double(RigidComponent.m_offset);

		writer.String("Friction");
		writer.Double(RigidComponent.m_body.friction);

		writer.EndObject();
	}

	/*!***********************************************************************
	\brief		Serializes function for Camera component
	*************************************************************************/
	void Serializer::SerializeCameraComponent(Writer& writer, const Components& component)
	{
		const SOL::CameraComponent& Cameracomponent = static_cast<const SOL::CameraComponent&>(component);
		writer.StartObject();

		writer.String("m_Active");
		writer.Bool(Cameracomponent.m_IsActive);

		writer.String("m_SmoothDampActive");
		writer.Bool(Cameracomponent.m_SmoothDampActive);

		writer.String("Identity");
		writer.Int(Cameracomponent.m_EntityIdentity);

		writer.String("m_Fov");
		writer.Double(Cameracomponent.m_FOV);

		writer.String("m_PerspectiveNear");
		writer.Double(Cameracomponent.m_PerspectiveNear);

		writer.String("m_PerspectiveFar");
		writer.Double(Cameracomponent.m_PerspectiveFar);

		writer.String("m_OrthoFar");
		writer.Double(Cameracomponent.m_OrthoFar);

		writer.String("m_OrthoNear");
		writer.Double(Cameracomponent.m_OrthoNear);

		writer.String("m_OrthoSize");
		writer.Double(Cameracomponent.m_OrthoSize);

		writer.String("m_CameraDistance");
		writer.Double(Cameracomponent.m_CameraDistance);


		writer.String("velocity");
		writer.StartArray();
		writer.Double(Cameracomponent.velocity.x);
		writer.Double(Cameracomponent.velocity.y);
		writer.EndArray();


		writer.EndObject();
	}

	/*!***********************************************************************
	\brief		Serializes function for font component
	*************************************************************************/
	void Serializer::SerializefontComponent(Writer& writer, const Components& component)
	{
		const SOL::FontComponent& Fontcomponent = static_cast<const SOL::FontComponent&>(component);
		writer.StartObject();

		writer.String("Identity");
		writer.Int(Fontcomponent.m_EntityIdentity);

		writer.String("UUID");
		writer.Uint64(Fontcomponent.UUID);

		writer.String("Text");
		writer.String(Fontcomponent.text.c_str());

		writer.String("Color");
		writer.StartArray();
		writer.Double(Fontcomponent.color.x);
		writer.Double(Fontcomponent.color.y);
		writer.Double(Fontcomponent.color.z);
		writer.EndArray();

		writer.EndObject();
	}

	/*!***********************************************************************
	\brief		Serializes function for animation component
	*************************************************************************/
	void Serializer::SerializeAnimationComponent(Writer& writer, const Components& component)
	{
		const SOL::AnimationComponent& Animcomponent = static_cast<const SOL::AnimationComponent&>(component);
		writer.StartObject();

		writer.String("Identity");
		writer.Int(Animcomponent.m_EntityIdentity);

		writer.String("MaxFrame");
		writer.Int(Animcomponent.m_MaxFrame);

		writer.String("CurrentFrameIndex");
		writer.Int(Animcomponent.m_CurrentFrameIndex);

		writer.String("StartingAnimIndex");
		writer.Int(Animcomponent.m_StartingAnimationIndex);

		writer.String("Interval");
		writer.Double(Animcomponent.m_Interval);


		writer.EndObject();
	}

	/*!***********************************************************************
	\brief		Serializes function for gem component
	*************************************************************************/
	void Serializer::SerializeGemComponent(Writer& writer, const Components& component)
	{
		const SOL::GemComponent& _Component = static_cast<const SOL::GemComponent&>(component);
		writer.StartObject();

		writer.String("Identity");
		writer.Int(_Component.m_EntityIdentity);
		writer.EndObject();
	}

	/*!***********************************************************************
	\brief		Serializes function for UI component
	*************************************************************************/
	void Serializer::SerializeUIComponent(Writer& writer, const Components& component)
	{
		const SOL::UIComponent& _Component = static_cast<const SOL::UIComponent&>(component);
		writer.StartObject();

		writer.String("Identity");
		writer.Int(_Component.m_EntityIdentity);
		writer.EndObject();
	}

	/*!***********************************************************************
	\brief		Serializes function for audio component
	*************************************************************************/
	void Serializer::SerializeAudioComponent(Writer& writer, const Components& component)
	{
		const SOL::AudioComponent& _Component = static_cast<const SOL::AudioComponent&>(component);
		writer.StartObject();

		writer.String("Identity");
		writer.Int(_Component.m_EntityIdentity);
		
		writer.String("AudioControlMap");
		writer.StartArray();
		for (auto& _audiocontroller : _Component.m_AudioControlMap)
		{
			//writer.String(_audiocontroller.first.c_str());
			writer.StartObject();
				writer.Key(_audiocontroller.first.c_str());
				writer.StartObject();
				//writer.Key(_audiocontroller.first.c_str());

				writer.String("UUID");
				writer.Uint64(_audiocontroller.second.UUID);

				writer.String("AudioKey");
				writer.String(_audiocontroller.second.m_AudioKey.c_str());

				writer.String("Loop");
				writer.Bool(_audiocontroller.second.m_IsLooping);

				writer.String("Volume");
				writer.Double(_audiocontroller.second.m_Volume);
			
				writer.EndObject();
			writer.EndObject();
		}
		
		writer.EndArray();

		writer.EndObject();
	}

	/*!***********************************************************************
	\brief		Serializes function for enemy component
	*************************************************************************/
	void Serializer::SerializeEnemyComponent(Writer& writer, const Components& component)
	{
		const SOL::EnemyComponent& _Component = static_cast<const SOL::EnemyComponent&>(component);
		writer.StartObject();

		writer.String("Identity");
		writer.Int(_Component.m_EntityIdentity);

		writer.String("MaxDelta");
		writer.StartArray();
		writer.Double(_Component.m_maxDelta.x);
		writer.Double(_Component.m_maxDelta.y);
		writer.EndArray();


		writer.EndObject();
	}

	/*!***********************************************************************
	\brief		Serializes function for tile component
	*************************************************************************/
	void Serializer::SerializeTileComponent(Writer& writer, const Components& component)
	{
		const SOL::TileComponent& _Component = static_cast<const SOL::TileComponent&>(component);
		writer.StartObject();

		writer.String("Identity");
		writer.Int(_Component.m_EntityIdentity);

		writer.String("TileType");
		writer.Uint(_Component.m_TileType);

		writer.EndObject();
	}

	/*!***********************************************************************
	\brief		Serializes function for CPP script component
	*************************************************************************/
	void Serializer::SerializeCPPScriptComponent(Writer& writer, const Components& component)
	{
		const SOL::CPPScriptComponent& _Component = static_cast<const SOL::CPPScriptComponent&>(component);
		writer.StartObject();

		writer.String("Identity");
		writer.Int(_Component.m_EntityIdentity);

		writer.String("CPPScripts");
		writer.StartArray();
		for (auto& it : _Component.m_Scripts)
		{
			writer.Uint(it.first);
		}
		writer.EndArray();

		writer.EndObject();
	}

	
	/*!***********************************************************************
	\brief		deSerializes function for sprite component
	*************************************************************************/
	void SOL::Serializer::DeserializeSpriteComponent(const Reader& reader, Components& component)
	{
		SOL::SpriteComponent& spriteComponent = static_cast<SOL::SpriteComponent&>(component);

		if (reader.HasMember("Identity"))
		{
			spriteComponent.m_EntityIdentity = reader["Identity"].GetInt();
		}

		// Deserialize TextureKey
		if (reader.HasMember("TexKey"))
		{
			spriteComponent.m_TexKey = reader["TexKey"].GetString();
		}

		if (reader.HasMember("UUID"))
		{
			spriteComponent.UUID = reader["UUID"].GetUint64();
		}

		if (reader.HasMember("Width"))
		{
			spriteComponent.m_SpriteWidth = reader["Width"].GetFloat();
		}

		if (reader.HasMember("Height"))
		{
			spriteComponent.m_SpriteHeight = reader["Height"].GetFloat();
		}

		if (reader.HasMember("Alpha"))
		{
			spriteComponent.m_Alpha = reader["Alpha"].GetFloat();
		}

		// Deserialize Color
		if (reader.HasMember("Color") && reader["Color"].IsArray())
		{
			const auto& colorArray = reader["Color"].GetArray();
			if (colorArray.Size() == 3)
			{
				spriteComponent.m_Color.x = colorArray[0].GetFloat();
				spriteComponent.m_Color.y = colorArray[1].GetFloat();
				spriteComponent.m_Color.z = colorArray[2].GetFloat();
			}
		}
	}
	/*!***********************************************************************
	\brief		deserializes function for Name component
	*************************************************************************/
	void SOL::Serializer::DeserializeNameComponent(const Reader& reader, Components& component)
	{
		SOL::NameComponent& Name = static_cast<SOL::NameComponent&>(component);

		if (reader.HasMember("Identity"))
		{
			Name.m_EntityIdentity = reader["Identity"].GetInt();
		}

		if (reader.HasMember("Name"))
		{
			Name.m_name = reader["Name"].GetString();
		}
	}
	/*!***********************************************************************
	\brief		deserializes function for player component
	*************************************************************************/
	void SOL::Serializer::DeserializePlayerComponent(const Reader& reader, Components& component)
	{
		SOL::PlayerComponent& Player = static_cast<SOL::PlayerComponent&>(component);

		if (reader.HasMember("Identity"))
		{
			Player.m_EntityIdentity = reader["Identity"].GetInt();
		}

		if (reader.HasMember("TransformAmount"))
		{
			Player.transformAmount = reader["TransformAmount"].GetInt();
		}

		if (reader.HasMember("MoveSpeed"))
		{
			Player.moveSpeed = reader["MoveSpeed"].GetInt();
		}
	}
	/*!***********************************************************************
	\brief		deserializes function for rigidbody component
	*************************************************************************/
	void SOL::Serializer::DeserializeRigidBody2DComponent(const Reader& reader, Components& component)
	{
		SOL::RigidBody2DComponent& Rigid2D = static_cast<SOL::RigidBody2DComponent&>(component);
		Vec2 Scale{};
		float Mass{};
		int body_type{};

		if (reader.HasMember("Identity"))
		{
			Rigid2D.m_EntityIdentity = reader["Identity"].GetInt();
		}

		if (reader.HasMember("Position") && reader["Position"].IsArray())
		{
			const auto& Pos = reader["Position"].GetArray();
			Rigid2D.m_body.position.x = Pos[0].GetFloat();
			Rigid2D.m_body.position.y = Pos[1].GetFloat();
		}
		if (reader.HasMember("Width") && reader["Width"].IsArray())
		{
			const auto& Width = reader["Width"].GetArray();
			/*Rigid2D.m_body.width.x = Width[0].GetFloat();
			Rigid2D.m_body.width.y = Width[1].GetFloat();*/
			Scale.x = Width[0].GetFloat();
			Scale.y = Width[1].GetFloat();
		}
		if (reader.HasMember("Mass"))
		{
			//Rigid2D.m_body.mass = reader["Mass"].GetFloat();
			Mass = reader["Mass"].GetFloat();

		}

		if (reader.HasMember("BodyType"))
		{
			body_type = reader["BodyType"].GetInt();
		}

		Rigid2D.m_body.Set(Scale, Mass, static_cast<BodyType>(body_type));

		if (reader.HasMember("Offset"))
		{
			Rigid2D.m_offset = (int)(reader["Offset"].GetFloat());
		}

		if (reader.HasMember("Friction"))
		{
			Rigid2D.m_body.friction = reader["Friction"].GetFloat();
		}
	}

	/*!***********************************************************************
	\brief		deserializes function for camera component
	*************************************************************************/
	void SOL::Serializer::DeserializeCameraComponent(const Reader& reader, Components& component)
	{
		SOL::CameraComponent& Camera = static_cast<SOL::CameraComponent&>(component);

		if (reader.HasMember("Identity"))
		{
			Camera.m_EntityIdentity = reader["Identity"].GetInt();
		}

		if (reader.HasMember("m_Active"))
		{
			Camera.m_Active = reader["m_Active"].GetBool();
		}	

		if (reader.HasMember("m_SmoothDampActive"))
		{
			Camera.m_SmoothDampActive = reader["m_SmoothDampActive"].GetBool();
		}
		
		if (reader.HasMember("m_FOV"))
		{
			Camera.m_FOV = reader["m_FOV"].GetFloat();
		}

		if (reader.HasMember("m_FOV"))
		{
			Camera.m_FOV = reader["m_FOV"].GetFloat();
		}

		if (reader.HasMember("m_PerspectiveNear"))
		{
			Camera.m_PerspectiveNear = reader["m_PerspectiveNear"].GetFloat();
		}
		if (reader.HasMember("m_PerspectiveFar"))
		{
			Camera.m_PerspectiveFar = reader["m_PerspectiveFar"].GetFloat();
		}
		if (reader.HasMember("m_OrthoFar"))
		{
			Camera.m_OrthoFar = reader["m_OrthoFar"].GetFloat();
		}
		if (reader.HasMember("m_OrthoNear"))
		{
			Camera.m_OrthoNear = reader["m_OrthoNear"].GetFloat();
		}
		if (reader.HasMember("m_OrthoSize"))
		{
			Camera.m_OrthoSize = reader["m_OrthoSize"].GetFloat();
		}
		if (reader.HasMember("m_CameraDistance"))
		{
			Camera.m_CameraDistance = reader["m_CameraDistance"].GetFloat();
		}

		if (reader.HasMember("velocity") && reader["velocity"].IsArray())
		{
			const auto& velocity = reader["velocity"].GetArray();
			Camera.velocity.x = velocity[1].GetFloat();
			Camera.velocity.y = velocity[0].GetFloat();
		}
	}

	/*!***********************************************************************
	\brief		deserializes function for font component
	*************************************************************************/
	void SOL::Serializer::DeserializeFontComponent(const Reader& reader, Components& component)
	{
		SOL::FontComponent& font = static_cast<SOL::FontComponent&>(component);

		if (reader.HasMember("Identity"))
		{
			font.m_EntityIdentity = reader["Identity"].GetInt();
		}

		if (reader.HasMember("UUID"))
		{
			font.UUID = reader["UUID"].GetUint64();
		}

		if (reader.HasMember("Text"))
		{
			font.text = reader["Text"].GetString();
		}

		if (reader.HasMember("Color"))
		{
			const auto& col = reader["Color"].GetArray();
			font.color.x = col[0].GetFloat();
			font.color.y = col[1].GetFloat();
			font.color.z = col[2].GetFloat();
		}
	}

	/*!***********************************************************************
	\brief		deserializes function for animation component
	*************************************************************************/
	void SOL::Serializer::DeserializeAnimationComponent(const Reader& reader, Components& component)
	{
		SOL::AnimationComponent& Anim = static_cast<SOL::AnimationComponent&>(component);

		if (reader.HasMember("Identity"))
		{
			Anim.m_EntityIdentity = reader["Identity"].GetInt();
		}

		if (reader.HasMember("MaxFrame"))
		{
			Anim.m_MaxFrame = reader["MaxFrame"].GetInt();
		}

		if (reader.HasMember("CurrentFrameIndex"))
		{
			Anim.m_CurrentFrameIndex = reader["CurrentFrameIndex"].GetInt();
		}

		if (reader.HasMember("StartingAnimIndex"))
		{
			Anim.m_StartingAnimationIndex = reader["StartingAnimIndex"].GetInt();
		}

		if (reader.HasMember("Interval"))
		{
			Anim.m_Interval = reader["Interval"].GetFloat();
		}
	}

	/*!***********************************************************************
	\brief		deserializes function for gem component
	*************************************************************************/
	void Serializer::DeserializeGemComponent(const Reader& reader, Components& component)
	{
		SOL::GemComponent& _Component = static_cast<SOL::GemComponent&>(component);

		if (reader.HasMember("Identity"))
		{
			_Component.m_EntityIdentity = reader["Identity"].GetInt();
		}
	}

	/*!***********************************************************************
	\brief		deserializes function for UI component
	*************************************************************************/
	void Serializer::DeserializeUIComponent(const Reader& reader, Components& component)
	{
		SOL::UIComponent& _Component = static_cast<SOL::UIComponent&>(component);

		if (reader.HasMember("Identity"))
		{
			_Component.m_EntityIdentity = reader["Identity"].GetInt();
		}
	}

	/*!***********************************************************************
	\brief		deserializes function for audio component
	*************************************************************************/
	void Serializer::DeserializeAudioComponent(const Reader& reader, Components& component)
	{
		SOL::AudioComponent& _Component = static_cast<SOL::AudioComponent&>(component);

		if (reader.HasMember("Identity"))
		{
			_Component.m_EntityIdentity = reader["Identity"].GetInt();
		}

		if (reader.HasMember("AudioControlMap"))
		{
			const auto& Controller = reader["AudioControlMap"].GetArray();
			for (auto x = 0; x < static_cast<size_t>(Controller.Size()); ++x)
			{
				AudioComponent::AudioControl buffer{};
				const auto& obj = Controller[x].GetObj();
				const auto& obj_it = obj.MemberBegin();
				std::string name = obj_it->name.GetString();

				if (obj_it->value.HasMember("UUID"))
				{
					buffer.UUID = obj_it->value["UUID"].GetUint64();
					//std::cout << buffer.UUID << '\n';
				}

				if (obj_it->value.HasMember("AudioKey"))
				{
					buffer.m_AudioKey = obj_it->value["AudioKey"].GetString();
				}

				if (obj_it->value.HasMember("Loop"))
				{
					buffer.m_IsLooping = obj_it->value["Loop"].GetBool();
				}

				if (obj_it->value.HasMember("Volume"))
				{
					buffer.m_Volume = obj_it->value["Volume"].GetFloat();
				}

				_Component.m_AudioControlMap.emplace(name, buffer);
			}
		}
	}

	/*!***********************************************************************
	\brief		deserializes function for enemy component
	*************************************************************************/
	void Serializer::DeserializeEnemyComponent(const Reader& reader, Components& component)
	{
		SOL::EnemyComponent& _Component = static_cast<SOL::EnemyComponent&>(component);

		if (reader.HasMember("Identity"))
		{
			_Component.m_EntityIdentity = reader["Identity"].GetInt();
		}

		if (reader.HasMember("MaxDelta"))
		{
			const auto& maxDelta = reader["MaxDelta"].GetArray();
			_Component.m_maxDelta.x = maxDelta[0].GetFloat();
			_Component.m_maxDelta.y = maxDelta[1].GetFloat();
		}
	}

	/*!***********************************************************************
	\brief		deserializes function for tile component
	*************************************************************************/
	void Serializer::DeserializeTileComponent(const Reader& reader, Components& component)
	{
		SOL::TileComponent& _Component = static_cast<SOL::TileComponent&>(component);

		if (reader.HasMember("Identity"))
		{
			_Component.m_EntityIdentity = reader["Identity"].GetInt();
		}

		if (reader.HasMember("TileType"))
		{
			_Component.m_TileType = (TileComponent::TileType)reader["TileType"].GetUint();
		}
	}

	/*!***********************************************************************
	\brief		deserializes function for CPP script component
	*************************************************************************/
	void Serializer::DeserializeCPPScriptComponent(const Reader& reader, Components& component)
	{
		SOL::CPPScriptComponent& _Component = static_cast<SOL::CPPScriptComponent&>(component);

		if (reader.HasMember("Identity"))
		{
			_Component.m_EntityIdentity = reader["Identity"].GetInt();
		}

		if (reader.HasMember("CPPScripts"))
		{
			const auto& scripts = reader["CPPScripts"].GetArray();
			for (auto& it : scripts)
			{
				_Component.m_Scripts.emplace((CPPScript_Type)it.GetUint(), nullptr);
			}
		}
	}
}