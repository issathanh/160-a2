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
    char *incomeProv; 
    int year;
    int income; 
    int industry; 
    
};
int count_line( const char *filename);
void make_array_province(int num_lines, const char*filename, struct Province *provinceArr);
void make_array_income(int num_lines, const char*filename, struct Income *incomeARrr,struct Province *provinceArr,struct Industry *industryArr);
void freeProvinceArr(struct Province *provinceArr, int num_lines);
void freeIncomeArr(struct Income *incomeArr, int num_lines);
void freeIndustryArr(struct Industry *industryArr, int num_lines);
void make_array_industry (int num_lines, const char*filename, struct Industry * industryArr);
void find_helper (char* province, int year, char*industry, struct Income *incomeArr,struct Province *provinceArr,struct Industry *industryArr,int province_length, int income_length, int industry_length);


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
    struct Industry *industryArr = malloc(num_lines_industry*sizeof(struct Industry)); 
    // struct array for province
    make_array_province(num_lines_province,file_q1_province,provinceArr); 
    make_array_industry(num_lines_industry,file_q1_industry,industryArr);
    make_array_income(num_lines_income,file_q1_income,incomeArr,provinceArr,industryArr); 


    // for (int i =0 ; i < num_lines_income; i++)
    // {
    //     printf("%d\n", incomeArr[i].industry);
    // }
    find_helper("Ontario", 2017, "Aqua", incomeArr,provinceArr, industryArr,num_lines_province,num_lines_income,num_lines_industry);
    freeProvinceArr(provinceArr,num_lines_province); 
    freeIncomeArr(incomeArr,num_lines_income); 
    freeIndustryArr(industryArr,num_lines_industry); 
}

// void FIND(char *province, int year, char* Ind)
// {

// }
void find_helper (char* province, int year, char*industry, struct Income *incomeArr,struct Province *provinceArr,struct Industry *industryArr,int province_length, int income_length, int industry_length)
{
    int income = 0; 
    int industry_name_len = strlen(industry); 
    for (int i = 0; i < province_length; i++)
    {
        if(strcmp(province,provinceArr[i].provName)==0){
            for (int j =0; j<industry_length;j++)
            {
                if(strncmp(industry,industryArr[j].indName, industry_name_len) == 0)
                {
                    for (int k = 0; k < income_length; k++)
                    {
                        if(industryArr[j].code == incomeArr[k].industry&&incomeArr[k].year == year)
                        {
                           income+=incomeArr[k].income; 
                        }
                    }
                    printf("%s %d",industryArr[j].indName, income); 
                    income = 0; 
                }
            }
        }
    }
}
void make_array_industry (int num_lines, const char*filename, struct Industry * industryArr)
{
    FILE *file = fopen(filename, "r"); 
    char line [MAX_LINE_LENGTH]; 
    int dataIndex = 0; 
    char *end; 
    int val; 
    if (file == NULL)
    {
        perror("bad industry files"); 
        return; 
    }
    if (fgets(line,sizeof(line),file) == NULL)
    {
        perror("Bad industry file"); 
    }
    while (fgets(line,sizeof(line),file) != NULL)
    {
        char *token = strtok(line,","); 
        int tokenIndex = 0; 
        while(token != NULL && dataIndex < num_lines)
        {
            switch(tokenIndex)
            {
                case 0: 
                    val = strtol(token, &end,10);
                    //precondition to check for negative
                    if (val > 0 )
                    {
                        industryArr[dataIndex].code = val; 
                    } 
                    //precondition for no value found
                    if (end == token)
                    {
                        perror("no ind code was found"); 
                    }
                    break; 
                case 1: 
                    industryArr[dataIndex].indName = malloc(strlen(token) + 1); 
                    strcpy(industryArr[dataIndex].indName, token); 
                    break; 
            }
            tokenIndex++; 
            token = strtok(NULL,","); 
        }
        dataIndex++; 
    }
    fclose(file); 
}

void make_array_income(int num_lines, const char*filename, struct Income *incomeArr,struct Province *provinceArr,struct Industry *industryArr)
{
    FILE *file = fopen(filename, "r");
    char line[MAX_LINE_LENGTH]; 
    int dataIndex = 0; 
    char *end; 

    if (file == NULL)
    {
        perror("Error opening file"); 
        return; 
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

                    incomeArr[dataIndex].incomeProv = malloc(strlen(token) + 1); 
                    strcpy(incomeArr[dataIndex].incomeProv,token);  
                    break; 
                
                case 1://set the year
                    //end point to the first char of the string that wasnt a number 
                    incomeArr[dataIndex].year = strtol(token,&end,10);
                    if(end==token)
                    {
                        printf("no year were found at line %d", dataIndex+2); 
                    }
                    break; 
                case 2: //set the income
                    //end point to the first char of the string that wasnt a number 
                    incomeArr[dataIndex].income = strtol(token,&end,10);
                    if(end==token)
                    {
                        printf("no income were found at line %d", dataIndex+2); 
                    }
                    
                    break; 
                case 3: //set the industry id
                    //end point to the first char of the string that wasnt a number 
                    incomeArr[dataIndex].industry = strtol(token,&end,10);
                    if(end==token)
                    {
                        printf("no industry code were found at line %d", dataIndex+2); 
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
        if(incomeArr[i].incomeProv != NULL)
        {
            free(incomeArr[i].incomeProv); 
            incomeArr[i].incomeProv = NULL; 
        }
        
    }
    free(incomeArr); 
}

void freeIndustryArr(struct Industry *industryArr, int num_lines)
{
    for (int i = 0; i < num_lines; i++)
    {
        if(industryArr[i].indName != NULL)
        {
            free(industryArr[i].indName); 
            industryArr[i].indName = NULL; 
        }
    }
    free(industryArr); 
}