#ifndef SERVER_FUNCTIONS
#define SERVER_FUNCTIONS

#include <iostream>
#include <vector>
/**
 * Server functions are defined in separate namespace.
 */
namespace server_functions {
    /**
     * Try to delete file, else throw and cons char * exception.
     * @param file Path to file which should be deleted.
     */
    void delete_file(std::string file);

    /**
     * List files in directory.
     * @param path Path to directiry which should belisted.
     * @return vector of strings including files in dorecotry.
     */
    std::vector<std::string> ls_files(std::string path);

    /**
     * Making dir in specified file.
     * @param file Destination file
     * @return ) if success.
     */
    int make_dir(std::string file);

    /**
     * Removing dir in specified file.
     * @param file Path to removingfile.
     * @return ) if success.
     */
    int remove_dir(std::string file);
}

#endif //SERVER_FUNCTIONS