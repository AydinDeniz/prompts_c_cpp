#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <filesystem>
#include <unordered_map>
#include <functional>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/inotify.h>
#include <unistd.h>
#endif

class FileSystemWatcher {
public:
    enum class Action {
        Added,
        Removed,
        Modified,
        Renamed
    };

    using Callback = std::function<void(const std::string&, Action)>;

    FileSystemWatcher(const std::string& pathToWatch, Callback callback)
        : pathToWatch_(pathToWatch), callback_(callback), running_(true) {
        watcherThread_ = std::thread([this]() { watch(); });
    }

    ~FileSystemWatcher() {
        running_ = false;
        if (watcherThread_.joinable()) {
            watcherThread_.join();
        }
    }

private:
    std::string pathToWatch_;
    Callback callback_;
    bool running_;
    std::thread watcherThread_;

    void watch() {
#ifdef _WIN32
        HANDLE hDir = CreateFile(
            pathToWatch_.c_str(),
            FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS,
            NULL);

        if (hDir == INVALID_HANDLE_VALUE) {
            std::cerr << "Failed to create directory handle\n";
            return;
        }

        char buffer[1024];
        DWORD bytesReturned;

        while (running_) {
            if (ReadDirectoryChangesW(
                    hDir,
                    &buffer,
                    sizeof(buffer),
                    TRUE,
                    FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE,
                    &bytesReturned,
                    NULL,
                    NULL)) {

                FILE_NOTIFY_INFORMATION* fni = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer);
                do {
                    std::wstring fileName(fni->FileName, fni->FileNameLength / sizeof(WCHAR));
                    std::string actionPath(fileName.begin(), fileName.end());

                    Action action;
                    switch (fni->Action) {
                    case FILE_ACTION_ADDED:
                        action = Action::Added;
                        break;
                    case FILE_ACTION_REMOVED:
                        action = Action::Removed;
                        break;
                    case FILE_ACTION_MODIFIED:
                        action = Action::Modified;
                        break;
                    case FILE_ACTION_RENAMED_OLD_NAME:
                    case FILE_ACTION_RENAMED_NEW_NAME:
                        action = Action::Renamed;
                        break;
                    default:
                        continue;
                    }

                    callback_(actionPath, action);

                    fni = fni->NextEntryOffset ? reinterpret_cast<FILE_NOTIFY_INFORMATION*>(
                                                     (LPBYTE)fni + fni->NextEntryOffset)
                                               : nullptr;
                } while (fni);
            }
        }

        CloseHandle(hDir);
#else
        int fd = inotify_init();
        if (fd < 0) {
            std::cerr << "Failed to initialize inotify\n";
            return;
        }

        int wd = inotify_add_watch(fd, pathToWatch_.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE);
        if (wd < 0) {
            std::cerr << "Failed to add watch\n";
            return;
        }

        char buffer[1024];
        while (running_) {
            int length = read(fd, buffer, sizeof(buffer));
            if (length < 0) break;

            int i = 0;
            while (i < length) {
                struct inotify_event* event = (struct inotify_event*)&buffer[i];

                std::string fileName(event->name);
                Action action = Action::Modified;
                if (event->mask & IN_CREATE) action = Action::Added;
                if (event->mask & IN_DELETE) action = Action::Removed;
                if (event->mask & IN_MODIFY) action = Action::Modified;

                callback_(fileName, action);
                
                i += sizeof(struct inotify_event) + event->len;
            }
        }

        inotify_rm_watch(fd, wd);
        close(fd);
#endif
    }
};

int main() {
    FileSystemWatcher watcher(
        ".",
        [](const std::string& path, FileSystemWatcher::Action action) {
            std::string actionStr;
            switch (action) {
                case FileSystemWatcher::Action::Added: actionStr = "Added"; break;
                case FileSystemWatcher::Action::Removed: actionStr = "Removed"; break;
                case FileSystemWatcher::Action::Modified: actionStr = "Modified"; break;
                case FileSystemWatcher::Action::Renamed: actionStr = "Renamed"; break;
            }
            std::cout << "File " << path << " " << actionStr << '\n';
        });

    std::cout << "Press Enter to exit...\n";
    std::cin.get();
    return 0;
}