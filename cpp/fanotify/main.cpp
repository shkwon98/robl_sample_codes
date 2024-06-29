#include <iostream>
#include <fstream>
#include <filesystem>
#include <sys/inotify.h>
#include <unistd.h>

using namespace std::chrono_literals;
namespace fs = std::filesystem;

#define DEVICE_DIR_PATH                 "/dev"
#define STORAGE_DEV_NAME                "mmcblk1p1"
#define STORAGE_MNT_POINT               "/mnt/uSD"
#define MAX_EVENTS  1024  /* Maximum number of events to process*/
#define LEN_NAME    16    /* Assuming that the length of the filename won't exceed 16 bytes*/
#define EVENT_SIZE  ( sizeof(inotify_event) ) /*size of one event*/
#define BUF_LEN     ( MAX_EVENTS * ( EVENT_SIZE + LEN_NAME ))

void mountStorage(const std::string &devicePoint, const std::string &mountPoint)
{
    // Mount the storage
    auto mountCommand = "mount " + devicePoint + " " + mountPoint;
    system(mountCommand.c_str());
    std::cout << "External Storage mounted on " << mountPoint << std::endl;
}

void unmountStorage(const std::string &mountPoint)
{
    // Unmount the storage
    auto unmountCommand = "umount " + mountPoint;
    system(unmountCommand.c_str());
    std::cout << "External Storage unmounted from " << mountPoint << std::endl;
}

int main(void)
{
    char eventBuffer[BUF_LEN];
    int inotifyDescriptor, watchDescriptor;
    if ((inotifyDescriptor = inotify_init()) < 0)
    {
        std::cout << "Failed to initialize inotify" << std::endl;
        return -1;
    }
    if ((watchDescriptor = inotify_add_watch(inotifyDescriptor, "/dev", IN_CREATE | IN_DELETE)) < 0)
    {
        std::cout << "Failed to add watch" << std::endl;
        close(inotifyDescriptor);
        return -1;
    }

    // If the storage is already inserted, mount it immediately
    fs::path dev(DEVICE_DIR_PATH);
    if (std::filesystem::exists(dev / STORAGE_DEV_NAME))
    {
        mountStorage(dev / STORAGE_DEV_NAME, STORAGE_MNT_POINT);
    }

    while (true)
    {
        auto bytesRead = read(inotifyDescriptor, eventBuffer, BUF_LEN);
        if (bytesRead < 0)
        {
            std::cout << "Failed to read event" << std::endl;
            break;
        }

        ssize_t i = 0;
        while (i < bytesRead)
        {
            auto *event = reinterpret_cast<inotify_event*>(&eventBuffer[i]);
            if (event->len)
            {
                if ((event->mask & IN_CREATE) && (std::string(event->name) == STORAGE_DEV_NAME))
                {
                    mountStorage(dev / STORAGE_DEV_NAME, STORAGE_MNT_POINT);
                }
                else if ((event->mask & IN_DELETE) && (std::string(event->name) == STORAGE_DEV_NAME))
                {
                    unmountStorage(STORAGE_MNT_POINT);
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }

    inotify_rm_watch(inotifyDescriptor, watchDescriptor);
    close(inotifyDescriptor);
}