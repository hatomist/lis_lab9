#include "main.h"
//#include "gui.h"

int main(int argc, char *argv[])
{
    int opt;
    char *filepath = NULL;

    while((opt = getopt(argc, argv, ":f:")) != -1)
        switch(opt)
        {
            case 'f':
                if (filepath != NULL)
                {
                    (void)fprintf(stderr,"Error: \"f\" option was specified more than once\n");
                    return -1;
                }
                filepath = calloc(sizeof(char), strlen(optarg));
                strcpy(filepath, optarg);
                break;

            case ':':
                (void)fprintf(stderr, "Option \"%c\" needs a value\n", optopt);
                help(argv[0]);
                return -1;

            case '?':
                (void)fprintf(stderr,"Unknown option: %c\n", optopt);
                help(argv[0]);
                return -1;

            default:
                break;
        }

    FILE *pFile;

    if (filepath == NULL)
    {
        help(argv[0]);
        return 0;
    }

    errno = 0;
    pFile = fopen(filepath, "r+");
    if (pFile == NULL)
        switch (errno)
        {
            case EISDIR:
                (void)fprintf(stderr, "\"%s\" is a directory!\n", filepath);
                return errno;
            case ENOENT:
            case EFAULT:
                (void)fprintf(stderr, "Bad file path entered\n");
                return errno;
            case EACCES:
                (void)fprintf(stderr, "Cannot open the file: insufficient permissions\n");
                return errno;
            default:
                (void)fprintf(stderr, "Cannot open the file: unknown error %d\n", errno);
                return errno;
        }

    return tui(pFile);

}

void help(char *exec_name) {
    (void)fprintf(stdout, "Usage: %s [-f path/to/file] [other_opts]\n", exec_name);
}

int tui(FILE *pFile)
{

    const char* menu_items[MENU_ITEM_COUNT] = {"Show records", "Add record(s)", "Delete record(s)", "Sort by parameter",
                                         "Delete file", "Exit program"};
    int (*menu_funcs[MENU_ITEM_COUNT]) (FILE *pFile) = {show_records, add_records, delete_records,
                                                        sort_records, delete_file, exit_program};

    size_t file_size = get_file_size(pFile);
    record* records;
    if (file_size == 0) {
        printf("The file is empty!\n");
        records = NULL;
    } else {
        char *data = (char *) malloc((1000 + 1) * sizeof(char));

        fscanf(pFile, "%[^\n]", data);
        fgetc(pFile);

        char* test_str = "lis_lab_9";
        if (strcmp(test_str, data))
        {
            printf("Error: invalid file signature\n");
            return -1;
        }

        fscanf(pFile, "%[^\n]", data);
        fgetc(pFile);

        int sort_type = atoi(data);

        fscanf(pFile, "%[^\n]", data);
        fgetc(pFile);

        size_t records_num = atoi(data);

        records = malloc(records_num * sizeof(record));

        for (size_t i = 0; i < records_num; i++)
        {
            fscanf(pFile, "%[^\n]", data);
            fgetc(pFile);
            records[i].title = malloc((strlen(data) + 1) * sizeof(char));
            strcpy(records[i].title, data);

            fscanf(pFile, "%[^\n]", data);
            fgetc(pFile);
            records[i].area = atof(data);

            fscanf(pFile, "%[^\n]", data);
            fgetc(pFile);
            records[i].population = atof(data);
        }
    }


    for(;;)
    {
        printf("Select an action for this file\n");

        for (size_t i = 0; i < MENU_ITEM_COUNT; i++)
            printf("%lu - %s\n", i+1, menu_items[i]);
        unsigned long long choice = get_ull("\nEnter the number: ");
        while (choice < 1 || choice > 6) {
            printf("Incorrect input! It should be a value between %d and %d", 0, MENU_ITEM_COUNT);
            choice = get_ull("\nEnter the number: ");
        }
        menu_funcs[choice-1](pFile);
    }


    return 0;
}



int show_records(record *records, size_t records_num)
{


//    if (file_size == 0) {
//        printf("The file is empty!\n");
//        return 0;
//    }
//
//    if (file_size < 10) {
//        printf("Invalid file format!\n");
//        return -1;
//    }



    return 0;
}


int add_records(record *records, size_t records_num)
{
    printf("2\n");
    return 0;
}

int delete_records(record *records, size_t records_num){printf("3\n");return 0;}
int sort_records(record *records, size_t records_num){printf("4\n");return 0;}
int delete_file(record *records, size_t records_num){printf("5\n");return 0;}
int exit_program(record *records, size_t records_num){printf("6\n");exit(0);}

size_t get_file_size(FILE *pFile)
{
    size_t cur_pos = ftell(pFile);
    fseek(pFile, 0, SEEK_END);
    size_t file_size = ftell(pFile);
    fseek(pFile, cur_pos, SEEK_SET);
    return file_size;
}

// How to multiselect
//size_t count = 5;
//char *array = malloc(count);
//multiselect_question(array, count);
// \1 for selected, \0 for unselected
