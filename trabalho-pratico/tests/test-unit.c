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

int compare_files(const char *path1, const char *path2) {
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

void find_different_lines(const char *path1, const char *path2) {
    // Open the files for reading
    FILE *file1 = fopen(path1, "r");
    FILE *file2 = fopen(path2, "r");

    // Check if the files were successfully opened
    if (file1 == NULL || file2 == NULL) {
        printf("Error opening files: %s, %s\n", path1, path2);
        return;
    }

    // Allocate a buffer for reading the lines
    char *line1 = NULL;
    char *line2 = NULL;
    size_t len1 = 0;
    size_t len2 = 0;
    ssize_t nread1;
    ssize_t nread2;

    // Read the files line by line and compare the contents
    int line_number = 1;
    while ((nread1 = getline(&line1, &len1, file1)) != -1 && (nread2 = getline(&line2, &len2, file2)) != -1) {
        // Compare the lines
        if (nread1 != nread2 || strcmp(line1, line2) != 0) {
            printf("    %sLine %d:\n", YELLOW, line_number);
            printf("    %s-> %s", GREEN, line1);
            printf("    %s-> %s\n", RED, line2);
        }
        line_number++;
    }

    // Free the buffers
    free(line1);
    free(line2);
    // Close the files
    fclose(file1);
    fclose(file2);
}

int get_query_number(const char *file_path, int line_number) {
    FILE *file = fopen(file_path, "r");

    if (file == NULL) {
        printf("Error opening file: %s\n", file_path);
        abort();
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    int current_line = 1;
    int result = 0;

    while ((nread = getline(&line, &len, file)) != -1) {
        if (current_line == line_number) {
            result = line[0] - '0';
            break;
        }
        current_line++;
    }

    free(line);
    fclose(file);

    return result;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Wrong number of arguments!\n");
        printf("Usage: %s <folder1> <folder2> <input>\n", argv[0]);
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

    // Check if the directories have the same number of files
    struct dirent *entry1;
    struct dirent *entry2;

    int file_count1 = 0, file_count2 = 0;

    while ((entry1 = readdir(dir1)) != NULL) file_count1++;
    while ((entry2 = readdir(dir2)) != NULL) file_count2++;

    if (file_count1 != file_count2) {
        printf("\n%s%s and %s have a different number of files%s\n", YELLOW, argv[1], argv[2], NO_COLOR);
        return 1;
    }

    // Reset directories
    rewinddir(dir1);
    rewinddir(dir2);

    // Keep track of the total number of tests, passed tests, and failed tests
    int total_tests = 0, passed_tests = 0, failed_tests = 0;

    // Start the timer
    printf("\n%sRunning tests ...%s\n\n", YELLOW, NO_COLOR);
    clock_t start = clock();

    // Read the entries in each directory
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
            int line_number;
            sscanf(entry2->d_name, "command%d_output.txt", &line_number);
            int query = get_query_number(argv[3], line_number);
            printf("%s[FAILED] %s | Q%d\n", RED, entry2->d_name, query);
            find_different_lines(path1, path2);
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