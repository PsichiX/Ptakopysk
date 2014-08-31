#ifndef __TEMPLATE_ASSET__
#define __TEMPLATE_ASSET__

#include <Ptakopysk/System/Assets.h>
#include <Ptakopysk/System/Meta.h>

using namespace Ptakopysk;

META_ASSET(
    META_ATTR_DESCRIPTION( "TemplateAsset" )
)
class TemplateAsset
: public virtual XeCore::Common::IRtti
, public virtual XeCore::Common::MemoryManager::Manageable
, public ICustomAsset
{
    RTTI_CLASS_DECLARE( TemplateAsset );

public:
    TemplateAsset();

    FORCEINLINE static ICustomAsset* onBuildCustomAsset() { return xnew TemplateAsset(); };

	FORCEINLINE std::string getContent() { return m_content; };
	
protected:
    virtual bool onLoad( const std::string& path );

private:
	std::string m_content;
};

#endif
