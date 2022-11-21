#include <assert.h>

#include<fstream>
#include<iostream>
#include<filesystem>

namespace fs = std::filesystem;

int hello_filesystem(int argc, char argv[])
{
    std::cout << "fs demo:\n" << std::boolalpha;

    //<1> �ж��ļ����Ƿ����
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

    //<2> ��������Ŀ¼
    bool okey = fs::create_directory(dir_name);
    std::cout << "create_directory(" << std::quoted(dir_name) << "), result=" << okey << "\n";

    //<3> �𼶴������Ŀ¼
    std::error_code err;
    std::string subDir = dir_name + "/subdir";
    okey = fs::create_directory(subDir, err);
    std::cout << "create_directory(" << std::quoted(subDir) << "), result=" << okey << "\n";
    std::cout << "err.value()=" << err.value() << " err.message()=" << err.message() << "\n";

    //<4> �����༶Ŀ¼
    dir_name = "a/b//c/d/";
    okey = fs::create_directories(dir_name, err);
    std::cout << "create_directories(" << std::quoted(dir_name) << "), result=" << okey << "\n";
    std::cout << "create_directories, err.value()=" << err.value() << " err.message()=" << err.message() << "\n";

    //<5> ��ǰ�ļ�·��
    fs::path currentPath = fs::current_path();//C:\Users\Kandy\Desktop\fs\fs
    std::cout << "currentPath:\t" << currentPath << "\n";//��ǰ·��
    std::cout << "root_directory:\t" << currentPath.root_directory() << "\n";//��Ŀ¼
    std::cout << "relative_path:\t" << currentPath.relative_path() << "\n";//���·��
    std::cout << "root_name:\t" << currentPath.root_name() << "\n";//����
    std::cout << "root_path:\t" << currentPath.root_path() << "\n";//��·��

    //<6> �����ļ�"from.dat"
    fs::path oldPath(fs::current_path() / "from.dat");
    std::fstream file(oldPath, std::ios::out | std::ios::trunc);
    if (!file)
    {
        std::cout << "create file(" << oldPath.string() << ") failed!" << "\n";
    }
    file.close();

    //<7> ��ȡ�����base�ľ���·��
    fs::path abs_path = fs::absolute(oldPath/*, fs::current_path()*/);
    std::cout << "abs_path=" << abs_path.string() << "\n";//"C:\Users\Kandy\Desktop\filesystem\filesystem\from.dat"

    //<8> �ļ�����
    fs::create_directories(fs::current_path() / "to");
    fs::path to_path(fs::current_path() / "to/from0.dat");
    fs::copy(oldPath, to_path);

    //<9> �ƶ��ļ���������
    fs::path newPath(fs::current_path() / "to/to.dat");
    fs::rename(oldPath, newPath);

    //<10> �����ļ� "example.dat"
    fs::path _path = fs::current_path() / "example.dat";
    std::cout << "example.dat path:" << std::quoted(_path.string()) << "\n";
    //std::ofstream(_path).put('a'); // create file of size 1
    //???����ط��о�����
    //std::ofstream(_path).close();

    std::ofstream a_file(_path);
    a_file.put('a');
    a_file.put('a');
    a_file.close();

    //�ļ������ж�
    assert(fs::file_type::regular == fs::status(_path).type());

    //<11> ��ȡ�ļ���С
    auto size = fs::file_size(_path);
    std::cout << "file_size=" << size << std::endl;

    //<12> ��ȡ�ļ�����޸�ʱ��
    auto time = fs::last_write_time(_path);
    std::cout << "last_write_time=" << time.time_since_epoch().count() << std::endl;

    //<13> ɾ���ļ�
    okey = fs::remove(_path);
    std::cout << "remove(" << std::quoted(_path.string()) << ")" << okey << std::endl;

    //<14> �ݹ�ɾ��Ŀ¼�������ļ�,���ر��ɹ�ɾ�����ļ�����
    uintmax_t count = fs::remove_all(dir_name);//dir_name="a/b//c/d/",���dĿ¼Ҳɾ��
    std::cout << "remove_all(" << std::quoted(dir_name) << ")" << count << std::endl;

    //<15> ����ʱ�ļ����´����ļ��в�ɾ��
    fs::path tmp = fs::temp_directory_path();
    //"C:\Users\xxx\AppData\Local\Temp\"
    std::cout << "temp_directory_path=" << std::quoted(tmp.string()) << std::endl;
    fs::create_directories(tmp / "_abcdef/example");
    std::uintmax_t n = fs::remove_all(tmp / "_abcdef");
    std::cout << "Deleted " << n << " files or directories\n";

    const char *path = "C:\\Windows\\sysatem32";
    //�ļ����
    fs::directory_entry entry(path);
    //��������C++11��ǿö������
    if (entry.status().type() == fs::file_type::directory)
    {
        std::cout << "it is a directory" << std::endl;
    }
    fs::directory_iterator list(path);	        //�ļ��������
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