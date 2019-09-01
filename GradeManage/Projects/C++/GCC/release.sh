
CompileCmd='gcc -std=c11'

ProjectName=GradeManage
SourceDir=../Sources
OutputDir=../../../OutPut
InterDir=${OutputDir}/Intermediate
TargetFile=${OutputDir}/${ProjectName}

cd $(dirname "$0")

${CompileCmd} -O3 -c ${SourceDir}/gm_Utility.c  -o ${InterDir}/gm_Utility.o
${CompileCmd} -O3 -c ${SourceDir}/gm_Command.c  -o ${InterDir}/gm_Command.o
${CompileCmd} -O3 -c ${SourceDir}/gm_Manager.c  -o ${InterDir}/gm_Manager.o
${CompileCmd} -O3 -c ${SourceDir}/gm_Include.c  -o ${InterDir}/gm_Include.o
${CompileCmd} -O3 -c ${SourceDir}/gm_Person.c   -o ${InterDir}/gm_Person.o
${CompileCmd} -O3 -c ${SourceDir}/gm_Resource.c -o ${InterDir}/gm_Resource.o
${CompileCmd} -O3 -c ${SourceDir}/gm_Main.c     -o ${InterDir}/gm_Main.o

${CompileCmd} -o ${TargetFile} ${InterDir}/gm_Utility.o ${InterDir}/gm_Command.o ${InterDir}/gm_Manager.o ${InterDir}/gm_Include.o ${InterDir}/gm_Person.o ${InterDir}/gm_Resource.o ${InterDir}/gm_Main.o
