#include "TemplateComponent.h"
#include <Ptakopysk/System/GameObject.h>
#include <Ptakopysk/Components/TextRenderer.h>
#include <Ptakopysk/Components/SpriteRenderer.h>
#include <Ptakopysk/Components/Transform.h>
#include <sstream>

RTTI_CLASS_DERIVATIONS( TemplateComponent,
						RTTI_DERIVATION( Component ),
						RTTI_DERIVATIONS_END
						)

TemplateComponent::TemplateComponent()
: RTTI_CLASS_DEFINE( TemplateComponent )
, Component( Component::tUpdate )
//<TEMPLATE
, Value( this, &TemplateComponent::getValue, &TemplateComponent::setValue )
, m_value( 0.0f )
//TEMPLATE>
{
	//<TEMPLATE
	serializableProperty( "Value" );
	//TEMPLATE>
}

TemplateComponent::~TemplateComponent()
{
}

Json::Value TemplateComponent::onSerialize( const std::string& property )
{
	//<TEMPLATE
	if( property == "Value" )
		return Json::Value( m_value );
	else
	//TEMPLATE>
		return Component::onSerialize( property );
}

void TemplateComponent::onDeserialize( const std::string& property, const Json::Value& root )
{
	//<TEMPLATE
	if( property == "Value" && root.isNumeric() )
		m_value = (float)root.asDouble();
	else
	//TEMPLATE>
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
	//<TEMPLATE
	c->setValue( getValue() );
	//TEMPLATE>
}

void TemplateComponent::onUpdate( float dt )
{
	//<TEMPLATE
	m_value += dt;
	TextRenderer* text = getGameObject()->getComponent< TextRenderer >();
	SpriteRenderer* spr = getGameObject()->getComponent< SpriteRenderer >();
	Transform* trans = getGameObject()->getComponent< Transform >();
    if( text )
	{
	    std::stringstream ss;
	    ss << "Time: " << m_value;
	    text->Text = ss.str();
	}
	else if( spr && trans )
        trans->Rotation = m_value * 90.0f;
	//TEMPLATE>
}
