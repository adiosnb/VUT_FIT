#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "ServerFunctions.h"

void server_functions::delete_file(std::string file){
    struct stat file_stat;
    stat(file.c_str(), &file_stat);

    if (S_ISDIR(file_stat.st_mode)){
        throw "Not a file.";
    }

    if ((std::remove(file.c_str())) < 0){
        if (errno == ENOTDIR) throw "Not a directory.";
        else if (errno == ENOENT) throw "File not found.";

        else throw "Not a file.";

    }
}

std::vector<std::string> server_functions::ls_files(std::string path){
    std::vector<std::string> files;

    DIR *dp;
    struct dirent *dirp;

    if((dp  = opendir(path.c_str())) == NULL) {
        if (errno ==ENOTDIR) throw "Not a directory.";
        else throw  "Directory not found.";
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(std::string(dirp->d_name));
    }
    closedir(dp);

    return files;
}

int server_functions::make_dir(std::string file) {
    int ret_code;
    ret_code = mkdir(file.c_str(), S_IRUSR | S_IWUSR | S_IEXEC | S_IXGRP | S_IRGRP | S_IWGRP);

    if (ret_code < 0){
        if (errno == EEXIST) throw "Already exists.";
        else if( errno == ENOENT) throw "Directory not found.";
        else throw "Unknown error.";
    }
    return 0;
}

int server_functions::remove_dir(std::string file){
    if ((rmdir(file.c_str())) < 0){

        if (errno == ENOTDIR) throw "Not a directory.";
        else if (errno == ENOTEMPTY) throw "Directory not empty.";
        else if (errno == ENOENT) throw "Directory not found.";
        else throw "Unknown error.";
    }
}