#include "../../include/Ptakopysk/System/RenderMaterial.h"
#include "../../include/Ptakopysk/System/Assets.h"

namespace Ptakopysk
{

    RTTI_CLASS_DERIVATIONS( RenderMaterial,
                            RTTI_DERIVATIONS_END
                            )

    RenderMaterial::RenderMaterial()
    : RTTI_CLASS_DEFINE( RenderMaterial )
    {
    }

    RenderMaterial::~RenderMaterial()
    {
        clear();
    }

    float RenderMaterial::getFloat( const std::string& name )
    {
        return count( name ) > 0 ? access( name )[ 0 ] : 0.0f;
    }

    sf::Vector2f RenderMaterial::getVec2( const std::string& name )
    {
        return count( name ) > 1 ? *(sf::Vector2f*)access( name ).data() : sf::Vector2f();
    }

    sf::Vector3f RenderMaterial::getVec3( const std::string& name )
    {
        return count( name ) > 2 ? *(sf::Vector3f*)access( name ).data() : sf::Vector3f();
    }

    sf::Color RenderMaterial::getColor( const std::string& name )
    {
        if( count( name ) < 4 )
            return sf::Color::Transparent;
        std::vector< float >& d = access( name );
        return sf::Color(
            byte( CLAMP( d[ 0 ], 0.0f, 1.0f ) * 255.0f ),
            byte( CLAMP( d[ 1 ], 0.0f, 1.0f ) * 255.0f ),
            byte( CLAMP( d[ 2 ], 0.0f, 1.0f ) * 255.0f ),
            byte( CLAMP( d[ 3 ], 0.0f, 1.0f ) * 255.0f )
        );
    }

    sf::Transform RenderMaterial::getTransform( const std::string& name )
    {
        if( count( name ) < 16 )
            return sf::Transform::Identity;
        sf::Transform t;
        float* m = (float*)t.getMatrix();
        std::vector< float >& d = access( name );
        for( int i = 0; i < 16; i++ )
            m[ i ] = d[ i ];
        return t;
    }

    float* RenderMaterial::getRaw( const std::string& name )
    {
        return has( name ) ? access( name ).data() : 0;
    }

    sf::Texture* RenderMaterial::getTexture( const std::string& name )
    {
        return m_textures.count( name ) ? m_textures[ name ] : 0;
    }

    void RenderMaterial::set( const std::string& name, float x )
    {
        removeTexture( name );
        std::vector< float >& d = access( name );
        d.resize( 1 );
        d.clear();
        d.push_back( x );
    }

    void RenderMaterial::set( const std::string& name, float x, float y )
    {
        removeTexture( name );
        std::vector< float >& d = access( name );
        d.resize( 2 );
        d.clear();
        d.push_back( x );
        d.push_back( y );
    }

    void RenderMaterial::set( const std::string& name, float x, float y, float z )
    {
        removeTexture( name );
        std::vector< float >& d = access( name );
        d.resize( 3 );
        d.clear();
        d.push_back( x );
        d.push_back( y );
        d.push_back( z );
    }

    void RenderMaterial::set( const std::string& name, float x, float y, float z, float w )
    {
        removeTexture( name );
        std::vector< float >& d = access( name );
        d.resize( 4 );
        d.clear();
        d.push_back( x );
        d.push_back( y );
        d.push_back( z );
        d.push_back( w );
    }

    void RenderMaterial::set( const std::string& name, const sf::Vector2f& v )
    {
        removeTexture( name );
        std::vector< float >& d = access( name );
        d.resize( 2 );
        d.clear();
        d.push_back( v.x );
        d.push_back( v.y );
    }

    void RenderMaterial::set( const std::string& name, const sf::Vector3f& v )
    {
        removeTexture( name );
        std::vector< float >& d = access( name );
        d.resize( 3 );
        d.clear();
        d.push_back( v.x );
        d.push_back( v.y );
        d.push_back( v.z );
    }

    void RenderMaterial::set( const std::string& name, const sf::Color& v )
    {
        const float c = 1.0f / 255.0f;
        removeTexture( name );
        std::vector< float >& d = access( name );
        d.resize( 4 );
        d.clear();
        d.push_back( (float)v.r * c );
        d.push_back( (float)v.g * c );
        d.push_back( (float)v.b * c );
        d.push_back( (float)v.a * c );
    }

    void RenderMaterial::set( const std::string& name, const sf::Transform& v )
    {
        removeTexture( name );
        std::vector< float >& d = access( name );
        d.resize( 16 );
        d.clear();
        const float* m = v.getMatrix();
        for( int i = 0; i < 16; i++ )
            d.push_back( m[ i ] );
    }

    void RenderMaterial::set( const std::string& name, const sf::Texture* v )
    {
        remove( name );
        if( v )
            m_textures[ name ] = (sf::Texture*)v;
    }

    void RenderMaterial::apply( sf::Shader* shader )
    {
        if( !shader )
            return;
        std::string n;
        for( Properties::iterator it = m_properties.begin(); it != m_properties.end(); it++ )
        {
            n.assign( it->first );
            std::vector< float >& d = it->second;
            int s = d.size();
            if( s == 1 )
                shader->setParameter( n, d[ 0 ] );
            else if( s == 2 )
                shader->setParameter( n, d[ 0 ], d[ 1 ] );
            else if( s == 3 )
                shader->setParameter( n, d[ 0 ], d[ 1 ], d[ 2 ] );
            else if( s == 4 )
                shader->setParameter( n, d[ 0 ], d[ 1 ], d[ 2 ], d[ 3 ] );
            else if( s == 16 )
            {
                sf::Transform t;
                float* m = (float*)t.getMatrix();
                for( int i = 0; i < 16; i++ )
                    m[ i ] = d[ i ];
                shader->setParameter( n, t );
            }
        }
        for( Textures::iterator it = m_textures.begin(); it != m_textures.end(); it++ )
            shader->setParameter( it->first, *it->second );
    }

    void RenderMaterial::copyFrom( RenderMaterial& mat )
    {
        m_properties.clear();
        m_properties = mat.m_properties;
        m_textures.clear();
        m_textures = mat.m_textures;
    }

    Json::Value RenderMaterial::serialize()
    {
        Json::Value pv;
        for( Properties::iterator it = m_properties.begin(); it != m_properties.end(); it++ )
        {
            Json::Value a( Json::arrayValue );
            for( unsigned int i = 0; i < it->second.size(); i++ )
                a.append( Json::Value( it->second[ i ] ) );
            pv[ it->first ] = a;
        }
        Json::Value tv;
        for( Textures::iterator it = m_textures.begin(); it != m_textures.end(); it++ )
            tv[ it->first ] = Json::Value( Assets::use().findTexture( it->second ) );
        Json::Value v;
        if( !pv.isNull() )
            v[ "properties" ] = pv;
        if( !tv.isNull() )
            v[ "textures" ] = tv;
        return v;
    }

    void RenderMaterial::deserialize( const Json::Value& root )
    {
        if( !root.isObject() )
            return;
        Json::Value properties = root[ "properties" ];
        if( properties.isObject() )
        {
            Json::Value::Members m = properties.getMemberNames();
            Json::Value mv;
            Json::Value item;
            for( Json::Value::Members::iterator it = m.begin(); it != m.end(); it++ )
            {
                mv = properties[ *it ];
                if( mv.isArray() )
                {
                    std::vector< float >& d = access( *it );
                    d.clear();
                    for( unsigned int i = 0; i < mv.size(); i++ )
                    {
                        item = mv[ i ];
                        if( item.isNumeric() )
                            d.push_back( (float)item.asDouble() );
                    }
                }
            }
        }
        Json::Value textures = root[ "textures" ];
        if( textures.isObject() )
        {
            Json::Value::Members m = properties.getMemberNames();
            Json::Value mv;
            Json::Value item;
            for( Json::Value::Members::iterator it = m.begin(); it != m.end(); it++ )
            {
                mv = properties[ *it ];
                if( mv.isString() )
                {
                    sf::Texture* t = Assets::use().getTexture( mv.asString() );
                    if( t )
                        m_textures[ *it ] = t;
                }
            }
        }
    }

}
