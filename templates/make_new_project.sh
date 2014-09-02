#!/bin/bash
#
## usage example:
### ./make_new_project.sh -o /destination/path/ -p NewProjectName


# setup
MODE=-1
OUTPUT_PATH="./"
PROJECT_NAME="TemplateApp"

# parameters
for arg in ${@}; do
  if [ ${MODE} = -1 ]; then
    if [ ${arg} = "-o" ]; then
      MODE=0
    elif [ ${arg} = "-p" ]; then
      MODE=1
    else
      echo "Unknown parameter: ${arg}"
    fi
  elif [ ${MODE} = 0 ]; then
    OUTPUT_PATH="${arg}"
    MODE=-1
  elif [ ${MODE} = 1 ]; then
    PROJECT_NAME="${arg}"
    MODE=-1
  else
    echo "Unknown mode: ${MODE}"
    MODE=-1
  fi
done

# files structure
mkdir -p "${OUTPUT_PATH}/bin"

# manage file tempates
sed "s|TemplateApp|${PROJECT_NAME}|" < "TemplateApp.cbp" > "${OUTPUT_PATH}/${PROJECT_NAME}.cbp_"
sed "s|TemplateMain.cpp|main.cpp|" < "${OUTPUT_PATH}/${PROJECT_NAME}.cbp_" > "${OUTPUT_PATH}/${PROJECT_NAME}.cbp"
rm -f "${OUTPUT_PATH}/${PROJECT_NAME}.cbp_"
cp "TemplateMain.cpp" "${OUTPUT_PATH}/main.cpp"
cp "_include_components.h" "${OUTPUT_PATH}/_include_components.h"
cp "_register_components.inl" "${OUTPUT_PATH}/_register_components.inl"
cp "bin/CONTF.ttf" "${OUTPUT_PATH}/bin/"
cp "bin/libsndfile-1.dll" "${OUTPUT_PATH}/bin/"
cp "bin/logo.png" "${OUTPUT_PATH}/bin/"
cp "bin/openal32.dll" "${OUTPUT_PATH}/bin/"
sed "s|\"Template Application\"|\"${PROJECT_NAME}\"|" < "bin/config.json" > "${OUTPUT_PATH}/bin/config.json"
cp "bin/template_game.json" "${OUTPUT_PATH}/bin/"
./make_new_component.sh -o "${OUTPUT_PATH}" -c "TemplateComponent"
