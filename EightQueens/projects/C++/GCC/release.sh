
CompileCmd='g++ -std=c++17'

ProjectName=EightQueens
SourceDir=../Sources
OutputDir=../../../output
InterDir=${OutputDir}/Intermediate
TargetFile=${OutputDir}/${ProjectName}

cd $(dirname "$0")

${CompileCmd} -O3 -c ${SourceDir}/eq_Main.cpp -o ${InterDir}/eq_Main.o

${CompileCmd} -o ${TargetFile} ${InterDir}/eq_Main.o
