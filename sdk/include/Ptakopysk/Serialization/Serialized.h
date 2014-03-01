#ifndef __PTAKOPYSK__SERIALIZED__
#define __PTAKOPYSK__SERIALIZED__

#include <XeCore/Common/IRtti.h>
#include <XeCore/Common/MemoryManager.h>
#include <json/json.h>
#include <vector>
#include <string>

namespace Ptakopysk
{

    class Serialized
    : public virtual XeCore::Common::IRtti
    , public virtual XeCore::Common::MemoryManager::Manageable
    {
        RTTI_CLASS_DECLARE( Serialized );

    public:
        class ICustomSerializer
        {
        public:
            ICustomSerializer() {}
            virtual ~ICustomSerializer() {};

            virtual Json::Value serialize( const void* srcValue ) = 0;
            virtual void deserialize( const void* dstValue, const Json::Value& root ) = 0;
        };

        Serialized();
        virtual ~Serialized();

        static void registerCustomSerializer( const std::string& id, ICustomSerializer* serializer );
        static void unregisterCustomSerializer( const std::string& id, bool del = true );
        static void unregisterCustomSerializer( ICustomSerializer* serializer, bool del = true );
        static void unregisterAllCustomSerializers( bool del = true );
        static ICustomSerializer* getCustomSerializer( const std::string& id );

        FORCEINLINE void serializableProperty( const std::string& name );
        FORCEINLINE void notSerializableProperty( const std::string& name );
        void serialize( Json::Value& dstRoot );
        void deserialize( const Json::Value& srcRoot );

    protected:
        virtual Json::Value onSerialize( const std::string& property ) = 0;
        virtual void onDeserialize( const std::string& property, const Json::Value& root ) = 0;

    private:
        std::vector< std::string > m_properties;

        static std::map< std::string, ICustomSerializer* > s_customSerializers;
    };

}

#include "Serialized.inl"

#endif
