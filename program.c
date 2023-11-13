#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

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
    int pid;
    char intrare[100];
    char out[100];
    /*strcpy(intrare,"imagine");
    strcpy(out,"/statistica.txt");
    strcat(intrare,out);
    printf("%s",intrare);*/
    if (argc == 3)
    {
        
        DIR *dir = opendir(argv[1]); //folosit pentru deschiderea fisierului->opendir deschide directorul dat in linie de comanda

        if (dir == NULL)
        {
            perror("Error opening directory");
            exit(EXIT_FAILURE);
        }

        struct dirent *entry;

        while ((entry = readdir(dir)) != NULL) //se parcurg toate intrarile cu functia readdir
        {
            if (entry->d_type == DT_REG) //se parcurg pentru fiecare structura de intare numele si ripul ->d_reg pentru fisiere regulate 
            {
                char filePath[256];
                printf("%s",filePath);
              sprintf(intrare, "%s/statistica_%s.txt", argv[1], entry->d_name);
            
                int outputFile = creat(intrare, S_IRUSR | S_IWUSR);
                  if (outputFile == -1)
                    {
                          perror("nu s a putut crea directorul");
                         exit(-1);
                         }
                         
                
                snprintf(filePath, sizeof(filePath), "%s/%s", argv[1], entry->d_name);
                pid=fork();
                if(pid <0)
                {
                    perror("eroare");
                    exit(-1);
                }
                if(pid == 0)
                {
                int rez = open(filePath, O_RDWR);

                if (rez == -1)
                {
                    perror("eroare la deschidere");
                    exit(-1);
                }

                Antet antet;
                Info_antet info;

                int rez2 = read(rez, &antet, sizeof(Antet));
                if (rez2 == -1)
                {
                    perror("eroare la citire");
                    exit(-1);
                }

                rez2 = read(rez, &info, sizeof(Info_antet));
                if (rez2 == -1)
                {
                   perror("eroare la citire");
                    exit(-1);
                }

                char buff[1000];

                sprintf(buff, "\n\nFile: %s\n", filePath);
                write(outputFile, buff, strlen(buff));

                sprintf(buff, "Size: %d bytes\n", antet.file_size);
                write(outputFile, buff, strlen(buff));

                sprintf(buff, "Width: %d\n", info.latime);int outputFile = creat(intrare, S_IRUSR | S_IWUSR);
        if (outputFile == -1)
        {
            perror("nu s a putut crea directorul");
            exit(-1);
        }
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
                
            }
            
            }
            
        }

        closedir(dir);
    }
    else
    {
        printf("Usage: %s <input_directory>\n", argv[0]);
    }

    return 0;
}
