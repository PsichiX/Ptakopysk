#include "../../include/Ptakopysk/Components/SpriteAtlas.h"
#include "../../include/Ptakopysk/System/GameObject.h"
#include "../../include/Ptakopysk/Components/SpriteRenderer.h"

namespace Ptakopysk
{

    RTTI_CLASS_DERIVATIONS( SpriteAtlas,
                            RTTI_DERIVATION( Component ),
                            RTTI_DERIVATIONS_END
                            )

    SpriteAtlas::SpriteAtlas()
    : RTTI_CLASS_DEFINE( SpriteAtlas )
    , Component( Component::tNone )
    , SpriteAtlasInstance( this, &SpriteAtlas::getSpriteAtlasInstance, &SpriteAtlas::setSpriteAtlasInstance )
    , SubTextureName( this, &SpriteAtlas::getSubTextureName, &SpriteAtlas::setSubTextureName )
    , m_atlas( 0 )
    {
        serializableProperty( "SpriteAtlasInstance" );
        serializableProperty( "SubTextureName" );
    }

    SpriteAtlas::~SpriteAtlas()
    {
    }

    bool SpriteAtlas::applySubTexture()
    {
        if( !getGameObject() )
            return false;

        SpriteRenderer* spr = getGameObject()->getComponent< SpriteRenderer >();
        if( !spr )
            return false;

        if( m_atlas && m_atlas->hasSubTexture( m_subTexture ) )
        {
            sf::Vector2f op = spr->getOriginPercent();
            if( m_atlas->applySubTexture( m_subTexture, spr ) )
            {
                spr->setOriginPercent( op );
                return true;
            }
        }
        else
        {
            sf::Vector2f op = spr->getOriginPercent();
            spr->getRenderer()->setTexture( spr->getRenderer()->getTexture(), true );
            spr->setSize( sf::Vector2f( -1.0f, -1.0f ) );
            spr->setOriginPercent( op );
        }
        return false;
    }

    Json::Value SpriteAtlas::onSerialize( const std::string& property )
    {
        if( property == "SpriteAtlasInstance" )
            return Json::Value( Assets::use().findCustomAsset( m_atlas ) );
        else if( property == "SubTextureName" )
            return Json::Value( m_subTexture );
        else
            return Component::onSerialize( property );
    }

    void SpriteAtlas::onDeserialize( const std::string& property, const Json::Value& root )
    {
        if( property == "SpriteAtlasInstance" && root.isString() )
        {
            ICustomAsset* a = Assets::use().getCustomAsset( root.asString() );
            setSpriteAtlasInstance( a && IRtti::isType< SpriteAtlasAsset >( a ) ? (SpriteAtlasAsset*)a : 0 );
        }
        else if( property == "SubTextureName" && root.isString() )
            setSubTextureName( root.asString() );
        else
            Component::onDeserialize( property, root );
    }

    void SpriteAtlas::onCreate()
    {
        applySubTexture();
    }

    void SpriteAtlas::onDuplicate( Component* dst )
    {
        if( !dst )
            return;
        Component::onDuplicate( dst );
        if( !XeCore::Common::IRtti::isDerived< SpriteAtlas >( dst ) )
            return;
        SpriteAtlas* c = (SpriteAtlas*)dst;
        c->setSpriteAtlasInstance( getSpriteAtlasInstance() );
        c->setSubTextureName( getSubTextureName() );
    }

    bool SpriteAtlas::onTriggerFunctionality( const std::string& name )
    {
        if( name == "Apply to SpriteRenderer" )
            return applySubTexture();
        else
            return false;
    }

}
