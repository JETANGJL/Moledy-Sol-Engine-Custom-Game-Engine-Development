/******************************************************************************/
/*!
\file		AssetManager.h
\author 	Ang Jie Le Jet (100%)
\date       31 October 2023

\brief		This file consists of functions declarations for the UUID, AssetManager class

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#ifndef _ASSETMANAGER_H_
#define _ASSETMANAGER_H_

#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <filesystem>
#include <cstdlib>
#include <random>
#include <SOL/Serializer/Serializer.h>
#include <SOL/Graphics/Texture.h>
#include <SOL/Graphics/Font.h>
#include <AudioSystem/AudioSystem.h>
#include <SOL/AssetManager/AssetManager.h>

namespace SOL
{

    using TexPathPair = std::pair<Texture, std::string>;//assettype, name
    using FontPathPair = std::pair<Font, std::string>;//assettype, name

    class UUID
    {
    public:

        /*!**************************************************************************
        @brief Default constructor for UUID.

        This constructor initializes a UUID object with a random 64-bit unsigned integer.
        The random value is generated using a uniform distribution.

        *****************************************************************************/
        UUID();

        /*!**************************************************************************
        @brief Constructor for UUID with a specified value.

        This constructor initializes a UUID object with the given 64-bit unsigned integer value.

        @param uuid The specified UUID value.
        *****************************************************************************/
        UUID(uint64_t uuid);

        /*!**************************************************************************
        @brief Default copy constructor for UUID.

        This is the default copy constructor for the UUID class.

        @param[in] other The UUID to copy.
        *****************************************************************************/
        UUID(const UUID&) = default;

        /*!**************************************************************************
        @brief Conversion operator to uint64_t.

        This conversion operator allows converting a UUID object to a uint64_t value.

        @return The uint64_t representation of the UUID.
        *****************************************************************************/
        operator uint64_t() const { return m_UUID; }

        /*!**************************************************************************
        @brief Generate a new UUID.

        This function generates a new UUID by invoking the default constructor and returning
        the generated UUID object.

        @return A new UUID object.
        *****************************************************************************/
        static UUID generateUUID();//rmb generate in editor mode for every asset type.

    private:

        uint64_t m_UUID;

    };
}

namespace std 
{
    /*!**************************************************************************
    @brief Specialization of std::hash for SOL::UUID.

    This is a specialization of the std::hash struct for the SOL::UUID type.
    It provides a hash function that converts a SOL::UUID object into a
    std::size_t hash value.

    @param[in] uuid The SOL::UUID object to be hashed.
    @return The computed hash value.
    *****************************************************************************/
    template<>
    struct hash<SOL::UUID>
    {
        std::size_t operator()(const SOL::UUID& uuid) const
        {
            return static_cast<uint64_t>(uuid);
        }
    };
}

namespace SOL
{
    class AssetManager
    {
    public:

        enum class Asset_Type
        {
            ASSET_TEXTURES,
            ASSET_AUDIO,
            ASSET_FONT,

            MAX_ASSET_TYPE,
            UNKNOWN_ASSET_TYPE
        };

        AudioImplementation m_audioObj;

        /*!**************************************************************************
        @brief Initialize the AssetManager.

        This function initializes the AssetManager by reading and parsing an asset JSON
        file. It deserializes textures, audios, and fonts from the JSON file and populates
        internal data structures.

        Note: The function also logs any errors encountered during the process.
        *****************************************************************************/
        void initAssetManager();

        /*!**************************************************************************
        @brief Unload assets and clear data.

        This function unloads all assets using individual unload functions and clears
        internal data structures to release resources.

        Note: It logs messages for each asset unloaded.
        *****************************************************************************/
        void unloadAssetManager();
        
        /*!**************************************************************************
        @brief Constructor for the AssetManager class.

        This constructor initializes an instance of the AssetManager class.
        It sets the default asset file path for loading and saving asset data.

        The default asset file path is set to "./Json/asset.json".

        @return An instance of the AssetManager class.
        *****************************************************************************/
        AssetManager() 
        {
            //m_assetFilepath = "./Json/asset.json";
            m_assetFilepath = "./Json/assets_serialized.json";
        }

        /*!**************************************************************************
        @brief Destructor for the AssetManager class.

        This destructor is the default destructor for the AssetManager class and is
        used to clean up resources when an instance of the class is destroyed.
        *****************************************************************************/
        ~AssetManager() = default;

//_______________________________________TEXTURES_____________________________________________________//
        /*!**************************************************************************
        @brief Load a texture asset.

        This function loads a texture asset with the specified name and file path if it
        hasn't already been loaded. It also updates the editor map with the loaded asset
        information.

        @param _name The name of the texture.
        @param _filepath The file path of the texture.
        *****************************************************************************/
        void loadTexture(std::string _name, std::string _filepath);

        /*!**************************************************************************
        @brief Unload a texture asset.

        This function unloads a texture asset with the specified name and file path if it
        exists. It also updates the editor map and logs the operation.

        @param _name The name of the texture to unload.
        @param _filepath The file path of the texture.
        *****************************************************************************/
        void unloadTexture(UUID _uuid);

        /*!**************************************************************************
        @brief Modify a texture asset.

        This function modifies a texture asset with the specified UUID by unloading it
        and then loading it again with a new file path. It also updates the editor map
        with the modified asset information.

        @param _texUUID The UUID of the texture to modify.
        @param _filepath The new file path for the texture.
        *****************************************************************************/
        void modifyTexture(UUID _texUUID, std::string _filepath);

        /*!**************************************************************************
        @brief Get the texture map.

        This function returns a map of texture UUIDs to texture path pairs.

        @return A map containing texture UUIDs as keys and texture path pairs as values.
        *****************************************************************************/
        std::unordered_map<UUID, TexPathPair>& getTextureMap();

        /*!**************************************************************************
        @brief Get the UUID of a texture by name.

        This function searches for a texture by name and returns its UUID if found.

        @param _name The name of the texture to search for.
        @return The UUID of the texture if found; otherwise, an empty UUID.
        *****************************************************************************/
        UUID getTextureUUID(std::string _name);

        /*!**************************************************************************
        @brief Get a texture by UUID.

        This function retrieves a texture asset by UUID and returns it.

        @param _UUID The UUID of the texture to retrieve.
        @return A reference to the texture asset if found; otherwise, a default texture path pair.
        *****************************************************************************/
        TexPathPair& getTexture(UUID _UUID);
 
//________________________________________AUDIOS_____________________________________________________//
        /*!**************************************************************************
        @brief Load an audio asset.

        This function loads an audio asset with the specified name and file path if it
        hasn't already been loaded. It also updates the editor map with the loaded asset
        information.

        @param _name The name of the audio.
        @param _filepath The file path of the audio.
        *****************************************************************************/
        void loadAudio(std::string _name, std::string _filepath);

        /*!**************************************************************************
        @brief Unload an audio asset.

        This function unloads an audio asset with the specified name and file path if it
        exists. It also updates the editor map and logs the operation.

        @param _name The name of the audio to unload.
        @param _filepath The file path of the audio.
        *****************************************************************************/
        void unloadAudio(UUID _uuid);
       
        /*!**************************************************************************
        @brief Get the audio map.

        This function returns a map of audio UUIDs to audio names.

        @return A map containing audio UUIDs as keys and audio names as values.
        *****************************************************************************/
        std::unordered_map<UUID, std::string>& getAudioMap();

        /*!**************************************************************************
        @brief Get the UUID of an audio by name.

        This function searches for an audio by name and returns its UUID if found.

        @param _name The name of the audio to search for.
        @return The UUID of the audio if found; otherwise, an empty UUID.
        *****************************************************************************/
        UUID getAudioUUID(std::string _name);
       

//________________________________________FONTS_____________________________________________________//
        /*!**************************************************************************
        @brief Load a font asset.

        This function loads a font asset with the specified name and file path if it
        hasn't already been loaded. It also updates the editor map with the loaded asset
        information.

        @param _name The name of the font.
        @param _filepath The file path of the font.
        *****************************************************************************/
        void loadFont(std::string _name, std::string _filepath);

        /*!**************************************************************************
        @brief Unload a font asset.

        This function unloads a font asset with the specified name and file path if it
        exists. It also updates the editor map and logs the operation.

        @param _name The name of the font to unload.
        @param _filepath The file path of the font.
        *****************************************************************************/
        void unloadFont(UUID _UUID);

        /*!**************************************************************************
        @brief Modify a font asset.

        This function modifies a font asset with the specified UUID by unloading it
        and then loading it again with a new file path. It also updates the editor map
        with the modified asset information.

        @param _fontUUID The UUID of the font to modify.
        @param _filepath The new file path for the font.
        *****************************************************************************/
        void modifyFont(UUID _fontUUID, std::string _filepath);

        /*!**************************************************************************
        @brief Get the font map.

        This function returns a map of font UUIDs to font path pairs.

        @return A map containing font UUIDs as keys and font path pairs as values.
        *****************************************************************************/
        std::unordered_map<UUID, FontPathPair>& getFontMap();

        /*!**************************************************************************
        @brief Get the UUID of a font by name.

        This function searches for a font by name and returns its UUID if found.

        @param _name The name of the font to search for.
        @return The UUID of the font if found; otherwise, an empty UUID.
        *****************************************************************************/
        UUID getFontUUID(std::string _name);

        /*!**************************************************************************
        @brief Get a font by UUID.

        This function retrieves a font asset by UUID and returns it.

        @param _UUID The UUID of the font to retrieve.
        @return A reference to the font asset if found; otherwise, a default font path pair.
        *****************************************************************************/
        FontPathPair& getFont(UUID _UUID);

        std::unordered_map<Asset_Type, std::unordered_map<UUID, std::pair<std::string, std::string>>>& getEditorMap() { return m_EditorMap; }
    private:


        std::unordered_map<UUID, TexPathPair> m_textureMap; //UUID:ASSET:ASSETNAME
        std::unordered_map<UUID, std::string> m_audioMap;   //UUID:ASSETNAME
        std::unordered_map<UUID, FontPathPair> m_fontMap;   //UUID:ASSET:ASSETNAME

        std::unordered_map<Asset_Type, std::unordered_map<UUID, std::pair<std::string, std::string>>> m_EditorMap; //filepath last

        std::string m_assetFilepath;

        FontPathPair m_FontPair;
        TexPathPair m_TexPair;

    public:

        //_________________________________________________SHARED FUNCTION________________________________________________________________________
        /*!**************************************************************************
        @brief Log information about loaded objects.

        This function logs information about loaded textures, audios, and fonts,
        including their UUIDs and other relevant details.
        *****************************************************************************/
        void LogLoadedObj();

        /*!**************************************************************************
        @brief Serialize the editor map to a JSON file.

        This function serializes the editor map, which contains information about
        loaded assets, into a JSON file named "assets_serialized.json". The serialized
        JSON file can later be used to restore asset data.

        @param m_EditorMap The editor map to serialize.
        *****************************************************************************/
        void SerializeEditorMap(const std::unordered_map<Asset_Type, std::unordered_map<UUID, std::pair<std::string, std::string>>>& m_EditorMap);

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
        Asset_Type determineFileType(const std::string& filePath);

        /*!**************************************************************************
        @brief Extract the filename from a file path.

        This function takes a file path as input and extracts the filename without
        the file extension.

        @param filePath The path to the file.
        @return The extracted filename without the file extension.
        *****************************************************************************/
        std::string extractFilenameWithPath(const std::string& filePath);
    };
}
#endif // _ASSETMANAGER_H_