#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

/*
- On some systems, the <dirent.h> header does not provide a complete declaration of the opendir and closedir functions.
- Including <unistd.h> in addition to <dirent.h> may be necessary to access the complete declarations of these functions.
- In general, it is a good practice to include both <dirent.h> and <unistd.h> when working with directories in C,
as this ensures that you have access to the complete declarations of these functions and any related constants.
*/

// This is the dark version of each color (dark_yellow, dark_red, dark_green) and UTF-8 code for light cross and check marks 
#define YELLOW "\033[0;33m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define NO_COLOR "\033[m"
#define CROSS_MARK "\xE2\x9C\x97"
#define CHECK_MARK "\xE2\x9C\x93"

#define CHUNK_SIZE 512 * 1024 // Read the files in chunks of at most 512KB at a time

int compare_files(char *path1, char *path2) {
    // Get the size of each file
    struct stat st1;
    struct stat st2;
    int file_size1;
    int file_size2;

    file_size1 = stat(path1, &st1) == 0 ? st1.st_size : 0;
    file_size2 = stat(path2, &st2) == 0 ? st2.st_size : 0;

    // Check if the files have the same size
    if (file_size1 != file_size2)
        return 1;

    // Open the files to read in binary mode
    FILE *file1 = fopen(path1, "rb");
    FILE *file2 = fopen(path2, "rb");

    // Check if the files were successfully opened
    if (file1 == NULL || file2 == NULL) {
        printf("Error opening files: %s, %s\n", path1, path2);
        return -1;
    }

    // Allocate a buffer for reading the files in chunks
    char *buffer1 = malloc(CHUNK_SIZE * sizeof(char));
    char *buffer2 = malloc(CHUNK_SIZE * sizeof(char));

    // Check if the buffers were successfully allocated
    if (buffer1 == NULL || buffer2 == NULL) {
        printf("Error allocating buffers\n");
        return -1;
    }

    // Read the files in chunks and compare the contents
    int result = 0;
    size_t bytes_read1 = 0, bytes_read2 = 0;
    do {
        // Read a chunk from each file
        bytes_read1 = fread(buffer1, 1, CHUNK_SIZE, file1);
        bytes_read2 = fread(buffer2, 1, CHUNK_SIZE, file2);

        // Compare the chunks
        if (bytes_read1 != bytes_read2 || memcmp(buffer1, buffer2, bytes_read1) != 0) {
            result = 1;
            break;
        }
    } while (bytes_read1 == CHUNK_SIZE && bytes_read2 == CHUNK_SIZE);

    // Free the buffers
    free(buffer1);
    free(buffer2);
    // Close the files
    fclose(file1);
    fclose(file2);

    return result;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Wrong number of arguments!\n");
        printf("Usage: %s <folder1> <folder2>\n", argv[0]);
        return 1;
    }

    // Open the directories
    DIR *dir1 = opendir(argv[1]);
    DIR *dir2 = opendir(argv[2]);

    // Check if the directories were successfully opened
    if (dir1 == NULL || dir2 == NULL) {
        printf("Error opening directories: %s, %s\n", argv[1], argv[2]);
        return 1;
    }

    // Keep track of the total number of tests, passed tests, and failed tests
    int total_tests = 0, passed_tests = 0, failed_tests = 0;

    // Read the entries in each directory
    struct dirent *entry1;
    struct dirent *entry2;

    // Start the timer
    printf("\n%sRunning tests ...%s\n\n", YELLOW, NO_COLOR);
    clock_t start = clock();

    while ((entry1 = readdir(dir1)) != NULL && (entry2 = readdir(dir2)) != NULL) {
        // Skip hidden files and directories ("." and "..")
        if (entry1->d_name[0] == '.' || entry2->d_name[0] == '.') {
            continue;
        }

        // Build the full paths to the files
        char path1[1024];
        char path2[1024];
        snprintf(path1, sizeof(path1), "%s/%s", argv[1], entry1->d_name);
        snprintf(path2, sizeof(path2), "%s/%s", argv[2], entry2->d_name);

        // Compare the files
        int result = compare_files(path1, path2);
        if (result == 0) {
            passed_tests++;
        } else {
            printf("%s[FAILED] %s\n", RED, entry2->d_name);
            failed_tests++;
        }
        total_tests++;
    }

    if (failed_tests == 0)
        printf("%sAll tests passed!%s\n", GREEN, NO_COLOR);

    // Stop the timer
    clock_t end = clock();

    // Close the directories
    closedir(dir1);
    closedir(dir2);

    // Calculate the approximate test execution time (seconds)
    double time = (double)(end - start) / CLOCKS_PER_SEC;

    // Print the results
    printf("\n%sFinished %d tests in %.2f seconds: ", YELLOW, total_tests, time);
    printf("%s(%s) %d passes%s ", GREEN, CHECK_MARK, passed_tests, NO_COLOR);
    printf("%s(%s) %d failures%s\n\n", RED, CROSS_MARK, failed_tests, NO_COLOR);

    return 0;
}