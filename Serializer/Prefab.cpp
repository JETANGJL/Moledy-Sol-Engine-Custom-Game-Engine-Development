/******************************************************************************/
/*!
\file       Prefab.cpp
\author     Ang Jie Le Jet
\co-author  Xavier Lye Pin Liang
\date       25 September 2023

\brief      This file consists of functions definitions for the Prefab class

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include "SOLpch.h"
#include "Prefab.h"

namespace SOL
{

    /*!***********************************************************************
        \brief		Serializes saved data in prefab to write into json file
    *************************************************************************/
    void Prefab::SerializePrefab(Serializer& serializer)
    {
        // Initialize rapidjson Writer
        rapidjson::StringBuffer strbuf;
        Serializer::Writer writer(strbuf);

        // Serialize the components
        writer.StartObject();
        for (const auto& pair : m_components)
        {
            writer.String(pair.first.c_str());  // Write the type name as the JSON key
            serializer.Serialize(writer, pair.first, pair.second);  // Pass the type name along
        }
        writer.EndObject();

        // strbuf now contains the JSON string
        const char* jsonString = strbuf.GetString();

        std::cout << "Serialized JSON String: " << jsonString << std::endl;

        // Save jsonString to a text file
        std::ofstream outFile("./Json/EditedScene.json");
        if (outFile.is_open()) 
        {
            outFile << jsonString;
            outFile.close();
        }
        else 
        {
            std::cerr << "Could not open file for writing.\n";
        } 
    }


    /*!***********************************************************************
    \brief		Deserializes prefab and generate prefab ready to be loaded
                into entity
    *************************************************************************/
    void SOL::Prefab::DeserializePrefab(Serializer& serializer, const std::string& jsonString)
    {
        rapidjson::Document doc;
        doc.Parse(jsonString.c_str());

        if (!doc.IsObject())
        {
            ENGINE_ERROR("Error: Not a valid JSON object.\n");
            return;
        }

        for (auto it = doc.MemberBegin(); it != doc.MemberEnd(); ++it)
        {
            const std::string typeName = it->name.GetString();// Check for "NAME"

            // Fetch empty component of this type as in component type
            Components* P_Component = CreateComponentByTypeName(typeName);
            if(P_Component != nullptr)
            {
                //Components& newComponent = *P_Component;
                //m_components.emplace(typeName, *P_Component);
                AddComponent(typeName, *P_Component);

                //serializer.deserializeFunctions.at(typeName)(it->value, *P_Component);
                serializer.Deserialize(it->value, *P_Component, typeName);
                //AddComponent(typeName, newComponent);
            }
            else
            {
                // Handle error, log it
                ENGINE_ERROR("Unknown component type");
                std::cerr << "Error: Unknown component type " << typeName << ".\n";
            }
        }
    }

    /*!***********************************************************************
    \brief		Function to serialize a scene of entities
    *************************************************************************/
    void Prefab::SerializeSceneEntity(Serializer& _Serializer, Writer& _Writer)
    {
        for (const auto& pair : m_components)
        {
            _Writer.String(pair.first.c_str());  // Write the type name as the JSON key
            _Serializer.Serialize(_Writer, pair.first, pair.second);
        }
    }

    /*!***********************************************************************
    \brief		Deserializes a scene json and generate all scene entities
    *************************************************************************/
    void Prefab::DeserializeSceneEntity(Serializer& serializer, const rapidjson::Value& element)
    {
        /*rapidjson::Document doc;
        doc.Parse(jsonString.c_str());

        if (!doc.IsObject())
        {
            ENGINE_ERROR("Error: Not a valid JSON object.\n");
            return;
        }
        const auto& array = doc.GetArray();
        for (const auto& element : array)
        {*/
            for (auto it = element.MemberBegin(); it != element.MemberEnd(); ++it)
            {
                const std::string typeName = it->name.GetString();// Check for "NAME"

                // Fetch empty component of this type as in component type
                Components* P_Component = CreateComponentByTypeName(typeName);
                if (P_Component != nullptr)
                {
                    //Components& newComponent = *P_Component;
                    //m_components.emplace(typeName, *P_Component);
                    AddComponent(typeName, *P_Component);

                    //serializer.deserializeFunctions.at(typeName)(it->value, *P_Component);
                    serializer.Deserialize(it->value, *P_Component, typeName);
                    //AddComponent(typeName, newComponent);
                }
                else
                {
                    // Handle error, log it
                    ENGINE_ERROR("Unknown component type");
                    std::cerr << "Error: Unknown component type " << typeName << ".\n";
                }
            }
        /*}*/
        
    }

    // ------------------------------------- might delete, not used ---------------------------- //
        /*!***********************************************************************
        \brief		add a selected component to the prefab map for it to be valid
        *************************************************************************/
    void Prefab::AddComponent(const std::string& typeName, Components& component)
    {
        //m_components.at(typeName) = component;
        m_components.emplace(typeName, component);

        //m_components[typeName] = component;
        //m_components.emplace(typeName, component);
    }

    /*!***********************************************************************
    \brief		gets respective components from prefab by typename
    *************************************************************************/
    Components* Prefab::CreateComponentByTypeName(const std::string& typeName)
    {
        //_____________EXTEND WHEN NEW COMPONENTS IMPLEMENTED_________
        if (typeName == "TransformComponent")
        {
            return &m_TransformComponent;
        }
        else if (typeName == "MovementComponent")
        {
            return &m_MovementComponent;
        }
        else if (typeName == "PrimitiveComponent")
        {
            return &m_PrimitiveComponent;
        }
        else if (typeName == "SpriteComponent")
        {
            return &m_SpriteComponent;
        }
        else if (typeName == "PlayerComponent")
        {
            return &m_PlayerComponent;
        }
        else if (typeName == "NameComponent")
        {
            return &m_NameComponent;
        }
        else if (typeName == "RigidBody2DComponent")
        {
            return &m_RigidBody2DComponent;
        }
        else if (typeName == "FontComponent")
        {
            return &m_FontComponent;
        }
        else if (typeName == "CameraComponent")
        {
            return &m_CameraComponent;
        }
        else if (typeName == "AnimationComponent")
        {
            return &m_AnimationComponent;
        }
        else if (typeName == "GemComponent")
        {
            return &m_GemComponent;
        }
        else if (typeName == "UIComponent")
        {
            return &m_UIComponent;
        }
        else if (typeName == "AudioComponent")
        {
            return &m_AudioComponent;
        }
        else if (typeName == "EnemyComponent")
        {
            return &m_EnemyComponent;
        }
        else if (typeName == "TileComponent")
        {
            return &m_TileComponent;
        }
        else if (typeName == "CPPScriptComponent")
        {
            return &m_CPPScriptComponent;
        }

        //switch()
        //____________EXTEND WHEN NEW COMPONENTS IMPLEMENTED_________
        else
        {
            ENGINE_ERROR("Create component by type name failed");
            return nullptr;
        }
    }

    /*!***********************************************************************
    \brief		gets intialized component in container of prefab
    *************************************************************************/
    Components& SOL::Prefab::GetComponent(std::string type) 
    {      
        return m_components.at(type);
    }

    /*!***********************************************************************
     \brief		Used to test condition of serializer
     *************************************************************************/
    void SOL::Prefab::SerializerDeserializerTester() 
    {
        ENGINE_INFO("TESTING SERIALIZER");
        SOL::Serializer serializer;
        SOL::Prefab prefabSerialize;

        // Register the serialization function for TransformComponent
        serializer.RegisterSerializeFunction("TransformComponent", Serializer::SerializeTransformComponent);
        serializer.RegisterSerializeFunction("MovementComponent", Serializer::SerializeMovementComponent);
        serializer.RegisterSerializeFunction("PrimitiveComponent", Serializer::SerializePrimitiveComponent);
        serializer.RegisterSerializeFunction("SpriteComponent", Serializer::SerializeSpriteComponent);

        //Populate components to test
        auto& transform = static_cast<SOL::TransformComponent&>(prefabSerialize.GetComponent("TransformComponent"));
        transform.m_Transform = { 111.0f, 111.0f};
        transform.m_Scale = { 111.0f, 111.0f };
        transform.m_Rotation = 111.0f;

        auto& Movement = static_cast<SOL::MovementComponent&>(prefabSerialize.GetComponent("MovementComponent"));
        Movement.m_Direction = { 222.0, 222.0 };
        Movement.m_Speed = 222.0;

        auto& Primitive = static_cast<SOL::PrimitiveComponent&>(prefabSerialize.GetComponent("PrimitiveComponent"));
        Primitive.m_PrimitiveID = 1;
        Primitive.m_Offset = 3.0f;
        Primitive.m_Color = { 0.3f, 0.3f, 0.3f };
        Primitive.m_Alpha = 0.3f;

        // Retrieve the SpriteComponent from the prefab
        auto& Sprite = static_cast<SOL::SpriteComponent&>(prefabSerialize.GetComponent("SpriteComponent"));
        Sprite.m_TexKey = "MoleEnenmy";
        Sprite.m_Color = { 0.4f, 0.4f, 0.4f }; // Some RGB values as an example

        // Serialize the prefab
        prefabSerialize.SerializePrefab(serializer);


        //_____________________DESERIALIZE TESTING IM supposed to give loaded data as a set to give to__________________________________________
        ENGINE_INFO("TESTING DESERIALIZER");
        SOL::Serializer deserializer;
        SOL::Prefab prefabDeSerialize;

        // Register the deserialization function for TransformComponent
        deserializer.RegisterDeserializeFunction("TransformComponent", Serializer::DeserializeTransformComponent);
        deserializer.RegisterDeserializeFunction("MovementComponent", Serializer::DeserializeMovementComponent);
        deserializer.RegisterDeserializeFunction("PrimitiveComponent", Serializer::DeserializePrimitiveComponent);
        deserializer.RegisterDeserializeFunction("SpriteComponent", Serializer::DeserializeSpriteComponent);

        // Read JSON from a file
        std::string jsonString = Serializer::readJsonFile("./Json/deSerializeFromThis_prefab.json");

        // Deserialize the prefab from the JSON string
        prefabDeSerialize.DeserializePrefab(deserializer, jsonString);

        // Access the deserialized data
        auto& Transformcomponents = prefabDeSerialize.GetComponent("TransformComponent");
        SOL::TransformComponent& transformComponent = static_cast<SOL::TransformComponent&>(Transformcomponents);
        std::cout << "Transform: [" << transformComponent.m_Transform.x << ", " << transformComponent.m_Transform.y << "]\n";
        std::cout << "Scale: [" << transformComponent.m_Scale.x << ", " << transformComponent.m_Scale.y << "]\n";
        std::cout << "Rotation: " << transformComponent.m_Rotation << "\n\n";

        auto& Movementcomponents = prefabDeSerialize.GetComponent("MovementComponent");
        SOL::MovementComponent& movementComponent = static_cast<SOL::MovementComponent&>(Movementcomponents);
        std::cout << "Direction: [" << movementComponent.m_Direction.x << ", " << movementComponent.m_Direction.y << "]\n";
        std::cout << "Speed: " << movementComponent.m_Speed << "\n\n";

        auto& PrimitiveComponents = prefabDeSerialize.GetComponent("PrimitiveComponent");
        SOL::PrimitiveComponent& primitiveComponent = static_cast<SOL::PrimitiveComponent&>(PrimitiveComponents);

        std::cout << "Primitive ID: " << primitiveComponent.m_PrimitiveID << "\n";
        std::cout << "Offset: " << primitiveComponent.m_Offset << "\n";
        std::cout << "Color: [" << primitiveComponent.m_Color.x << ", " << primitiveComponent.m_Color.y << ", " << primitiveComponent.m_Color.z << "]\n";
        std::cout << "Alpha: " << primitiveComponent.m_Alpha << "\n\n";

        auto& SpriteComponents = prefabDeSerialize.GetComponent("SpriteComponent");
        SOL::SpriteComponent& spriteComponent = static_cast<SOL::SpriteComponent&>(SpriteComponents);

        std::cout << "Texture Key: " << spriteComponent.m_TexKey << "\n";
        std::cout << "Color: [" << spriteComponent.m_Color.x << ", " << spriteComponent.m_Color.y << ", " << spriteComponent.m_Color.z << "]\n\n";
    }

    /*!***********************************************************************
    \brief		Check if prefab has valid components
    *************************************************************************/
    bool Prefab::IsPrefabValid()
    {
        if (m_components.empty())
            return false;
        else
            return true;
    }

    /*!***********************************************************************
    \brief		check if prefab contains a specific component
    *************************************************************************/
    bool Prefab::HasComponent(std::string _component)
    {
        if (m_components.count(_component))
            return true;
        else
            return false;
    }
}