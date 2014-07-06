#!/bin/bash
#
# useful cases:
## overwrite only ptakopysk for include, lib and bin:
### makesdk.sh -O -A -e -all -A -i p
#

# setup
CP_PTAKOPYSK=(1 1 1)
CP_BOX2D=(1 1 1)
CP_JSONCPP=(1 1 1)
CP_SFML=(1 1 1)
CP_XECORE=(1 1 1)

TYPE=-1
MODE=1
DEBUG=0
TEST_ONLY=0
OVERWRITE=0

# parameters
for arg in ${@}; do
  if [ ${arg} = "-D" ]; then
    DEBUG=1
  elif [ ${arg} = "-T" ]; then
    TEST_ONLY=1
  elif [ ${arg} = "-O" ]; then
    OVERWRITE=1
  elif [ ${arg} = "-I" ]; then
    TYPE=0
    MODE=1
  elif [ ${arg} = "-L" ]; then
    TYPE=1
    MODE=1
  elif [ ${arg} = "-B" ]; then
    TYPE=2
    MODE=1
  elif [ ${arg} = "-A" ]; then
    TYPE=-1
	MODE=1
  elif [ ${arg} = "-include" ] || [ ${arg} = "-i" ]; then
    MODE=1
  elif [ ${arg} = "-exclude" ] || [ ${arg} = "-e" ]; then
    MODE=0
  elif [ ${arg} = "ptakopysk" ] || [ ${arg} = "p" ]; then
    if [ ${TYPE} = -1 ]; then
	  CP_PTAKOPYSK=(${MODE} ${MODE} ${MODE})
	else
	  CP_PTAKOPYSK[${TYPE}]=${MODE}
	fi
  elif [ ${arg} = "box2d" ] || [ ${arg} = "b" ]; then
    if [ ${TYPE} = -1 ]; then
	  CP_BOX2D=(${MODE} ${MODE} ${MODE})
	else
	  CP_BOX2D[${TYPE}]=${MODE}
	fi
  elif [ ${arg} = "jsoncpp" ] || [ ${arg} = "j" ]; then
    if [ ${TYPE} = -1 ]; then
	  CP_JSONCPP=(${MODE} ${MODE} ${MODE})
	else
	  CP_JSONCPP[${TYPE}]=${MODE}
	fi
  elif [ ${arg} = "sfml" ] || [ ${arg} = "s" ]; then
    if [ ${TYPE} = -1 ]; then
	  CP_SFML=(${MODE} ${MODE} ${MODE})
	else
	  CP_SFML[${TYPE}]=${MODE}
	fi
  elif [ ${arg} = "xecore" ] || [ ${arg} = "x" ]; then
    if [ ${TYPE} = -1 ]; then
	  CP_XECORE=(${MODE} ${MODE} ${MODE})
	else
	  CP_XECORE[${TYPE}]=${MODE}
	fi
  elif [ ${arg} = "-all" ] || [ ${arg} = "-a" ]; then
    if [ ${TYPE} = -1 ]; then
	  CP_PTAKOPYSK=(${MODE} ${MODE} ${MODE})
      CP_BOX2D=(${MODE} ${MODE} ${MODE})
      CP_JSONCPP=(${MODE} ${MODE} ${MODE})
      CP_SFML=(${MODE} ${MODE} ${MODE})
      CP_XECORE=(${MODE} ${MODE} ${MODE})
	else
	  CP_PTAKOPYSK[${TYPE}]=${MODE}
      CP_BOX2D[${TYPE}]=${MODE}
      CP_JSONCPP[${TYPE}]=${MODE}
      CP_SFML[${TYPE}]=${MODE}
      CP_XECORE[${TYPE}]=${MODE}
	fi
  else
	echo "Unknown parameter: ${arg}"
  fi
done

# debug
if [ ${DEBUG} = 1 ]; then
  echo "PTAKOPYSK:"
  for val in ${CP_PTAKOPYSK[*]}; do
    echo "${val}"
  done
  echo "BOX2D:"
  for val in ${CP_BOX2D[*]}; do
    echo "${val}"
  done
  echo "JSONCPP:"
  for val in ${CP_JSONCPP[*]}; do
    echo "${val}"
  done
  echo "SFML:"
  for val in ${CP_SFML[*]}; do
    echo "${val}"
  done
  echo "XECORE:"
  for val in ${CP_XECORE[*]}; do
    echo "${val}"
  done
fi

# test only
if [ ${TEST_ONLY} = 1 ]; then
  exit 0
fi

# do not overwrite
if [ ${OVERWRITE} = 0 ]; then
  rm -r ./sdk/
fi

mkdir -p ./sdk/
cp ./LICENSE ./sdk/LICENSE

# include
echo "Copying includes..."
mkdir -p ./sdk/include/
if [ ${CP_PTAKOPYSK[0]} = 1 ]; then
  cp -r ./development/Ptakopysk/include/* ./sdk/include/
fi
if [ ${CP_BOX2D[0]} = 1 ]; then
  cp -r ./development/requirements/Box2D_v2.2.1/Box2D/ ./sdk/include/
fi
if [ ${CP_JSONCPP[0]} = 1 ]; then
  cp -r ./development/requirements/jsoncpp-src-0.5.0/include/* ./sdk/include/
  cp -r ./development/requirements/BinaryJson/include/* ./sdk/include/
fi
if [ ${CP_SFML[0]} = 1 ]; then
  cp -r ./development/requirements/SFML-2.1/include/* ./sdk/include/
fi
if [ ${CP_XECORE[0]} = 1 ]; then
  cp -r ./development/requirements/xenon-core-3-sdk/Code/Engine/XenonCore3/include/* ./sdk/include/
fi

# lib
echo "Copying libs..."
mkdir -p ./sdk/lib/
if [ ${CP_PTAKOPYSK[1]} = 1 ]; then
  cp ./development/Ptakopysk/bin/*.a ./sdk/lib/
fi
if [ ${CP_BOX2D[1]} = 1 ]; then
  cp ./development/requirements/Box2D/bin/Debug/libBox2D.a ./sdk/lib/libBox2D-d.a
  cp ./development/requirements/Box2D/bin/Release/libBox2D.a ./sdk/lib/libBox2D.a
fi
if [ ${CP_JSONCPP[1]} = 1 ]; then
  cp ./development/requirements/JsonCpp/bin/Debug/libJsonCpp.a ./sdk/lib/libJsonCpp-d.a
  cp ./development/requirements/JsonCpp/bin/Release/libJsonCpp.a ./sdk/lib/libJsonCpp.a
  cp ./development/requirements/BinaryJson/libs/* ./sdk/lib/
fi
if [ ${CP_SFML[1]} = 1 ]; then
  cp ./development/requirements/SFML-2.1/lib/*.a ./sdk/lib/
fi
if [ ${CP_XECORE[1]} = 1 ]; then
  cp ./development/requirements/xenon-core-3-sdk/Code/Engine/XenonCore3/libs/*.a ./sdk/lib/
fi

# bin
echo "Copying binaries..."
mkdir -p ./sdk/bin/
if [ ${CP_SFML[1]} = 1 ]; then
  cp ./development/requirements/SFML-2.1/bin/*.dll ./sdk/bin/
fi

# templates
echo "Copying templates..."
mkdir -p ./sdk/templates/
cp ./templates/*.h ./sdk/templates/
cp ./templates/*.cpp ./sdk/templates/
cp ./templates/*.cbp ./sdk/templates/
mkdir -p ./sdk/templates/bin/
cp ./templates/bin/*.exe ./sdk/templates/bin/
cp ./templates/bin/*.dll ./sdk/templates/bin/
cp ./templates/bin/*.png ./sdk/templates/bin/
cp ./templates/bin/*.ttf ./sdk/templates/bin/
cp ./templates/bin/template_game.json ./sdk/templates/bin/
cp ./templates/make_new_project.sh ./sdk/templates/
cp ./templates/make_new_component.sh ./sdk/templates/

# IDE
echo "Copying IDE..."
mkdir -p ./sdk/IDE/resources/icons/
cp -r ./development/ZasuvkaPtakopyska/ZasuvkaPtakopyska/bin/Release/resources/icons/* ./sdk/IDE/resources/icons/
cp -r ./development/ZasuvkaPtakopyska/ZasuvkaPtakopyska/bin/Release/*.dll ./sdk/IDE/
cp -r ./development/ZasuvkaPtakopyska/ZasuvkaPtakopyska/bin/Release/ZasuvkaPtakopyska.exe ./sdk/IDE/
