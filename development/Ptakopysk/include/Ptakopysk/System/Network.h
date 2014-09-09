#ifndef __PTAKOPYSK__NETWORK__
#define __PTAKOPYSK__NETWORK__

#include <XeCore/Common/Base.h>
#include <XeCore/Common/IRtti.h>
#include <XeCore/Common/MemoryManager.h>
#include <XeCore/Common/Concurrent/Thread.h>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <string>
#include <map>
#include <list>

namespace Ptakopysk
{

    class Network
    : public virtual XeCore::Common::IRtti
    , public virtual XeCore::Common::MemoryManager::Manageable
    , public XeCore::Common::Singleton< Network >
    , public XeCore::Common::Concurrent::Thread
    {
        RTTI_CLASS_DECLARE( Network );

    public:
        class ICallback;
        class Server;
        class Client;
        //class Handshake;

        Network();
        virtual ~Network();

        void start( unsigned int receivingIntervalMs = 100 );
        FORCEINLINE bool isWorking() { return m_working; };
        FORCEINLINE void stop() { m_working = false; join(); };
        Server* createServer( const std::string& id, unsigned short port, unsigned int listeningIntervalMs = 100 );
        bool addServer( const std::string& id, Server* server );
        Server* getServer( const std::string& id );
        std::string getServerId( Server* server );
        bool removeServer( const std::string& id, bool destroyObject = true );
        bool removeServer( Server* server, bool destroyObject = true );
        void removeAllServers( bool destroyObjects = true );
        Client* createClient( const std::string& id, const sf::IpAddress& address, unsigned short port );
        bool addClient( const std::string& id, Client* client );
        Client* getClient( const std::string& id );
        std::string getClientId( Client* client );
        bool removeClient( const std::string& id, bool destroyObject = true );
        bool removeClient( Client* client, bool destroyObject = true );
        void removeAllClients( bool destroyObjects = true );
        bool removeClientDelayed( const std::string& id, bool destroyObject = true );
        bool removeClientDelayed( Client* client, bool destroyObject = true );
        void removeAllClientsDelayed( bool destroyObjects = true );
        void fetchPendingClients( ICallback* onFetchClientCallback = 0 );
        void sendPacketToClients( sf::Packet& packet );

    private:
        virtual void run();
        bool onFetchClient( Client* client );

        XeCore::Common::Concurrent::Synchronized m_sync;
        XeCore::Common::Concurrent::Synchronized m_syncToRemove;
        unsigned long m_lastAcceptedId;
        volatile bool m_working;
        unsigned int m_receivingInterval;
        std::map< std::string, Server* > m_servers;
        std::map< std::string, Client* > m_clients;
        std::map< std::string, bool > m_clientsToRemoveId;
        std::map< Client*, bool > m_clientsToRemoveObject;
        std::pair< bool, bool > m_clientsToRemoveAll;
        //std::map< std::string, Handshakes* > m_handshakes;

    public:
        class ICallback
        {
        public:
            friend class Network;

            virtual ~ICallback() {}

        protected:
            virtual bool onFetchClient( Server* server, Client* client ) = 0;
        };

        class Server
        : public virtual XeCore::Common::IRtti
        , public virtual XeCore::Common::MemoryManager::Manageable
        , public XeCore::Common::Concurrent::Thread
        {
            RTTI_CLASS_DECLARE( Server )

        public:
            friend class Network;

            Server( unsigned short portToListen, unsigned int listeningIntervalMs = 100 );
            virtual ~Server();

            FORCEINLINE bool isWorking() { return m_working; };
            FORCEINLINE void stop() { m_working = false; join(); };

        private:
            virtual void run();
            void fetchPendingClients( Network* network, ICallback* onFetchClientCallback );

            XeCore::Common::Concurrent::Synchronized m_sync;
            unsigned short m_port;
            unsigned int m_listeningInterval;
            volatile bool m_working;
            sf::TcpListener m_listener;
            std::list< Client* > m_pendingClients;
            Client* m_acceptingClientCache;
        };

        class Client
        : public virtual XeCore::Common::IRtti
        , public virtual XeCore::Common::MemoryManager::Manageable
        {
            RTTI_CLASS_DECLARE( Client )

        public:
            friend class Network;
            friend class Server;

            class ICallback;

            Client();
            Client( const sf::IpAddress& address, unsigned short port );
            virtual ~Client();

            FORCEINLINE bool isValid() { return m_socket.getRemoteAddress() != sf::IpAddress::None; }
            FORCEINLINE ICallback* getCallback() { return m_callback; };
            FORCEINLINE void setCallback( ICallback* c ) { m_callback = c; };
            void sendPacket( sf::Packet& packet );

        private:
            void receivePacket();

            sf::TcpSocket m_socket;
            ICallback* m_callback;

        public:
            class ICallback
            {
            public:
                friend class Client;

                virtual ~ICallback() {}

            protected:
                virtual void onReceivePacket( Client* client, sf::Packet& packet ) = 0;
                virtual void onDisconnected( Client* client ) = 0;
            };
        };
    };

}

#endif

