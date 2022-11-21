#include <assert.h>

#include<fstream>
#include<iostream>
#include<filesystem>

namespace fs = std::filesystem;

int hello_filesystem(int argc, char argv[])
{
    std::cout << "fs demo:\n" << std::boolalpha;

    //<1> 判断文件夹是否存在
    std::string dir_name("log");
    fs::path url(dir_name);
    if (!fs::exists(url))
    {
        std::cout << std::quoted(dir_name) << " not exist" << "\n";
    }
    else
    {
        std::cout << std::quoted(dir_name) << " not exist" << "\n";
    }

    //<2> 创建单层目录
    bool okey = fs::create_directory(dir_name);
    std::cout << "create_directory(" << std::quoted(dir_name) << "), result=" << okey << "\n";

    //<3> 逐级创建多层目录
    std::error_code err;
    std::string subDir = dir_name + "/subdir";
    okey = fs::create_directory(subDir, err);
    std::cout << "create_directory(" << std::quoted(subDir) << "), result=" << okey << "\n";
    std::cout << "err.value()=" << err.value() << " err.message()=" << err.message() << "\n";

    //<4> 创建多级目录
    dir_name = "a/b//c/d/";
    okey = fs::create_directories(dir_name, err);
    std::cout << "create_directories(" << std::quoted(dir_name) << "), result=" << okey << "\n";
    std::cout << "create_directories, err.value()=" << err.value() << " err.message()=" << err.message() << "\n";

    //<5> 当前文件路径
    fs::path currentPath = fs::current_path();//C:\Users\Kandy\Desktop\fs\fs
    std::cout << "currentPath:\t" << currentPath << "\n";//当前路径
    std::cout << "root_directory:\t" << currentPath.root_directory() << "\n";//根目录
    std::cout << "relative_path:\t" << currentPath.relative_path() << "\n";//相对路径
    std::cout << "root_name:\t" << currentPath.root_name() << "\n";//根名
    std::cout << "root_path:\t" << currentPath.root_path() << "\n";//根路径

    //<6> 创建文件"from.dat"
    fs::path oldPath(fs::current_path() / "from.dat");
    std::fstream file(oldPath, std::ios::out | std::ios::trunc);
    if (!file)
    {
        std::cout << "create file(" << oldPath.string() << ") failed!" << "\n";
    }
    file.close();

    //<7> 获取相对于base的绝对路径
    fs::path abs_path = fs::absolute(oldPath/*, fs::current_path()*/);
    std::cout << "abs_path=" << abs_path.string() << "\n";//"C:\Users\Kandy\Desktop\filesystem\filesystem\from.dat"

    //<8> 文件拷贝
    fs::create_directories(fs::current_path() / "to");
    fs::path to_path(fs::current_path() / "to/from0.dat");
    fs::copy(oldPath, to_path);

    //<9> 移动文件或重命名
    fs::path newPath(fs::current_path() / "to/to.dat");
    fs::rename(oldPath, newPath);

    //<10> 创建文件 "example.dat"
    fs::path _path = fs::current_path() / "example.dat";
    std::cout << "example.dat path:" << std::quoted(_path.string()) << "\n";
    //std::ofstream(_path).put('a'); // create file of size 1
    //???这个地方感觉不对
    //std::ofstream(_path).close();

    std::ofstream a_file(_path);
    a_file.put('a');
    a_file.put('a');
    a_file.close();

    //文件类型判定
    assert(fs::file_type::regular == fs::status(_path).type());

    //<11> 获取文件大小
    auto size = fs::file_size(_path);
    std::cout << "file_size=" << size << std::endl;

    //<12> 获取文件最后修改时间
    auto time = fs::last_write_time(_path);
    std::cout << "last_write_time=" << time.time_since_epoch().count() << std::endl;

    //<13> 删除文件
    okey = fs::remove(_path);
    std::cout << "remove(" << std::quoted(_path.string()) << ")" << okey << std::endl;

    //<14> 递归删除目录下所有文件,返回被成功删除的文件个数
    uintmax_t count = fs::remove_all(dir_name);//dir_name="a/b//c/d/",会把d目录也删掉
    std::cout << "remove_all(" << std::quoted(dir_name) << ")" << count << std::endl;

    //<15> 在临时文件夹下创建文件夹并删除
    fs::path tmp = fs::temp_directory_path();
    //"C:\Users\xxx\AppData\Local\Temp\"
    std::cout << "temp_directory_path=" << std::quoted(tmp.string()) << std::endl;
    fs::create_directories(tmp / "_abcdef/example");
    std::uintmax_t n = fs::remove_all(tmp / "_abcdef");
    std::cout << "Deleted " << n << " files or directories\n";

    const char *path = "C:\\Windows\\sysatem32";
    //文件入口
    fs::directory_entry entry(path);
    //这里用了C++11的强枚举类型
    if (entry.status().type() == fs::file_type::directory)
    {
        std::cout << "it is a directory" << std::endl;
    }
    fs::directory_iterator list(path);	        //文件入口容器
    for (auto& it : list)
    {
        std::cout << it.path().filename() << std::endl;
    }

    fs::path pathToShow("C:\\Windows\\system.ini");
    std::cout << "exists() = " << fs::exists(pathToShow) << "\n"
        << "root_name() = " << pathToShow.root_name() << "\n"
        << "root_path() = " << pathToShow.root_path() << "\n"
        << "relative_path() = " << pathToShow.relative_path() << "\n"
        << "parent_path() = " << pathToShow.parent_path() << "\n"
        << "filename() = " << pathToShow.filename() << "\n"
        << "stem() = " << pathToShow.stem() << "\n"
        << "extension() = " << pathToShow.extension() << "\n";
    return 0;
}