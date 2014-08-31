#include "TemplateAsset.h"

RTTI_CLASS_DERIVATIONS( TemplateAsset,
						RTTI_DERIVATION( ICustomAsset ),
						RTTI_DERIVATIONS_END
						)

TemplateAsset::TemplateAsset()
: RTTI_CLASS_DEFINE( TemplateAsset )
{
}

bool TemplateAsset::onLoad( const std::string& path )
{
	m_content = Assets::use().loadText( path );
    return true;
}
