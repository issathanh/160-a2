#include <stdio.h>

#define MAX_LINE_LENGTH 500 
struct Province {
    char * provName; 
    char * prov;  
}; 

struct Industry 
{
    int code; 
    char * indName; 
};

struct Income
{
    struct Province *prov; 
    int year;
    int income; 
    struct Industry *industry; 
    
};
int count_line( const char *filename);

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

    struct Province *provinceArr = (struct Province*)malloc(num_lines_income,sizeof(struct Province)); 

    // struct array for province
    make_array_province(num_lines_province,file_q1_province,&provinceArr); 
    for (int i =0; i < num_lines_province; i++)
    {
        
    }
}

void make_array_province(int num_lines, const char*filename, struct Province *provinceArr)
{
    FILE *file = fopen(filename, "r"); 
    char line[MAX_LINE_LENGTH]; 
    int dataIndex = 0; 

    if(file == NULL)
    {
        perror("Error opening file");
        return -1; 
    }


    //skip first line and also check if its null
    if(fgets(line,sizeof(line),file) == NULL)
    {
        perror("File error");
        return -1;
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
                    provinceArr[dataIndex].prov = malloc((token)+1);
                    strcpy(provinceArr[dataIndex].prov,token); 
                    break; 
            }

            
            tokenIndex++; 
            token = strtok(NULL,","); 
        }
        dataIndex++; 
    }

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