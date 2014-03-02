#ifndef __PTAKOPYSK__BIT_FIELD_SERIALIZER__
#define __PTAKOPYSK__BIT_FIELD_SERIALIZER__

#include "Serialized.h"

namespace Ptakopysk
{

    class BitFieldSerializer
    : public Serialized::ICustomSerializer
    {
    public:
        typedef std::map< std::string, int > BitValues;

        BitFieldSerializer();
        BitFieldSerializer( const BitValues& bitvalues );
        virtual ~BitFieldSerializer();

        virtual Json::Value serialize( const void* srcValue );
        virtual void deserialize( const void* dstValue, const Json::Value& root );

    protected:
        void setup( const BitValues& bitvalues );

    private:
        BitValues m_bitValues;
    };

}

#endif
