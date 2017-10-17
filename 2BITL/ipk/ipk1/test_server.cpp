#include "ServerFunctions.h"

using namespace std;

int main(){

    try {


        vector<string> files = server_functions::ls_files(".");

        for (int i = 0; i < files.size(); i++) {
            cout << files[i] << endl;
        }

        cout << server_functions::make_dir("test_dir") << " dir created" << endl;

        cout << server_functions::remove_dir("test_dir") << " dir deleted" << endl;

        cout << server_functions::delete_file("test.kk") << " file test.kk deleted" << endl;

    }
    catch (const char *error){
        cout << error << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}