#include "TemplateAsset.h"

RTTI_CLASS_DERIVATIONS( TemplateAsset,
						RTTI_DERIVATION( ICustomAsset ),
						RTTI_DERIVATIONS_END
						)

TemplateAsset::TemplateAsset( Assets* owner )
: RTTI_CLASS_DEFINE( TemplateAsset )
, ICustomAsset( owner )
{
}

bool TemplateAsset::onLoad( const std::string& path )
{
	if( !getOwner() )
		return false;
    
	m_content = getOwner()->loadText( path );
    return true;
}
