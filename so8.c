#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct
{
    char signature[2];
    int file_size;
    int reserved;
    int data_offset;
} Antet;

typedef struct
{
    int size;
    int latime;
    int lungime;
    int x_pixel;
    int y_pixel;
    int user_id;
    int date;
    char drept_user;
} Info_antet;

int main(int argc, char *argv[])
{
    if (argc == 3)
    {
        DIR *inputDir = opendir(argv[1]);
        DIR *outputDir = opendir(argv[2]);

        if (inputDir == NULL || outputDir == NULL)
        {
            perror("Error opening directory");
            exit(EXIT_FAILURE);
        }

        struct dirent *entry;

        while ((entry = readdir(inputDir)) != NULL)
        {
            if (entry->d_type == DT_REG && strstr(entry->d_name, ".bmp") != NULL)
            {
                char inputFilePath[256];
                char outputFilePath[256];
                printf("%s", inputFilePath);

                snprintf(inputFilePath, sizeof(inputFilePath), "%s/%s", argv[1], entry->d_name);
                snprintf(outputFilePath, sizeof(outputFilePath), "%s/statistica_%s.txt", argv[2], entry->d_name);

                int outputFile = creat(outputFilePath, S_IRUSR | S_IWUSR);
                if (outputFile == -1)
                {
                    perror("Error creating output file");
                    exit(EXIT_FAILURE);
                }

                int pid = fork();
                if (pid < 0)
                {
                    perror("Error forking");
                    exit(EXIT_FAILURE);
                }

                if (pid == 0)
                {
                    // Child Process
                    int rez = open(inputFilePath, O_RDWR);

                    if (rez == -1)
                    {
                        perror("Error opening file");
                        exit(EXIT_FAILURE);
                    }

                    Antet antet;
                    Info_antet info;

                    int rez2 = read(rez, &antet, sizeof(Antet));
                    if (rez2 == -1)
                    {
                        perror("Error reading");
                        exit(EXIT_FAILURE);
                    }

                    rez2 = read(rez, &info, sizeof(Info_antet));
                    if (rez2 == -1)
                    {
                        perror("Error reading");
                        exit(EXIT_FAILURE);
                    }

                    char buff[1000];

                    sprintf(buff, "\n\nFile: %s\n", inputFilePath);
                    write(outputFile, buff, strlen(buff));

                    sprintf(buff, "Size: %d bytes\n", antet.file_size);
                    write(outputFile, buff, strlen(buff));

                    sprintf(buff, "Width: %d\n", info.latime);
                    write(outputFile, buff, strlen(buff));

                    sprintf(buff, "Height: %d\n", info.lungime);
                    write(outputFile, buff, strlen(buff));

                    sprintf(buff, "X Pixel: %d\n", info.x_pixel);
                    write(outputFile, buff, strlen(buff));

                    sprintf(buff, "Y Pixel: %d\n", info.y_pixel);
                    write(outputFile, buff, strlen(buff));

                    sprintf(buff, "User ID: %d\n", info.user_id);
                    write(outputFile, buff, strlen(buff));

                    sprintf(buff, "Date: %d\n", info.date);
                    write(outputFile, buff, strlen(buff));

                    sprintf(buff, "Drept User: %c\n", info.drept_user);
                    write(outputFile, buff, strlen(buff));

                    close(rez);
                    close(outputFile);

                    // Send the number of lines written to the parent
                    int linesWritten = 0;
                    FILE *outputFilePtr = fopen(outputFilePath, "r");
                    while (fgets(buff, sizeof(buff), outputFilePtr) != NULL)
                    {
                        linesWritten++;
                    }
                    fclose(outputFilePtr);

                    write(STDOUT_FILENO, &linesWritten, sizeof(int));

                    exit(EXIT_SUCCESS);
                }
                else
                {
                    // Parent Process
                    int status;
                    waitpid(pid, &status, 0);

                    if (WIFEXITED(status))
                    {
                        int linesWritten;
                        read(STDIN_FILENO, &linesWritten, sizeof(int));
                        printf("Child process %d wrote %d lines.\n", pid, linesWritten);
                    }
                }
            }
        }

        closedir(inputDir);
        closedir(outputDir);
    }
    else
    {
        printf("Usage: %s <input_directory> <output_directory>\n", argv[0]);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    // ...

    if (pid == 0)
    {
        // Child Process
        // ...

        // Send the number of lines written to the parent
        int linesWritten = 0;
        FILE *outputFilePtr = fopen(outputFilePath, "r");
        while (fgets(buff, sizeof(buff), outputFilePtr) != NULL)
        {
            linesWritten++;
        }
        fclose(outputFilePtr);

        write(STDOUT_FILENO, &linesWritten, sizeof(int));

        exit(EXIT_SUCCESS);
    }
    else
    {
        // Parent Process
        int status;
        waitpid(pid, &status, 0);

        // Check if the child process terminated
        if (WIFEXITED(status))
        {
            int linesWritten;
            read(STDIN_FILENO, &linesWritten, sizeof(int));
            printf("Child process %d wrote %d lines.\n", pid, linesWritten);
        }
        else
        {
            printf("Child process %d did not terminate successfully.\n", pid);
        }
    }

    // ...
}






