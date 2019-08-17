
CompileCmd='g++-8 -std=c++17'

Config=Release
ProjectName=ResourcePackager
SourceDir=../../../sources/${ProjectName}
OutputDir=../../../output/${ProjectName}/${Config}
InterDir=../../../output/${ProjectName}/Intermediate
TargetFile=${OutputDir}/${ProjectName}

cd $(dirname "$0")

${CompileCmd} -O3 -c ${SourceDir}/rp_ResPackager.cpp -o ${InterDir}/rp_ResPackager.o
${CompileCmd} -O3 -c ${SourceDir}/rp_main.cpp -o ${InterDir}/rp_main.o
${CompileCmd} -o ${TargetFile} ${InterDir}/rp_ResPackager.o ${InterDir}/rp_main.o -lstdc++fs

