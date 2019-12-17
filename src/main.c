#include "main.h"
#include "gui.h"

int main(int argc, char *argv[]) {

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

    return gui(pFile);

}

void help(char *exec_name) {
    (void)fprintf(stdout, "Usage: %s [-f path/to/file] [other_opts]\n", exec_name);
}
