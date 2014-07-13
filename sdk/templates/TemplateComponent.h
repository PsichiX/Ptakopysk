#ifndef __TEMPLATE_COMPONENT__
#define __TEMPLATE_COMPONENT__

#include <Ptakopysk/Components/Component.h>

using namespace Ptakopysk;

META_COMPONENT(
    META_ATTR_DESCRIPTION( "TemplateComponent" )
)
class TemplateComponent
: public virtual XeCore::Common::IRtti
, public virtual XeCore::Common::MemoryManager::Manageable
, public Component
{
	RTTI_CLASS_DECLARE( TemplateComponent );

public:
	TemplateComponent();
	virtual ~TemplateComponent();

	FORCEINLINE static Component* onBuildComponent() { return xnew TemplateComponent(); }

	//<TEMPLATE
	FORCEINLINE float getValue() { return m_value; };
	FORCEINLINE void setValue( float v ) { m_value = v; };

	META_PROPERTY(
        META_ATTR_DESCRIPTION( "Property value" ),
        META_ATTR_DEFAULT_VALUE( "0" )
    )
	XeCore::Common::Property< float, TemplateComponent > Value;
	//TEMPLATE>
protected:
	virtual Json::Value onSerialize( const std::string& property );
	virtual void onDeserialize( const std::string& property, const Json::Value& root );

	virtual void onDuplicate( Component* dst );
	virtual void onUpdate( float dt );

private:
	//<TEMPLATE
	float m_value;
	//TEMPLATE>
};

#endif
