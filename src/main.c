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
    pFile = fopen(filepath, "a");
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



int show_records(FILE *pFile){printf("1\n"); return 0;}
int add_records(FILE *pFile){printf("2\n");return 0;}
int delete_records(FILE *pFile){printf("3\n");return 0;}
int sort_records(FILE *pFile){printf("4\n");return 0;}
int delete_file(FILE *pFile){printf("5\n");return 0;}
int exit_program(FILE *pFile){printf("6\n");return 0;}


// How to multiselect
//size_t count = 5;
//char *array = malloc(count);
//multiselect_question(array, count);
// \1 for selected, \0 for unselected
