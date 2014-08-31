#include "../../include/Ptakopysk/CustomAssets/SpriteAtlasAsset.h"
#include <sstream>

namespace Ptakopysk
{

    RTTI_CLASS_DERIVATIONS( SpriteAtlasAsset,
                            RTTI_DERIVATION( ICustomAsset ),
                            RTTI_DERIVATIONS_END
                            )

    SpriteAtlasAsset::SpriteAtlasAsset()
    : RTTI_CLASS_DEFINE( SpriteAtlasAsset )
    {
    }

    bool SpriteAtlasAsset::getSubTexture( const std::string& name, sf::IntRect& outRect )
    {
        if( m_rects.count( name ) )
        {
            outRect = m_rects[ name ];
            return true;
        }
        else
        {
            outRect = sf::IntRect();
            return false;
        }
    }

    bool SpriteAtlasAsset::applySubTexture( const std::string& name, SpriteRenderer* sprite )
    {
        if( sprite && m_rects.count( name ) )
        {
            sf::IntRect rect = m_rects[ name ];
            sprite->setSize( sf::Vector2f( (float)rect.width, (float)rect.height ) );
            sprite->getRenderer()->setTextureRect( rect );
            return true;
        }
        return false;
    }

    bool SpriteAtlasAsset::addSubTexture( const std::string& name, const sf::IntRect& rect )
    {
        if( !m_rects.count( name ) )
        {
            m_rects[ name ] = rect;
            return true;
        }
        return false;
    }

    bool SpriteAtlasAsset::removeSubTexture( const std::string& name )
    {
        if( m_rects.count( name ) )
        {
            m_rects.erase( name );
            return true;
        }
        return false;
    }

    bool SpriteAtlasAsset::onLoad( const std::string& path )
    {
        m_rects.clear();
        Json::Value root = Assets::use().loadJson( path );
        if( root.isObject() && root.isMember( "TextureAtlas" ) )
        {
            Json::Value textureAtlas = root[ "TextureAtlas" ];
            if( textureAtlas.isObject() && textureAtlas.isMember( "SubTexture" ) )
            {
                Json::Value subTexture = textureAtlas[ "SubTexture" ];
                if( subTexture.isArray() && subTexture.size() > 0 )
                {
                    for( unsigned int i = 0; i < subTexture.size(); i++ )
                        readSubTexture( subTexture[ i ] );
                }
                else if( subTexture.isObject() )
                    readSubTexture( subTexture );
                return m_rects.size() > 0;
            }
        }
        return false;
    }

    void SpriteAtlasAsset::readSubTexture( const Json::Value& root )
    {
        if( !root.isObject() || !root.isMember( "@name" ) || !root.isMember( "@x" ) || !root.isMember( "@y" ) || !root.isMember( "@width" ) || !root.isMember( "@height" ) )
            return;

        Json::Value name = root[ "@name" ];
        if( name.isString() )
        {
            Json::Value x = root[ "@x" ];
            Json::Value y = root[ "@y" ];
            Json::Value width = root[ "@width" ];
            Json::Value height = root[ "@height" ];
            sf::IntRect rect;
            if( x.isString() )
            {
                std::stringstream ss( x.asString() );
                ss >> rect.left;
            }
            if( y.isString() )
            {
                std::stringstream ss( y.asString() );
                ss >> rect.top;
            }
            if( width.isString() )
            {
                std::stringstream ss( width.asString() );
                ss >> rect.width;
            }
            if( height.isString() )
            {
                std::stringstream ss( height.asString() );
                ss >> rect.height;
            }
            if( rect.width > 0 && rect.height > 0 )
                m_rects[ name.asString() ] = rect;
        }
    }

}
