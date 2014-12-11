// ./build.sh ;  ./build/bin/idlgen -t ./idlgen/tpl/generated.tpl -o KVStoreExample/ -r ./idlgen/tpl/registrar.tpl -x ./idlgen/tpl/client_registrar.tpl KVStoreExample/KVStore.hpp
// c++ -std=c++11 -I.. client.cpp ClientRegistrar.cpp ../nos/NOSCommon.cpp ../nos/NOSClient.cpp ../nos/RPCRequest.cpp ../nos/RPCResponse.cpp -L../tcpsockets -ltcpsockets -o client
// ./client 2> /dev/null
#include <iostream>
#include <string>
#include <fstream>

#include "KVStoreGenerated.hpp"

#define DEBUG_MAX_ARGS      5
#define DEBUG_MAX_LINE      256

NOSClient* NOSClient::_instance = nullptr;

namespace KVSClientShell {
    std::string address = "localhost";
    uint32_t port = 5555;
    KVStoreClient* _kvstore;

    void init() {
      _kvstore = NOSClient::Instance()->Import<KVStoreClient>("KVStore",
          KVSClientShell::address, KVSClientShell::port);
      if (_kvstore == NULL) {
        std::cout << "could not get remote object" << std::endl;
        std::exit(1);
      }
    }

    void cmd_help(int argc, const char *argv[]) {
      cout << "    help                  Display the list of commands" << endl;
      cout << "    createdb <DB>         Create remote KVStore object" << endl;
      cout << "    removedb <DB>         Remove remote KVStore object" << endl;
      cout << "    put <DB> <KEY> <VAL>  Set a KV pair" << endl;
      cout << "    get <DB> <KEY>        Get a value given the key" << endl;
      cout << "    del <DB> <KEY>        Delete a KV pair" << endl;
      cout << "    list <DB>             List all KV pairs" << endl;
      cout << "    exit                  Exit shell" << endl;
    }

    void cmd_createdb(int argc, const char *argv[]) {
      if (argc != 2) {
        std::cout << "Usage: createdb <DB>" << std::endl; return;
      }

      std::string dbname = argv[1];
      std::cout << _kvstore->createdb(dbname) << std::endl;
    }

    void cmd_removedb(int argc, const char *argv[]) {
      if (argc != 2) {
        std::cout << "Usage: removedb <DB>" << std::endl; return;
      }

      std::string dbname = argv[1];
      std::cout << _kvstore->removedb(dbname) << std::endl;
    }

    void cmd_put(int argc, const char *argv[]) {
      if (argc != 4) {
        std::cout << "Usage: put <DB> <KEY> <VAL>" << std::endl; return;
      }

      std::string dbname = argv[1];
      std::string key = argv[2];
      std::string val = argv[3];

      std::cout << _kvstore->put(dbname, key, val) << std::endl;
    }

    void cmd_get(int argc, const char *argv[]) {
      if (argc != 3) {
        std::cout << "Usage: get <DB> <KEY>" << std::endl; return;
      }
      std::string dbname = argv[1];
      std::string key = argv[2];

      std::cout << _kvstore->get(dbname, key) << std::endl;
    }

    void cmd_del(int argc, const char *argv[]) {
      if (argc != 3) {
        std::cout << "Usage: del <DB> <KEY>" << std::endl; return;
      }
      std::string dbname = argv[1];
      std::string key = argv[2];

      std::cout << _kvstore->del(dbname, key) << std::endl;
    }

    void cmd_list(int argc, const char *argv[]) {
      if (argc != 2) { std::cout << "Usage: list <DB>" << std::endl; return; }
      std::string dbname = argv[1];

      auto result = _kvstore->list(dbname);
      std::cout << "{\n";
      for (const auto &i : result) {
          std::cout << "    " << i.first << ": " << i.second << "\n";
      } std::cout << "}\n";
    }

    void dispatch_command(char *buf) {
        int argc;
        char *argv[DEBUG_MAX_ARGS];
        char *nextArg;

        // parse input
        argv[0] = buf;
        for (argc = 1; argc < DEBUG_MAX_ARGS; argc++) {
            nextArg = strchr(argv[argc - 1], ' ');
            if (nextArg == NULL) { break; }
            *nextArg = '\0';
            argv[argc] = nextArg + 1;
        }

        // execute command
        string cmd = argv[0];
        if (cmd == "help") {
            cmd_help(argc, (const char **)argv);
        } else if (cmd == "exit") {
            std::cout << "Good bye!\n\n";
            exit(0);
        } else if (cmd == "createdb") {
            cmd_createdb(argc, (const char **)argv);
        } else if (cmd == "removedb") {
            cmd_removedb(argc, (const char **)argv);
        } else if (cmd == "put") {
            cmd_put(argc, (const char **)argv);
        } else if (cmd == "get") {
            cmd_get(argc, (const char **)argv);
        } else if (cmd == "del") {
            cmd_del(argc, (const char **)argv);
        } else if (cmd == "list") {
            cmd_list(argc, (const char **)argv);
        } else if (cmd == "#") {
            // Ignore comments
        } else if (cmd != "") {
            printf("Unknown command '%s'\n", argv[0]);
        }
    }


    void prompt() {
        char buf[DEBUG_MAX_LINE];
        cout << "\nKVStore Client Shell\nType 'help' for list of commands\n\n";
        while (cin.good()) {
            cout << ">> ";
            cin.getline((char *)&buf, DEBUG_MAX_LINE);
            dispatch_command(buf);
        }
    }

    void run_script(const char *file) {
        char buf[DEBUG_MAX_LINE];
        fstream fin {file};
        while (!fin.eof()) {
            fin.getline((char *)&buf, DEBUG_MAX_LINE);
            dispatch_command(buf);
        }
    }
}



int main(int argc, const char *argv[]) {
    if (argc != 1 && argc != 2) {
        cout << "Usage: " << argv[0] << " [SCRIPT]" << endl;
        exit(1);
    }
    // std::string address = "localhost"; int port = 5555;
    NOSClient::Instance()->initialize(5);

    /*
    KVStoreClient* kvstore = NOSClient::Instance()->Import<KVStoreClient>("KVStore", address, port);
    if (kvstore == NULL) {
        std::cout << "could not get remote object" << std::endl;
        exit(1);
    }

    auto result1 = kvstore->put("foo", "baz");
    std::cout << "kvstore->put(\"foo\", \"baz\"): " << (result1 ? "true" : "false") << std::endl;

    auto result2 = kvstore->get("foo");
    std::cout << "kvstore->get(\"foo\"): " << result2 << std::endl;
    */

    try {
        KVSClientShell::init();
        if (argc == 1) KVSClientShell::prompt();
        else KVSClientShell::run_script(argv[2]);
    } catch(exception &e) {
        cout << e.what() << endl;
        exit(1);
    }
}
