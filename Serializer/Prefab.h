/******************************************************************************/
/*!
\file       Prefab.h
\author     Ang Jie Le Jet
\date       25 September 2023

\brief      This file consists of functions declarations for the Prefab class

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#ifndef _PREFAB_H_
#define _PREFAB_H_

#include <unordered_map>
#include <string>
#include <fstream>
#include "SOL/ECS/Components/Components.h"  // Include your base Components class
#include "SOL/ECS/Components/TransformComponent.h"  // Include your specific component types
#include "SOL/ECS/Components/MovementComponent.h"  // Include your specific component types
#include "SOL/ECS/Components/PrimitiveComponent.h"  // Include your specific component types
#include "SOL/ECS/Components/SpriteComponent.h"  // Include your specific component types
#include "SOL/ECS/Components/NameComponent.h"
#include "SOL/ECS/Components/PlayerComponent.h"
#include "SOL/ECS/Components/RigidBody2DComponent.h"
#include "SOL/ECS/Components/CameraComponent.h"
#include "SOL/ECS/Components/AnimationComponent.h"
#include "SOL/ECS/Components/FontComponent.h"
#include "SOL/ECS/Components/UIComponent.h"
#include "SOL/ECS/Components/GemComponent.h"
#include "SOL/ECS/Components/AudioComponent.h"
#include "SOL/ECS/Components/CPPScriptComponent.h"
#include "SOL/ECS/Components/EnemyComponent.h"
#include "SOL/ECS/Components/TileComponent.h"

#include "Serializer.h"  // Include your Serializer
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include "rapidjson/document.h"

namespace SOL
{
     /*!***********************************************************************
     \brief		Forward Declaration For Serializer class.
     *************************************************************************/
    class Serializer;

    class Prefab
    {
    public:

        using Writer = rapidjson::PrettyWriter<rapidjson::StringBuffer>;

        /*!***********************************************************************
        \brief		Constructor for Prefab class.
        *************************************************************************/
        Prefab()
        {
            //_____________________EXTEND WHEN NEW COMPONENTS IMPLEMENTED______________________________

            //temporary disable to tweak prefab functionality
            /*m_components.emplace("TransformComponent", static_cast<Components&>(m_TranslationComponent));
            m_components.emplace("MovementComponent", static_cast<Components&>(m_MovementComponent));
            m_components.emplace("PrimitiveComponent", static_cast<Components&>(m_PrimitiveComponent));
            m_components.emplace("SpriteComponent", static_cast<Components&>(m_SpriteComponent));
            m_components.emplace("NameComponent", static_cast<Components&>(m_NameComponent));
            m_components.emplace("PlayerComponent", static_cast<Components&>(m_PlayerComponent));
            m_components.emplace("RigidBody2DComponent", static_cast<Components&>(m_RigidBody2DComponent));*/
        }

        /*!***********************************************************************
        \brief		Destructor for Prefab class.
        *************************************************************************/
        ~Prefab() 
        {
        }

        /*!***********************************************************************
        \brief		gets intialized component in container of prefab
        *************************************************************************/
        Components& GetComponent(std::string type);

        /*!***********************************************************************
         \brief		Serializes saved data in prefab to write into json file
        *************************************************************************/
        void SerializePrefab(Serializer& serializer);

        /*!***********************************************************************
        \brief		Deserializes prefab and generate prefab ready to be loaded
                    into entity
        *************************************************************************/
        void DeserializePrefab(Serializer& serializer, const std::string& jsonString);

        /*!***********************************************************************
        \brief		Function to serialize a scene of entities
        *************************************************************************/
        void SerializeSceneEntity(Serializer& _Serializer, Writer& _Writer);

        /*!***********************************************************************
        \brief		Deserializes a scene json and generate all scene entities
        *************************************************************************/
        void DeserializeSceneEntity(Serializer& _Serializer, const rapidjson::Value& _jsonString);

        /*!***********************************************************************
        \brief		gets respective components from prefab by typename
        *************************************************************************/
        Components* CreateComponentByTypeName(const std::string& typeName);

        /*!***********************************************************************
        \brief		add a selected component to the prefab map for it to be valid
        *************************************************************************/
        void AddComponent(const std::string& typeName, Components& component);

        /*!***********************************************************************
        \brief		Function to test Serializer class (For Developers to Maintain/Debug code).
        *************************************************************************/
        static void SerializerDeserializerTester();

        /*!***********************************************************************
        \brief		Check if prefab has valid components
        *************************************************************************/
        bool IsPrefabValid();
        
        /*!***********************************************************************
        \brief		To clear prefab
        *************************************************************************/
        void ClearPrefab() 
        {
            for (auto& it : m_components)
            {
                it.second.FreeComponent();
            }
            m_components.clear(); 
        }

        /*!***********************************************************************
        \brief		check if prefab contains a specific component
        *************************************************************************/
        bool HasComponent(std::string _component);

        /*!***********************************************************************
        \brief		Get entity component map
        *************************************************************************/
        std::unordered_map<std::string, Components&> GetEntityComponentMap() { return m_components; }

    private:
         /*!***********************************************************************
         \brief		This is where you define the components variable.
         *************************************************************************/
        std::unordered_map<std::string, Components&> m_components;

        //____________EXTEND WHEN NEW COMPONENTS IMPLEMENTED(ADD MORE)________________________
        SOL::TransformComponent m_TransformComponent;
        SOL::MovementComponent m_MovementComponent;
        SOL::PrimitiveComponent m_PrimitiveComponent;
        SOL::SpriteComponent m_SpriteComponent;
        SOL::NameComponent m_NameComponent;
        SOL::PlayerComponent m_PlayerComponent;
        SOL::RigidBody2DComponent m_RigidBody2DComponent;
        SOL::FontComponent m_FontComponent;
        SOL::CameraComponent m_CameraComponent;
        SOL::AnimationComponent m_AnimationComponent;
        SOL::AudioComponent m_AudioComponent;
        SOL::GemComponent m_GemComponent;
        SOL::UIComponent m_UIComponent;
        SOL::CPPScriptComponent m_CPPScriptComponent;
        SOL::EnemyComponent m_EnemyComponent;
        SOL::TileComponent m_TileComponent;
    };
}

#endif  //_PREFAB_H_