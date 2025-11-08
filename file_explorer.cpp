#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;
using namespace std;

void listFiles(const fs::path &path) {
    cout << "\nFiles in Directory: " << path << "\n";
    try {
        for (const auto &entry : fs::directory_iterator(path)) {
            cout << (entry.is_directory() ? "[DIR] " : "[FILE] ")
                 << entry.path().filename().string() << "\n";
        }
    } catch (const fs::filesystem_error &e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void changeDirectory(fs::path &currentPath, const string &dirName) {
    fs::path newPath = currentPath / dirName;
    if (fs::exists(newPath) && fs::is_directory(newPath)) {
        currentPath = fs::canonical(newPath);
    } else {
        cout << "Directory not found.\n";
    }
}

void createFile(const fs::path &filePath) {
    ofstream file(filePath);
    if (file) cout << "File created: " << filePath.filename().string() << "\n";
    else cout << "Failed to create file.\n";
}

void createDirectory(const fs::path &dirPath) {
    if (fs::create_directory(dirPath))
        cout << "Directory created: " << dirPath.filename().string() << "\n";
    else
        cout << "Failed to create directory.\n";
}

void deleteItem(const fs::path &path) {
    try {
        if (fs::is_directory(path)) fs::remove_all(path);
        else if (fs::is_regular_file(path)) fs::remove(path);
        else cout << "Path not found.\n";
        cout << "Deleted.\n";
    } catch (const fs::filesystem_error &e) {
        cerr << "Error deleting: " << e.what() << endl;
    }
}

void copyItem(const fs::path &source, const fs::path &destination) {
    try {
        if (fs::is_directory(source))
            fs::copy(source, destination, fs::copy_options::recursive);
        else
            fs::copy_file(source, destination, fs::copy_options::overwrite_existing);
        cout << "Copied successfully.\n";
    } catch (const fs::filesystem_error &e) {
        cerr << "Error copying: " << e.what() << endl;
    }
}

void moveItem(const fs::path &source, const fs::path &destination) {
    try {
        fs::rename(source, destination);
        cout << "Moved successfully.\n";
    } catch (const fs::filesystem_error &e) {
        cerr << "Error moving: " << e.what() << endl;
    }
}

void searchFile(const fs::path &path, const string &fileName) {
    try {
        for (const auto &entry : fs::recursive_directory_iterator(path)) {
            if (entry.path().filename() == fileName)
                cout << "Found: " << entry.path() << "\n";
        }
    } catch (const fs::filesystem_error &e) {
        cerr << "Error searching: " << e.what() << endl;
    }
}

void showPermissions(const fs::path &path) {
    auto perms = fs::status(path).permissions();
    cout << "Permissions for " << path.filename().string() << ": ";
    cout << ((perms & fs::perms::owner_read) != fs::perms::none ? "r" : "-");
    cout << ((perms & fs::perms::owner_write) != fs::perms::none ? "w" : "-");
    cout << ((perms & fs::perms::owner_exec) != fs::perms::none ? "x" : "-");
    cout << ((perms & fs::perms::group_read) != fs::perms::none ? "r" : "-");
    cout << ((perms & fs::perms::group_write) != fs::perms::none ? "w" : "-");
    cout << ((perms & fs::perms::group_exec) != fs::perms::none ? "x" : "-");
    cout << ((perms & fs::perms::others_read) != fs::perms::none ? "r" : "-");
    cout << ((perms & fs::perms::others_write) != fs::perms::none ? "w" : "-");
    cout << ((perms & fs::perms::others_exec) != fs::perms::none ? "x" : "-");
    cout << "\n";
}

void setPermissions(const fs::path &path, fs::perms permissions) {
    fs::permissions(path, permissions);
    cout << "Permissions updated.\n";
}

int main() {
    fs::path currentPath = fs::current_path();
    int choice;

    while (true) {
        cout << "\nCurrent Directory: " << currentPath << "\n";
        cout << "1. List files\n2. Change directory\n3. Create file\n4. Create directory\n";
        cout << "5. Delete file/directory\n6. Copy file/directory\n7. Move file/directory\n";
        cout << "8. Search file\n9. Show permissions\n10. Set permissions\n0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 0) break;

        string name, dest;
        switch (choice) {
            case 1: listFiles(currentPath); break;
            case 2: cout << "Enter directory name: "; getline(cin, name); changeDirectory(currentPath, name); break;
            case 3: cout << "Enter file name: "; getline(cin, name); createFile(currentPath / name); break;
            case 4: cout << "Enter directory name: "; getline(cin, name); createDirectory(currentPath / name); break;
            case 5: cout << "Enter file/directory name: "; getline(cin, name); deleteItem(currentPath / name); break;
            case 6: cout << "Enter source name: "; getline(cin, name); cout << "Enter destination name: "; getline(cin, dest); copyItem(currentPath / name, currentPath / dest); break;
            case 7: cout << "Enter source name: "; getline(cin, name); cout << "Enter destination name: "; getline(cin, dest); moveItem(currentPath / name, currentPath / dest); break;
            case 8: cout << "Enter file name to search: "; getline(cin, name); searchFile(currentPath, name); break;
            case 9: cout << "Enter file/directory name: "; getline(cin, name); showPermissions(currentPath / name); break;
            case 10: cout << "Enter file/directory name: "; getline(cin, name); cout << "Enter permission mode (e.g., 644): "; int perm; cin >> oct >> perm; setPermissions(currentPath / name, static_cast<fs::perms>(perm)); cin.ignore(); break;
            default: cout << "Invalid choice.\n";
        }
    }
    cout << "Exiting File Explorer...\n";
    return 0;
}

