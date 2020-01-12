#include "util.h"

int multiselect_question(char *array, int count)
{
    char *response, *lastchar;
    FILE *stream;
    size_t response_len = 64;

    stream = stdout;

    response = malloc(response_len);

    if(!response) {
        return -1;
    }

    lastchar = response + response_len - 1;
    /* sentinel byte to later see if we filled up the entire string */
    *lastchar = 1;

    while(1) {
        memset(array, 1, count);

        fprintf(stream, "\nEnter a selection (default=all): ");
        fflush(stream);

        if(fgets(response, response_len, stdin)) {
            const size_t response_incr = 64;
            size_t len;
            /* handle buffer not being large enough to read full line case */
            while(*lastchar == '\0' && lastchar[-1] != '\n') {
                response_len += response_incr;
                response = realloc(response, response_len);
                if(!response) {
                    return -1;
                }
                lastchar = response + response_len - 1;
                /* sentinel byte */
                *lastchar = 1;
                if(fgets(response + response_len - response_incr - 1,
                         response_incr + 1, stdin) == 0) {
                    free(response);
                    return -1;
                }
            }

            len = strtrim(response);
            if(len > 0) {
                if(multiselect_parse(array, count, response) == -1) {
                    /* only loop if user gave an invalid answer */
                    continue;
                }
            }
            break;
        } else {
            free(response);
            return -1;
        }
    }

    free(response);
    return 0;
}

static int multiselect_parse(char *array, int count, char *response)
{
    char *str, *saveptr;

    for(str = response; ; str = NULL) {
        int include = 1;
        int start, end;
        size_t len;
        char *ends = NULL;
        char *starts = strtok_r(str, " ,", &saveptr);

        if(starts == NULL) {
            break;
        }
        len = strtrim(starts);
        if(len == 0)
            continue;

        if(*starts == '^') {
            starts++;
            len--;
            include = 0;
        } else if(str) {
            /* if first token is including, we unselect all targets */
            memset(array, 0, count);
        }

        if(len > 1) {
            /* check for range */
            char *p;
            if((p = strchr(starts + 1, '-'))) {
                *p = 0;
                ends = p + 1;
            }
        }

        if(parseindex(starts, &start, 1, count) != 0)
            return -1;

        if(!ends) {
            array[start - 1] = include;
        } else {
            int d;
            if(parseindex(ends, &end, start, count) != 0) {
                return -1;
            }
            for(d = start; d <= end; d++) {
                array[d - 1] = include;
            }
        }
    }

    return 0;
}

size_t strtrim(char *str)
{
    char *end, *pch = str;

    if(str == NULL || *str == '\0') {
        /* string is empty, so we're done. */
        return 0;
    }

    while(isspace((unsigned char)*pch)) {
        pch++;
    }
    if(pch != str) {
        size_t len = strlen(pch);
        if(len) {
            memmove(str, pch, len + 1);
        } else {
            *str = '\0';
        }
    }

    /* check if there wasn't anything but whitespace in the string. */
    if(*str == '\0') {
        return 0;
    }

    end = (str + strlen(str) - 1);
    while(isspace((unsigned char)*end)) {
        end--;
    }
    *++end = '\0';

    return end - pch;
}


static int parseindex(char *s, int *val, int min, int max)
{
    char *endptr = NULL;
    int n = strtol(s, &endptr, 10);
    if(*endptr == '\0') {
        if(n < min || n > max) {
            printf("invalid value: %d is not between %d and %d\n", n, min, max);
            return -1;
        }
        *val = n;
        return 0;
    } else {
        printf("invalid number: %s\n", s);
        return -1;
    }
}

