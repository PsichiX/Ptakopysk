#ifndef __PTAKOPYSK_INTERFACE__
#define __PTAKOPYSK_INTERFACE__

#include <Ptakopysk/System/Assets.h>
#include <Ptakopysk/System/GameManager.h>
#include <XeCore/Common/Singleton.h>
#include <XeCore/Common/Logger.h>
#include <XeCore/Common/Concurrent/Thread.h>
#include <XeCore/Common/Timer.h>

using namespace Ptakopysk;

class PtakopyskInterface
: public virtual XeCore::Common::IRtti
, public virtual XeCore::Common::MemoryManager::Manageable
, public XeCore::Common::Singleton< PtakopyskInterface >
{
    RTTI_CLASS_DECLARE( PtakopyskInterface );

public:
    PtakopyskInterface();
    virtual ~PtakopyskInterface();

    void initialize( sf::WindowHandle windowHandle );
    void release();
    void processEvents();
    void processPhysics( float deltaTime, int velocityIterations, int positionIterations );
    void processUpdate( float deltaTime, bool sortInstances );
    void processRender();
    void setVerticalSyncEnabled( bool enabled );

private:
    sf::RenderWindow*           m_renderWindow;
    GameManager*                m_gameManager;
};

#endif
