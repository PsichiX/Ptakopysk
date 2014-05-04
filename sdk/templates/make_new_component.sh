#!/bin/bash
#
## usage example:
### ./make_new_project.sh -o /destination/path/ -c NewComponentName


# setup
MODE=-1
OUTPUT_PATH="./"
COMPONENT_NAME="TemplateComponent"

# parameters
for arg in ${@}; do
  if [ ${MODE} = -1 ]; then
    if [ ${arg} = "-o" ]; then
      MODE=0
    elif [ ${arg} = "-c" ]; then
      MODE=1
    else
      echo "Unknown parameter: ${arg}"
    fi
  elif [ ${MODE} = 0 ]; then
    OUTPUT_PATH="${arg}"
    MODE=-1
  elif [ ${MODE} = 1 ]; then
    COMPONENT_NAME="${arg}"
    MODE=-1
  else
    echo "Unknown mode: ${MODE}"
    MODE=-1
  fi
done

COMPONENT_NAME_UPPER="$(tr '[:lower:]' '[:upper:]' <<< ${COMPONENT_NAME})"

# files structure
mkdir "${OUTPUT_PATH}"

# manage file tempates
sed "s|TemplateComponent|${COMPONENT_NAME}|" < "TemplateComponent.h" > "${OUTPUT_PATH}/${COMPONENT_NAME}.h"
sed "s|__TEMPLATE_COMPONENT__|__${COMPONENT_NAME_UPPER}__|" < "${OUTPUT_PATH}/${COMPONENT_NAME}.h" > "${OUTPUT_PATH}/${COMPONENT_NAME}.h_"
sed '/\/\/<TEMPLATE/,/\/\/TEMPLATE>/d' < "${OUTPUT_PATH}/${COMPONENT_NAME}.h_" > "${OUTPUT_PATH}/${COMPONENT_NAME}.h"
rm -f "${OUTPUT_PATH}/${COMPONENT_NAME}.h_"
sed "s|TemplateComponent|${COMPONENT_NAME}|" < "TemplateComponent.cpp" > "${OUTPUT_PATH}/${COMPONENT_NAME}.cpp"
sed "s|TemplateComponent|${COMPONENT_NAME}|" < "${OUTPUT_PATH}/${COMPONENT_NAME}.cpp" > "${OUTPUT_PATH}/${COMPONENT_NAME}.cpp_"
sed '/\/\/<TEMPLATE/,/\/\/TEMPLATE>/d' < "${OUTPUT_PATH}/${COMPONENT_NAME}.cpp_" > "${OUTPUT_PATH}/${COMPONENT_NAME}.cpp"
rm -f "${OUTPUT_PATH}/${COMPONENT_NAME}.cpp_"
