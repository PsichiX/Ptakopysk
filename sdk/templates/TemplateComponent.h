#ifndef __TEMPLATE_COMPONENT__
#define __TEMPLATE_COMPONENT__

#include <Ptakopysk/Components/Component.h>

using namespace Ptakopysk;

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

	FORCEINLINE float getValue() { return m_value; };
	FORCEINLINE void setValue( float v ) { m_value = v; };
	
	XeCore::Common::Property< float, TemplateComponent > Value;

protected:
	virtual Json::Value onSerialize( const std::string& property );
	virtual void onDeserialize( const std::string& property, const Json::Value& root );

	virtual void onDuplicate( Component* dst );
	virtual void onUpdate( float dt );
	
private:
	float m_value;
};

#endif
