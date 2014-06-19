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
sed "s|<Unit filename=\"TemplateComponent.cpp\" />||" < "${OUTPUT_PATH}/${PROJECT_NAME}.cbp" > "${OUTPUT_PATH}/${PROJECT_NAME}.cbp_"
sed "s|<Unit filename=\"TemplateComponent.h\" />||" < "${OUTPUT_PATH}/${PROJECT_NAME}.cbp_" > "${OUTPUT_PATH}/${PROJECT_NAME}.cbp"
rm -f "${OUTPUT_PATH}/${PROJECT_NAME}.cbp_"
sed "s|\"Template Application\"|\"${PROJECT_NAME}\"|" < "TemplateMain.cpp" > "${OUTPUT_PATH}/main.cpp_"
sed '/\/\/<TEMPLATE/,/\/\/TEMPLATE>/d' < "${OUTPUT_PATH}/main.cpp_" > "${OUTPUT_PATH}/main.cpp"
rm -f "${OUTPUT_PATH}/main.cpp_"
cp "bin/CONTF.ttf" "${OUTPUT_PATH}/bin/"
cp "bin/libsndfile-1.dll" "${OUTPUT_PATH}/bin/"
cp "bin/logo.png" "${OUTPUT_PATH}/bin/"
cp "bin/openal32.dll" "${OUTPUT_PATH}/bin/"
cp "bin/template_game.json" "${OUTPUT_PATH}/bin/"
