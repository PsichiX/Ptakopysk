#ifndef __PTAKOPYSK__SPRITE_ATLAS__
#define __PTAKOPYSK__SPRITE_ATLAS__

#include "Component.h"
#include "../CustomAssets/SpriteAtlasAsset.h"

namespace Ptakopysk
{

    META_COMPONENT(
        META_ATTR_DESCRIPTION( "Sprite atlas component that apply subtexture into SpriteRenderer." ),
        META_ATTR_FUNCTIONALITY_TRIGGERS( "Apply to SpriteRenderer" )
    )
    class SpriteAtlas
    : public virtual XeCore::Common::IRtti
    , public virtual XeCore::Common::MemoryManager::Manageable
    , public Component
    {
        RTTI_CLASS_DECLARE( SpriteAtlas );

    public:
        SpriteAtlas();
        virtual ~SpriteAtlas();

        FORCEINLINE static Component* onBuildComponent() { return xnew SpriteAtlas(); }

        FORCEINLINE SpriteAtlasAsset* getSpriteAtlasInstance() { return m_atlas; };
        FORCEINLINE void setSpriteAtlasInstance( SpriteAtlasAsset* v ) { m_atlas = v; applySubTexture(); };
        FORCEINLINE std::string getSubTextureName() { return m_subTexture; };
        FORCEINLINE void setSubTextureName( std::string v ) { m_subTexture = v; applySubTexture(); };

        bool applySubTexture();

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Sprite atlas asset." ),
            META_ATTR_VALUE_TYPE( "ICustomAsset" )
        )
        XeCore::Common::Property< SpriteAtlasAsset*, SpriteAtlas > SpriteAtlasInstance;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Subtexture name." )
        )
        XeCore::Common::Property< std::string, SpriteAtlas > SubTextureName;

    protected:
        virtual Json::Value onSerialize( const std::string& property );
        virtual void onDeserialize( const std::string& property, const Json::Value& root );

        virtual void onCreate();
        virtual void onDuplicate( Component* dst );
        virtual bool onTriggerFunctionality( const std::string& name );

    private:
        SpriteAtlasAsset* m_atlas;
        std::string m_subTexture;
    };

}

#endif
