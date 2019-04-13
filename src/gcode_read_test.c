#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

FILE *fp;

//detect if it raises the nozzle to get rid of extra fillament
int detect_gcode_mode(){
    return EXIT_FAILURE;
}


int insert_steps(){
    return EXIT_FAILURE;
}

int find_jumps(){
    return EXIT_FAILURE;
}




int main(){
    char ch, file_name[250];
    // FILE *fp;
    size_t len = 0;
    ssize_t read;
    size_t maxGroups=5;
    regmatch_t groupArray[maxGroups];
    char *line = NULL;

    // printf("Enter name of a file you wish to see\n");
    // gets(file_name);
    // strcpy(file_name, "2xgearwithlift.gcode");
    strcpy(file_name, "2xgearwithlift.gcode");

    fp = fopen(file_name, "r"); // read mode

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    regex_t regex;
    char *re = "Z([[:digit:]]+\\.[[:digit:]]*)[[:space:]]*"; //Eg. Z10.000 \n
    int reti = regcomp(&regex, re, REG_EXTENDED);
    if(reti){
        fprintf(stderr, "Could not compile regex, error code %d\n", reti);
        goto exit_program;
    }

    // printf("The contents of %s file are:\n", file_name);

    // reti = regexec
    // while ((ch = fgetc(fp)) != EOF)
    //     printf("%c", ch);
    double buttom_number = -1.0;
    while ((read = getline(&line, &len, fp)) != -1){
        // printf("%s", line);
        reti = regexec(&regex, line, maxGroups, groupArray, 0);
        if(!reti){
            // printf("Match!\n");
        
            if(line[0] != ';'){//ignore comments 

                printf("%s", line);
                // printf("%u::%u\n",groupArray[0].rm_so, groupArray[0].rm_eo);
                unsigned int g = 1;
                char sourceCopy[strlen(line) + 1];
                strcpy(sourceCopy, line);
                sourceCopy[groupArray[g].rm_eo] = 0;
                printf("Group %u: [%u - %u]: %s\n",
                       g, groupArray[g].rm_so, groupArray[g].rm_eo,
                       sourceCopy + groupArray[g].rm_so);
                double number = atoi(sourceCopy+groupArray[g].rm_so);
                printf("the number is: %.3f\n", number);
                if(number>=buttom_number){
                    buttom_number = number;
                }else{
                    printf("This file has raiscleanmodestuff\n");
                    break;
                }

                
            }
        }else if(reti == REG_NOMATCH){
            // printf("No match\n");
        }else{
            // regerror(reti, &regex, msgbuf, sizeof(msgbuf));
            // fprintf(stderr, "Regex match failed: %s\n", msgbuf);
            goto exit_program;
        }

    }

exit_program:

    regfree(&regex);
    if(line)
        free(line);
    fclose(fp);
    return EXIT_SUCCESS;
}