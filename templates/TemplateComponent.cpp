#include "TemplateComponent.h"

RTTI_CLASS_DERIVATIONS( TemplateComponent,
						RTTI_DERIVATION( Component ),
						RTTI_DERIVATIONS_END
						)

TemplateComponent::TemplateComponent()
: RTTI_CLASS_DEFINE( TemplateComponent )
, Component( Component::tUpdate )
, Value( this, &TemplateComponent::getValue, &TemplateComponent::setValue )
, m_value( 0.0f )
{
	serializableProperty( "Value" );
}

TemplateComponent::~TemplateComponent()
{
}

Json::Value TemplateComponent::onSerialize( const std::string& property )
{
	if( property == "Value" )
		return Json::Value( m_value );
	else
		return Component::onSerialize( property );
}

void TemplateComponent::onDeserialize( const std::string& property, const Json::Value& root )
{
	if( property == "Value" && root.isNumeric() )
		m_value = (float)root.asDouble()
	else
		Component::onDeserialize( property, root );
}

void TemplateComponent::onDuplicate( Component* dst )
{
	if( !dst )
		return;
	Component::onDuplicate( dst );
	if( !XeCore::Common::IRtti::isDerived< TemplateComponent >( dst ) )
		return;
	TemplateComponent* c = (TemplateComponent*)dst;
	c->setValue( getValue() );
}

void TemplateComponent::onUpdate( float dt )
{
	m_value += dt;
}
