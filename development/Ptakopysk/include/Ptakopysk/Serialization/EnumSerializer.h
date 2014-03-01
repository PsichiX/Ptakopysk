#ifndef __PTAKOPYSK__ENUM_SERIALIZER__
#define __PTAKOPYSK__ENUM_SERIALIZER__

#include "Serialized.h"

namespace Ptakopysk
{

    class EnumSerializer
    : public Serialized::ICustomSerializer
    {
    public:
        typedef std::map< std::string, int > EnumKeyValues;

        EnumSerializer();
        EnumSerializer( const EnumKeyValues& keyvalues );
        virtual ~EnumSerializer();

        virtual Json::Value serialize( const void* srcValue );
        virtual void deserialize( const void* dstValue, const Json::Value& root );

    protected:
        void setup( const EnumKeyValues& keyvalues );

    private:
        EnumKeyValues m_keyValues;
    };

}

#endif
