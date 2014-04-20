#ifndef __BINARY_JSON__
#define __BINARY_JSON__

#include <XeCore/Common/Buffer.h>
#include <json/json.h>

namespace BinaryJson
{
    typedef XeCore::Common::ByteBuffer Buffer;

    bool jsonToBinary( Json::Value& root, Buffer* buff, dword keyhash = 0, bool dontClearBuffer = false );
    bool binaryToJson( Buffer* buff, Json::Value& root, dword keyhash = 0 );

}

#endif
