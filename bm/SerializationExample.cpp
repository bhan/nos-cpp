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
        Foo *obj = lookup_table(request.ObjectID)

        // de-serialize args
        auto args = Serializer::unpack<std::tuple<int, double, std::string>>(request.Arguments);

        // bind the args to the method of the object
        std::bind(&Foo::bar_method, obj, args);
*/

struct Bar {
    void func(int a, const std::vector<int8_t> &b, double c, std::string &d) {
        std::cout << "Inside function: "
                  << a << " "
                  << c << " "
                  << d << std::endl;
        for (const auto &i : b) std::cout << int32_t(i) << std::endl;
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
        // func( std::get<0>(the_tuple), std::get<1>(the_tuple), std::get<2>(the_tuple), std::get<3>(the_tuple) );
        std::bind(&Bar::func, bar, the_tuple);
    }
}
