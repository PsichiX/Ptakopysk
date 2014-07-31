#include "PtakopyskInterface.h"

RTTI_CLASS_DERIVATIONS( PtakopyskInterface,
                        RTTI_DERIVATIONS_END
                        )

PtakopyskInterface::PtakopyskInterface()
: RTTI_CLASS_DEFINE( PtakopyskInterface )
, m_renderWindow( 0 )
, m_gameManager( 0 )
{
}

PtakopyskInterface::~PtakopyskInterface()
{
    release();
}

void PtakopyskInterface::initialize( sf::WindowHandle windowHandle )
{
    release();

    GameManager::initialize();
    m_renderWindow = xnew sf::RenderWindow( windowHandle );
    m_gameManager = xnew GameManager();
    m_gameManager->RenderWindow = m_renderWindow;
}

void PtakopyskInterface::release()
{
    DELETE_OBJECT( m_gameManager );
    if( m_renderWindow )
        m_renderWindow->close();
    DELETE_OBJECT( m_renderWindow );
    Assets::destroy();
    GameManager::cleanup();
}

void PtakopyskInterface::processEvents()
{
    if( !m_renderWindow || !m_renderWindow->isOpen() || !m_gameManager )
        return;

    sf::Event event;
    while( m_renderWindow->pollEvent( event ) )
        m_gameManager->processEvents( event );
}

void PtakopyskInterface::processPhysics( float deltaTime, int velocityIterations, int positionIterations )
{
    if( !m_renderWindow || !m_renderWindow->isOpen() || !m_gameManager )
        return;

    m_gameManager->processPhysics( deltaTime, velocityIterations, positionIterations );
}

void PtakopyskInterface::processUpdate( float deltaTime, bool sortInstances )
{
    if( !m_renderWindow || !m_renderWindow->isOpen() || !m_gameManager )
        return;

    m_gameManager->processUpdate( deltaTime, sortInstances );
}

void PtakopyskInterface::processRender()
{
    if( !m_renderWindow || !m_renderWindow->isOpen() || !m_gameManager )
        return;

    m_renderWindow->clear( sf::Color( 64, 64, 64, 255 ) );
    m_gameManager->processRender( m_renderWindow );
    m_renderWindow->display();
}

void PtakopyskInterface::setVerticalSyncEnabled( bool enabled )
{
    if( !m_renderWindow || !m_renderWindow->isOpen() )
        return;

    m_renderWindow->setVerticalSyncEnabled( enabled );
}
