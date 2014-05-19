#ifndef __PTAKOPYSK__RENDER_MATERIAL__
#define __PTAKOPYSK__RENDER_MATERIAL__

#include <XeCore/Common/Base.h>
#include <XeCore/Common/IRtti.h>
#include <XeCore/Common/MemoryManager.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <json/json.h>
#include <string>
#include <map>
#include <vector>

namespace Ptakopysk
{

    class RenderMaterial
    : public virtual XeCore::Common::IRtti
    , public virtual XeCore::Common::MemoryManager::Manageable
    {
        RTTI_CLASS_DECLARE( RenderMaterial );

    public:
        typedef std::map< std::string, std::vector< float > > Properties;
        typedef std::map< std::string, sf::Texture* > Textures;

        RenderMaterial();
        ~RenderMaterial();

        float getFloat( const std::string& name );
        sf::Vector2f getVec2( const std::string& name );
        sf::Vector3f getVec3( const std::string& name );
        sf::Color getColor( const std::string& name );
        sf::Transform getTransform( const std::string& name );
        float* getRaw( const std::string& name );
        sf::Texture* getTexture( const std::string& name );

        void set( const std::string& name, float x );
        void set( const std::string& name, float x, float y );
        void set( const std::string& name, float x, float y, float z );
        void set( const std::string& name, float x, float y, float z, float w );
        void set( const std::string& name, const sf::Vector2f& v );
        void set( const std::string& name, const sf::Vector3f& v );
        void set( const std::string& name, const sf::Color& v );
        void set( const std::string& name, const sf::Transform& v );
        void set( const std::string& name, const sf::Texture* v );

        FORCEINLINE bool has( const std::string& name ) { return m_properties.count( name ); };
        FORCEINLINE bool hasTexture( const std::string& name ) { return m_textures.count( name ); };
        FORCEINLINE std::vector< float >& access( const std::string& name ) { return m_properties[ name ]; };
        FORCEINLINE int count( const std::string& name ) { return has( name ) ? access( name ).size() : -1; };
        FORCEINLINE void remove( const std::string& name ) { if( has( name ) ) m_properties.erase( name ); };
        FORCEINLINE void removeTexture( const std::string& name ) { if( hasTexture( name ) ) m_textures.erase( name ); };
        FORCEINLINE void clear() { m_properties.clear(); m_textures.clear(); };
        void apply( sf::Shader* shader, bool propsValidation = false );
        void copyFrom( RenderMaterial& mat );
        FORCEINLINE Properties& rawProperties() { return m_properties; };
        FORCEINLINE Textures& rawTextures() { return m_textures; };
        Json::Value serialize();
        void deserialize( const Json::Value& root );

    private:
        Properties m_properties;
        Textures m_textures;
    };
}

#endif

