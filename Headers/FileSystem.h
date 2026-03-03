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
    // Note: 'unique_ptr' handles the memory of all children automatically
    std::vector<std::unique_ptr<File>> children; 

    File(std::string n, bool d, File *p = nullptr);
};

class OrbitManager {
public:
    File *current;

    OrbitManager();  // Constructor will "bootstrap" the root
    ~OrbitManager(); // Destructor will clean up the root
};

#endif
