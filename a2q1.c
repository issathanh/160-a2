#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define MAX_LINE_LENGTH 500

#define MAX_INPUT_LENGTH 200
#define MAX_PROVINCE_LENGTH 100
struct Province
{
    char *prov_name;
    char *prov_short;
};

struct Industry
{
    int code;
    char *ind_name;
};

struct Income
{
    char *incomeProv;
    int year;
    int income;
    int industry;
};

struct Joint
{
    char *province_name;
    char *province_shortcut;
    int year;
    int income;
    int industry_code;
    char *industry_name;
};

int count_line(const char *filename);
void make_array_province(int num_lines, const char *filename, struct Province *province_arr);
void make_array_income(int num_lines, const char *filename, struct Income *incomeARrr, struct Province *province_arr, struct Industry *industry_arr);
void freeProvinceArr(struct Province *province_arr, int num_lines);
void freeIncomeArr(struct Income *income_arr, int num_lines);
void freeIndustryArr(struct Industry *industry_arr, int num_lines);
void make_array_industry(int num_lines, const char *filename, struct Industry *industry_arr);
void find_helper(char *province, int year, char *industry, struct Income *income_arr, struct Province *province_arr, struct Industry *industry_arr, int province_length, int income_length, int industry_length);
int find_max_year(struct Income *income_arr, int num_lines);
int find_min_year(struct Income *income_arr, int num_lines);
void sum_province(char *province, struct Income *income_arr, int income_length, struct Province *province_arr, int province_length);
void sum_ind_by_year(char *industry, int year, struct Province *province_arr, int num_length_province, struct Income *income_arr, int num_length_income, struct Industry *industry_arr, int num_length_industry);

int main(void)
{
    // count lines for income
    const char *file_q1_income = "q1_income.txt";
    int num_lines_income = count_line(file_q1_income) - 1;

    // count line for province
    const char *file_q1_province = "q1_province.txt";
    int num_lines_province = count_line(file_q1_province) - 1;

    // count lines industry
    const char *file_q1_industry = "q1_industry.txt";
    int num_lines_industry = count_line(file_q1_industry) - 1;

    struct Province *province_arr = malloc(num_lines_province * sizeof(struct Province));
    struct Income *income_arr = malloc(num_lines_income * sizeof(struct Income));
    struct Industry *industry_arr = malloc(num_lines_industry * sizeof(struct Industry));
    // struct array for province
    make_array_province(num_lines_province, file_q1_province, province_arr);
    make_array_industry(num_lines_industry, file_q1_industry, industry_arr);
    make_array_income(num_lines_income, file_q1_income, income_arr, province_arr, industry_arr);

    char input[MAX_INPUT_LENGTH];
    char province[MAX_PROVINCE_LENGTH];
    char ind[MAX_PROVINCE_LENGTH]; // Ensure ind is declared with adequate space for a string
    int year;

    // Preconditions: None specific beyond ensuring fgets reads something.
    // This is more about input validation than a precondition.

    printf("Enter one of the commands:\n");
    printf("1. SUM [Ind] IN [Year]\n");
    printf("2. SUM [Province] BY Year\n");
    printf("3. FIND [Province],[Year],[Ind]\n");

    assert(fgets(input, MAX_INPUT_LENGTH, stdin) != NULL); // Ensures we have read input successfully.

    // Removing any newline character - Not strictly a DbC feature but necessary for processing.
    input[strcspn(input, "\n")] = 0;

    // The contract here is that one of the sscanf calls must succeed; otherwise, the input is invalid.
    if (sscanf(input, "SUM %s IN %d", ind, &year) == 2)
    {
        // Postcondition: ind and year must be properly assigned.
        assert(year > 0); // Example postcondition, assuming year should be positive.
        sum_ind_by_year(ind, year, province_arr, num_lines_province, income_arr, num_lines_income, industry_arr, num_lines_industry);
    }
    else if (sscanf(input, "SUM %99s BY Year", province) == 1)
    {
        // Postcondition: province must be a non-empty string.
        assert(strlen(province) > 0); // Ensures province is not empty.
        sum_province(province, income_arr, num_lines_income, province_arr, num_lines_province);
    }
    else if (sscanf(input, "FIND %99s,%d,%99s", province, &year, ind) == 3)
    {
        // Postconditions: ind and year must be properly assigned, province non-empty.
        assert(year > 0 && strlen(province) > 0); // Combining multiple conditions.
        find_helper(province, year, ind, income_arr, province_arr, industry_arr, num_lines_province, num_lines_income, num_lines_industry);
    }
    else
    {
        printf("Invalid command or format.\n");
    }

    // No specific invariant is demonstrated, as the function's primary job is to process input rather than maintain state.

    freeProvinceArr(province_arr, num_lines_province);
    freeIncomeArr(income_arr, num_lines_income);
    freeIndustryArr(industry_arr, num_lines_industry);
}

void sum_ind_by_year(char *industry, int year, struct Province *province_arr, int num_length_province, struct Income *income_arr, int num_length_income, struct Industry *industry_arr, int num_length_industry)
{
    int income = 0;
    for (int i = 0; i < num_length_province; i++)
    {
        // go through each province
        for (int j = 0; j < num_length_industry; j++)
        {
            // found matching industry
            if (strncmp(industry, industry_arr[j].ind_name, strlen(industry)) == 0)
            {
                for (int k = 0; k < num_length_income; k++)
                {
                    if (strncmp(province_arr[i].prov_short, income_arr[k].incomeProv, strlen(income_arr[i].incomeProv)) == 0 &&
                        year == income_arr[k].year && income_arr[k].industry == industry_arr[j].code)
                    {
                        income += income_arr[k].income;
                    }
                }
            }
        }
        printf("%s %d: %d\n", province_arr[i].prov_name, year, income);
        income = 0;
    }
}
void sum_province(char *province, struct Income *income_arr, int income_length, struct Province *province_arr, int province_length)
{
    char *prov_short;
    for (int i = 0; i < province_length; i++)
    {
        if (strcmp(province, province_arr[i].prov_name) == 0)
        {
            prov_short = malloc(strlen(province_arr[i].prov_short));
            strcpy(prov_short, province_arr[i].prov_short);
        }
    }

    int income = 0;
    int min = find_min_year(income_arr, income_length);
    int max = find_max_year(income_arr, income_length);
    for (int j = min; j <= max; j++)
    {
        for (int i = 0; i < income_length; i++)
        {
            if (strncmp(prov_short, income_arr[i].incomeProv, strlen(income_arr[i].incomeProv)) == 0 && income_arr[i].year == j)
            {
                income += income_arr[i].income;
            }
        }
        printf("For year: %d, income is: %d\n", j, income);
        income = 0;
    }
    free(prov_short);
}
int find_max_year(struct Income *income_arr, int num_lines)
{
    int max;
    for (int i = 0; i < num_lines; i++)
    {
        if (max < income_arr[i].year)
        {
            max = income_arr[i].year;
        }
    }
    return max;
}
int find_min_year(struct Income *income_arr, int num_lines)
{
    int min = income_arr[0].year;
    for (int i = 1; i < num_lines; i++)
    {
        if (min > income_arr[i].year)
        {
            min = income_arr[i].year;
        }
    }
    return min;
}
void find_helper(char *province, int year, char *industry, struct Income *income_arr, struct Province *province_arr, struct Industry *industry_arr, int province_length, int income_length, int industry_length)
{
    printf("it got heere"); 
    int income = 0;
    int industry_name_len = strlen(industry);
    char *prov_short;
    int curr_ind_code = 0;
    char *industry_name;
    // get prov short
    for (int i = 0; i < province_length; i++)
    { // if the province name is the same
        if (strcmp(province, province_arr[i].prov_name) == 0)
        {
            prov_short = malloc(strlen(province_arr[i].prov_short) + 1);
            strcpy(prov_short, province_arr[i].prov_short);
            // printf("%s",prov_short);
        }
    }

    // get industry code
    for (int i = 0; i < industry_length; i++)
    {
        if (strncmp(industry, industry_arr[i].ind_name, industry_name_len) == 0)
        {
            curr_ind_code = industry_arr[i].code;
            industry_name = malloc(strlen(industry_arr[i].ind_name) + 1);
            strcpy(industry_name, industry_arr[i].ind_name);
            // printf("%d", curr_ind_code );
        }
    }

    // print the output
    for (int i = 0; i < income_length; i++)
    {

        // printf("%d,%d\n", strlen(income_arr[i].incomeProv), strlen(prov_short) );
        if (strncmp(prov_short, income_arr[i].incomeProv, 2) == 0)
        {
            if (income_arr[i].year == year && income_arr[i].industry == curr_ind_code)
            {
                income += income_arr[i].income;
                printf("%s %d", industry_name, income);
            }
        }
    }
    // printf("it got here");
    free(prov_short);
    free(industry_name);
}

void make_array_industry(int num_lines, const char *filename, struct Industry *industry_arr)
{
    FILE *file = fopen(filename, "r");
    char line[MAX_LINE_LENGTH];
    int dataIndex = 0;
    char *end;
    int val;
    if (file == NULL)
    {
        perror("bad industry files");
        return;
    }
    if (fgets(line, sizeof(line), file) == NULL)
    {
        perror("Bad industry file");
    }
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *token = strtok(line, ",");
        int tokenIndex = 0;
        while (token != NULL && dataIndex < num_lines)
        {
            switch (tokenIndex)
            {
            case 0:
                val = strtol(token, &end, 10);
                // precondition to check for negative
                if (val > 0)
                {
                    industry_arr[dataIndex].code = val;
                }
                // precondition for no value found
                if (end == token)
                {
                    perror("no ind code was found");
                }
                break;
            case 1:
                industry_arr[dataIndex].ind_name = malloc(strlen(token) + 1);
                strcpy(industry_arr[dataIndex].ind_name, token);
                break;
            }
            tokenIndex++;
            token = strtok(NULL, ",");
        }
        dataIndex++;
    }
    fclose(file);
}

void make_array_income(int num_lines, const char *filename, struct Income *income_arr, struct Province *province_arr, struct Industry *industry_arr)
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
    // skip first line and check for NULL
    if (fgets(line, sizeof(line), file) == NULL)
    {
        perror("File error");
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *token = strtok(line, ",");
        int tokenIndex = 0;

        while (token != NULL && dataIndex <= num_lines)
        {
            switch (tokenIndex)
            {
            case 0:

                income_arr[dataIndex].incomeProv = malloc(strlen(token) + 1);
                strcpy(income_arr[dataIndex].incomeProv, token);
                break;

            case 1: // set the year
                // end point to the first char of the string that wasnt a number
                income_arr[dataIndex].year = strtol(token, &end, 10);
                if (end == token)
                {
                    printf("no year were found at line %d", dataIndex + 2);
                }
                break;
            case 2: // set the income
                // end point to the first char of the string that wasnt a number
                income_arr[dataIndex].income = strtol(token, &end, 10);
                if (end == token)
                {
                    printf("no income were found at line %d", dataIndex + 2);
                }

                break;
            case 3: // set the industry id
                // end point to the first char of the string that wasnt a number
                income_arr[dataIndex].industry = strtol(token, &end, 10);
                if (end == token)
                {
                    printf("no industry code were found at line %d", dataIndex + 2);
                }
                break;
            }
            tokenIndex++;
            token = strtok(NULL, ",");
        }
        dataIndex++;
    }
    fclose(file);
}
void make_array_province(int num_lines, const char *filename, struct Province *province_arr)
{
    FILE *file = fopen(filename, "r");
    char line[MAX_LINE_LENGTH];
    int dataIndex = 0;

    if (file == NULL)
    {
        perror("Error opening file");
    }

    // skip first line and also check if its null
    if (fgets(line, sizeof(line), file) == NULL)
    {
        perror("File error");
    }
    // put every line in file to line
    while (fgets(line, sizeof(line), file) != NULL)
    {
        // get the first token before comma
        char *token = strtok(line, ",");
        int tokenIndex = 0;
        while (token != NULL && dataIndex <= num_lines)
        {
            switch (tokenIndex)
            {
            case 0: // first part of token will be prov_short name
                province_arr[dataIndex].prov_name = malloc(strlen(token) + 1);
                strcpy(province_arr[dataIndex].prov_name, token);
                break;

            case 1: // 2nd part of token will be prov_short
                province_arr[dataIndex].prov_short = malloc(strlen(token) + 1);
                strcpy(province_arr[dataIndex].prov_short, token);
                break;
            }

            tokenIndex++;
            token = strtok(NULL, ",");
        }
        dataIndex++;
    }
    fclose(file);
}

int count_line(const char *filename)
{
    // open file for reading
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Unable to open the file");
        return -1;
    }

    int lines = 0;
    char buffer[MAX_LINE_LENGTH]; // Buffer to hold each lines

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        lines++;
    }
    fclose(file);
    return lines;
}

void freeProvinceArr(struct Province *province_arr, int num_lines)
{
    for (int i = 0; i < num_lines; i++)
    {
        if (province_arr[i].prov_short != NULL)
        {
            free(province_arr[i].prov_short);
            province_arr[i].prov_short = NULL;
        }
        if (province_arr[i].prov_name != NULL)
        {
            free(province_arr[i].prov_name);
            province_arr[i].prov_name = NULL;
        }
    }
    free(province_arr);
}

void freeIncomeArr(struct Income *income_arr, int num_lines)
{
    for (int i = 0; i < num_lines; i++)
    {
        if (income_arr[i].incomeProv != NULL)
        {
            free(income_arr[i].incomeProv);
            income_arr[i].incomeProv = NULL;
        }
    }
    free(income_arr);
}

void freeIndustryArr(struct Industry *industry_arr, int num_lines)
{
    for (int i = 0; i < num_lines; i++)
    {
        if (industry_arr[i].ind_name != NULL)
        {
            free(industry_arr[i].ind_name);
            industry_arr[i].ind_name = NULL;
        }
    }
    free(industry_arr);
}