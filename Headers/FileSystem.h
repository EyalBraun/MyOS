#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <vector>
#include <memory>

class File {
public:
    std::string name;
    bool isDir;
    bool isFav;
		bool isExc;
    File *parent;
    std::string Content;
    std::vector<std::unique_ptr<File>> children; 

    File(std::string n, bool d, bool f, bool e, File *p = nullptr, std::string c = "");
};

class OrbitManager {
public:
    File *current;
    OrbitManager();
    ~OrbitManager(); 
};

#endif
