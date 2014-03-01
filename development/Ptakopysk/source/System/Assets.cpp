#include "../../include/Ptakopysk/System/Assets.h"
#include <XeCore/Common/String.h>

namespace Ptakopysk
{

    RTTI_CLASS_DERIVATIONS( Assets,
                            RTTI_DERIVATIONS_END
                            )

    Assets::Assets()
    : RTTI_CLASS_DEFINE( Assets )
    {
    }

    Assets::~Assets()
    {
        freeAll();
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

    sf::Texture* Assets::jsonToTexture( const Json::Value& root )
    {
        if( !root.isObject() )
            return 0;
        Json::Value id = root[ "id" ];
        Json::Value path = root[ "path" ];
        if( id.isString() && path.isString() )
            return loadTexture( id.asString(), path.asString() );
        return 0;
    }

    sf::Shader* Assets::jsonToShader( const Json::Value& root )
    {
        if( !root.isObject() )
            return 0;
        Json::Value id = root[ "id" ];
        Json::Value vspath = root[ "vspath" ];
        Json::Value fspath = root[ "fspath" ];
        if( id.isString() && vspath.isString() && fspath.isString() )
            return loadShader( id.asString(), vspath.asString(), fspath.asString() );
        return 0;
    }

    sf::Sound* Assets::jsonToSound( const Json::Value& root )
    {
        if( !root.isObject() )
            return 0;
        Json::Value id = root[ "id" ];
        Json::Value path = root[ "path" ];
        if( id.isString() && path.isString() )
            return loadSound( id.asString(), path.asString() );
        return 0;
    }

    sf::Music* Assets::jsonToMusic( const Json::Value& root )
    {
        if( !root.isObject() )
            return 0;
        Json::Value id = root[ "id" ];
        Json::Value path = root[ "path" ];
        if( id.isString() && path.isString() )
            return loadMusic( id.asString(), path.asString() );
        return 0;
    }

    sf::Font* Assets::jsonToFont( const Json::Value& root )
    {
        if( !root.isObject() )
            return 0;
        Json::Value id = root[ "id" ];
        Json::Value path = root[ "path" ];
        if( id.isString() && path.isString() )
            return loadFont( id.asString(), path.asString() );
        return 0;
    }

    Json::Value Assets::assetsToJson()
    {
        Json::Value textures = texturesToJson();
        Json::Value shaders = shadersToJson();
        Json::Value sounds = soundsToJson();
        Json::Value musics = musicsToJson();
        Json::Value fonts = fontsToJson();
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

    Json::Value Assets::textureToJson( const std::string& id )
    {
        if( !m_metaTextures.count( id ) )
            return Json::Value::null;
        Json::Value root;
        root[ "id" ] = id;
        root[ "path" ] = m_metaTextures[ id ];
        return root;
    }

    Json::Value Assets::shaderToJson( const std::string& id )
    {
        if( !m_metaShaders.count( id ) )
            return Json::Value::null;
        Json::Value root;
        root[ "id" ] = id;
        XeCore::Common::String paths = m_metaShaders[ id ];
        unsigned int pc = 0;
        XeCore::Common::String* p = paths.explode( "|", pc, false );
        if( p && pc == 2 )
        {
            root[ "vspath" ] = p[ 0 ];
            root[ "fspath" ] = p[ 1 ];
        }
        else
            root = Json::Value::null;
        DELETE_ARRAY( p );
        return root;
    }

    Json::Value Assets::soundToJson( const std::string& id )
    {
        if( !m_metaSounds.count( id ) )
            return Json::Value::null;
        Json::Value root;
        root[ "id" ] = id;
        root[ "path" ] = m_metaSounds[ id ];
        return root;
    }

    Json::Value Assets::musicToJson( const std::string& id )
    {
        if( !m_metaMusics.count( id ) )
            return Json::Value::null;
        Json::Value root;
        root[ "id" ] = id;
        root[ "path" ] = m_metaMusics[ id ];
        return root;
    }

    Json::Value Assets::fontToJson( const std::string& id )
    {
        if( !m_metaFonts.count( id ) )
            return Json::Value::null;
        Json::Value root;
        root[ "id" ] = id;
        root[ "path" ] = m_metaFonts[ id ];
        return root;
    }

    sf::Texture* Assets::addTexture( const std::string& id, const sf::Texture* ptr )
    {
        sf::Texture* t = getTexture( id );
        if( !t && ptr )
        {
            t = (sf::Texture*)ptr;
            m_textures[ id ] = t;
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
        }
        return t;
    }

    sf::Texture* Assets::loadTexture( const std::string& id, const std::string& path )
    {
        sf::Texture* t = getTexture( id );
        if( !t )
        {
            t = xnew sf::Texture();
            if( !t->loadFromFile( path ) )
            {
                DELETE_OBJECT( t );
                return 0;
            }
            m_textures[ id ] = t;
            m_metaTextures[ id ] = path;
        }
        return t;
    }

    sf::Shader* Assets::loadShader( const std::string& id, const std::string& vspath, const std::string& fspath )
    {
        sf::Shader* t = getShader( id );
        if( !t )
        {
            t = xnew sf::Shader();
            if( !t->loadFromFile( vspath, fspath ) )
            {
                DELETE_OBJECT( t );
                return 0;
            }
            m_shaders[ id ] = t;
            m_metaShaders[ id ] = vspath + "|" + fspath;
        }
        return t;
    }

    sf::Sound* Assets::loadSound( const std::string& id, const std::string& path )
    {
        sf::Sound* t = getSound( id );
        if( !t )
        {
            sf::SoundBuffer* tb = xnew sf::SoundBuffer();
            if( !tb->loadFromFile( path ) )
            {
                DELETE_OBJECT( tb );
                return 0;
            }
            m_soundsBuffs[ id ] = tb;
            t = xnew sf::Sound();
            t->setBuffer( *tb );
            m_sounds[ id ] = t;
            m_metaSounds[ id ] = path;
        }
        return t;
    }

    sf::Music* Assets::loadMusic( const std::string& id, const std::string& path )
    {
        sf::Music* t = getMusic( id );
        if( !t )
        {
            t = xnew sf::Music();
            if( !t->openFromFile( path ) )
            {
                DELETE_OBJECT( t );
                return 0;
            }
            m_musics[ id ] = t;
            m_metaMusics[ id ] = path;
        }
        return t;
    }

    sf::Font* Assets::loadFont( const std::string& id, const std::string& path )
    {
        sf::Font* t = getFont( id );
        if( !t )
        {
            t = xnew sf::Font();
            if( !t->loadFromFile( path ) )
            {
                DELETE_OBJECT( t );
                return 0;
            }
            m_fonts[ id ] = t;
            m_metaFonts[ id ] = path;
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

    void Assets::freeTexture( const std::string& id )
    {
        if( m_textures.count( id ) )
        {
            sf::Texture* t = m_textures[ id ];
            DELETE_OBJECT( t );
            m_textures.erase( id );
        }
        if( m_metaTextures.count( id ) )
            m_metaTextures.erase( id );
    }

    void Assets::freeShader( const std::string& id )
    {
        if( m_shaders.count( id ) )
        {
            sf::Shader* t = m_shaders[ id ];
            DELETE_OBJECT( t );
            m_shaders.erase( id );
        }
        if( m_metaShaders.count( id ) )
            m_metaShaders.erase( id );
    }

    void Assets::freeSound( const std::string& id )
    {
        if( m_sounds.count( id ) )
        {
            sf::Sound* t = m_sounds[ id ];
            DELETE_OBJECT( t );
            m_sounds.erase( id );
        }
        if( m_metaSounds.count( id ) )
            m_metaSounds.erase( id );
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
            DELETE_OBJECT( t );
            m_musics.erase( id );
        }
        if( m_metaMusics.count( id ) )
            m_metaMusics.erase( id );
    }

    void Assets::freeFont( const std::string& id )
    {
        if( m_fonts.count( id ) )
        {
            sf::Font* t = m_fonts[ id ];
            DELETE_OBJECT( t );
            m_fonts.erase( id );
        }
        if( m_metaFonts.count( id ) )
            m_metaFonts.erase( id );
    }

    void Assets::freeAllTextures()
    {
        for( std::map< std::string, sf::Texture* >::iterator it = m_textures.begin(); it != m_textures.end(); it++ )
            DELETE_OBJECT( it->second );
        m_textures.clear();
        m_metaTextures.clear();
    }

    void Assets::freeAllShaders()
    {
        for( std::map< std::string, sf::Shader* >::iterator it = m_shaders.begin(); it != m_shaders.end(); it++ )
            DELETE_OBJECT( it->second );
        m_shaders.clear();
        m_metaShaders.clear();
    }

    void Assets::freeAllSounds()
    {
        for( std::map< std::string, sf::SoundBuffer* >::iterator it = m_soundsBuffs.begin(); it != m_soundsBuffs.end(); it++ )
            DELETE_OBJECT( it->second );
        m_soundsBuffs.clear();
        for( std::map< std::string, sf::Sound* >::iterator it = m_sounds.begin(); it != m_sounds.end(); it++ )
            DELETE_OBJECT( it->second );
        m_sounds.clear();
        m_metaSounds.clear();
    }

    void Assets::freeAllMusics()
    {
        for( std::map< std::string, sf::Music* >::iterator it = m_musics.begin(); it != m_musics.end(); it++ )
            DELETE_OBJECT( it->second );
        m_musics.clear();
        m_metaMusics.clear();
    }

    void Assets::freeAllFonts()
    {
        for( std::map< std::string, sf::Font* >::iterator it = m_fonts.begin(); it != m_fonts.end(); it++ )
            DELETE_OBJECT( it->second );
        m_fonts.clear();
        m_metaFonts.clear();
    }

    void Assets::freeAll()
    {
        freeAllTextures();
        freeAllShaders();
        freeAllSounds();
        freeAllMusics();
        freeAllFonts();
    }

}
