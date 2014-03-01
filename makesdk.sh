rm -r ./sdk/

mkdir -p ./sdk/
cp ./LICENSE ./sdk/LICENSE

# include
mkdir -p ./sdk/include/
cp -r ./development/Ptakopysk/include/* ./sdk/include/
cp -r ./development/requirements/Box2D_v2.2.1/Box2D/ ./sdk/include/
cp -r ./development/requirements/jsoncpp-src-0.5.0/include/* ./sdk/include/
cp -r ./development/requirements/SFML-2.1/include/* ./sdk/include/
cp -r ./development/requirements/xenon-core-3-sdk/Code/Engine/XenonCore3/include/* ./sdk/include/

# lib
mkdir -p ./sdk/lib/
cp ./development/Ptakopysk/bin/*.a ./sdk/lib/
cp ./development/requirements/Box2D/bin/Debug/libBox2D.a ./sdk/lib/libBox2D-d.a
cp ./development/requirements/Box2D/bin/Release/libBox2D.a ./sdk/lib/libBox2D.a
cp ./development/requirements/JsonCpp/bin/Debug/libJsonCpp.a ./sdk/lib/libJsonCpp-d.a
cp ./development/requirements/JsonCpp/bin/Release/libJsonCpp.a ./sdk/lib/libJsonCpp.a
cp ./development/requirements/SFML-2.1/lib/*.a ./sdk/lib/
cp ./development/requirements/xenon-core-3-sdk/Code/Engine/XenonCore3/libs/*.a ./sdk/lib/

# bin
mkdir -p ./sdk/bin/
cp ./development/Ptakopysk/bin/*.dll ./sdk/bin/
cp ./development/requirements/SFML-2.1/bin/*.dll ./sdk/bin/
