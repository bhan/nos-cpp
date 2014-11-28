#include <utility>
#include "TupleFunctional.hpp"
#include "Serializer.hpp"

/*
    Serialization should be cleaner/consistent to code-generate

    In the IDL:

        bar_method void(uint32_t, vector<string>, bool)

    This will translate to the following generated client code:

        void Foo::bar_method(uint32_t v1, const std::vector<std::string> &v2, bool v3) {
            auto arguments = std::make_tuple(v1, v2, v3);
            rpc_send({
                ClassID:   _class_id,
                ObjectID:  _object_id,
                Type:      INVOKE,
                Method:    "bar_method",
                Arguments: Serializer::pack<decltype(arguments)>(arguments);
            })
            // do other stuff...
        }


    and the following server code:

        // lookup object
        Foo *obj = lookup_object(request.ObjectID);

        // lookup address to member function (i.e. &Bar::bar_method)
        auto func = lookup_method(request.methodID);

        // de-serialize args to method call
        auto args = Serializer::unpack<std::tuple<int, double, std::string>>(request.Arguments);

        // Apply args tuple to function
        TupleFunctional::apply_nonstatic_fn(func, obj, the_tuple);
*/


double func(int a, const std::vector<int8_t> &b, double c, std::string &d) {
    std::cout << "Inside static function: "
              << a << " "
              << c << " "
              << d << std::endl;
    for (const auto &i : b) std::cout << int32_t(i) << std::endl;
    return 2.71;
}


struct Bar {
    double bar_method(int a, const std::vector<int8_t> &b, double c, std::string &d) {
        std::cout << "Inside non-static function: "
                  << a << " "
                  << c << " "
                  << d << std::endl;
        for (const auto &i : b) std::cout << int32_t(i) << std::endl;
        return 3.14159265;
    }

    std::vector<std::string> baz_method(int a, const std::vector<int8_t> &b, double c, std::string &d) {
        std::cout << "Inside non-static function: "
                  << a << ", "
                  << c << ", "
                  << d << std::endl;
        for (const auto &i : b) std::cout << int32_t(i) << "," << std::endl;
        return std::vector<std::string> { "here", "is", "a", "sample", "vector", "of", "strings", };
    }
};

int main(int argc, char** argv) {
    std::string packet;
    {
        // A complex template parameter like std::vector<int8_t> cannot be the last argument in the tuple, because
        // the compiler freaks out with "candidate template ignored: invalid explicitly-specified argument for template parameter 'T'"
        // (Need to search up more on this issue)
        auto the_tuple = std::make_tuple(42, std::vector<int8_t>{ 1, -2, 2, -37 }, 12.3, std::string("hello"));
        packet = Serializer::pack<decltype(the_tuple)>(the_tuple);
        std::cerr << typeid(the_tuple).name() << std::endl;
    }

    {
        Bar *bar = new Bar();
        auto the_tuple = Serializer::unpack<std::tuple<int, std::vector<int8_t>, double, std::string>>(packet);

        // To apply tuple as arguments to static function
        std::cout << "Applying tuple to static function...\n";
        double result = TupleFunctional::apply_fn(func, the_tuple);
        std::cout << "result is " << result << "\n\n";

        // To apply tuple as arguments to non-static member functions (object methods)
        std::cout << "Applying tuple to non-static member function converted to static function by std::mem_fn...\n";
        auto mem_fn_tuple = std::tuple_cat( std::tuple<Bar*> { bar }, the_tuple );
        result = TupleFunctional::apply_fn(std::mem_fn(&Bar::bar_method), mem_fn_tuple);
        std::cout << "result is " << result << "\n\n";

        // The cleaner alternative
        std::cout << "Applying tuple to non-static member function using TupleFunctional::apply_nonstatic_fn()...\n";
        double result2 = TupleFunctional::apply_nonstatic_fn(&Bar::bar_method, bar, the_tuple);
        std::cout << "result2 is " << result2 << "\n\n";

        // The cleaner alternative
        std::cout << "Applying tuple to non-static member function that returns vector<string> using TupleFunctional::apply_nonstatic_fn()...\n";
        auto result3 = TupleFunctional::apply_nonstatic_fn(&Bar::baz_method, bar, the_tuple);
        std::cout << "\nresult3 is:\n";
        for (const auto &i : result3) std::cout << i << "\n";
    }
}
