#!/bin/bash

# Code::Blocks aplication path.
CODEBLOCKS="codeblocks.exe" # /d/Applications/CodeBlocks/codeblocks.exe
MSBUILD="MSBuild.exe" # /c/WINDOWS/Microsoft.NET/Framework/v4.0.30319/MSBuild.exe
MODE=-1
BUILDMODE="--build"
MSBUILDMODE="build"
BUILDMSG="Build"

for arg in ${@}; do
	if [ ${MODE} = -1 ]; then
		if [ ${arg} = "-cb" ]; then
			MODE=0
		elif [ ${arg} = "-msb" ]; then
			MODE=1
		elif [ ${arg} = "-r" ]; then
			BUILDMODE="--rebuild"
			MSBUILDMODE="rebuild"
			BUILDMSG="Rebuild"
		elif [ ${arg} = "-c" ]; then
			BUILDMODE="--clean"
			MSBUILDMODE="clean"
			BUILDMSG="Clean"
		fi
	elif [ ${MODE} = 0 ]; then
		MODE=-1
		CODEBLOCKS=${arg}
	elif [ ${MODE} = 1 ]; then
		MODE=-1
		MSBUILD=${arg}
	fi
done

echo "=== Ptakopysk ==="

echo "${BUILDMSG}: Debug..."
${CODEBLOCKS} /na /nd /ns --multiple-instance ${BUILDMODE} --no-batch-window-close --target="Debug" ./development/ptakopysk.workspace

echo "${BUILDMSG}: Release..."
${CODEBLOCKS} /na /nd /ns --multiple-instance ${BUILDMODE} --no-batch-window-close --target="Release" ./development/ptakopysk.workspace

if [ ${BUILDMODE} = "--build" ] || [ ${BUILDMODE} = "--rebuild" ]; then
	echo "=== Partial SDK ==="
	./makesdk.sh -O -A -e -all -A -i p -A -i b -A -i j -A -i s -A -i x -A -i t
fi

echo "=== Scene View Interface ==="

echo "${BUILDMSG}: Debug..."
${CODEBLOCKS} /na /nd /ns --multiple-instance ${BUILDMODE} --no-batch-window-close --target="Debug" ./development/SceneViewInterface/SceneViewInterface.cbp

echo "${BUILDMSG}: Release..."
${CODEBLOCKS} /na /nd /ns --multiple-instance ${BUILDMODE} --no-batch-window-close --target="Release" ./development/SceneViewInterface/SceneViewInterface.cbp

if [ ${BUILDMODE} = "--build" ] || [ ${BUILDMODE} = "--rebuild" ]; then
	echo "=== Partial SDK ==="
	./makesdk.sh -O -A -e -all -A -i svi
fi

echo "=== Scene View Plugin ==="

echo "${BUILDMSG}: Debug..."
${CODEBLOCKS} /na /nd /ns --multiple-instance ${BUILDMODE} --no-batch-window-close --target="Debug" ./development/SceneViewPlugin/SceneViewPlugin.cbp

echo "${BUILDMSG}: Release..."
${CODEBLOCKS} /na /nd /ns --multiple-instance ${BUILDMODE} --no-batch-window-close --target="Release" ./development/SceneViewPlugin/SceneViewPlugin.cbp

echo "=== Zasuvka Ptakopyska IDE ==="

echo "${BUILDMSG}: Debug"
${MSBUILD} ./development/ZasuvkaPtakopyska/ZasuvkaPtakopyska.sln /target:${MSBUILDMODE} /property:Configuration=Debug

echo "${BUILDMSG}: Release"
${MSBUILD} ./development/ZasuvkaPtakopyska/ZasuvkaPtakopyska.sln /target:${MSBUILDMODE} /property:Configuration=Release
