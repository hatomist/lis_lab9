#include "main.h"
//#include "gui.h"

int sort_type = 0;
FILE *pFile;

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
    int (*menu_funcs[MENU_ITEM_COUNT]) (record **records, size_t *records_num) = {show_records, add_records,
                                                                                  delete_records, change_records_sort,
                                                                                  delete_file, exit_program};

    size_t file_size = get_file_size(pFile);
    record* records;
    size_t records_num = 0;

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

        sort_type = atoi(data);

        fscanf(pFile, "%[^\n]", data);
        fgetc(pFile);

        records_num = atoi(data);

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
        menu_funcs[choice-1](&records, &records_num);
    }


    return 0;
}


int show_records(const record **records, const size_t *records_num)
{

    if (*records_num == 0)
        printf("The file is empty!\n");
    else
    {
        for (size_t i = 0; i < *records_num; i++) {
            printf("Record №%zu:\n"
                   "Region title: %s\n"
                   "Region area: %.2lf\n"
                   "Region population: %.2lf\n\n",
                   i+1, (*records)[i].title, (*records)[i].area, (*records)[i].population);
        }
    }

    return 0;
}


int add_records(record **records, size_t *records_num) {
    size_t records_to_add_num = get_ull("Enter number of new records: ");
    if (records_to_add_num == 0) {
        printf("Haven't added any records.\n");
        return 0;
    }

    *records = realloc(*records, (*records_num + records_to_add_num) * sizeof(record));

    for (size_t i = *records_num; i < *records_num + records_to_add_num; i++)
    {
        char *title = malloc((100 + 1) * sizeof(char));
        printf("Enter record №%zu title ", i+1);
        fgets(title, (int) 100, stdin);
        strtrim(title);
        while (*title == '\0') {
            printf("Title must not be empty! Try again: ");
            fgets(title, (int) 100, stdin);
            strtrim(title);
        }
        (*records)[i].title = title;
        (*records)[i].area = get_double("Enter record's area: ", 1, 0);
        (*records)[i].population = get_double("Enter record's population: ", 1, 0);

    }

    *records_num = *records_num + records_to_add_num;
    sort_records(records, records_num);
    printf("\nSuccessfully added %zu records!\n", records_to_add_num);

    return 0;
}

int delete_records(record **records, size_t *records_num) {
    if (*records_num == 0) {
        printf("The file is empty!");
        return 0;
    }

    show_records((const record**)records, records_num);
    char *selection = malloc(*records_num * sizeof(char));
    multiselect_question(selection, *records_num);
    size_t delete_num = 0;
    for (size_t i = 0; i < *records_num; i++)
        if (selection[i] == '\1')
            delete_num++;
    printf("Do you really want to delete %zu records? [Y/n]: ", delete_num);
    char response[101];
    scanf("%s", response);
    while (strlen(response) != 1 && !(toupper(response[0]) == 'Y' || response[0] == 'N')) {
        printf("Incorrect input, please enter \"y/Y\" or \"n/N\": ");
        scanf("%s", response);
    }
    if (toupper(response[0]) == 'N')
    {
        printf("No records were deleted\n");
        return 0;
    }

    for (size_t i = 0; i < *records_num; i++)
        if (selection[i] == '\1')
        {
            (*records)[i].title = NULL;
            (*records)[i].population = 0;
            (*records)[i].area = 0;
        }

    for (size_t i = 0; i < (*records_num - delete_num); i++)
        if ((*records)[i].title == NULL)
            for(size_t j = 0; j < *records_num; j++)
                if ((*records)[j].title != NULL)
                {
                    (*records)[i].title = (*records)[j].title;
                    (*records)[i].area = (*records)[j].area;
                    (*records)[i].population = (*records)[j].population;
                    (*records)[j].title = NULL;
                    break;
                }
    *records_num -= delete_num;
    *records = realloc(*records, *records_num * sizeof(record));
    printf("Successfully deleted %zu records!\n", delete_num);
    return 0;
}

int change_records_sort(record **records, size_t *records_num)
{
    printf("4\n");
    return 0;
}

int delete_file(record **records, size_t *records_num){printf("5\n");return 0;}
int exit_program(record **records, size_t *records_num){printf("6\n");exit(0);}

int sort_records(record **records, size_t *records_num)
{
    if (sort_type == SORT_OFF)
        return 0;
    return -1;
}

size_t get_file_size(FILE *pFile)
{
    size_t cur_pos = ftell(pFile);
    fseek(pFile, 0, SEEK_END);
    size_t file_size = ftell(pFile);
    fseek(pFile, cur_pos, SEEK_SET);
    return file_size;
}
