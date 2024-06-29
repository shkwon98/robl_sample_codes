#include <archive.h>
#include <archive_entry.h>
#include <cstring>
#include <fstream>
#include <iostream>

void extract(const char *filename)
{
    struct archive *a;
    struct archive *ext;
    struct archive_entry *entry;
    int r;
    size_t total_size = 0;
    size_t extracted_size = 0;

    // Calculate the total size of the archive for progress tracking
    a = archive_read_new();
    archive_read_support_format_tar(a);
    archive_read_support_filter_gzip(a);
    if ((r = archive_read_open_filename(a, filename, 10240)))
    {
        std::cerr << "Failed to open archive: " << archive_error_string(a) << std::endl;
        return;
    }
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK)
    {
        total_size += archive_entry_size(entry);
        archive_read_data_skip(a);
    }
    archive_read_close(a);
    archive_read_free(a);

    // Extract the archive
    a = archive_read_new();
    ext = archive_write_disk_new();
    archive_read_support_format_tar(a);
    archive_read_support_filter_gzip(a);
    if ((r = archive_read_open_filename(a, filename, 10240)))
    {
        std::cerr << "Failed to open archive: " << archive_error_string(a) << std::endl;
        return;
    }

    while (archive_read_next_header(a, &entry) == ARCHIVE_OK)
    {
        std::cout << "Extracting: " << archive_entry_pathname(entry) << std::endl;
        r = archive_write_header(ext, entry);
        if (r != ARCHIVE_OK)
        {
            std::cerr << archive_error_string(ext) << std::endl;
        }
        else
        {
            const void *buff;
            size_t size;
            int64_t offset;

            while (true)
            {
                r = archive_read_data_block(a, &buff, &size, &offset);
                if (r == ARCHIVE_EOF)
                    break;
                if (r != ARCHIVE_OK)
                {
                    std::cerr << "Error reading data: " << archive_error_string(a) << std::endl;
                    return;
                }
                r = archive_write_data_block(ext, buff, size, offset);
                if (r != ARCHIVE_OK)
                {
                    std::cerr << "Error writing data: " << archive_error_string(ext) << std::endl;
                    return;
                }
                extracted_size += size;
                std::cout << "\rProgress: " << (100.0 * extracted_size / total_size) << "%" << std::flush;
            }
        }
        archive_write_finish_entry(ext);
    }

    std::cout << std::endl;

    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(ext);
    archive_write_free(ext);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <tar.gz file>" << std::endl;
        return 1;
    }
    extract(argv[1]);
    return 0;
}