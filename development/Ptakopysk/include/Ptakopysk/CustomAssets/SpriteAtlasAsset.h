#ifndef __PTAKOPYSK__SPRITE_ATLAS_ASSET__
#define __PTAKOPYSK__SPRITE_ATLAS_ASSET__

#include "../System/Assets.h"
#include "../Components/SpriteRenderer.h"
#include <SFML/Graphics/Rect.hpp>
#include <map>

namespace Ptakopysk
{

    META_ASSET(
        META_ATTR_DESCRIPTION( "Sprite atlas descriptor." )
    )
    class SpriteAtlasAsset
    : public virtual XeCore::Common::IRtti
    , public virtual XeCore::Common::MemoryManager::Manageable
    , public ICustomAsset
    {
        RTTI_CLASS_DECLARE( SpriteAtlasAsset );

    public:
        FORCEINLINE static ICustomAsset* onBuildCustomAsset() { return xnew SpriteAtlasAsset(); };

        SpriteAtlasAsset();

        FORCEINLINE bool hasSubTexture( const std::string& name ) { return m_rects.count( name ); };
        bool getSubTexture( const std::string& name, sf::IntRect& outRect );
        bool applySubTexture( const std::string& name, SpriteRenderer* sprite );
        bool addSubTexture( const std::string& name, const sf::IntRect& rect );
        bool removeSubTexture( const std::string& name );

    protected:
        virtual bool onLoad( const std::string& path );

    private:
        void readSubTexture( const Json::Value& root );

        std::map< std::string, sf::IntRect > m_rects;
    };

}

#endif
