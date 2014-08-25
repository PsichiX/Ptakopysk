#!/bin/bash
#
## usage example:
### ./make_new_asset.sh -o /destination/path/ -c NewAssetName


# setup
MODE=-1
OUTPUT_PATH="./"
ASSET_NAME="TemplateAsset"

# parameters
for arg in ${@}; do
  if [ ${MODE} = -1 ]; then
    if [ ${arg} = "-o" ]; then
      MODE=0
    elif [ ${arg} = "-a" ]; then
      MODE=1
    else
      echo "Unknown parameter: ${arg}"
    fi
  elif [ ${MODE} = 0 ]; then
    OUTPUT_PATH="${arg}"
    MODE=-1
  elif [ ${MODE} = 1 ]; then
    ASSET_NAME="${arg}"
    MODE=-1
  else
    echo "Unknown mode: ${MODE}"
    MODE=-1
  fi
done

ASSET_NAME_UPPER="$(tr '[:lower:]' '[:upper:]' <<< ${ASSET_NAME})"

# files structure
mkdir -p "${OUTPUT_PATH}"

# manage file tempates
sed "s|TemplateAsset|${ASSET_NAME}|" < "TemplateAsset.h" > "${OUTPUT_PATH}/${ASSET_NAME}.h"
sed "s|__TEMPLATE_COMPONENT__|__${ASSET_NAME_UPPER}__|" < "${OUTPUT_PATH}/${ASSET_NAME}.h" > "${OUTPUT_PATH}/${ASSET_NAME}.h_"
sed '/\/\/<TEMPLATE/,/\/\/TEMPLATE>/d' < "${OUTPUT_PATH}/${ASSET_NAME}.h_" > "${OUTPUT_PATH}/${ASSET_NAME}.h"
rm -f "${OUTPUT_PATH}/${ASSET_NAME}.h_"
sed "s|TemplateAsset|${ASSET_NAME}|" < "TemplateAsset.cpp" > "${OUTPUT_PATH}/${ASSET_NAME}.cpp"
sed "s|TemplateAsset|${ASSET_NAME}|" < "${OUTPUT_PATH}/${ASSET_NAME}.cpp" > "${OUTPUT_PATH}/${ASSET_NAME}.cpp_"
sed '/\/\/<TEMPLATE/,/\/\/TEMPLATE>/d' < "${OUTPUT_PATH}/${ASSET_NAME}.cpp_" > "${OUTPUT_PATH}/${ASSET_NAME}.cpp"
rm -f "${OUTPUT_PATH}/${ASSET_NAME}.cpp_"
