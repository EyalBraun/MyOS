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
    File *parent;
    std::string Content;
    // unique_ptr vectors handle the recursive destruction of children automatically
    std::vector<std::unique_ptr<File>> children; 

    File(std::string n, bool d, bool f, File *p = nullptr, std::string c = "");
};

class OrbitManager {
public:
    File *current;

    OrbitManager();
    ~OrbitManager(); // Standard destructor to handle root-less cleanup
};

#endif
