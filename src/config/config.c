#include "config.h"

size_t l1_cap = def_l1_cap;
size_t l2_cap = def_l2_cap;
char listen_ip[20] = def_listen_ip;
uint16_t listen_port = def_listen_port;
char log_path[MAX_PATH_LENGTH] = def_log_path;
char log_prefix[MAX_PATH_LENGTH] = def_log_prefix;
int log_lines = def_log_lines;

void Strip_Whitespace(char* str) 
{
    char* write = str;
    char* read = str;
    while (*read) {
        if (*read != ' ' && *read != '\t' && *read != '\n' && *read != '\r') {
            *write++ = *read;
        }
        read++;
    }
    *write = '\0';
}

void Read_Config(const char* filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Can not open config file: %s, use default parameters.\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    char section[MAX_SECTION_LENGTH] = "";

    while (fgets(line, sizeof(line), file))
    {
        Strip_Whitespace(line);

        if (line[0] == ';' || line[0] == '#' || line[0] == '\0')
        {
            continue;
        }

        if (line[0] == '[' && line[strlen(line) - 1] == ']')
        {
            strncpy(section, line + 1, strlen(line) - 2);
            section[strlen(line) - 2] = '\0';
        }
        else
        {
            char key[MAX_KEY_LENGTH];
            char value[MAX_LINE_LENGTH];
            char* delimiter = strchr(line, '=');
            if (delimiter != NULL) {
                strncpy(key, line, delimiter - line);
                key[delimiter - line] = '\0';
                strcpy(value, delimiter + 1);

                if (strcmp(section, "buffer") == 0)
                {
                    printf("buffer:\n");
                    if (strcmp(key, "l1_cap") == 0)
                    {
                        l1_cap = atoi(value);
                        printf("l1_cap: %d\n", l1_cap);
                    }
                    else if (strcmp(key, "l2_cap") == 0)
                    {
                        l2_cap = atoi(value);
                        printf("l2_cap: %d\n", l2_cap);
                    }
                }
                if (strcmp(section, "monitor") == 0)
                {
                    printf("monitor:\n");
                    if (strcmp(key, "listen_ip") == 0)
                    {
                        strncpy(listen_ip, value, sizeof(listen_ip) - 1);
                        listen_ip[sizeof(listen_ip) - 1] = '\0';
                        printf("listen_ip: %s\n", listen_ip);
                    }
                    else if (strcmp(key, "listen_port") == 0)
                    {
                        listen_port = atoi(value);
                        printf("listen_port: %d\n", listen_port);
                    }
                }
                if (strcmp(section, "writer") == 0)
                {
                    printf("writer:\n");
                    if (strcmp(key, "log_path") == 0)
                    {
                        strncpy(log_path, value, sizeof(log_path) - 1);
                        log_path[sizeof(log_path) - 1] = '\0';
                        printf("log_path: %s\n", log_path);
                    }
                    else if (strcmp(key, "log_prefix") == 0)
                    {
                        strncpy(log_prefix, value, sizeof(log_prefix) - 1);
                        log_prefix[sizeof(log_prefix) - 1] = '\0';
                        printf("log_prefix: %d\n", log_prefix);
                    }
                    else if (strcmp(key, "log_lines") == 0)
                    {
                        log_lines = atoi(value);
                        printf("log_lines: %d\n", log_lines);
                    }
                }
            }
        }
    }

    fclose(file);
}

int Mkdir_Recursive(const char *path)
{
    char path_buffer[MAX_PATH_LENGTH];
    char *p = NULL;
    size_t len = strlen(path);

    strcpy(path_buffer, path);
    if (path_buffer[len - 1] == '/')
    {
        path_buffer[len - 1] = '\0';
    }

    for (p = path_buffer + 1; *p; p++)
    {
        if (*p == '/')
        {
            *p = '\0';
            if (mkdir(path_buffer, S_IRWXU) != 0 && errno != EEXIST)
            {
                return -1;
            }
            *p = '/';
        }
    }

    if (mkdir(path_buffer, S_IRWXU) != 0 && errno != EEXIST)
    {
        return -1;
    }

    return 0;
}

void Init()
{
    Read_Config("litelog.ini");
    Mkdir_Recursive(log_path);
}