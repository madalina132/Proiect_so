#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

// Structura pentru antetul imaginii BMP
typedef struct {
    char signature[2];
    int file_size;
    int reserved;
    int data_offset;
} Antet;

// Structura pentru informațiile despre imagine
typedef struct {
    int size;
    int latime;
    int lungime;
    int x_pixel;
    int y_pixel;
    int user_id;
    int date;
    char drept_user;
} Info_antet;

// Funcție pentru procesarea conținutului unui fișier regulat
void processRegularFile(char *inputFilePath, char *outputFilePath, char character) {
    int pid = fork();
    if (pid < 0) {
        perror("Error forking");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Proces fiu

        // Deschide fișierul de intrare
        int inputFile = open(inputFilePath, O_RDONLY);
        if (inputFile == -1) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        // Citeste caracter cu caracter din fișier și afișează conținutul
        char ch;
        char outputContent[1000];
        sprintf(outputContent, "Content of the regular file '%s':\n", inputFilePath);

        while (read(inputFile, &ch, 1) > 0) {
            sprintf(outputContent + strlen(outputContent), "%c", ch);
        }

        // Afișează conținutul
        printf("%s", outputContent);

        // Închide fișierul de intrare
        close(inputFile);
        //printf("----------------");
        // Creează un pipe pentru comunicarea cu procesul părinte
        int parentPipe[2];
        if (pipe(parentPipe) == -1) {
            perror("Pipe failed");
            exit(EXIT_FAILURE);
        }

        // Scrie conținutul în pipe
        write(parentPipe[1], outputContent, strlen(outputContent));
        close(parentPipe[1]);

        // Închide procesul fiu
        exit(EXIT_SUCCESS);
    }
}

// Funcție pentru procesarea statisticilor unui fișier regulat
void processRegularFileStats(char *outputFilePath, char character) {
    int pid = fork();
    if (pid < 0) {
        perror("Error forking");
        exit(EXIT_FAILURE);
    }
      //printf("----------------");
    if (pid == 0) {
        // Proces fiu

        // Creează un pipe pentru comunicarea cu procesele copil ale sale
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("Pipe failed");
            exit(EXIT_FAILURE);
        }

        // Creează un nou proces pentru execuția script-ului shell
        int pid3 = fork();
        if (pid3 < 0) {
            perror("Error forking");
            exit(EXIT_FAILURE);
        }
        //printf("----------------");
        if (pid3 == 0) {
            // Procesul copil executa script-ul shell
            close(pipefd[0]);

            // Redirectează ieșirea standard către pipe
            dup2(pipefd[1], STDOUT_FILENO);
         // printf("----------------------------");
            // Execută script-ul shell cu argumentul c
           execlp("/home/madalina/Desktop/SO/f.sh", "/home/madalina/Desktop/SO/f.sh",NULL);
          //execlp("./f.sh", "./f.sh",NULL);

            // În caz de eroare la execuția script-ului
            perror("Error executing script");
            exit(EXIT_FAILURE);
        } else {
            // Procesul părinte
            close(pipefd[1]);

            // Așteaptă ca procesul copil să termine execuția script-ului
            waitpid(pid3, NULL, 0);

            // Citește rezultatul din pipe (numărul de propoziții corecte)
            int numberOfCorrectSentences;
            read(pipefd[0], &numberOfCorrectSentences, sizeof(int));
            close(pipefd[0]);

            // Deschide fișierul de ieșire pentru statisticile procesate
            int outputFile = open(outputFilePath, O_WRONLY | O_APPEND);

            // Scrie rezultatul în fișierul de ieșire
            char buff[1000];
            sprintf(buff, "Number of correct sentences with character '%c': %d\n", character, numberOfCorrectSentences);
            write(outputFile, buff, strlen(buff));

            // Închide fișierul de ieșire
            close(outputFile);

            // Afișează mesajul de final
            printf("Processed file statistics: %s\n", outputFilePath);

            // Închide procesul fiu
            exit(EXIT_SUCCESS);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc == 4) {
        // Deschide directoarele de intrare și ieșire
        DIR *inputDir = opendir(argv[1]);
        DIR *outputDir = opendir(argv[2]);

        if (inputDir == NULL || outputDir == NULL) {
            perror("Error opening directory");
            exit(EXIT_FAILURE);
        }

        // Extrage caracterul din argumente
        char character = argv[3][0];

        struct dirent *entry;

        // Parcurge fiecare intrare în directorul de intrare
        while ((entry = readdir(inputDir)) != NULL) {
            // Verifică dacă intrarea este un fișier regulat
            if (entry->d_type == DT_REG) {
                char inputFilePath[256];
                char outputFilePath[256];

                // Construiește calea către fișierul de intrare și de ieșire
                snprintf(inputFilePath, sizeof(inputFilePath), "%s/%s", argv[1], entry->d_name);
                snprintf(outputFilePath, sizeof(outputFilePath), "%s/statistica_%s.txt", argv[2], entry->d_name);

                // Creează fișierul de ieșire
                int outputFile = creat(outputFilePath, S_IRUSR | S_IWUSR);
                if (outputFile == -1) {
                    perror("Error creating output file");
                    exit(EXIT_FAILURE);
                }

                // Deschide fișierul de intrare
                int inputFile = open(inputFilePath, O_RDWR);

                if (inputFile == -1) {
                    perror("Error opening file");
                    exit(EXIT_FAILURE);
                }

                // Citeste antetul și informațiile despre imagine
                Antet antet;
                Info_antet info;

                int rez2 = read(inputFile, &antet, sizeof(Antet));
                if (rez2 == -1) {
                    perror("Error reading");
                    exit(EXIT_FAILURE);
                }

                rez2 = read(inputFile, &info, sizeof(Info_antet));
                if (rez2 == -1) {
                    perror("Error reading");
                    exit(EXIT_FAILURE);
                }

                char buff[1000];

                // Scrie informații în fișierul de ieșire
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

                // Repozitionează cursorul la începutul fișierului
                lseek(inputFile, 0, SEEK_SET);

                int linesWithCharacter = 0;
                char ch;

                // Numără liniile care conțin caracterul specificat
                while (read(inputFile, &ch, 1) > 0) {
                    if (ch == '\n' || ch == ' ') {
                        linesWithCharacter++;
                    }
                }

                // Scrie statistici în fișierul de ieșire
                sprintf(buff, "Number of lines with character '%c': %d\n", character, linesWithCharacter);
                write(outputFile, buff, strlen(buff));

                // Închide fișierul de intrare și fișierul de ieșire
                close(inputFile);
                close(outputFile);

                // Afișează mesajul de final
                printf("Processed file: %s\n", inputFilePath);

                // Procesează fișierele cu excepția celor cu extensia .bmp
                if (strstr(entry->d_name, ".bmp") == NULL) {
                    processRegularFile(inputFilePath, outputFilePath, character);
                    processRegularFileStats(outputFilePath, character);
                }
            }
        }

        // Închide directoarele de intrare și ieșire
        closedir(inputDir);
        closedir(outputDir);
    }

    return 0;
}
