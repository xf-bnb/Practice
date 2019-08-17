
CompileCmd='g++ -std=c++17'
ArchiveCmd=ar

Config=Debug
ProjectName=ResourceLoader
SourceDir=../../sources/${ProjectName}
OutputDir=../../../output/${ProjectName}/${Config}
InterDir=../../../output/${ProjectName}/Intermediate
TargetFile=${OutputDir}/lib${ProjectName}.a

cd $(dirname "$0")

${CompileCmd} -c ${SourceDir}/ResReader.cpp -o ${InterDir}/ResReader.o
${CompileCmd} -c ${SourceDir}/ResLoader.cpp -o ${InterDir}/ResLoader.o
${ArchiveCmd} rcs ${TargetFile} ${InterDir}/ResReader.o ${InterDir}/ResLoader.o

