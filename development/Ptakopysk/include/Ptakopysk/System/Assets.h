#ifndef __PTAKOPYSK__ASSETS__
#define __PTAKOPYSK__ASSETS__

#include <XeCore/Common/Base.h>
#include <XeCore/Common/IRtti.h>
#include <XeCore/Common/MemoryManager.h>
#include <XeCore/Common/Singleton.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <json/json.h>
#include <map>

namespace Ptakopysk
{

    class AssetsChangedListener
    {
    public:
        virtual ~AssetsChangedListener() {}

        virtual void onTextureChanged( const std::string& id, const sf::Texture* asset, bool addedOrRemoved ) = 0;
        virtual void onShaderChanged( const std::string& id, const sf::Shader* asset, bool addedOrRemoved ) = 0;
        virtual void onSoundChanged( const std::string& id, const sf::Sound* asset, bool addedOrRemoved ) = 0;
        virtual void onMusicChanged( const std::string& id, const sf::Music* asset, bool addedOrRemoved ) = 0;
        virtual void onFontChanged( const std::string& id, const sf::Font* asset, bool addedOrRemoved ) = 0;
    };

    class Assets
    : public virtual XeCore::Common::IRtti
    , public virtual XeCore::Common::MemoryManager::Manageable
    , public XeCore::Common::Singleton< Assets >
    {
        RTTI_CLASS_DECLARE( Assets );

    public:
        Assets();
        virtual ~Assets();

        FORCEINLINE void setFileSystemRoot( const std::string& path ) { m_fileSystemRoot = path; };
        FORCEINLINE std::string getFileSystemRoot() { return m_fileSystemRoot; };
        FORCEINLINE void setAssetsChangedListener( AssetsChangedListener* listener ) { m_assetsChangedListener = listener; };
        FORCEINLINE AssetsChangedListener* getAssetsChangedListener() { return m_assetsChangedListener; };

        void jsonToAssets( const Json::Value& root );
        void jsonToTextures( const Json::Value& root );
        void jsonToShaders( const Json::Value& root );
        void jsonToSounds( const Json::Value& root );
        void jsonToMusics( const Json::Value& root );
        void jsonToFonts( const Json::Value& root );

        sf::Texture* jsonToTexture( const Json::Value& root );
        sf::Shader* jsonToShader( const Json::Value& root );
        sf::Sound* jsonToSound( const Json::Value& root );
        sf::Music* jsonToMusic( const Json::Value& root );
        sf::Font* jsonToFont( const Json::Value& root );

        Json::Value assetsToJson();
        Json::Value texturesToJson();
        Json::Value shadersToJson();
        Json::Value soundsToJson();
        Json::Value musicsToJson();
        Json::Value fontsToJson();

        Json::Value textureToJson( const std::string& id );
        Json::Value shaderToJson( const std::string& id );
        Json::Value soundToJson( const std::string& id );
        Json::Value musicToJson( const std::string& id );
        Json::Value fontToJson( const std::string& id );

        sf::Texture* addTexture( const std::string& id, const sf::Texture* ptr );
        sf::Shader* addShader( const std::string& id, const sf::Shader* ptr );
        sf::Sound* addSound( const std::string& id, const sf::SoundBuffer* ptrbuff, const sf::Sound* ptr );
        sf::Music* addMusic( const std::string& id, const sf::Music* ptr );
        sf::Font* addFont( const std::string& id, const sf::Font* ptr );

        sf::Texture* loadTexture( const std::string& id, const std::string& path );
        sf::Shader* loadShader( const std::string& id, const std::string& vspath, const std::string& fspath, const std::string* uniforms = 0, unsigned int uniformsCount = 0 );
        sf::Sound* loadSound( const std::string& id, const std::string& path );
        sf::Music* loadMusic( const std::string& id, const std::string& path );
        sf::Font* loadFont( const std::string& id, const std::string& path );

        sf::Texture* getTexture( const std::string& id );
        sf::Shader* getShader( const std::string& id );
        sf::Sound* getSound( const std::string& id );
        sf::Music* getMusic( const std::string& id );
        sf::Font* getFont( const std::string& id );

        FORCEINLINE sf::Texture* getDefaultTexture() { return m_defaultTexture; };

        std::string findTexture( const sf::Texture* ptr );
        std::string findShader( const sf::Shader* ptr );
        std::string findSound( const sf::Sound* ptr );
        std::string findMusic( const sf::Music* ptr );
        std::string findFont( const sf::Font* ptr );

        FORCEINLINE std::string getTextureMeta( const std::string& id ) { return m_metaTextures.count( id ) ? m_metaTextures[ id ] : ""; };
        FORCEINLINE std::string getShaderMeta( const std::string& id ) { return m_metaShaders.count( id ) ? m_metaShaders[ id ] : ""; };
        FORCEINLINE std::string getSoundMeta( const std::string& id ) { return m_metaSounds.count( id ) ? m_metaSounds[ id ] : ""; };
        FORCEINLINE std::string getMusicMeta( const std::string& id ) { return m_metaMusics.count( id ) ? m_metaMusics[ id ] : ""; };
        FORCEINLINE std::string getFontMeta( const std::string& id ) { return m_metaFonts.count( id ) ? m_metaFonts[ id ] : ""; };

        FORCEINLINE std::vector< std::string >* accessTextureTags( const std::string& id ) { return m_tagsTextures.count( id ) ? &m_tagsTextures[ id ] : 0; };
        FORCEINLINE std::vector< std::string >* accessShaderTags( const std::string& id ) { return m_tagsShaders.count( id ) ? &m_tagsShaders[ id ] : 0; };
        FORCEINLINE std::vector< std::string >* accessSoundTags( const std::string& id ) { return m_tagsSounds.count( id ) ? &m_tagsSounds[ id ] : 0; };
        FORCEINLINE std::vector< std::string >* accessMusicTags( const std::string& id ) { return m_tagsMusics.count( id ) ? &m_tagsMusics[ id ] : 0; };
        FORCEINLINE std::vector< std::string >* accessFontTags( const std::string& id ) { return m_tagsFonts.count( id ) ? &m_tagsFonts[ id ] : 0; };

        FORCEINLINE std::vector< std::string >* getShaderUniforms( const std::string& id ) { return m_uniformsShaders.count( id ) ? &m_uniformsShaders[ id ] : 0; };

        FORCEINLINE unsigned int getTexturesCount() { return m_textures.size(); };
        FORCEINLINE std::map< std::string, sf::Texture* >::iterator getTextureAtBegin() { return m_textures.begin(); };
        FORCEINLINE std::map< std::string, sf::Texture* >::iterator getTextureAtEnd() { return m_textures.end(); };
        FORCEINLINE unsigned int getShadersCount() { return m_shaders.size(); };
        FORCEINLINE std::map< std::string, sf::Shader* >::iterator getShaderAtBegin() { return m_shaders.begin(); };
        FORCEINLINE std::map< std::string, sf::Shader* >::iterator getShaderAtEnd() { return m_shaders.end(); };
        FORCEINLINE unsigned int getSoundsCount() { return m_sounds.size(); };
        FORCEINLINE std::map< std::string, sf::Sound* >::iterator getSoundAtBegin() { return m_sounds.begin(); };
        FORCEINLINE std::map< std::string, sf::Sound* >::iterator getSoundAtEnd() { return m_sounds.end(); };
        FORCEINLINE unsigned int getMusicsCount() { return m_musics.size(); };
        FORCEINLINE std::map< std::string, sf::Music* >::iterator getMusicAtBegin() { return m_musics.begin(); };
        FORCEINLINE std::map< std::string, sf::Music* >::iterator getMusicAtEnd() { return m_musics.end(); };
        FORCEINLINE unsigned int getFontsCount() { return m_fonts.size(); };
        FORCEINLINE std::map< std::string, sf::Font* >::iterator getFontAtBegin() { return m_fonts.begin(); };
        FORCEINLINE std::map< std::string, sf::Font* >::iterator getFontAtEnd() { return m_fonts.end(); };

        void freeTexture( const std::string& id );
        void freeShader( const std::string& id );
        void freeSound( const std::string& id );
        void freeMusic( const std::string& id );
        void freeFont( const std::string& id );
        void freeAllTextures();
        void freeAllShaders();
        void freeAllSounds();
        void freeAllMusics();
        void freeAllFonts();
        void freeAll();

    private:
        void parseTags( const Json::Value& inRoot, std::vector< std::string >& outArray );
        Json::Value jsonTags( std::vector< std::string >& inArray );

        std::string m_fileSystemRoot;
        std::map< std::string, sf::Texture* > m_textures;
        std::map< std::string, sf::Shader* > m_shaders;
        std::map< std::string, sf::Sound* > m_sounds;
        std::map< std::string, sf::SoundBuffer* > m_soundsBuffs;
        std::map< std::string, sf::Music* > m_musics;
        std::map< std::string, sf::Font* > m_fonts;
        std::map< std::string, std::string > m_metaTextures;
        std::map< std::string, std::string > m_metaShaders;
        std::map< std::string, std::string > m_metaSounds;
        std::map< std::string, std::string > m_metaMusics;
        std::map< std::string, std::string > m_metaFonts;
        std::map< std::string, std::vector< std::string > > m_tagsTextures;
        std::map< std::string, std::vector< std::string > > m_tagsShaders;
        std::map< std::string, std::vector< std::string > > m_tagsSounds;
        std::map< std::string, std::vector< std::string > > m_tagsMusics;
        std::map< std::string, std::vector< std::string > > m_tagsFonts;
        std::map< std::string, std::vector< std::string > > m_uniformsShaders;
        sf::Texture* m_defaultTexture;
        AssetsChangedListener* m_assetsChangedListener;
    };

}

#endif
