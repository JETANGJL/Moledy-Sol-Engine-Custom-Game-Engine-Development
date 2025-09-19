/******************************************************************************/
/*!
\file       Serializer.h
\author     Ang Jie Le Jet
\date       25 September 2023

\brief      This file consists of functions declarations for the Serializer class

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#ifndef _SERIALIZER_H_
#define _SERIALIZER_H_

#include "Prefab.h"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <string>
#include <functional>
#include <map>
#include <fstream>
#include <sstream>

namespace SOL
{
    class Prefab;

    class Serializer
    {
    public:

        /*!***********************************************************************
        \brief		Constructor for Serializer class.
        *************************************************************************/
        Serializer()
        {
            RegisterDeserializeFunction("TransformComponent", Serializer::DeserializeTransformComponent);
            RegisterDeserializeFunction("MovementComponent", Serializer::DeserializeMovementComponent);
            RegisterDeserializeFunction("PrimitiveComponent", Serializer::DeserializePrimitiveComponent);
            RegisterDeserializeFunction("SpriteComponent", Serializer::DeserializeSpriteComponent);
            RegisterDeserializeFunction("PlayerComponent", Serializer::DeserializePlayerComponent);
            RegisterDeserializeFunction("NameComponent", Serializer::DeserializeNameComponent);
            RegisterDeserializeFunction("RigidBody2DComponent", Serializer::DeserializeRigidBody2DComponent);
            RegisterDeserializeFunction("CameraComponent", Serializer::DeserializeCameraComponent);
            RegisterDeserializeFunction("FontComponent", Serializer::DeserializeFontComponent);
            RegisterDeserializeFunction("AnimationComponent", Serializer::DeserializeAnimationComponent);
            RegisterDeserializeFunction("GemComponent", Serializer::DeserializeGemComponent);
            RegisterDeserializeFunction("UIComponent", Serializer::DeserializeUIComponent);
            RegisterDeserializeFunction("AudioComponent", Serializer::DeserializeAudioComponent);
            RegisterDeserializeFunction("EnemyComponent", Serializer::DeserializeEnemyComponent);
            RegisterDeserializeFunction("TileComponent", Serializer::DeserializeTileComponent);
            RegisterDeserializeFunction("CPPScriptComponent", Serializer::DeserializeCPPScriptComponent);

            RegisterSerializeFunction("TransformComponent", Serializer::SerializeTransformComponent);
            RegisterSerializeFunction("MovementComponent", Serializer::SerializeMovementComponent);
            RegisterSerializeFunction("PrimitiveComponent", Serializer::SerializePrimitiveComponent);
            RegisterSerializeFunction("SpriteComponent", Serializer::SerializeSpriteComponent);
            RegisterSerializeFunction("PlayerComponent", Serializer::SerializePlayerComponent);
            RegisterSerializeFunction("NameComponent", Serializer::SerializeNameComponent);
            RegisterSerializeFunction("RigidBody2DComponent", Serializer::SerializeRigidBody2DComponent);
            RegisterSerializeFunction("CameraComponent", Serializer::SerializeCameraComponent);
            RegisterSerializeFunction("FontComponent", Serializer::SerializefontComponent);
            RegisterSerializeFunction("AnimationComponent", Serializer::SerializeAnimationComponent);
            RegisterSerializeFunction("GemComponent", Serializer::SerializeGemComponent);
            RegisterSerializeFunction("UIComponent", Serializer::SerializeUIComponent);
            RegisterSerializeFunction("AudioComponent", Serializer::SerializeAudioComponent);
            RegisterSerializeFunction("EnemyComponent", Serializer::SerializeEnemyComponent);
            RegisterSerializeFunction("TileComponent", Serializer::SerializeTileComponent);
            RegisterSerializeFunction("CPPScriptComponent", Serializer::SerializeCPPScriptComponent);
        }

        /*!***********************************************************************
        \brief		Destructor for Serializer class.
        *************************************************************************/
        ~Serializer()
        {

        }

        using Writer = rapidjson::PrettyWriter<rapidjson::StringBuffer>;
        using Reader = rapidjson::GenericValue<rapidjson::UTF8<>>;

        /*!***********************************************************************
        \brief		Function objects for serialization.
        *************************************************************************/
        std::map<std::string, std::function<void(Writer&, const Components&)>> serializeFunctions;

        /*!***********************************************************************
        \brief		Function objects for deserialization.
        *************************************************************************/
        std::map<std::string, std::function<void(const Reader&, Components&)>> deserializeFunctions;

        /*!***********************************************************************
        \brief		Serialize by functions.
        \param      writer Writer reference, typeName const std::string reference,
                    component const Components reference.
        *************************************************************************/
        void Serialize(Writer& writer, const std::string& typeName, const Components& component);

        /*!***********************************************************************
        \brief		Deerialize by functions.
        \param      writer Writer reference, typeName const std::string reference,
                    component const Components reference.
        *************************************************************************/
        void Deserialize(const Reader& reader, Components& component, const std::string& typeName);

        /*!***********************************************************************
        \brief		Register serialization functions.
        \param      typeName const std::string reference,
                    func std::function<void(Writer&, const Components&)>.
        *************************************************************************/
        void RegisterSerializeFunction(const std::string& typeName, std::function<void(Writer&, const Components&)> func);

        /*!***********************************************************************
        \brief		Register deserialization functions.
        \param      typeName const std::string reference,
                    func std::function<void(Writer&, const Components&)>.
        *************************************************************************/
        void RegisterDeserializeFunction(const std::string& typeName, std::function<void(const Reader&, Components&)> func);

        /*!***********************************************************************
        \brief		Function to read Json File.
        \param      filePath const std::string reference.
        *************************************************************************/
        static std::string readJsonFile(const std::string& filePath);


//________________________EXTEND WHEN NEW COMPONENTS IMPLEMENTED(ADD MORE)________________________
        /*!***********************************************************************
        \brief		Serializes function for transform component
        *************************************************************************/
        static void SerializeTransformComponent(Writer& writer, const Components& component);

        /*!***********************************************************************
        \brief		Serializes function for movement component
        *************************************************************************/
        static void SerializeMovementComponent(Writer& writer, const Components& component);

        /*!***********************************************************************
        \brief		Serializes function for primitive component
        *************************************************************************/
        static void SerializePrimitiveComponent(Writer& writer, const Components& component);

        /*!***********************************************************************
        \brief		Serializes function for sprite component
        *************************************************************************/
        static void SerializeSpriteComponent(Writer& writer, const Components& component);

        /*!***********************************************************************
        \brief		Serializes function for Name component
        *************************************************************************/
        static void SerializeNameComponent(Writer& writer, const Components& component);

        /*!***********************************************************************
        \brief		Serializes function for Player component
        *************************************************************************/
        static void SerializePlayerComponent(Writer& writer, const Components& component);

        /*!***********************************************************************
        \brief		Serializes function for Rigidbody component
        *************************************************************************/
        static void SerializeRigidBody2DComponent(Writer& writer, const Components& component);

        /*!***********************************************************************
        \brief		Serializes function for Camera component
        *************************************************************************/
        static void SerializeCameraComponent(Writer& writer, const Components& component);
        /*!***********************************************************************
        \brief		Serializes function for font component
        *************************************************************************/
        static void SerializefontComponent(Writer& writer, const Components& component);

        /*!***********************************************************************
        \brief		Serializes function for animation component
        *************************************************************************/
        static void SerializeAnimationComponent(Writer& writer, const Components& component);

        /*!***********************************************************************
        \brief		Serializes function for gem component
        *************************************************************************/
        static void SerializeGemComponent(Writer& writer, const Components& component);

        /*!***********************************************************************
        \brief		Serializes function for UI component
        *************************************************************************/
        static void SerializeUIComponent(Writer& writer, const Components& component);

        /*!***********************************************************************
        \brief		Serializes function for audio component
        *************************************************************************/
        static void SerializeAudioComponent(Writer& writer, const Components& component);

        /*!***********************************************************************
        \brief		Serializes function for enemy component
        *************************************************************************/
        static void SerializeEnemyComponent(Writer& writer, const Components& component);

        /*!***********************************************************************
        \brief		Serializes function for tile component
        *************************************************************************/
        static void SerializeTileComponent(Writer& writer, const Components& component);

        /*!***********************************************************************
        \brief		Serializes function for CPP script component
        *************************************************************************/
        static void SerializeCPPScriptComponent(Writer& writer, const Components& component);

        /*!***********************************************************************
        \brief		deserializes function for transform component
        *************************************************************************/
        static void DeserializeTransformComponent(const Reader& reader, Components& component);

        /*!***********************************************************************
        \brief		deserializes function for movement component
        *************************************************************************/
        static void DeserializeMovementComponent(const Reader& reader, Components& component);

        /*!***********************************************************************
        \brief		deserializes function for Primitive component
        *************************************************************************/
        static void DeserializePrimitiveComponent(const Reader& reader, Components& component);

        /*!***********************************************************************
        \brief		deSerializes function for sprite component
        *************************************************************************/
        static void DeserializeSpriteComponent(const Reader& reader, Components& component);

        /*!***********************************************************************
        \brief		deserializes function for Name component
        *************************************************************************/
        static void DeserializeNameComponent(const Reader& reader, Components& component);

        /*!***********************************************************************
        \brief		deserializes function for player component
        *************************************************************************/
        static void DeserializePlayerComponent(const Reader& reader, Components& component);

        /*!***********************************************************************
        \brief		deserializes function for rigidbody component
        *************************************************************************/
        static void DeserializeRigidBody2DComponent(const Reader& reader, Components& component);

        /*!***********************************************************************
        \brief		deserializes function for camera component
        *************************************************************************/
        static void DeserializeCameraComponent(const Reader& reader, Components& component);


        /*!***********************************************************************
        \brief		deserializes function for font component
        *************************************************************************/
        static void DeserializeFontComponent(const Reader& reader, Components& component);

        /*!***********************************************************************
        \brief		deserializes function for animation component
        *************************************************************************/
        static void DeserializeAnimationComponent(const Reader& reader, Components& component);

        /*!***********************************************************************
        \brief		deserializes function for gem component
        *************************************************************************/
        static void DeserializeGemComponent(const Reader& reader, Components& component);
                    
        /*!***********************************************************************
        \brief		deserializes function for UI component
        *************************************************************************/
        static void DeserializeUIComponent(const Reader& reader, Components& component);
                    
        /*!***********************************************************************
        \brief		deserializes function for audio component
        *************************************************************************/
        static void DeserializeAudioComponent(const Reader& reader, Components& component);

        /*!***********************************************************************
        \brief		deserializes function for enemy component
        *************************************************************************/
        static void DeserializeEnemyComponent(const Reader& reader, Components& component);

        /*!***********************************************************************
        \brief		deserializes function for tile component
        *************************************************************************/
        static void DeserializeTileComponent(const Reader& reader, Components& component);

        /*!***********************************************************************
        \brief		deserializes function for CPP script component
        *************************************************************************/
        static void DeserializeCPPScriptComponent(const Reader& reader, Components& component);

    private:

    };
}
#endif  //_SERIALIZER_H_