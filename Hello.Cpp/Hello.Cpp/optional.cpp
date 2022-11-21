#include <optional>
#include <iostream>

// optional can be used as the return type of a factory that may fail
std::optional<std::string> create(bool b) {
    if (b)
        return "Godzilla";
    return {};
}

// std::nullopt can be used to create any (empty) std::optional
auto create2(bool b) {
    return b ? std::optional<std::string>{"Godzilla"} : std::nullopt;
}

// std::reference_wrapper may be used to return a reference
auto create_ref(bool b) {
    static std::string value_ = "Godzilla";
    return b ? std::optional<std::reference_wrapper<std::string>>{value_}
    : std::nullopt;
}

int test_optional(int argc, char* argv[])
{
    std::cout << "create(false) returned "
        << create(false).value_or("empty") << '\n';

    // optional-returning factory functions are usable as conditions of while and if
    //ͬʱ�ж���str �Ƿ���Ч��
    if (auto str = create2(true))
    {
        std::cout << "create2(true) returned " << *str << '\n';
    }

    std::cout << create2(false).value();

    if (auto str = create_ref(true))
    {
        // using get() to access the reference_wrapper's value
        std::cout << "create_ref(true) returned " << str->get() << '\n';
        str->get() = "Mothra";
        std::cout << "modifying it changed it to " << str->get() << '\n';
    }

    return 0;
}