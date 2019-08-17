
CompileCmd='g++-8 -std=c++17'

Config=Release
ProjectName=ResourceTest
RelyOnName=ResourceLoader
IncludeDir=../../../sources/${RelyOnName}
SourceDir=../../../sources/${ProjectName}
OutputDir=../../../output/${ProjectName}/${Config}
RelyOnDir=../../../output/${RelyOnName}/${Config}
InterDir=../../../output/${ProjectName}/Intermediate
TargetFile=${OutputDir}/${ProjectName}

cd $(dirname "$0")

${CompileCmd} -c -O3 -I${IncludeDir} ${SourceDir}/rt_ResTest.cpp -o ${InterDir}/rt_ResTest.o
${CompileCmd} -c -O3 ${SourceDir}/rt_TestLog.cpp -o ${InterDir}/rt_TestLog.o
${CompileCmd} -c -O3 ${SourceDir}/rt_main.cpp -o ${InterDir}/rt_main.o
${CompileCmd} -o ${TargetFile} ${InterDir}/rt_ResTest.o ${InterDir}/rt_TestLog.o ${InterDir}/rt_main.o -lstdc++fs -L${RelyOnDir} -l${RelyOnName}

