#include "../../include/Ptakopysk/System/Assets.h"
#include <XeCore/Common/String.h>
#include <XeCore/Common/Logger.h>
#include <fstream>
#include <BinaryJson/BinaryJson.h>

namespace Ptakopysk
{

    RTTI_CLASS_DERIVATIONS( ICustomAsset,
                            RTTI_DERIVATIONS_END
                            )

    ICustomAsset::ICustomAsset()
    : RTTI_CLASS_DEFINE( ICustomAsset )
    {
    }

    RTTI_CLASS_DERIVATIONS( Assets,
                            RTTI_DERIVATIONS_END
                            )

    Assets::Assets()
    : RTTI_CLASS_DEFINE( Assets )
    , m_loadingMode( LoadOnlyValidAssets )
    , m_assetsChangedListener( 0 )
    {
        m_defaultTexture = xnew sf::Texture();
        m_defaultTexture->create( 1, 1 );
        m_defaultTexture->update( &sf::Color::White.r );
    }

    Assets::~Assets()
    {
        freeAll();
        DELETE_OBJECT( m_defaultTexture );
    }

    Json::Value Assets::loadJson( const std::string& path, bool binary, dword binaryKeyHash )
    {
        std::ifstream file;
        if( m_fileSystemRoot.empty() )
        {
            file.open( path.c_str(), std::ifstream::in | std::ifstream::binary );
            if( !file.good() )
            {
                file.close();
                return Json::Value::null;
            }
        }
        else
        {
            std::stringstream ss;
            ss << m_fileSystemRoot.c_str() << path;
            file.open( ss.str().c_str(), std::ifstream::in | std::ifstream::binary );
            if( !file.good() )
            {
                file.close();
                return Json::Value::null;
            }
        }
        file.seekg( 0, std::ifstream::end );
        unsigned int fsize = file.tellg();
        file.seekg( 0, std::ifstream::beg );
        Json::Value root;
        std::string content;
        if( binary )
        {
            BinaryJson::Buffer buffer;
            buffer.resize( fsize );
            file.read( (char*)buffer.data(), fsize );
            BinaryJson::binaryToJson( &buffer, root, binaryKeyHash );
        }
        else
        {
            content.resize( fsize + 1, 0 );
            file.read( (char*)content.c_str(), fsize );
        }
        file.close();
        Json::Reader reader;
        reader.parse( content, root );
        return root;
    }

    bool Assets::saveJson( const std::string& path, const Json::Value& root, bool binary, dword binaryKeyHash )
    {
        std::ofstream file;
        if( m_fileSystemRoot.empty() )
        {
            file.open( path.c_str(), std::ofstream::out | std::ofstream::binary );
            if( !file.good() )
            {
                file.close();
                return false;
            }
        }
        else
        {
            std::stringstream ss;
            ss << m_fileSystemRoot.c_str() << path;
            file.open( ss.str().c_str(), std::ofstream::out | std::ofstream::binary );
            if( !file.good() )
            {
                file.close();
                return false;
            }
        }
        if( binary )
        {
            BinaryJson::Buffer buffer;
            buffer.setAutoResize();
            BinaryJson::jsonToBinary( (Json::Value&)root, &buffer, binaryKeyHash );
            unsigned int pos = buffer.position();
            buffer.reposition();
            file.write( (char*)buffer.data(), pos );
        }
        else
        {
            Json::StyledWriter writer;
            std::string content = writer.write( root );
            file.write( content.c_str(), content.length() );
        }
        file.close();
        return true;
    }

    std::string Assets::loadText( const std::string& path )
    {
        std::ifstream file;
        if( m_fileSystemRoot.empty() )
        {
            file.open( path.c_str(), std::ifstream::in | std::ifstream::binary );
            if( !file.good() )
            {
                file.close();
                return "";
            }
        }
        else
        {
            std::stringstream ss;
            ss << m_fileSystemRoot.c_str() << path;
            file.open( ss.str().c_str(), std::ifstream::in | std::ifstream::binary );
            if( !file.good() )
            {
                file.close();
                return "";
            }
        }
        file.seekg( 0, std::ifstream::end );
        unsigned int fsize = file.tellg();
        file.seekg( 0, std::ifstream::beg );
        std::string content;
        content.resize( fsize + 1, 0 );
        file.read( (char*)content.c_str(), fsize );
        file.close();
        return content;
    }

    bool Assets::saveText( const std::string& path, const std::string& content )
    {
        std::ofstream file;
        if( m_fileSystemRoot.empty() )
        {
            file.open( path.c_str(), std::ofstream::out | std::ofstream::binary );
            if( !file.good() )
            {
                file.close();
                return false;
            }
        }
        else
        {
            std::stringstream ss;
            ss << m_fileSystemRoot.c_str() << path;
            file.open( ss.str().c_str(), std::ofstream::out | std::ofstream::binary );
            if( !file.good() )
            {
                file.close();
                return false;
            }
        }
        file.write( content.c_str(), content.length() );
        file.close();
        return true;
    }

    bool Assets::registerCustomAssetFactory( const std::string& id, XeCore::Common::IRtti::Derivation type, ICustomAsset::OnBuildCustomAssetCallback builder )
    {
        if( id.empty() || m_customFactory.count( id ) || !type || !builder )
            return false;
        CustomAssetFactoryData d;
        d.type = type;
        d.builder = builder;
        m_customFactory[ id ] = d;
        return true;
    }

    bool Assets::unregisterCustomAssetFactory( const std::string& id )
    {
        if( m_customFactory.count( id ) )
        {
            m_customFactory.erase( id );
            return true;
        }
        return false;
    }

    bool Assets::unregisterCustomAssetFactory( XeCore::Common::IRtti::Derivation type )
    {
        for( std::map< std::string, CustomAssetFactoryData >::iterator it = m_customFactory.begin(); it != m_customFactory.end(); it++ )
        {
            if( it->second.type == type )
            {
                m_customFactory.erase( it );
                return true;
            }
        }
        return false;
    }

    bool Assets::unregisterCustomAssetFactory( ICustomAsset::OnBuildCustomAssetCallback builder )
    {
        for( std::map< std::string, CustomAssetFactoryData >::iterator it = m_customFactory.begin(); it != m_customFactory.end(); it++ )
        {
            if( it->second.builder == builder )
            {
                m_customFactory.erase( it );
                return true;
            }
        }
        return false;
    }

    void Assets::unregisterAllCustomAssetFactories()
    {
        m_customFactory.clear();
    }

    XeCore::Common::IRtti::Derivation Assets::findCustomAssetFactoryTypeById( const std::string& id )
    {
        if( m_customFactory.count( id ) )
            return m_customFactory[ id ].type;
        return 0;
    }

    XeCore::Common::IRtti::Derivation Assets::findCustomAssetFactoryTypeByBuilder( ICustomAsset::OnBuildCustomAssetCallback builder )
    {
        for( std::map< std::string, CustomAssetFactoryData >::iterator it = m_customFactory.begin(); it != m_customFactory.end(); it++ )
            if( it->second.builder == builder )
                return it->second.type;
        return 0;
    }

    std::string Assets::findCustomAssetFactoryIdByType( XeCore::Common::IRtti::Derivation type )
    {
        for( std::map< std::string, CustomAssetFactoryData >::iterator it = m_customFactory.begin(); it != m_customFactory.end(); it++ )
            if( it->second.type == type )
                return it->first;
        return std::string();
    }

    std::string Assets::findCustomAssetFactoryIdByBuilder( ICustomAsset::OnBuildCustomAssetCallback builder )
    {
        for( std::map< std::string, CustomAssetFactoryData >::iterator it = m_customFactory.begin(); it != m_customFactory.end(); it++ )
            if( it->second.builder == builder )
                return it->first;
        return std::string();
    }

    ICustomAsset::OnBuildCustomAssetCallback Assets::findCustomAssetFactoryBuilderById( const std::string& id )
    {
        if( m_customFactory.count( id ) )
            return m_customFactory[ id ].builder;
        return 0;
    }

    ICustomAsset::OnBuildCustomAssetCallback Assets::findCustomAssetFactoryBuilderByType( XeCore::Common::IRtti::Derivation type )
    {
        for( std::map< std::string, CustomAssetFactoryData >::iterator it = m_customFactory.begin(); it != m_customFactory.end(); it++ )
            if( it->second.type == type )
                return it->second.builder;
        return 0;
    }

    ICustomAsset* Assets::buildCustomAsset( const std::string& id )
    {
        if( m_customFactory.count( id ) )
            return m_customFactory[ id ].builder();
        return 0;
    }

    ICustomAsset* Assets::buildCustomAsset( XeCore::Common::IRtti::Derivation type )
    {
        for( std::map< std::string, CustomAssetFactoryData >::iterator it = m_customFactory.begin(); it != m_customFactory.end(); it++ )
            if( it->second.type == type )
                return it->second.builder();
        return 0;
    }

    void Assets::jsonToAssets( const Json::Value& root )
    {
        if( !root.isObject() )
            return;
        jsonToTextures( root[ "textures" ] );
        jsonToShaders( root[ "shaders" ] );
        jsonToSounds( root[ "sounds" ] );
        jsonToMusics( root[ "musics" ] );
        jsonToFonts( root[ "fonts" ] );
        jsonToCustomAssets( root[ "custom" ] );
    }

    void Assets::jsonToTextures( const Json::Value& root )
    {
        if( !root.isArray() )
            return;
        for( unsigned int i = 0; i < root.size(); i++ )
            jsonToTexture( root[ i ] );
    }

    void Assets::jsonToShaders( const Json::Value& root )
    {
        if( !root.isArray() )
            return;
        for( unsigned int i = 0; i < root.size(); i++ )
            jsonToShader( root[ i ] );
    }

    void Assets::jsonToSounds( const Json::Value& root )
    {
        if( !root.isArray() )
            return;
        for( unsigned int i = 0; i < root.size(); i++ )
            jsonToSounds( root[ i ] );
    }

    void Assets::jsonToMusics( const Json::Value& root )
    {
        if( !root.isArray() )
            return;
        for( unsigned int i = 0; i < root.size(); i++ )
            jsonToMusic( root[ i ] );
    }

    void Assets::jsonToFonts( const Json::Value& root )
    {
        if( !root.isArray() )
            return;
        for( unsigned int i = 0; i < root.size(); i++ )
            jsonToFont( root[ i ] );
    }

    void Assets::jsonToCustomAssets( const Json::Value& root )
    {
        if( !root.isArray() )
            return;
        for( unsigned int i = 0; i < root.size(); i++ )
            jsonToCustomAsset( root[ i ] );
    }

    sf::Texture* Assets::jsonToTexture( const Json::Value& root )
    {
        if( !root.isObject() )
            return 0;
        Json::Value id = root[ "id" ];
        Json::Value path = root[ "path" ];
        if( id.isString() && path.isString() )
        {
            Json::Value tags = root[ "tags" ];
            if( tags.isArray() && tags.size() )
                parseTags( tags, m_tagsTextures[ id.asString() ] );
            return loadTexture( id.asString(), path.asString() );
        }
        return 0;
    }

    sf::Shader* Assets::jsonToShader( const Json::Value& root )
    {
        if( !root.isObject() )
            return 0;
        Json::Value id = root[ "id" ];
        Json::Value vspath = root[ "vspath" ];
        Json::Value fspath = root[ "fspath" ];
        Json::Value uniforms = root[ "uniforms" ];
        Json::Value item;
        std::vector< std::string > u;
        if( uniforms.isArray() && uniforms.size() > 0 )
        {
            for( unsigned int i = 0; i < uniforms.size(); i++ )
            {
                item = uniforms[ i ];
                if( item.isString() )
                    u.push_back( item.asString() );
            }
        }
        if( id.isString() && vspath.isString() && fspath.isString() )
        {
            Json::Value tags = root[ "tags" ];
            if( tags.isArray() && tags.size() )
                parseTags( tags, m_tagsShaders[ id.asString() ] );
            return loadShader( id.asString(), vspath.asString(), fspath.asString(), u.data(), u.size() );
        }
        return 0;
    }

    sf::Sound* Assets::jsonToSound( const Json::Value& root )
    {
        if( !root.isObject() )
            return 0;
        Json::Value id = root[ "id" ];
        Json::Value path = root[ "path" ];
        if( id.isString() && path.isString() )
        {
            Json::Value tags = root[ "tags" ];
            if( tags.isArray() && tags.size() )
                parseTags( tags, m_tagsSounds[ id.asString() ] );
            return loadSound( id.asString(), path.asString() );
        }
        return 0;
    }

    sf::Music* Assets::jsonToMusic( const Json::Value& root )
    {
        if( !root.isObject() )
            return 0;
        Json::Value id = root[ "id" ];
        Json::Value path = root[ "path" ];
        if( id.isString() && path.isString() )
        {
            Json::Value tags = root[ "tags" ];
            if( tags.isArray() && tags.size() )
                parseTags( tags, m_tagsMusics[ id.asString() ] );
            return loadMusic( id.asString(), path.asString() );
        }
        return 0;
    }

    sf::Font* Assets::jsonToFont( const Json::Value& root )
    {
        if( !root.isObject() )
            return 0;
        Json::Value id = root[ "id" ];
        Json::Value path = root[ "path" ];
        if( id.isString() && path.isString() )
        {
            Json::Value tags = root[ "tags" ];
            if( tags.isArray() && tags.size() )
                parseTags( tags, m_tagsFonts[ id.asString() ] );
            return loadFont( id.asString(), path.asString() );
        }
        return 0;
    }

    ICustomAsset* Assets::jsonToCustomAsset( const Json::Value& root )
    {
        if( !root.isObject() )
            return 0;
        Json::Value id = root[ "id" ];
        Json::Value type = root[ "type" ];
        Json::Value path = root[ "path" ];
        if( id.isString() && type.isString() && m_customFactory.count( type.asString() ) && path.isString() )
        {
            Json::Value tags = root[ "tags" ];
            if( tags.isArray() && tags.size() )
                parseTags( tags, m_tagsCustom[ id.asString() ] );
            return loadCustomAsset( id.asString(), type.asString(), path.asString() );
        }
        return 0;
    }

    Json::Value Assets::assetsToJson()
    {
        Json::Value textures = texturesToJson();
        Json::Value shaders = shadersToJson();
        Json::Value sounds = soundsToJson();
        Json::Value musics = musicsToJson();
        Json::Value fonts = fontsToJson();
        Json::Value custom = customAssetsToJson();
        Json::Value root;
        if( !textures.isNull() )
            root[ "textures" ] = textures;
        if( !shaders.isNull() )
            root[ "shaders" ] = shaders;
        if( !sounds.isNull() )
            root[ "sounds" ] = sounds;
        if( !musics.isNull() )
            root[ "musics" ] = musics;
        if( !fonts.isNull() )
            root[ "fonts" ] = fonts;
        if( !custom.isNull() )
            root[ "custom" ] = custom;
        return root;
    }

    Json::Value Assets::texturesToJson()
    {
        Json::Value root;
        Json::Value item;
        for( std::map< std::string, std::string >::iterator it = m_metaTextures.begin(); it != m_metaTextures.end(); it++ )
        {
            item = textureToJson( it->first );
            if( !item.isNull() )
                root.append( item );
        }
        return root;
    }

    Json::Value Assets::shadersToJson()
    {
        Json::Value root;
        Json::Value item;
        for( std::map< std::string, std::string >::iterator it = m_metaShaders.begin(); it != m_metaShaders.end(); it++ )
        {
            item = shaderToJson( it->first );
            if( !item.isNull() )
                root.append( item );
        }
        return root;
    }

    Json::Value Assets::soundsToJson()
    {
        Json::Value root;
        Json::Value item;
        for( std::map< std::string, std::string >::iterator it = m_metaSounds.begin(); it != m_metaSounds.end(); it++ )
        {
            item = soundToJson( it->first );
            if( !item.isNull() )
                root.append( item );
        }
        return root;
    }

    Json::Value Assets::musicsToJson()
    {
        Json::Value root;
        Json::Value item;
        for( std::map< std::string, std::string >::iterator it = m_metaMusics.begin(); it != m_metaMusics.end(); it++ )
        {
            item = musicToJson( it->first );
            if( !item.isNull() )
                root.append( item );
        }
        return root;
    }

    Json::Value Assets::fontsToJson()
    {
        Json::Value root;
        Json::Value item;
        for( std::map< std::string, std::string >::iterator it = m_metaFonts.begin(); it != m_metaFonts.end(); it++ )
        {
            item = fontToJson( it->first );
            if( !item.isNull() )
                root.append( item );
        }
        return root;
    }

    Json::Value Assets::customAssetsToJson()
    {
        Json::Value root;
        Json::Value item;
        for( std::map< std::string, std::string >::iterator it = m_metaCustom.begin(); it != m_metaCustom.end(); it++ )
        {
            item = customAssetToJson( it->first );
            if( !item.isNull() )
                root.append( item );
        }
        return root;
    }

    Json::Value Assets::textureToJson( const std::string& id )
    {
        if( !m_metaTextures.count( id ) )
            return Json::Value::null;
        Json::Value root;
        root[ "id" ] = id;
        root[ "path" ] = m_metaTextures[ id ];
        if( m_tagsTextures.count( id ) )
            root[ "tags" ] = jsonTags( m_tagsTextures[ id ] );
        return root;
    }

    Json::Value Assets::shaderToJson( const std::string& id )
    {
        if( !m_metaShaders.count( id ) )
            return Json::Value::null;
        Json::Value root;
        root[ "id" ] = id;
        XeCore::Common::String meta = m_metaShaders[ id ];
        unsigned int mc = 0;
        XeCore::Common::String* m = meta.explode( "|", mc, false );
        if( m && mc == 2 )
        {
            root[ "vspath" ] = m[ 0 ];
            root[ "fspath" ] = m[ 1 ];
            if( m_tagsShaders.count( id ) )
                root[ "tags" ] = jsonTags( m_tagsShaders[ id ] );
            if( m_uniformsShaders.count( id ) )
            {
                std::vector< std::string > u = m_uniformsShaders[ id ];
                Json::Value uniforms;
                for( unsigned int i = 0; i < u.size(); i++ )
                    uniforms.append( u[ i ] );
                if( !uniforms.isNull() )
                    root[ "uniforms" ] = uniforms;
            }
        }
        else
            root = Json::Value::null;
        DELETE_ARRAY( m );
        return root;
    }

    Json::Value Assets::soundToJson( const std::string& id )
    {
        if( !m_metaSounds.count( id ) )
            return Json::Value::null;
        Json::Value root;
        root[ "id" ] = id;
        root[ "path" ] = m_metaSounds[ id ];
        if( m_tagsSounds.count( id ) )
            root[ "tags" ] = jsonTags( m_tagsSounds[ id ] );
        return root;
    }

    Json::Value Assets::musicToJson( const std::string& id )
    {
        if( !m_metaMusics.count( id ) )
            return Json::Value::null;
        Json::Value root;
        root[ "id" ] = id;
        root[ "path" ] = m_metaMusics[ id ];
        if( m_tagsMusics.count( id ) )
            root[ "tags" ] = jsonTags( m_tagsMusics[ id ] );
        return root;
    }

    Json::Value Assets::fontToJson( const std::string& id )
    {
        if( !m_metaFonts.count( id ) )
            return Json::Value::null;
        Json::Value root;
        root[ "id" ] = id;
        root[ "path" ] = m_metaFonts[ id ];
        if( m_tagsFonts.count( id ) )
            root[ "tags" ] = jsonTags( m_tagsFonts[ id ] );
        return root;
    }

    Json::Value Assets::customAssetToJson( const std::string& id )
    {
        if( !m_metaCustom.count( id ) )
            return Json::Value::null;
        Json::Value root;
        root[ "id" ] = id;
        XeCore::Common::String meta = m_metaCustom[ id ];
        unsigned int mc = 0;
        XeCore::Common::String* m = meta.explode( "|", mc, false );
        if( m && mc == 2 )
        {
            root[ "type" ] = m[ 0 ];
            root[ "path" ] = m[ 1 ];
            if( m_tagsCustom.count( id ) )
                root[ "tags" ] = jsonTags( m_tagsCustom[ id ] );
        }
        else
            root = Json::Value::null;
        DELETE_ARRAY( m );
        return root;
    }

    sf::Texture* Assets::addTexture( const std::string& id, const sf::Texture* ptr )
    {
        sf::Texture* t = getTexture( id );
        if( !t && ptr )
        {
            t = (sf::Texture*)ptr;
            m_textures[ id ] = t;
            if( m_assetsChangedListener )
                m_assetsChangedListener->onTextureChanged( id, ptr, true );
        }
        return t;
    }

    sf::Shader* Assets::addShader( const std::string& id, const sf::Shader* ptr )
    {
        sf::Shader* t = getShader( id );
        if( !t && ptr )
        {
            t = (sf::Shader*)ptr;
            m_shaders[ id ] = t;
            if( m_assetsChangedListener )
                m_assetsChangedListener->onShaderChanged( id, ptr, true );
        }
        return t;
    }

    sf::Sound* Assets::addSound( const std::string& id, const sf::SoundBuffer* ptrbuff, const sf::Sound* ptr )
    {
        sf::Sound* t = getSound( id );
        if( !t && ptrbuff && ptr )
        {
            t = (sf::Sound*)ptr;
            m_soundsBuffs[ id ] = (sf::SoundBuffer*)ptrbuff;
            m_sounds[ id ] = t;
            if( m_assetsChangedListener )
                m_assetsChangedListener->onSoundChanged( id, ptr, true );
        }
        return t;
    }

    sf::Music* Assets::addMusic( const std::string& id, const sf::Music* ptr )
    {
        sf::Music* t = getMusic( id );
        if( !t && ptr )
        {
            t = (sf::Music*)ptr;
            m_musics[ id ] = t;
            if( m_assetsChangedListener )
                m_assetsChangedListener->onMusicChanged( id, ptr, true );
        }
        return t;
    }

    sf::Font* Assets::addFont( const std::string& id, const sf::Font* ptr )
    {
        sf::Font* t = getFont( id );
        if( !t && ptr )
        {
            t = (sf::Font*)ptr;
            m_fonts[ id ] = t;
            if( m_assetsChangedListener )
                m_assetsChangedListener->onFontChanged( id, ptr, true );
        }
        return t;
    }

    ICustomAsset* Assets::addCustomAsset( const std::string& id, const ICustomAsset* ptr )
    {
        ICustomAsset* t = getCustomAsset( id );
        if( !t && ptr )
        {
            t = (ICustomAsset*)ptr;
            m_custom[ id ] = t;
            if( m_assetsChangedListener )
                m_assetsChangedListener->onCustomAssetChanged( id, ptr, true );
        }
        return t;
    }

    sf::Texture* Assets::loadTexture( const std::string& id, const std::string& path )
    {
        sf::Texture* t = getTexture( id );
        if( !t )
        {
            t = xnew sf::Texture();
            if( m_fileSystemRoot.empty() )
            {
                if( !t->loadFromFile( path ) )
                {
                    if( m_loadingMode != LoadIfFilesExists || !fileExists( path ) )
                    {
                        DELETE_OBJECT( t );
                        return 0;
                    }
                }
            }
            else
            {
                std::stringstream ss;
                ss << m_fileSystemRoot.c_str() << path;
                if( !t->loadFromFile( ss.str() ) )
                {
                    if( m_loadingMode != LoadIfFilesExists || !fileExists( ss.str() ) )
                    {
                        DELETE_OBJECT( t );
                        return 0;
                    }
                }
            }
            m_textures[ id ] = t;
            m_metaTextures[ id ] = path;
            if( m_assetsChangedListener )
                m_assetsChangedListener->onTextureChanged( id, t, true );
        }
        return t;
    }

    sf::Shader* Assets::loadShader( const std::string& id, const std::string& vspath, const std::string& fspath, const std::string* uniforms, unsigned int uniformsCount )
    {
        sf::Shader* t = getShader( id );
        if( !t )
        {
            if( !sf::Shader::isAvailable() && m_loadingMode == LoadOnlyValidAssets )
                return 0;
            t = xnew sf::Shader();
            if( m_fileSystemRoot.empty() )
            {
                if( !t->loadFromFile( vspath, fspath ) )
                {
                    if( m_loadingMode != LoadIfFilesExists || !fileExists( vspath ) || !fileExists( fspath ) )
                    {
                        DELETE_OBJECT( t );
                        return 0;
                    }
                }
            }
            else
            {
                std::stringstream ssv;
                ssv << m_fileSystemRoot.c_str() << vspath;
                std::stringstream ssf;
                ssf << m_fileSystemRoot.c_str() << fspath;
                if( !t->loadFromFile( ssv.str(), ssf.str() ) )
                {
                    if( m_loadingMode != LoadIfFilesExists || !fileExists( ssv.str() ) || !fileExists( ssf.str() ) )
                    {
                        DELETE_OBJECT( t );
                        return 0;
                    }
                }
            }
            m_shaders[ id ] = t;
            m_metaShaders[ id ] = vspath + "|" + fspath;
            m_uniformsShaders[ id ].clear();
            for( unsigned int i = 0; i < uniformsCount; i++ )
                m_uniformsShaders[ id ].push_back( uniforms[ i ] );
            if( m_assetsChangedListener )
                m_assetsChangedListener->onShaderChanged( id, t, true );
        }
        return t;
    }

    sf::Sound* Assets::loadSound( const std::string& id, const std::string& path )
    {
        sf::Sound* t = getSound( id );
        if( !t )
        {
            sf::SoundBuffer* tb = xnew sf::SoundBuffer();
            if( m_fileSystemRoot.empty() )
            {
                if( !tb->loadFromFile( path ) )
                {
                    if( m_loadingMode != LoadIfFilesExists || !fileExists( path ) )
                    {
                        DELETE_OBJECT( tb );
                        return 0;
                    }
                }
            }
            else
            {
                std::stringstream ss;
                ss << m_fileSystemRoot.c_str() << path;
                if( !tb->loadFromFile( ss.str() ) )
                {
                    if( m_loadingMode != LoadIfFilesExists || !fileExists( ss.str() ) )
                    {
                        DELETE_OBJECT( tb );
                        return 0;
                    }
                }
            }
            m_soundsBuffs[ id ] = tb;
            t = xnew sf::Sound();
            t->setBuffer( *tb );
            m_sounds[ id ] = t;
            m_metaSounds[ id ] = path;
            if( m_assetsChangedListener )
                m_assetsChangedListener->onSoundChanged( id, t, true );
        }
        return t;
    }

    sf::Music* Assets::loadMusic( const std::string& id, const std::string& path )
    {
        sf::Music* t = getMusic( id );
        if( !t )
        {
            t = xnew sf::Music();
            if( m_fileSystemRoot.empty() )
            {
                if( !t->openFromFile( path ) )
                {
                    if( m_loadingMode != LoadIfFilesExists || !fileExists( path ) )
                    {
                        DELETE_OBJECT( t );
                        return 0;
                    }
                }
            }
            else
            {
                std::stringstream ss;
                ss << m_fileSystemRoot.c_str() << path;
                if( !t->openFromFile( ss.str() ) )
                {
                    if( m_loadingMode != LoadIfFilesExists || !fileExists( ss.str() ) )
                    {
                        DELETE_OBJECT( t );
                        return 0;
                    }
                }
            }
            m_musics[ id ] = t;
            m_metaMusics[ id ] = path;
            if( m_assetsChangedListener )
                m_assetsChangedListener->onMusicChanged( id, t, true );
        }
        return t;
    }

    sf::Font* Assets::loadFont( const std::string& id, const std::string& path )
    {
        sf::Font* t = getFont( id );
        if( !t )
        {
            t = xnew sf::Font();
            if( m_fileSystemRoot.empty() )
            {
                if( !t->loadFromFile( path ) )
                {
                    if( m_loadingMode != LoadIfFilesExists || !fileExists( path ) )
                    {
                        DELETE_OBJECT( t );
                        return 0;
                    }
                }
            }
            else
            {
                std::stringstream ss;
                ss << m_fileSystemRoot.c_str() << path;
                if( !t->loadFromFile( ss.str() ) )
                {
                    if( m_loadingMode != LoadIfFilesExists || !fileExists( ss.str() ) )
                    {
                        DELETE_OBJECT( t );
                        return 0;
                    }
                }
            }
            m_fonts[ id ] = t;
            m_metaFonts[ id ] = path;
            if( m_assetsChangedListener )
                m_assetsChangedListener->onFontChanged( id, t, true );
        }
        return t;
    }

    ICustomAsset* Assets::loadCustomAsset( const std::string& id, const std::string& type, const std::string& path )
    {
        ICustomAsset* t = getCustomAsset( id );
        if( !t && m_customFactory.count( type ) )
        {
            t = m_customFactory[ type ].builder();
            if( !t )
                return 0;
            if( m_fileSystemRoot.empty() )
            {
                if( !t->onLoad( path ) )
                {
                    if( m_loadingMode != LoadIfFilesExists || !fileExists( path ) )
                    {
                        DELETE_OBJECT( t );
                        return 0;
                    }
                }
            }
            else
            {
                std::stringstream ss;
                ss << m_fileSystemRoot.c_str() << path;
                if( !t->onLoad( ss.str() ) )
                {
                    if( m_loadingMode != LoadIfFilesExists || !fileExists( ss.str() ) )
                    {
                        DELETE_OBJECT( t );
                        return 0;
                    }
                }
            }
            m_custom[ id ] = t;
            m_metaCustom[ id ] = type + "|" + path;
            if( m_assetsChangedListener )
                m_assetsChangedListener->onCustomAssetChanged( id, t, true );
        }
        return t;
    }

    sf::Texture* Assets::getTexture( const std::string& id )
    {
        return m_textures.count( id ) ? m_textures[ id ] : 0;
    }

    sf::Shader* Assets::getShader( const std::string& id )
    {
        return m_shaders.count( id ) ? m_shaders[ id ] : 0;
    }

    sf::Sound* Assets::getSound( const std::string& id )
    {
        return m_sounds.count( id ) ? m_sounds[ id ] : 0;
    }

    sf::Music* Assets::getMusic( const std::string& id )
    {
        return m_musics.count( id ) ? m_musics[ id ] : 0;
    }

    sf::Font* Assets::getFont( const std::string& id )
    {
        return m_fonts.count( id ) ? m_fonts[ id ] : 0;
    }

    ICustomAsset* Assets::getCustomAsset( const std::string& id )
    {
        return m_custom.count( id ) ? m_custom[ id ] : 0;
    }

    std::string Assets::findTexture( const sf::Texture* ptr )
    {
        if( !ptr )
            return "";
        for( std::map< std::string, sf::Texture* >::iterator it = m_textures.begin(); it != m_textures.end(); it++ )
            if( it->second == ptr )
                return it->first;
        return "";
    }

    std::string Assets::findShader( const sf::Shader* ptr )
    {
        if( !ptr )
            return "";
        for( std::map< std::string, sf::Shader* >::iterator it = m_shaders.begin(); it != m_shaders.end(); it++ )
            if( it->second == ptr )
                return it->first;
        return "";
    }

    std::string Assets::findSound( const sf::Sound* ptr )
    {
        if( !ptr )
            return "";
        for( std::map< std::string, sf::Sound* >::iterator it = m_sounds.begin(); it != m_sounds.end(); it++ )
            if( it->second == ptr )
                return it->first;
        return "";
    }

    std::string Assets::findMusic( const sf::Music* ptr )
    {
        if( !ptr )
            return "";
        for( std::map< std::string, sf::Music* >::iterator it = m_musics.begin(); it != m_musics.end(); it++ )
            if( it->second == ptr )
                return it->first;
        return "";
    }

    std::string Assets::findFont( const sf::Font* ptr )
    {
        if( !ptr )
            return "";
        for( std::map< std::string, sf::Font* >::iterator it = m_fonts.begin(); it != m_fonts.end(); it++ )
            if( it->second == ptr )
                return it->first;
        return "";
    }

    std::string Assets::findCustomAsset( const ICustomAsset* ptr )
    {
        if( !ptr )
            return "";
        for( std::map< std::string, ICustomAsset* >::iterator it = m_custom.begin(); it != m_custom.end(); it++ )
            if( it->second == ptr )
                return it->first;
        return "";
    }

    bool Assets::shadersAvailable()
    {
        return sf::Shader::isAvailable();
    }

    void Assets::freeTexture( const std::string& id )
    {
        if( m_textures.count( id ) )
        {
            sf::Texture* t = m_textures[ id ];
            if( m_assetsChangedListener )
                m_assetsChangedListener->onTextureChanged( id, t, false );
            DELETE_OBJECT( t );
            m_textures.erase( id );
        }
        if( m_metaTextures.count( id ) )
            m_metaTextures.erase( id );
        if( m_tagsTextures.count( id ) )
            m_tagsTextures.erase( id );
    }

    void Assets::freeShader( const std::string& id )
    {
        if( m_shaders.count( id ) )
        {
            sf::Shader* t = m_shaders[ id ];
            if( m_assetsChangedListener )
                m_assetsChangedListener->onShaderChanged( id, t, false );
            DELETE_OBJECT( t );
            m_shaders.erase( id );
        }
        if( m_metaShaders.count( id ) )
            m_metaShaders.erase( id );
        if( m_tagsShaders.count( id ) )
            m_tagsShaders.erase( id );
        if( m_uniformsShaders.count( id ) )
            m_uniformsShaders.erase( id );
    }

    void Assets::freeSound( const std::string& id )
    {
        if( m_sounds.count( id ) )
        {
            sf::Sound* t = m_sounds[ id ];
            if( m_assetsChangedListener )
                m_assetsChangedListener->onSoundChanged( id, t, false );
            DELETE_OBJECT( t );
            m_sounds.erase( id );
        }
        if( m_metaSounds.count( id ) )
            m_metaSounds.erase( id );
        if( m_tagsSounds.count( id ) )
            m_tagsSounds.erase( id );
        if( m_soundsBuffs.count( id ) )
        {
            sf::SoundBuffer* t = m_soundsBuffs[ id ];
            DELETE_OBJECT( t );
            m_soundsBuffs.erase( id );
        }
    }

    void Assets::freeMusic( const std::string& id )
    {
        if( m_musics.count( id ) )
        {
            sf::Music* t = m_musics[ id ];
            if( m_assetsChangedListener )
                m_assetsChangedListener->onMusicChanged( id, t, false );
            DELETE_OBJECT( t );
            m_musics.erase( id );
        }
        if( m_metaMusics.count( id ) )
            m_metaMusics.erase( id );
        if( m_tagsMusics.count( id ) )
            m_tagsMusics.erase( id );
    }

    void Assets::freeFont( const std::string& id )
    {
        if( m_fonts.count( id ) )
        {
            sf::Font* t = m_fonts[ id ];
            if( m_assetsChangedListener )
                m_assetsChangedListener->onFontChanged( id, t, false );
            DELETE_OBJECT( t );
            m_fonts.erase( id );
        }
        if( m_metaFonts.count( id ) )
            m_metaFonts.erase( id );
        if( m_tagsFonts.count( id ) )
            m_tagsFonts.erase( id );
    }

    void Assets::freeCustomAsset( const std::string& id )
    {
        if( m_custom.count( id ) )
        {
            ICustomAsset* t = m_custom[ id ];
            if( m_assetsChangedListener )
                m_assetsChangedListener->onCustomAssetChanged( id, t, false );
            DELETE_OBJECT( t );
            m_custom.erase( id );
        }
        if( m_metaCustom.count( id ) )
            m_metaCustom.erase( id );
        if( m_tagsCustom.count( id ) )
            m_tagsCustom.erase( id );
    }

    void Assets::freeAllTextures()
    {
        for( std::map< std::string, sf::Texture* >::iterator it = m_textures.begin(); it != m_textures.end(); it++ )
        {
            if( m_assetsChangedListener )
                m_assetsChangedListener->onTextureChanged( it->first, it->second, false );
            DELETE_OBJECT( it->second );
        }
        m_textures.clear();
        m_metaTextures.clear();
        m_tagsTextures.clear();
    }

    void Assets::freeAllShaders()
    {
        for( std::map< std::string, sf::Shader* >::iterator it = m_shaders.begin(); it != m_shaders.end(); it++ )
        {
            if( m_assetsChangedListener )
                m_assetsChangedListener->onShaderChanged( it->first, it->second, false );
            DELETE_OBJECT( it->second );
        }
        m_shaders.clear();
        m_metaShaders.clear();
        m_tagsShaders.clear();
        m_uniformsShaders.clear();
    }

    void Assets::freeAllSounds()
    {
        for( std::map< std::string, sf::SoundBuffer* >::iterator it = m_soundsBuffs.begin(); it != m_soundsBuffs.end(); it++ )
            DELETE_OBJECT( it->second );
        m_soundsBuffs.clear();
        for( std::map< std::string, sf::Sound* >::iterator it = m_sounds.begin(); it != m_sounds.end(); it++ )
        {
            if( m_assetsChangedListener )
                m_assetsChangedListener->onSoundChanged( it->first, it->second, false );
            DELETE_OBJECT( it->second );
        }
        m_sounds.clear();
        m_metaSounds.clear();
        m_tagsSounds.clear();
    }

    void Assets::freeAllMusics()
    {
        for( std::map< std::string, sf::Music* >::iterator it = m_musics.begin(); it != m_musics.end(); it++ )
        {
            if( m_assetsChangedListener )
                m_assetsChangedListener->onMusicChanged( it->first, it->second, false );
            DELETE_OBJECT( it->second );
        }
        m_musics.clear();
        m_metaMusics.clear();
        m_tagsMusics.clear();
    }

    void Assets::freeAllFonts()
    {
        for( std::map< std::string, sf::Font* >::iterator it = m_fonts.begin(); it != m_fonts.end(); it++ )
        {
            if( m_assetsChangedListener )
                m_assetsChangedListener->onFontChanged( it->first, it->second, false );
            DELETE_OBJECT( it->second );
        }
        m_fonts.clear();
        m_metaFonts.clear();
        m_tagsFonts.clear();
    }

    void Assets::freeAllCustomAssets()
    {
        for( std::map< std::string, ICustomAsset* >::iterator it = m_custom.begin(); it != m_custom.end(); it++ )
        {
            if( m_assetsChangedListener )
                m_assetsChangedListener->onCustomAssetChanged( it->first, it->second, false );
            DELETE_OBJECT( it->second );
        }
        m_custom.clear();
        m_metaCustom.clear();
        m_tagsCustom.clear();
    }

    void Assets::freeAll()
    {
        freeAllTextures();
        freeAllShaders();
        freeAllSounds();
        freeAllMusics();
        freeAllFonts();
        freeAllCustomAssets();
    }

    void Assets::parseTags( const Json::Value& inRoot, std::vector< std::string >& outArray )
    {
        outArray.clear();
        if( inRoot.isArray() )
            for( unsigned int i = 0; i < inRoot.size(); i++ )
                outArray.push_back( inRoot[ i ].asString() );
    }

    Json::Value Assets::jsonTags( std::vector< std::string >& inArray )
    {
        Json::Value root = Json::Value( Json::arrayValue );
        for( std::vector< std::string >::iterator it = inArray.begin(); it != inArray.end(); it++ )
            root.append( *it );
        return root;
    }

    bool Assets::fileExists( const std::string& path )
    {
        std::ifstream f( path.c_str() );
        bool status = f.good();
        f.close();
        return status;
    }

}
