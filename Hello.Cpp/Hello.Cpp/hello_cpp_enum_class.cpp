
enum class EC_A
{
    A1_1,
    A1_2,
};

enum class EC_B1 :int
{

};

int hello_cpp_enum_class(int argc,char *argv[])
{
    //���󣬲���ʹ�� + | & ���������
    //int a = EC_A::A1_1 + EC_A::A1_2;
    int a = static_cast<int>(EC_A::A1_1) + static_cast<int>(EC_A::A1_2);
    return 0;
}