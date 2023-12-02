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
    if (argc == 4)
    {
        DIR *inputDir = opendir(argv[1]);
        DIR *outputDir = opendir(argv[2]);

        if (inputDir == NULL || outputDir == NULL)
        {
            perror("Error opening directory");
            exit(EXIT_FAILURE);
        }

        char character = argv[3][0];  // Ia primul caracter din al treilea argument

        struct dirent *entry;

        while ((entry = readdir(inputDir)) != NULL)
        {
            if (entry->d_type == DT_REG)
            {
                char inputFilePath[256];
                char outputFilePath[256];

                snprintf(inputFilePath, sizeof(inputFilePath), "%s/%s", argv[1], entry->d_name);
                snprintf(outputFilePath, sizeof(outputFilePath), "%s/statistica_%s.txt", argv[2], entry->d_name);

                int outputFile = creat(outputFilePath, S_IRUSR | S_IWUSR);
                if (outputFile == -1)
                {
                    perror("Error creating output file");
                    exit(EXIT_FAILURE);
                }

                int inputFile = open(inputFilePath, O_RDWR);

                if (inputFile == -1)
                {
                    perror("Error opening file");
                    exit(EXIT_FAILURE);
                }

                Antet antet;
                Info_antet info;

                int rez2 = read(inputFile, &antet, sizeof(Antet));
                if (rez2 == -1)
                {
                    perror("Error reading");
                    exit(EXIT_FAILURE);
                }

                rez2 = read(inputFile, &info, sizeof(Info_antet));
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

                // Repozitionare la începutul fișierului
                lseek(inputFile, 0, SEEK_SET);

                int linesWithCharacter = 0;
                char ch;

                while (read(inputFile, &ch, 1) > 0)
                {
                    if (ch == '\n' || ch == ' ')
                    {
                        linesWithCharacter++;
                    }
                }

                sprintf(buff, "Number of lines with character '%c': %d\n", character, linesWithCharacter);
                write(outputFile, buff, strlen(buff));

                close(inputFile);
                close(outputFile);

                printf("Processed file: %s\n", inputFilePath);
            }
            else if (entry->d_type == DT_REG && strstr(entry->d_name, ".bmp") == NULL)
            {
                // Fisier obisnuit fara extensia .bmp

                int pid = fork();
                if (pid < 0)
                {
                    perror("Error forking");
                    exit(EXIT_FAILURE);
                }

                if (pid == 0)
                {
                    // Proces fiu pentru generarea continutului fisierului obisnuit

                    char inputFilePath[256];
                    snprintf(inputFilePath, sizeof(inputFilePath), "%s/%s", argv[1], entry->d_name);

                    char outputContent[1000];
                    sprintf(outputContent, "Content of the regular file '%s':\n", inputFilePath);

                    int inputFile = open(inputFilePath, O_RDONLY);
                    if (inputFile == -1)
                    {
                        perror("Error opening file");
                        exit(EXIT_FAILURE);
                    }

                    char ch;
                    while (read(inputFile, &ch, 1) > 0)
                    {
                        sprintf(outputContent + strlen(outputContent), "%c", ch);
                    }

                    printf("%s", outputContent);

                    close(inputFile);
                    exit(EXIT_SUCCESS);
                }
                else
                {
                    // Proces parinte

                    int status;
                    waitpid(pid, &status, 0);

                    if (WIFEXITED(status))
                    {
                        if (entry->d_type == DT_REG && strstr(entry->d_name, ".bmp") == NULL)
                        {
                            // Al doilea proces fiu pentru numarul de propozitii corecte

                            int pid2 = fork();
                            if (pid2 < 0)
                            {
                                perror("Error forking");
                                exit(EXIT_FAILURE);
                            }

                            if (pid2 == 0)
                            {
                                // Proces fiu pentru numarul de propozitii corecte

                                int pipefd[2];
                                if (pipe(pipefd) == -1)
                                {
                                    perror("Pipe failed");
                                    exit(EXIT_FAILURE);
                                }

                                int pid3 = fork();
                                if (pid3 < 0)
                                {
                                    perror("Error forking");
                                    exit(EXIT_FAILURE);
                                }

                                if (pid3 == 0)
                                {
                                    // Al treilea proces fiu pentru script

                                    close(pipefd[0]); // Inchide capul de citire al pipe-ului

                                    dup2(pipefd[1],

