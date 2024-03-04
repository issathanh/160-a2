#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#define MAX_LINE_LENGTH 500 
struct Province {
    char * provName; 
    char * prov;  
}; 

struct Industry 
{
    int code; 
    char *indName; 
};

struct Income
{
    struct Province *prov; 
    int year;
    int income; 
    struct Industry *industry; 
    
};
int count_line( const char *filename);
void make_array_province(int num_lines, const char*filename, struct Province *provinceArr);
void make_array_income(int num_lines, const char*filename, struct Income *incomeARrr);
void freeProvinceArr(struct Province *provinceArr, int num_lines);
void freeIncomeArr(struct Income *incomeArr, int num_lines);


int main (void)
{
    //count lines for income
    const  char *file_q1_income = "q1_income.txt"; 
    int num_lines_income = count_line(file_q1_income)-1; 
    
    //count line for province
    const  char *file_q1_province = "q1_province.txt"; 
    int num_lines_province = count_line(file_q1_province) -1; 
    
    //count lines industry
    const char *file_q1_industry = "q1_industry.txt";
    int num_lines_industry = count_line(file_q1_industry)-1; 

    struct Province *provinceArr = malloc(num_lines_province * sizeof(struct Province)); 
    struct Income *incomeArr = malloc(num_lines_income * sizeof(struct Income)); 
    // struct array for province
    make_array_province(num_lines_province,file_q1_province,provinceArr); 
    make_array_income(num_lines_income,file_q1_income,incomeArr); 
    for (int i =0; i < num_lines_province; i++)
    {
        printf("province number %d: %s\n",i,incomeArr[i].prov); 
    }
    freeProvinceArr(provinceArr,num_lines_province); 
    freeIncomeArr(incomeArr,num_lines_income); 
}
void make_array_income(int num_lines, const char*filename, struct Income *incomeArr)
{
    FILE *file = fopen(filename, "r");
    char line[MAX_LINE_LENGTH]; 
    int dataIndex = 0; 

    if (file == NULL)
    {
        perror("Error opening file"); 
    }
    //skip first line and check for NULL 
    if (fgets(line,sizeof(line),file)==NULL)
    {
        perror("File error"); 
    }

    while(fgets(line,sizeof(line),file) != NULL)
    {
        char *token = strtok(line,","); 
        int tokenIndex = 0; 
        while(token!=NULL && dataIndex <= num_lines)
        {
            switch(tokenIndex)
            {
                case 0:

                    incomeArr[dataIndex].prov = malloc(strlen(token) + 1); 
                    strcpy(incomeArr[dataIndex].prov,token);  
                    break; 
                
                case 1:
                    char *end; 
                    //end point to the first char of the string that wasnt a number 
                    incomeArr[dataIndex].year = strtol(token,&end,10);
                    if(end==token)
                    {
                        perror(printf("no year were found at line %d", dataIndex+2)); 
                    }
                    if(*end != '\0')
                    {
                        perror(printf("further character exist after year at line: %d", dataIndex+2)); 
                    }
                    break; 
                case 3:
                    char *end; 
                    //end point to the first char of the string that wasnt a number 
                    incomeArr[dataIndex].income = strtol(token,&end,10);
                    if(end==token)
                    {
                        perror(printf("no income were found at line %d", dataIndex+2)); 
                    }
                    if(*end != '\0')
                    {
                        perror(printf("further character exist after income at line: %d", dataIndex+2)); 
                    }
                    break; 
                case 4: 
                    char *end; 
                    //end point to the first char of the string that wasnt a number 
                    incomeArr[dataIndex].industry = strtol(token,&end,10);
                    if(end==token)
                    {
                        perror(printf("no industry code were found at line %d", dataIndex+2)); 
                    }
                    if(*end != '\0')
                    {
                        perror(printf("further character exist after industry code at line: %d", dataIndex+2)); 
                    }
                    break; 
            }   
            tokenIndex++; 
            token=strtok(NULL,","); 
        }
        dataIndex++; 
    }
    fclose(file); 

}
void make_array_province(int num_lines, const char*filename, struct Province *provinceArr)
{
    FILE *file = fopen(filename, "r"); 
    char line[MAX_LINE_LENGTH]; 
    int dataIndex = 0; 

    if(file == NULL)
    {
        perror("Error opening file");
    }


    //skip first line and also check if its null
    if(fgets(line,sizeof(line),file) == NULL)
    {
        perror("File error");
    }
    //put every line in file to line
    while(fgets(line,sizeof(line),file) != NULL)
    {
        //get the first token before comma
        char *token = strtok (line, ","); 
        int tokenIndex = 0; 
        while(token != NULL && dataIndex <= num_lines)
        {
            switch(tokenIndex)
            {
                case 0: //first part of token will be prov name
                    provinceArr[dataIndex].provName = malloc(strlen(token) + 1); 
                    strcpy(provinceArr[dataIndex].provName,token); 
                    break; 


                case 1: //2nd part of token will be prov
                    provinceArr[dataIndex].prov = malloc(strlen(token)+1);
                    strcpy(provinceArr[dataIndex].prov,token); 
                    break; 
            }

            
            tokenIndex++; 
            token = strtok(NULL,","); 
        }
        dataIndex++; 
    }
    fclose(file); 
}



int count_line( const char *filename)
{
    //open file for reading
    FILE *file = fopen(filename,"r");
    if (file == NULL)
    {
        perror("Unable to open the file");
        return -1; 
    }

    int lines = 0; 
    char buffer[MAX_LINE_LENGTH]; //Buffer to hold each lines

    while(fgets(buffer,sizeof(buffer),file) != NULL)
    {
        lines++; 
    }
    fclose(file); 
    return lines; 
}

void freeProvinceArr(struct Province *provinceArr, int num_lines)
{
    for (int i = 0; i < num_lines; i++)
    {
        if(provinceArr[i].prov != NULL)
        {
            free(provinceArr[i].prov); 
            provinceArr[i].prov = NULL; 
        }
        if(provinceArr[i].provName != NULL)
        {
            free(provinceArr[i].provName);
            provinceArr[i].provName = NULL; 
        }
    }
    free(provinceArr); 
}

void freeIncomeArr(struct Income *incomeArr, int num_lines)
{
    for (int i = 0; i < num_lines; i++)
    {
        if(incomeArr[i].prov != NULL)
        {
            free(incomeArr[i].prov); 
            incomeArr[i].prov = NULL; 
        }
        
    }
    free(incomeArr); 
}