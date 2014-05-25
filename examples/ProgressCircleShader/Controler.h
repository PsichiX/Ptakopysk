#ifndef __CONTROLER__
#define __CONTROLER__

#include <Ptakopysk/Components/Component.h>

using namespace Ptakopysk;

class Controler
: public virtual XeCore::Common::IRtti
, public virtual XeCore::Common::MemoryManager::Manageable
, public Component
{
	RTTI_CLASS_DECLARE( Controler );

public:
	Controler();
	virtual ~Controler();

	FORCEINLINE static Component* onBuildComponent() { return xnew Controler(); }

protected:
	virtual Json::Value onSerialize( const std::string& property );
	virtual void onDeserialize( const std::string& property, const Json::Value& root );

    virtual void onCreate();
    virtual void onDuplicate( Component* dst );
	virtual void onEvent( const sf::Event& event );
	virtual void onUpdate( float dt );

private:
    void applyShader();
    void applyStatus();

    GameObject* m_status;
    float m_phase;
    unsigned int m_current;
    std::vector< sf::Shader* > m_shaders;
};

#endif
