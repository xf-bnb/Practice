
CompileCmd='g++-8 -std=c++17'
ArchiveCmd=ar

Config=Release
ProjectName=ResourceLoader
SourceDir=../../../sources/${ProjectName}
OutputDir=../../../output/${ProjectName}/${Config}
InterDir=../../../output/${ProjectName}/Intermediate
TargetFile=${OutputDir}/lib${ProjectName}.a

cd $(dirname "$0")

${CompileCmd} -O3 -c ${SourceDir}/ResReader.cpp -o ${InterDir}/ResReader.o
${CompileCmd} -O3 -c ${SourceDir}/ResLoader.cpp -o ${InterDir}/ResLoader.o
${ArchiveCmd} rcs ${TargetFile} ${InterDir}/ResReader.o ${InterDir}/ResLoader.o
