#include "../../include/Ptakopysk/System/Network.h"
#include <sstream>

namespace Ptakopysk
{

    RTTI_CLASS_DERIVATIONS( Network,
                            RTTI_DERIVATION( XeCore::Common::Concurrent::Thread ),
                            RTTI_DERIVATIONS_END
                            )

    Network::Network()
    : RTTI_CLASS_DEFINE( Network )
    , m_lastAcceptedId( 0 )
    , m_working( false )
    , m_receivingInterval( 0 )
    , m_clientsToRemoveAll( std::make_pair< bool, bool >( false, false ) )
    {
    }

    Network::~Network()
    {
        stop();
        removeAllClients();
        removeAllServers();
    }

    void Network::start( unsigned int receivingIntervalMs )
    {
        m_receivingInterval = receivingIntervalMs;
        XeCore::Common::Concurrent::Thread::start();
    }

    Network::Server* Network::createServer( const std::string& id, unsigned short port, unsigned int listeningIntervalMs )
    {
        Server* server = xnew Server( port, listeningIntervalMs );
        if( addServer( id, server ) )
            return server;
        else
        {
            DELETE_OBJECT( server );
            return 0;
        }
    }

    bool Network::addServer( const std::string& id, Network::Server* server )
    {
        if( m_servers.count( id ) )
            return false;
        m_servers[ id ] = server;
        return true;
    }

    Network::Server* Network::getServer( const std::string& id )
    {
        return m_servers.count( id ) ? m_servers[ id ] : 0;
    }

    std::string Network::getServerId( Network::Server* server )
    {
        for( std::map< std::string, Server* >::iterator it = m_servers.begin(); it != m_servers.end(); it++ )
            if( it->second == server )
                return it->first;
        return "";
    }

    bool Network::removeServer( const std::string& id, bool destroyObject )
    {
        if( m_servers.count( id ) )
        {
            if( destroyObject )
            {
                Server* server = m_servers[ id ];
                DELETE_OBJECT( server );
            }
            m_servers.erase( id );
            return true;
        }
        return false;
    }

    bool Network::removeServer( Network::Server* server, bool destroyObject )
    {
        for( std::map< std::string, Server* >::iterator it = m_servers.begin(); it != m_servers.end(); it++ )
        {
            if( it->second == server )
            {
                if( destroyObject )
                    DELETE_OBJECT( server );
                m_servers.erase( it );
                return true;
            }
        }
        return false;
    }

    void Network::removeAllServers( bool destroyObjects )
    {
        if( destroyObjects )
        {
            Server* server;
            for( std::map< std::string, Server* >::iterator it = m_servers.begin(); it != m_servers.end(); it++ )
            {
                server = it->second;
                DELETE_OBJECT( server );
            }
        }
        m_servers.clear();
    }

    Network::Client* Network::createClient( const std::string& id, const sf::IpAddress& address, unsigned short port )
    {
        Client* client = xnew Client( address, port );
        if( addClient( id, client ) )
            return client;
        else
        {
            DELETE_OBJECT( client );
            return 0;
        }
    }

    bool Network::addClient( const std::string& id, Client* client )
    {
        if( m_sync.tryLock() )
        {
            if( m_clients.count( id ) )
            {
                m_sync.unlock();
                return false;
            }
            m_clients[ id ] = client;
            m_sync.unlock();
            return true;
        }
        return false;
    }

    Network::Client* Network::getClient( const std::string& id )
    {
        if( m_sync.tryLock() )
        {
            Client* c = m_clients.count( id ) ? m_clients[ id ] : 0;
            m_sync.unlock();
            return c;
        }
        return 0;
    }

    std::string Network::getClientId( Network::Client* client )
    {
        if( m_sync.tryLock() )
        {
            for( std::map< std::string, Client* >::iterator it = m_clients.begin(); it != m_clients.end(); it++ )
            {
                if( it->second == client )
                {
                    std::string id = it->first;
                    m_sync.unlock();
                    return id;
                }
            }
            m_sync.unlock();
            return "";
        }
        return "";
    }

    bool Network::removeClient( const std::string& id, bool destroyObject )
    {
        if( m_sync.tryLock() )
        {
            if( m_clients.count( id ) )
            {
                if( destroyObject )
                {
                    Client* client = m_clients[ id ];
                    DELETE_OBJECT( client );
                }
                m_clients.erase( id );
                m_sync.unlock();
                return true;
            }
            m_sync.unlock();
            return false;
        }
        return false;
    }

    bool Network::removeClient( Client* client, bool destroyObject )
    {
        if( m_sync.tryLock() )
        {
            for( std::map< std::string, Client* >::iterator it = m_clients.begin(); it != m_clients.end(); it++ )
            {
                if( it->second == client )
                {
                    if( destroyObject )
                        DELETE_OBJECT( client );
                    m_clients.erase( it );
                    m_sync.unlock();
                    return true;
                }
            }
            m_sync.unlock();
            return false;
        }
        return false;
    }

    void Network::removeAllClients( bool destroyObjects )
    {
        if( m_sync.tryLock() )
        {
            if( destroyObjects )
            {
                Client* client;
                for( std::map< std::string, Client* >::iterator it = m_clients.begin(); it != m_clients.end(); it++ )
                {
                    client = it->second;
                    DELETE_OBJECT( client );
                }
            }
            m_clients.clear();
            m_sync.unlock();
        }
    }

    bool Network::removeClientDelayed( const std::string& id, bool destroyObject )
    {
        SYNCHRONIZED_OBJECT( m_syncToRemove );
        m_clientsToRemoveId[ id ] = destroyObject;
        return true;
    }

    bool Network::removeClientDelayed( Client* client, bool destroyObject )
    {
        SYNCHRONIZED_OBJECT( m_syncToRemove );
        m_clientsToRemoveObject[ client ] = destroyObject;
        return true;
    }

    void Network::removeAllClientsDelayed( bool destroyObjects )
    {
        SYNCHRONIZED_OBJECT( m_syncToRemove );
        m_clientsToRemoveAll = std::make_pair< bool, bool >( true, destroyObjects );
    }

    void Network::fetchPendingClients( ICallback* onFetchClientCallback )
    {
        Server* server;
        for( std::map< std::string, Server* >::iterator it = m_servers.begin(); it != m_servers.end(); it++ )
        {
            server = it->second;
            server->fetchPendingClients( this, onFetchClientCallback );
        }
    }

    void Network::process()
    {
        if( m_sync.tryLock() )
        {
            for( std::map< std::string, Client* >::iterator it = m_clients.begin(); it != m_clients.end(); it++ )
                it->second->receivePacket();
            m_sync.unlock();
        }
        if( m_syncToRemove.tryLock() )
        {
            if( m_clientsToRemoveAll.first )
            {
                removeAllClients( m_clientsToRemoveAll.second );
                m_clientsToRemoveAll = std::make_pair< bool, bool >( false, false );
                m_clientsToRemoveId.clear();
                m_clientsToRemoveObject.clear();
            }
            else
            {
                for( std::map< std::string, bool >::iterator it = m_clientsToRemoveId.begin(); it != m_clientsToRemoveId.end(); it++ )
                    removeClient( it->first, it->second );
                m_clientsToRemoveId.clear();
                for( std::map< Client*, bool >::iterator it = m_clientsToRemoveObject.begin(); it != m_clientsToRemoveObject.end(); it++ )
                    removeClient( it->first, it->second );
                m_clientsToRemoveObject.clear();
            }
            m_syncToRemove.unlock();
        }
        if( m_syncToSend.tryLock() )
        {
            for( std::list< DelayedPacket >::iterator it = m_packetsToSend.begin(); it != m_packetsToSend.end(); it++ )
                sendPacketToClients( it->packet, it->excludeClient );
            m_packetsToSend.clear();
            m_syncToSend.unlock();
        }
    }

    bool Network::sendPacketToClients( sf::Packet& packet, Client* excludeClient )
    {
        if( m_sync.tryLock() )
        {
            for( std::map< std::string, Client* >::iterator it = m_clients.begin(); it != m_clients.end(); it++ )
                if( it->second != excludeClient )
                    it->second->sendPacket( packet );
            m_sync.unlock();
            return true;
        }
        return false;
    }

    void Network::sendPacketToClientsDelayed( sf::Packet& packet, Client* excludeClient )
    {
        SYNCHRONIZED_OBJECT( m_syncToSend );
        m_packetsToSend.push_back( DelayedPacket( packet, excludeClient ) );
    }

    void Network::run()
    {
        m_working = true;
        while( true )
        {
            if( !m_working )
                break;
            process();
            if( m_receivingInterval > 0 )
                XeCore::Common::Concurrent::Thread::sleep( m_receivingInterval );
        }
    }

    bool Network::onFetchClient( Client* client )
    {
        std::stringstream ss;
        ss << "accepted" << m_lastAcceptedId;
        m_lastAcceptedId++;
        return addClient( ss.str(), client );
    }

    RTTI_CLASS_DERIVATIONS( Network::Server,
                            RTTI_DERIVATION( XeCore::Common::Concurrent::Thread ),
                            RTTI_DERIVATIONS_END
                            )

    Network::Server::Server( unsigned short portToListen, unsigned int listeningIntervalMs )
    : RTTI_CLASS_DEFINE( Server )
    , m_port( portToListen )
    , m_listeningInterval( listeningIntervalMs )
    , m_working( false )
    , m_acceptingClientCache( 0 )
    {
    }

    Network::Server::~Server()
    {
        stop();
        m_listener.close();
        DELETE_OBJECT( m_acceptingClientCache );
        Client* client;
        for( std::list< Client* >::iterator it = m_pendingClients.begin(); it != m_pendingClients.end(); it++ )
        {
            client = *it;
            DELETE_OBJECT( client );
        }
        m_pendingClients.clear();
    }

    void Network::Server::run()
    {
        if( m_listener.listen( m_port ) != sf::Socket::Done )
            return;
        m_listener.setBlocking( false );
        m_working = true;
        while( true )
        {
            if( !m_working )
                break;
            if( !m_acceptingClientCache )
                m_acceptingClientCache = xnew Client();
            if( m_listener.accept( m_acceptingClientCache->m_socket ) == sf::Socket::Done && m_acceptingClientCache->isValid() )
            {
                SYNCHRONIZED_OBJECT( m_sync );
                m_acceptingClientCache->m_socket.setBlocking( false );
                m_pendingClients.push_back( m_acceptingClientCache );
                m_acceptingClientCache = 0;
            }
            if( m_listeningInterval > 0 )
                XeCore::Common::Concurrent::Thread::sleep( m_listeningInterval );
        }
        m_listener.close();
        DELETE_OBJECT( m_acceptingClientCache );
    }

    void Network::Server::fetchPendingClients( Network* network, ICallback* onFetchClientCallback )
    {
        if( !network )
            return;
        SYNCHRONIZED_OBJECT( m_sync );
        if( !m_pendingClients.empty() )
        {
            Client* client;
            for( std::list< Client* >::iterator it = m_pendingClients.begin(); it != m_pendingClients.end(); it++ )
            {
                client = *it;
                if( ( onFetchClientCallback ? !onFetchClientCallback->onFetchClient( this, client ) : false ) || !network->onFetchClient( client ) )
                    DELETE_OBJECT( client );
            }
            m_pendingClients.clear();
        }
    }

    RTTI_CLASS_DERIVATIONS( Network::Client,
                            RTTI_DERIVATIONS_END
                            )

    Network::Client::Client()
    : RTTI_CLASS_DEFINE( Client )
    , m_callback( 0 )
    {
    }

    Network::Client::Client( const sf::IpAddress& address, unsigned short port )
    : RTTI_CLASS_DEFINE( Client )
    , m_callback( 0 )
    {
        m_socket.connect( address, port );
        m_socket.setBlocking( false );
    }

    Network::Client::~Client()
    {
        m_socket.disconnect();
    }

    void Network::Client::sendPacket( sf::Packet& packet )
    {
        if( isValid() )
        {
            sf::Socket::Status status = m_socket.send( packet );
            if( status == sf::Socket::Disconnected && m_callback )
                m_callback->onDisconnected( this );
        }
    }

    void Network::Client::receivePacket()
    {
        if( m_callback )
        {
            sf::Packet packet;
            sf::Socket::Status status =  m_socket.receive( packet );
            if( status == sf::Socket::Done )
                m_callback->onReceivePacket( this, packet );
            else if( status == sf::Socket::Disconnected )
                m_callback->onDisconnected( this );
        }
    }

}
