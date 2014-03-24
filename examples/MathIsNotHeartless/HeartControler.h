#ifndef __TEMPLATE_COMPONENT__
#define __TEMPLATE_COMPONENT__

#include <Ptakopysk/Components/Component.h>

using namespace Ptakopysk;

class HeartControler
: public virtual XeCore::Common::IRtti
, public virtual XeCore::Common::MemoryManager::Manageable
, public Component
{
	RTTI_CLASS_DECLARE( HeartControler );

public:
	HeartControler();
	virtual ~HeartControler();

	FORCEINLINE static Component* onBuildComponent() { return xnew HeartControler(); }

	FORCEINLINE float getSize() { return m_size; };
	void setSize( float v );
	FORCEINLINE float getPingSize() { return m_pingSize; };
	FORCEINLINE void setPingSize( float v ) { m_pingSize = v; };
	FORCEINLINE float getPongSize() { return m_pongSize; };
	FORCEINLINE void setPongSize( float v ) { m_pongSize = v; };
	FORCEINLINE float getDuration() { return m_duration; };
	FORCEINLINE void setDuration( float v ) { m_duration = v; };
    FORCEINLINE float getPingDuration() { return m_pingDuration; };
	FORCEINLINE void setPingDuration( float v ) { m_pingDuration = v; };
    FORCEINLINE float getPongDuration() { return m_pongDuration; };
	FORCEINLINE void setPongDuration( float v ) { m_pongDuration = v; };
    FORCEINLINE float getTimer() { return m_timer; };
	FORCEINLINE void setTimer( float v ) { m_timer = v; };

	XeCore::Common::Property< float, HeartControler > Size;
	XeCore::Common::Property< float, HeartControler > PingSize;
	XeCore::Common::Property< float, HeartControler > PongSize;
	XeCore::Common::Property< float, HeartControler > Duration;
	XeCore::Common::Property< float, HeartControler > PingDuration;
	XeCore::Common::Property< float, HeartControler > PongDuration;
    XeCore::Common::Property< float, HeartControler > Timer;

protected:
    virtual void onCreate();
	virtual Json::Value onSerialize( const std::string& property );
	virtual void onDeserialize( const std::string& property, const Json::Value& root );

	virtual void onDuplicate( Component* dst );
	virtual void onUpdate( float dt );

private:
	float m_size;
	float m_pingSize;
	float m_pongSize;
	float m_duration;
	float m_pingDuration;
	float m_pongDuration;
    float m_timer;
};

#endif
