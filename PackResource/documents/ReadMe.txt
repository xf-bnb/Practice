1：工程结构

PackResource
    document
        ReadMe.txt
    output
        ResourcePackager
            Debug
            Release
        ResourceLoader
            Debug
            Release
        ResourceTest
            Debug
            Release
    projects
        vs_2017
            ResourcePackager
                ResourceLoader.vcxproj
            ResourceLoader
                ResourceLoader.vcxproj
            ResourceTest
                ResourceLoader.vcxproj
            PackResource.sln
        gcc
            ResourcePackager
                debug.sh
                release.sh
            ResourceLoader
                debug.sh
                release.sh
            ResourceTest
                debug.sh
                release.sh
        xcode
            PackResource.xcworkspace
            ResourcePackager
            ResourceLoader
            ResourceTest
    sources
        ResourcePackager
            source files ...
        ResourceLoader
            source files ...
        ResourceTest
            source files ...
    .gitignore
    .gitattributes


整个工程分为3个模块：
ResourcePackager  - 资源打包模块，生成资源打包工具对指定目录进行打包。
ResourceLoader    - 资源加载模块，实现对资源包加载解析，提供资源访问接口。
ResourceTest      - 资源测试模块，提供对上述资源打包和加载模块的测试功能。


所有工程文件分类放在4个目录中：
document          - 项目文档目录，存放项目的说明文档和介绍。
output            - 项目输出目录，存放项目编译输出的目标文件。
projects          - 工程文件目录，存放各平台或IDE的工程组织文件。
sources           - 源代码目录，存放不同模块的源代码。

该项目支持跨平台编译，已针对 Visual Studio、GCC、Xcode 配置好了工程结构或编译脚本。
进入 projects 目录对应的模块，直接编译对应的配置类型(Debug/Release)即可生成目标输出文件到 output 目录中。
详细编译步骤参见第3点：编译。

2：运行和使用

a：ResourcePackager
在终端中执行生成好的可执行程序，并指定要进行打包的目录路径参数，或继续指定输出资源包名称参数(可选)，回车之后将在打包目录平级的路径中生成资源包文件。
运行步骤：
Windows 平台：
1：打开一个命令提示符(CMD)窗口
2：输入：.../ResourcePackager.exe .../a/b

Linux/OS X 平台：
1：打开终端
2：输入：.../ResourcePackager .../a/b

格式为：.../ResourcePackager [res directory] [pack name]

其中 [pack name](资源包文件名)参数可选
若未指定，则根据最后一级目录名以及当前的时间戳和成功打包的文件个数生成资源包文件名。
执行：.../ResourcePackager .../a/b
输出文件：.../a/b-20180723120708123-32.respackage    // 可能的输出

若指定包名且带扩展名，则使用指定的名称参数作为资源包文件名。
执行：.../ResourcePackager .../a/b x.ext
输出文件：.../a/x.ext

若指定但未带扩展名，则使用指定的名称和默认的扩展名(.respackage)作为资源包文件名。
执行：.../ResourcePackager .../a/b x
输出文件：.../a/x.respackage

目录路径参数支持绝对路径和相对路径，资源包文件名只支持相对路径(相对于目录参数平级位置)，因此建议仅指定名称或不指定。

b：ResourceLoader
此模块为接口项目，编译后将生成C++静态库文件，可以通过源码引用或库依赖引用的方式使用提供的接口。

源码引用：
直接将 .../sources/ResourceLoader/ 目录中的所有源代码文件拷贝到自己的项目中，要调用资源访问接口包含 ResLoader.h 头文件即可，所有访问资源接口都声明在该头文件中。
注意：资源访问操作实现采用 C++17 标准，若要采用源码引用参与编译，编译器必须支持 C++17。

静态库引用：
1：为项目添加静态库文件依赖
Visual Studio 在[项目属性->链接器->输入->附加依赖项]中添加 ResourceLoader.lib 库文件依赖，并在[附加库目录]选项中配置好库文件搜索路径。
Xcode 在[Build Phases->Link Binary With Libraries] 添加 libResourceLoader.a 库文件依赖，并在构建配置中配置好库文件搜索路径。
GCC 编译器在链接的时候指定链接选项：-lResourceLoader，可能需要使用 -L 选项指定库文件搜索路径。



2：包含模块头文件
将 .../sources/ResourceLoader/ 目录中的所有头文件拷贝到自己的项目中，在需要使用资源访问接口的时候包含 ResLoader.h 头文件。

参见 ResourceTest 模块对 ResourceLoader 的引用。

部分接口介绍：
bool LoadResource(const char* packName);
加载资源包，参数 packName 指定要加载的资源包路径，返回加载是否成功。
若 packName 参数指定的路径无效，或不是一个有效的资源包文件将返回失败。若对一个已经加载成功的资源包重复加载将直接返回成功。

bool FreeResource(const char* packName);
释放资源包，参数 packName 指定要释放的资源包路径，返回释放是否成功。
仅能对已经成功加载过的资源包进行释放操作，若 packName 参数指定的路径无效，或该资源包未加载，则返回失败。

ResData      GetResource(const char* resName);
unsigned int GetResource(const char* resName, void* buffer, unsigned int start, unsigned int n);
获取资源包数据，resName 指定要获取的资源名，buffer 指定要获取的资源存放的缓冲区，start 指定要获取的资源数据开始位置，n 指定要获取的字节数。
版本1：返回获取到的资源数据对象，若 resName 指定的资源不存在，则返回无效对象。
版本2：返回成功接收的字节数，若 resName 指定的资源不存在，或 buffer 参数为空，返回0。

std::set<std::string> ResourceNameList();
查看所有存在的资源名列表

c：ResourceTest
在终端中执行生成好的测试可执行程序，并指定要进行测试的参数。

先决条件：
准备好多个目录放入若干不同类型的资源文件：
.../TestPack/res1
.../TestPack/res2
.../TestPack/res3
.../TestPack/res4
...

使用打包工具依次对各目录打包：.../ResourcePackager .../TestPack/res[n] res[n]
生成对应的资源包文件：.../TestPack/res[n].respackage 以备测试使用

测试指令说明：(在终端执行)
.../ResourceTest 0
加载资源包，并将资源包中的资源数据读取出来写入到本地目录中，并与之前打包的目录中的文件进行比较查看是否一致。以此检验打包加载和读取数据的准确性。

.../ResourceTest 1
依次加载多个资源包，同时打印已存在的资源名称列表，检查与之前的包目录中的文件信息是否对应。然后依次释放资源包，同时打印已存在的资源列表，检查与之前的包目录中的文件信息是否对应。
以此检验加载和释放资源的准确性。

.../ResourceTest 2
加载所有资源包，列出存在的资源列表，依次读取所有资源数据，同时依据资源列表依次读取磁盘文件。对比从包中获取资源和从磁盘读取文件的速度。

.../ResourceTest 3
加载所有资源包，列出存在的资源列表，使用若干线程同时读取资源，并开启一个线程同时释放资源，查看日志，以检验资源接口在多线程环境中的准确性。

详细测试步骤和测试报告，参见：验证期项目测试报告



3：编译
整个项目仅依赖C++标准库实现，支持跨平台编译，已在 Windows 和 OS X 上编译通过。
在 projects 目录中提供了各平台对应的工程组织文件或脚本，配置好了对应的依赖和输出路径，进入对应平台的工程目录编译生成即可。

Visual Studio 2017：
进入 .../PackResource/projects/vs_2017 目录中，双击打开 PackResource.sln 文件以启动 Visual Studio。
以 ResourcePackager 为例：在 ResourcePackager 项目上右键重新生成，目标可执行程序文件将输出到：
.../PackResource/output/ResourcePackager/[Debug/Release]/ResourcePackager.exe

Xcode：
进入 .../PackResource/projects/xcode 目录中，双击打开 PackResource.xcworkspace 以启动 Xcode。
注意：由于目前 Xcode 尚不完整支持 C++17，故 Xcode 中不能编译项目，仅能打开查看或编辑。

GCC：
打开终端
进入 .../PackResource/projects/gcc 对应模块的目录，以 ResourcePackager 为例：
进入 .../PackResource/projects/gcc/ResourcePackager 目录，执行 debug.sh 或 release.sh 脚本(分别对应不同编译配置)，待编译完成。
目标可执行程序文件将输出到：
.../PackResource/output/ResourcePackager/[Debug/Release]/ResourcePackager

Visual Studio 和 GCC 编译都提供两种方式：Debug/Release
Debug 版本是未经编译优化的版本，携带可调式信息，性能可能低于 Release 版本。
Release 版本是经过编译优化的版本，生成的目标文件较小，性能可能会较 Debug 版本提升一些。

注意：ResourceLoader 模块被配置成静态库项目
在 Windows 中编译生成的库文件名为：ResourceLoader.lib
在 Linux 或 Mac 中生成的库文件名为：libResourceLoader.a



4：程序说明
a：资源包路径支持绝对路径和相对路径，并且资源包路径和资源名不区分大小写，所有路径分隔符支持两种斜杠([/] 和 [\])混用。
示例说明：
假定当前程序工作目录：”E:\\a\b\c\d”
资源包路径：”E:\\a\b\e\f.respackage”
意味着：
LoadResource(”E:/a/b/e/f.respackage”) 与 LoadResource(”..\..\E\F.respackage”) 加载的是同一个资源包
GetResource(“x/y/m.png”) 与 GetResource(“X\Y\M.png”) 获取的是同一个资源

b：在 Windows 平台下，对资源包的加载采取的是独占模式。
意味着：LoadResource(”E:/a/b/e/f.respackage”) 一旦调用成功，文件 f.respackage 将不可被删除和修改，但可以打开读取。
调用 FreeResource(”E:/a/b/e/f.respackage”) 可以解除对该文件的占用。

c：建议调用 ResData GetResource(const char* resName); 此版本接口来获取资源。
其返回类型 ResData 封装对内存的管理操作，调用者可以不用关心内存的分配和释放。
ResData 类型还可以隐式的转化成 bool 类型。
例如：ResData rd = GetResource(“key”); 可以使用 if (rd) 或 if (rd.IsValid()) 来判定获取资源是否成功。
若调用成功，使用 rd.Data() 为获取到的资源数据，rd.Size() 为资源数据的长度。

