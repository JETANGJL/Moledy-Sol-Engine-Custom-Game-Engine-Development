/******************************************************************************/
/*!
\file		AssetManager.cpp
\author 	Ang Jie Le Jet (100%)
\date       31 October 2023

\brief		This file consists of functions definintions for the UUID, AssetManager class.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "SOLpch.h"
#include "SOL/AssetManager/AssetManager.h"
#include "SOL/Application.h"

namespace SOL
{
    static std::random_device s_RandomDevice;
    static std::mt19937_64 s_Engine(s_RandomDevice());
    static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

    /*!**************************************************************************
    @brief Default constructor for UUID.

    This constructor initializes a UUID object with a random 64-bit unsigned integer.
    The random value is generated using a uniform distribution.

    *****************************************************************************/
    UUID::UUID()
        : m_UUID(s_UniformDistribution(s_Engine))
    {
    }

    /*!**************************************************************************
    @brief Constructor for UUID with a specified value.

    This constructor initializes a UUID object with the given 64-bit unsigned integer value.

    @param uuid The specified UUID value.
    *****************************************************************************/
    UUID::UUID(uint64_t uuid)
        : m_UUID(uuid)
    {
    }

    /*!**************************************************************************
    @brief Generate a new UUID.

    This function generates a new UUID by invoking the default constructor and returning
    the generated UUID object.

    @return A new UUID object.
    *****************************************************************************/
    UUID UUID::generateUUID() 
    {
        return UUID();
    }

    /*!**************************************************************************
    @brief Initialize the AssetManager.

    This function initializes the AssetManager by reading and parsing an asset JSON
    file. It deserializes textures, audios, and fonts from the JSON file and populates
    internal data structures.

    Note: The function also logs any errors encountered during the process.
    *****************************************************************************/
    void AssetManager::initAssetManager() //DESERIALIZE
    {
        std::ifstream file(m_assetFilepath);
        if (!file.is_open())
        {
            ANALYTICS_INFO("Failed to open asset file:");
            return;
        }

        std::stringstream buff;
        buff << file.rdbuf();
        std::string jsonString = buff.str();
        file.close();

        rapidjson::Document doc;
        if (doc.Parse(jsonString.c_str()).HasParseError())
        {
            ANALYTICS_INFO("Failed to parse assets JSON file.");
            return;
        }

        // Deserialize textures
        if (doc.HasMember("textures"))
        {
            const rapidjson::Value& tex_obj = doc["textures"];
            for (auto it = tex_obj.MemberBegin(); it != tex_obj.MemberEnd(); ++it)
            {
                std::string tex_name = it->name.GetString();
                UUID texUUID(it->value["UUID"].GetUint64()); // Use provided UUID from json
                std::string filepath = it->value["filepath"].GetString();

                m_textureMap[texUUID].first.LoadTexture(filepath);
                m_textureMap[texUUID].second = tex_name;

                m_EditorMap[Asset_Type::ASSET_TEXTURES][texUUID].first = tex_name;
                m_EditorMap[Asset_Type::ASSET_TEXTURES][texUUID].second = filepath;
            } 
            ANALYTICS_INFO("Textures successfully deserialized.");
        }

        //deserialize audios
        if (doc.HasMember("audios"))
        {
            const rapidjson::Value& audio_obj = doc["audios"];
            for (auto it = audio_obj.MemberBegin(); it != audio_obj.MemberEnd(); ++it)
            {
                std::string audio_name = it->name.GetString();
                UUID audioUUID(it->value["UUID"].GetUint64());
                std::string filepath = it->value["filepath"].GetString();
                //LoadAudio(audio_name, filepath);

                AudioSystem& _audioSystem = Application::Get().GetAudioSystem();
                _audioSystem.LoadAudio(audio_name, filepath);

                m_audioMap[audioUUID] = audio_name;
                
                m_EditorMap[Asset_Type::ASSET_AUDIO][audioUUID].first = audio_name;
                m_EditorMap[Asset_Type::ASSET_AUDIO][audioUUID].second = filepath;
            }
            ANALYTICS_INFO("Audios successfully initialized.");
        }

        // Deserialize fonts
        if (doc.HasMember("fonts"))
        {
            const rapidjson::Value& font_obj = doc["fonts"];
            for (auto it = font_obj.MemberBegin(); it != font_obj.MemberEnd(); ++it)
            {
                std::string font_name = it->name.GetString();
                UUID fontUUID(it->value["UUID"].GetUint64()); // Use provided UUID from json
                std::string filepath = it->value["filepath"].GetString();

                m_fontMap[fontUUID].first.LoadFont(filepath);
                m_fontMap[fontUUID].second = font_name;

                m_EditorMap[Asset_Type::ASSET_TEXTURES][fontUUID].first = font_name;
                m_EditorMap[Asset_Type::ASSET_TEXTURES][fontUUID].second = filepath;
            }
            ANALYTICS_INFO("Fonts successfully deserialized.");
        }
    }

    /*!**************************************************************************
    @brief Unload assets and clear data.

    This function unloads all assets using individual unload functions and clears
    internal data structures to release resources.

    Note: It logs messages for each asset unloaded.
    *****************************************************************************/
    void AssetManager::unloadAssetManager()
    {
        // Unload all assets using individual unload functions
        for (const auto& [uuid, assetPair] : m_EditorMap[Asset_Type::ASSET_TEXTURES])
        {
            unloadTexture(uuid);
            ANALYTICS_INFO(assetPair.first + "Texture Unloaded.");
        }

        for (const auto& [uuid, assetPair] : m_EditorMap[Asset_Type::ASSET_AUDIO])
        {
            unloadAudio(uuid);
            ANALYTICS_INFO(assetPair.first + "Audio Unloaded.");
        }

        for (const auto& [uuid, assetPair] : m_EditorMap[Asset_Type::ASSET_FONT])
        {
            unloadFont(uuid);
            ANALYTICS_INFO(assetPair.first + "Font Unloaded.");
        }

        // Clear all maps
        m_textureMap.clear();
        m_audioMap.clear();
        m_fontMap.clear();
        m_EditorMap.clear();
    }

    /*!**************************************************************************
    @brief Load a texture asset.

    This function loads a texture asset with the specified name and file path if it
    hasn't already been loaded. It also updates the editor map with the loaded asset
    information.

    @param _name The name of the texture.
    @param _filepath The file path of the texture.
    *****************************************************************************/
    void AssetManager::loadTexture(std::string _name, std::string _filepath)
    {
        bool found{};

        for (auto it = m_EditorMap[Asset_Type::ASSET_TEXTURES].begin(); it != m_EditorMap[Asset_Type::ASSET_TEXTURES].end(); ++it)
        {
            if (_filepath == (*it).second.second)
            {
                found = true;
                ANALYTICS_CRITICAL(_name + " has already been loaded");
                break;
            }
        }
        if (!found)
        {
            UUID texUUID = UUID::generateUUID();
            m_textureMap[texUUID].first.LoadTexture(_filepath);
            m_textureMap[texUUID].second = _name;

            m_EditorMap[Asset_Type::ASSET_TEXTURES][texUUID].first = _name;
            m_EditorMap[Asset_Type::ASSET_TEXTURES][texUUID].second = _filepath;
            ANALYTICS_INFO(m_EditorMap[Asset_Type::ASSET_TEXTURES][texUUID].first + "Texture successfully loaded.");
            SerializeEditorMap(m_EditorMap);
        }
    }

    /*!**************************************************************************
    @brief Unload a texture asset.

    This function unloads a texture asset with the specified name and file path if it
    exists. It also updates the editor map and logs the operation.

    @param _name The name of the texture to unload.
    @param _filepath The file path of the texture.
    *****************************************************************************/
    void AssetManager::unloadTexture(UUID _uuid)
    {
        m_textureMap.erase(_uuid);
        m_EditorMap[Asset_Type::ASSET_TEXTURES].erase(_uuid);
    }

    /*!**************************************************************************
    @brief Modify a texture asset.

    This function modifies a texture asset with the specified UUID by unloading it
    and then loading it again with a new file path. It also updates the editor map
    with the modified asset information.

    @param _texUUID The UUID of the texture to modify.
    @param _filepath The new file path for the texture.
    *****************************************************************************/
    void AssetManager::modifyTexture(UUID _texUUID, std::string _filepath)
    {
        //unload texture wait for HAFIZ
        m_textureMap[_texUUID].first.UnloadTexture();

        m_textureMap[_texUUID].first.LoadTexture(_filepath);
        m_EditorMap[Asset_Type::ASSET_TEXTURES][_texUUID].second = _filepath;
        ANALYTICS_INFO(m_EditorMap[Asset_Type::ASSET_TEXTURES][_texUUID].first + "Textures successfully modified.");
    }

    /*!**************************************************************************
    @brief Get the texture map.

    This function returns a map of texture UUIDs to texture path pairs.

    @return A map containing texture UUIDs as keys and texture path pairs as values.
    *****************************************************************************/
    std::unordered_map<UUID, TexPathPair>& AssetManager::getTextureMap()
    {
        return m_textureMap;
    }

    /*!**************************************************************************
    @brief Get the UUID of a texture by name.

    This function searches for a texture by name and returns its UUID if found.

    @param _name The name of the texture to search for.
    @return The UUID of the texture if found; otherwise, an empty UUID.
    *****************************************************************************/
    UUID AssetManager::getTextureUUID(std::string _name)
    {
        for (auto it = m_textureMap.begin(); it != m_textureMap.end(); ++it)
        {
            if ((*it).second.second == _name)
                //ANALYTICS_ERROR((*it).first);
                return (*it).first;
        }
        return UUID();
    }

    /*!**************************************************************************
    @brief Get a texture by UUID.

    This function retrieves a texture asset by UUID and returns it.

    @param _UUID The UUID of the texture to retrieve.
    @return A reference to the texture asset if found; otherwise, a default texture path pair.
    *****************************************************************************/
    TexPathPair& AssetManager::getTexture(UUID _UUID)
    {
        m_TexPair = TexPathPair();
        if (m_textureMap.count(_UUID))
        {
            //ANALYTICS_CRITICAL(_UUID);

            return m_textureMap[_UUID];
        }
        return m_TexPair;
    }

    /*!**************************************************************************
    @brief Load an audio asset.

    This function loads an audio asset with the specified name and file path if it
    hasn't already been loaded. It also updates the editor map with the loaded asset
    information.

    @param _name The name of the audio.
    @param _filepath The file path of the audio.
    *****************************************************************************/
    void AssetManager::loadAudio(std::string _name, std::string _filepath)
    {
        bool found{};
        for (auto it = m_EditorMap[Asset_Type::ASSET_AUDIO].begin(); it != m_EditorMap[Asset_Type::ASSET_AUDIO].end(); ++it)
        {
            if (_filepath == (*it).second.second)
            {
                found = true;
                ANALYTICS_CRITICAL(_name + "Audio has already been loaded");
                break;
            }
        }
        if (!found)
        {
            AudioSystem& _audioSystem = Application::Get().GetAudioSystem();
            _audioSystem.LoadAudio(_name, _filepath);
            UUID audioUUID = UUID::generateUUID();
            m_audioMap[audioUUID] = _name;

            m_EditorMap[Asset_Type::ASSET_AUDIO][audioUUID].first = _name;
            m_EditorMap[Asset_Type::ASSET_AUDIO][audioUUID].second = _filepath;
            ANALYTICS_INFO(m_EditorMap[Asset_Type::ASSET_AUDIO][audioUUID].first + " Audio successfully loaded.");
            SerializeEditorMap(m_EditorMap);
        }
    }

    /*!**************************************************************************
    @brief Unload an audio asset.

    This function unloads an audio asset with the specified name and file path if it
    exists. It also updates the editor map and logs the operation.

    @param _name The name of the audio to unload.
    @param _filepath The file path of the audio.
    *****************************************************************************/
    void AssetManager::unloadAudio(UUID _uuid)
    {
        bool found{};

        for (auto it = m_EditorMap[Asset_Type::ASSET_AUDIO].begin(); it != m_EditorMap[Asset_Type::ASSET_AUDIO].end(); ++it)
        {
            if (_uuid == (*it).first)
            {
                found = true;
                ANALYTICS_INFO(m_audioMap[_uuid] + "Loaded audio found");

                AudioSystem& _audioSystem = Application::Get().GetAudioSystem();
                _audioSystem.UnLoadAudio(m_audioMap[_uuid]);


                ANALYTICS_INFO(m_audioMap[_uuid] + "Audio found in m_EditorMap was deleted");
            }
        }
        m_audioMap.erase(_uuid);
        m_EditorMap[Asset_Type::ASSET_AUDIO].erase(_uuid);
        if (!found)
        {
            ANALYTICS_INFO(m_audioMap[_uuid] + " Audio does not exist in m_EditorMap");
        }
    }

    /*!**************************************************************************
    @brief Get the audio map.

    This function returns a map of audio UUIDs to audio names.

    @return A map containing audio UUIDs as keys and audio names as values.
    *****************************************************************************/
    std::unordered_map<UUID, std::string>& AssetManager::getAudioMap()
    {
        return m_audioMap;
    }
    
    /*!**************************************************************************
    @brief Get the UUID of an audio by name.

    This function searches for an audio by name and returns its UUID if found.

    @param _name The name of the audio to search for.
    @return The UUID of the audio if found; otherwise, an empty UUID.
    *****************************************************************************/
    UUID AssetManager::getAudioUUID(std::string _name)
    {
        for (auto it = m_audioMap.begin(); it != m_audioMap.end(); ++it)
        {
            if ((*it).second == _name)
                //ANALYTICS_ERROR((*it).first);
                return (*it).first;
        }
        return UUID();
    }

    /*!**************************************************************************
    @brief Load a font asset.

    This function loads a font asset with the specified name and file path if it
    hasn't already been loaded. It also updates the editor map with the loaded asset
    information.

    @param _name The name of the font.
    @param _filepath The file path of the font.
    *****************************************************************************/
    void AssetManager::loadFont(std::string _name, std::string _filepath)
    {
        bool found{};

        for (auto it = m_EditorMap[Asset_Type::ASSET_FONT].begin(); it != m_EditorMap[Asset_Type::ASSET_FONT].end(); ++it)
        {
            if (_filepath == (*it).second.second)
            {
                found = true;
                ANALYTICS_CRITICAL(_name + "Font has already been loaded");
                break;
            }
        }
        if (!found)
        {
            UUID texUUID = UUID::generateUUID();
            m_fontMap[texUUID].first.LoadFont(_filepath);
            m_fontMap[texUUID].second = _name;

            m_EditorMap[Asset_Type::ASSET_FONT][texUUID].first = _name;
            m_EditorMap[Asset_Type::ASSET_FONT][texUUID].second = _filepath;
            ANALYTICS_INFO(m_EditorMap[Asset_Type::ASSET_FONT][texUUID].first + "Font successfully loaded.");
            SerializeEditorMap(m_EditorMap);
        }
    }

    /*!**************************************************************************
    @brief Unload a font asset.

    This function unloads a font asset with the specified name and file path if it
    exists. It also updates the editor map and logs the operation.

    @param _name The name of the font to unload.
    @param _filepath The file path of the font.
    *****************************************************************************/
    void AssetManager::unloadFont(UUID _uuid)
    {
        m_fontMap.erase(_uuid);
        m_EditorMap[Asset_Type::ASSET_FONT].erase(_uuid);
    }

    /*!**************************************************************************
    @brief Modify a font asset.

    This function modifies a font asset with the specified UUID by unloading it
    and then loading it again with a new file path. It also updates the editor map
    with the modified asset information.

    @param _fontUUID The UUID of the font to modify.
    @param _filepath The new file path for the font.
    *****************************************************************************/
    void AssetManager::modifyFont(UUID _fontUUID, std::string _filepath)
    {
        //unload font wait for HAFIZ
        m_fontMap[_fontUUID].first.UnloadFont();

        m_fontMap[_fontUUID].first.LoadFont(_filepath);
        m_EditorMap[Asset_Type::ASSET_FONT][_fontUUID].second = _filepath;
        ANALYTICS_INFO(m_EditorMap[Asset_Type::ASSET_FONT][_fontUUID].first + "Textures successfully modified.");
    }

    /*!**************************************************************************
    @brief Get the font map.

    This function returns a map of font UUIDs to font path pairs.

    @return A map containing font UUIDs as keys and font path pairs as values.
    *****************************************************************************/
    std::unordered_map<UUID, FontPathPair>& AssetManager::getFontMap()
    {
        return m_fontMap;
    }

    /*!**************************************************************************
    @brief Get the UUID of a font by name.

    This function searches for a font by name and returns its UUID if found.

    @param _name The name of the font to search for.
    @return The UUID of the font if found; otherwise, an empty UUID.
    *****************************************************************************/
    UUID AssetManager::getFontUUID(std::string _name)
    {
        for (auto it = m_fontMap.begin(); it != m_fontMap.end(); ++it)
        {
            if ((*it).second.second == _name)
                //ANALYTICS_ERROR((*it).first);
                return (*it).first;
        }
        return UUID();
    }

    /*!**************************************************************************
    @brief Get a font by UUID.

    This function retrieves a font asset by UUID and returns it.

    @param _UUID The UUID of the font to retrieve.
    @return A reference to the font asset if found; otherwise, a default font path pair.
    *****************************************************************************/
    FontPathPair& AssetManager::getFont(UUID _UUID)
    {
        m_FontPair = FontPathPair();
        if (m_fontMap.count(_UUID))
        {
            //ANALYTICS_CRITICAL(_UUID);

            return m_fontMap[_UUID];
        }
        return m_FontPair;
    }
     /*!**************************************************************************
    @brief Log information about loaded objects.

    This function logs information about loaded textures, audios, and fonts,
    including their UUIDs and other relevant details.
    *****************************************************************************/
    void AssetManager::LogLoadedObj()
    {
        //texture
        for (auto& pair : m_textureMap)
        {
            ANALYTICS_INFO(pair.first);
            ANALYTICS_INFO(pair.second.second);
            ANALYTICS_INFO(pair.second.first.getHandlerID());
        }
        //audio
        for (auto& pair : m_audioMap)
        {
            ANALYTICS_INFO(pair.first);
            ANALYTICS_INFO(pair.second);
        }
        //font
        for (auto& pair : m_fontMap)
        {
            ANALYTICS_INFO(pair.first);
            ANALYTICS_INFO(pair.second.second);
            ANALYTICS_INFO(pair.second.first.getTextureID());
        }
    }

    /*!**************************************************************************
    @brief Serialize the editor map to a JSON file.

    This function serializes the editor map, which contains information about
    loaded assets, into a JSON file named "assets_serialized.json". The serialized
    JSON file can later be used to restore asset data.

    @param m_EditorMap The editor map to serialize.
    *****************************************************************************/
    void AssetManager::SerializeEditorMap(const std::unordered_map<Asset_Type, std::unordered_map<UUID, std::pair<std::string, std::string>>>& _editormap) {
        rapidjson::Document doc;
        rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

        doc.SetObject();
        rapidjson::Value textures(rapidjson::kObjectType);
        rapidjson::Value audios(rapidjson::kObjectType);
        rapidjson::Value fonts(rapidjson::kObjectType);

        // Serialize textures
        if (_editormap.count(Asset_Type::ASSET_TEXTURES)) 
        {
            for (const auto& [uuid, assetPair] : _editormap.at(Asset_Type::ASSET_TEXTURES)) 
            {
                rapidjson::Value textureObj(rapidjson::kObjectType);
                textureObj.AddMember("UUID", rapidjson::Value(static_cast<uint64_t>(uuid)), allocator);
                textureObj.AddMember("filepath", rapidjson::Value(assetPair.second.c_str(), allocator), allocator);
                textures.AddMember(rapidjson::Value(assetPair.first.c_str(), allocator), textureObj, allocator);
            }
            doc.AddMember("textures", textures, allocator);
            ANALYTICS_INFO("Texture successfully saved.");
        }

        // Serialize audios
        if (_editormap.count(Asset_Type::ASSET_AUDIO)) 
        {
            for (const auto& [uuid, assetPair] : _editormap.at(Asset_Type::ASSET_AUDIO)) 
            {
                rapidjson::Value audioObj(rapidjson::kObjectType);
                audioObj.AddMember("UUID", rapidjson::Value(static_cast<uint64_t>(uuid)), allocator);
                audioObj.AddMember("filepath", rapidjson::Value(assetPair.second.c_str(), allocator), allocator);
                audios.AddMember(rapidjson::Value(assetPair.first.c_str(), allocator), audioObj, allocator);
            }
            doc.AddMember("audios", audios, allocator);
        }

        // Serialize fonts
        if (_editormap.count(Asset_Type::ASSET_FONT)) 
        {
            for (const auto& [uuid, assetPair] : _editormap.at(Asset_Type::ASSET_FONT)) 
            {
                rapidjson::Value fontObj(rapidjson::kObjectType);
                fontObj.AddMember("UUID", rapidjson::Value(static_cast<uint64_t>(uuid)), allocator);
                fontObj.AddMember("filepath", rapidjson::Value(assetPair.second.c_str(), allocator), allocator);
                fonts.AddMember(rapidjson::Value(assetPair.first.c_str(), allocator), fontObj, allocator);
            }
            doc.AddMember("fonts", fonts, allocator);
        }

        // Convert the document to a string using StringBuffer.
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);

        // Write the string to a file.
        std::ofstream file("./Json/assets_serialized.json");
        if (file.is_open()) 
        {
            file << buffer.GetString();
            file.close();
            ANALYTICS_INFO("Assets serialized to assets.json");
        }
        else {
            ANALYTICS_ERROR("Failed to open file for writing.");
        }
    }

    /*!**************************************************************************
    @brief Determine the asset type based on the file extension.

    This function takes a file path as input and analyzes its file extension to
    determine the asset type.

    @param filePath The path to the file.
    @return The asset type determined from the file extension.
             - If the file extension is ".png," it returns Asset_Type::ASSET_TEXTURES.
             - If the file extension is ".wav," it returns Asset_Type::ASSET_AUDIO.
             - If the file extension is ".ttf," it returns Asset_Type::ASSET_FONT.
             - If the file extension is not recognized, it returns Asset_Type::UNKNOWN_ASSET_TYPE.
    *****************************************************************************/
    AssetManager::Asset_Type AssetManager::determineFileType(const std::string& filePath)
    {
        // Find the last '.' character
        size_t lastDotPos = filePath.find_last_of('.');
        if (lastDotPos != std::string::npos)
        {
            std::string extension = filePath.substr(lastDotPos);
            std::transform(extension.begin(), extension.end(), extension.begin(), 
                [](unsigned char c) { return std::tolower(c); });

            if (extension == ".png")
            {
                return Asset_Type::ASSET_TEXTURES;
            }
            else if (extension == ".wav" || extension == ".ogg")
            {
                return Asset_Type::ASSET_AUDIO;
            }
            else if (extension == ".ttf") 
            {
                return Asset_Type::ASSET_FONT;
            }
        }
        return Asset_Type::UNKNOWN_ASSET_TYPE;
    }

    /*!**************************************************************************
    @brief Extract the filename from a file path.

    This function takes a file path as input and extracts the filename without
    the file extension.

    @param filePath The path to the file.
    @return The extracted filename without the file extension.
    *****************************************************************************/
    std::string AssetManager::extractFilenameWithPath(const std::string& filePath)
    {
        size_t lastSlashPos = filePath.find_last_of('/');
        size_t lastDotPos = filePath.find_last_of('.');

        if (lastSlashPos == std::string::npos) 
        {
            lastSlashPos = static_cast<size_t>(-1);
        }
        if (lastDotPos == std::string::npos) 
        {
            lastDotPos = filePath.length(); 
        }
        std::string filenameWithoutExtension = filePath.substr(lastSlashPos + 1, lastDotPos - lastSlashPos - 1);

        return filenameWithoutExtension;
    }
}