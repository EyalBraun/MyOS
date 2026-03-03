#include "../Headers/FileSystem.h"

using namespace std;

// Constructor for the File object
File::File(string n, bool d, File *p) 
    : name(n), isDir(d), isFav(false), parent(p) {}

// OrbitManager implementation
OrbitManager::OrbitManager() {
    // We create the ROOT here. Since it has no parent, we pass nullptr.
    // We set isFav to true so the root can never be deleted.
    current = new File("/", true, nullptr);
    current->isFav = true; 
}

OrbitManager::~OrbitManager() {
    // To delete the WHOLE tree, we climb to the top and delete the root.
    File* temp = current;
    while (temp->parent != nullptr) {
        temp = temp->parent;
    }
    delete temp; // This triggers the unique_ptr chain reaction!
}
