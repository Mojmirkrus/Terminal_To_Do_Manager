#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <shlwapi.h>
#include "used_functions.h"

//TIME FUNCTIONS//TIME_UTILS//
struct tm *get_and_print_old_time() {
    struct tm *time_info_old = malloc(sizeof(struct tm));
    if (time_info_old == NULL) {
        printf("Memory allocation for time_info_old failed");
    }
    FILE *saved_time = fopen("C:/Users/mojmi/CLionProjects/Mojmir/Learning/Terminal_To_Do_Manager/Saved Time", "rb");
    if (saved_time != NULL) {
        fread(time_info_old, sizeof(struct tm), 1, saved_time);
        printf("\nOld local time and date     : %s", asctime(time_info_old));
    } else { printf("'saved_time' couldn't be read by fread"); }
    fclose(saved_time);
    return time_info_old;
}

struct tm *get_new_time() {
    time_t raw_time;
    struct tm *time_info = malloc(sizeof(struct tm));
    if (time_info == NULL) {
        printf("Memory allocation for time_info failed");
    }
    time(&raw_time);
    time_info = localtime(&raw_time);
    if (time_info) {
        return time_info;
    }
    return NULL;
}

void print_time(struct tm *time_info) {
    if (asctime(time_info)) {
        printf("Current local time and date : %s\n", asctime(time_info));
    } else { printf("Error in asctime(time_info)"); }
}

void rewrite_time_in_file(struct tm *time_info) {
    FILE *saved_time = fopen("C:/Users/mojmi/CLionProjects/Mojmir/Learning/Terminal_To_Do_Manager/Saved Time", "wb");
    if (saved_time != NULL) {
        fwrite(time_info, sizeof(struct tm), 1, saved_time);
        fclose(saved_time);
    }
}

void print_if_times_passed(struct tm *time_info, struct tm *time_info_old) {
    bool if_year_passed = (time_info->tm_year > time_info_old->tm_year);
    printf("Did a year pass?  : %s[%d]\n", if_year_passed ? "Yes" : "No", if_year_passed);
    bool if_month_passed = (if_year_passed == true || time_info->tm_mon > time_info_old->tm_mon);
    printf("Did a month pass? : %s[%d]\n", if_month_passed ? "Yes" : "No", if_month_passed);
    bool if_week_passed = (if_month_passed == true || (time_info->tm_yday - time_info_old->tm_yday) >= 7);
    printf("Did a week pass?  : %s[%d]\n", if_week_passed ? "Yes" : "No", if_week_passed);
    bool if_day_passed = (if_week_passed == true || time_info->tm_yday > time_info_old->tm_yday);
    printf("Did a day pass?   : %s[%d]\n", if_day_passed ? "Yes" : "No", if_day_passed);
}

//ASK FOR INFO FUNTIONS//
int ask_task_info_all(FILE *saved_task_data) {
    int task_order = 0;
    task current_task;
    //Entering tasks to buffer file//
    printf(
        "\nEnter the name of your task and select it's 'frequency' ,'importance' and how many times do you need to complete the task in your chosen interval(frequency), after you have added all your tasks, please enter 'Done'.\n\n");
    printf("Please choose the name of your task:");
    while (fgets(current_task.task_create_name, sizeof(current_task.task_create_name), stdin)) {
        while (!is_name_valid(current_task.task_create_name)) {
            printf("\nPlease choose the name of your task:");
            fgets(current_task.task_create_name, sizeof(current_task.task_create_name), stdin);
        }
        // if (is_name_valid(current_task.task_create_name)) {
        //     break;
        // }
        current_task.task_is_completed = 0;
        current_task.task_create_name[strcspn(current_task.task_create_name, "\r\n")] = '\0';
        //If 'Done' is typed, this while loop will end//
        if (str_word(current_task.task_create_name, "Done") != NULL) {
            //check for 'Done' without whitespace//
            printf("\nEnd of adding tasks\n\n");
            task_order--;
            break;
        }

        //frequency of task
        char task_frequency_char[2];
        while (1) {
            printf("\nPlease choose the frequency of this task by typing the corresponding number"
                "(One and Done = 0, Daily = 1, Weekly = 2, Monthly = 3, Yearly = 4):");
            fgets(task_frequency_char, 10, stdin);
            if (is_frequency_valid(task_frequency_char[0])) {
                break;
            }
        }
        current_task.task_frequency = strtol(task_frequency_char, NULL, 10);

        //importance of task
        char task_importance_char[2];
        while (1) {
            printf("\nPlease choose the importance of this task by typing the corresponding number"
             "(Not_Important = 0, Low = 1, Default = 2, High = 3, Extremely_Important = 4):");
            fgets(task_importance_char, 10, stdin);
            if (is_importance_valid(task_importance_char[0])) {
                break;
            }
        }
        current_task.task_importance = strtol(task_importance_char, NULL, 10);

        //count of task
        char task_count_till_completion_char[2];
        while (1) {
            printf("\nPlease choose the number of time the task has to be completed for it's completion"
            " by typing the corresponding number:");
            fgets(task_count_till_completion_char, 10, stdin);
            if (is_count_valid(task_count_till_completion_char)) {
                break;
            }
        }
        current_task.task_count_till_completion = strtol(task_count_till_completion_char, NULL, 10);

        printf(
            "\nFrequency of this task: %d\nImportance of this task: "
            "%d\nCount till completion of this task: %d\nTask completion of this task: %d\n"
            , current_task.task_frequency, current_task.task_importance,
            current_task.task_count_till_completion, current_task.task_is_completed);

        fprintf(saved_task_data, "task_%d\nname:%s\nfrequency:%d\nimportance:%d\ncount:%d\ncompletion:%i\n\n",
                task_order, current_task.task_create_name, current_task.task_frequency, current_task.task_importance,
                current_task.task_count_till_completion, current_task.task_is_completed);

        printf(
            "\nEnter the name of your task and select it's 'frequency' and 'importance', "
            "after you have added all your tasks, please enter 'Done'.\n-");
        task_order++;
    }
    if (task_order >= 0) {
        return task_order;
    }
    if (task_order == -1) {
    printf("Warning: No tasks added\n");
        return task_order;
    }
    printf("Error in 'ask_task_info_all' - function: task_order below -1\n");
    return -1;
}

bool is_name_valid(char *name) {
    int sizeof_name = strlen(name); //sizeof includes '\n' because of stdin requiring user to press enter
    if (sizeof_name == 1) {
        return false;
    }
    for (int i = 0; i < sizeof_name; i++) {
        bool is_name_index_valid = !(name[i] == '\n' || name[i] == ' ');
        if (is_name_index_valid) {
            return true;
        }
    }
    return false;
}

bool is_frequency_valid(char frequency) {
    frequency = frequency - '0';
    switch (frequency) {
        case ONE_AND_DONE: break;
        case DAILY: break;
        case WEEKLY: break;
        case MONTHLY: break;
        case YEARLY: break;
        default: return false;
    }
    return true;
}

bool is_importance_valid(char importance) {
    importance = importance - '0';
    switch (importance) {
        case NOT_IMPORTANT: break;
        case LOW: break;
        case DEFAULT: break;
        case HIGH: break;
        case EXTREMELY_IMPORTANT: break;
        default: return false;
    }
    return true;
}

bool is_count_valid(char* count) {
    int count_int;
    if (count_int = strtol(count, NULL, 10)>0) {
        return true;
    }
    return false;
}

//GET FILE SIZE FUNCTION//
int get_file_size(FILE *file) {
    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (file_size >= 0) {
        return file_size;
    }
    printf("Error in 'get_file_size' - function: 'file_size' bellow 0");
    return -1;
}

//WRITING TO FILE FUNCTIONS//
int check_frequency(char *string, int current_frequency) {
    char *current_frequency_string[11];
    switch (current_frequency) {
        case 0: *current_frequency_string = "frequency:0";
            break;
        case 1: *current_frequency_string = "frequency:1";
            break;
        case 2: *current_frequency_string = "frequency:2";
            break;
        case 3: *current_frequency_string = "frequency:3";
            break;
        case 4: *current_frequency_string = "frequency:4";
            break;
        default: printf("Error in 'check_frequency' - function: 'current_frequency' is not a valid option\n");
            return -1;
    }
    char *checked_frequency = strstr(string, *current_frequency_string);
    if (checked_frequency) {
        return 1;
    }
    return 0;
}

void write_task_to_file(char *string, int current_task_order, FILE *file, int *current_frequency_order) {
    char *current_task_position = get_task_position(string, current_task_order);
    int current_task_frequency = get_task_frequency(current_task_position);
    if (current_task_frequency != *current_frequency_order) {
        return;
    }
    int task_name_size = get_task_name_size(current_task_position);
    char current_task_name_string[task_name_size + 1];
    get_task_name(current_task_position, task_name_size, current_task_name_string);
    int current_task_importance = get_task_importance(current_task_position);
    int current_task_count = get_task_count(current_task_position);
    int current_task_completion = get_task_completion(current_task_position);
    char *task_group_position_end = get_task_group_position_end(string, current_task_frequency);
    fprintf(file, "\n-[ ] %s\n", current_task_name_string);
}

char *get_task_position(char *string, int task_position) {
    char task_x[6 + sizeof(int) + 1] = "task_x"; //change "task_x" to "task_0", "task_1", ...
    sprintf(task_x, "task_%d", task_position);
    char *current_task_position = strstr(string, task_x);
    if (current_task_position) {
        return current_task_position;
    }
    printf("Error in 'current_task_position' - function: Couldn't find substring task_x\n");
    return NULL;
}

int get_task_name_size(char *current_task_position) {
    char *name_position_start = strstr(current_task_position, "name:") + 5;
    char *name_position_end = strstr(name_position_start, "\n");
    int task_name_size = name_position_end - name_position_start;
    if (task_name_size >= 0) {
        return task_name_size;
    }
    printf("Error in 'get_task_name_size' - function: Couldn't find size of task's name\n");
    return -1;
}

void get_task_name(char *current_task_position, int task_name_size, char *current_task_name) {
    char *name_position_start = strstr(current_task_position, "name:") + 5;
    int ending_position = 0;
    for (int i = 0; i <= task_name_size - 1; i++) {
        current_task_name[i] = name_position_start[i];
        ending_position++;
    }
    current_task_name[ending_position] = '\0';
}

int get_task_frequency(char *current_task_position) {
    char *frequency_position = strstr(current_task_position, "frequency:");
    int current_task_frequency = frequency_position[10] - 48;
    if (current_task_frequency == 0 || current_task_frequency == 1 || current_task_frequency == 2 ||
        current_task_frequency == 3 || current_task_frequency == 4) {
        return current_task_frequency;
    }
    printf("Error in 'get_task_frequency' - function: Couldn't find frequency of task\n");
    return -1;
}

int get_task_importance(char *current_task_position) {
    char *importance_position = strstr(current_task_position, "importance:") + 11;
    int current_task_importance = *importance_position - 48;
    if (current_task_importance == 0 || current_task_importance == 1 || current_task_importance == 2 ||
        current_task_importance == 3 || current_task_importance == 4) {
        return current_task_importance;
    }
    printf("Error in 'get_task_importance' - function: Couldn't find importance of task\n");
    return -1;
}

int get_task_count(char *current_task_position) {
    char *count_position = strstr(current_task_position, "count:") + 6;
    int current_task_count = *count_position - 48;
    if (current_task_count > 0) {
        return current_task_count;
    }
    printf("Error in 'get_task_count' - function: Couldn't find count of task\n");
    return -1;
}

int get_task_completion(char *current_task_position) {
    char *completion_position = strstr(current_task_position, "completion:") + 11;
    int current_task_completion = *completion_position - 48;
    if (current_task_completion == 0 || current_task_completion == 1) {
        return current_task_completion;
    }
    printf("Error in 'get_task_importance' - function: Couldn't find completion of task\n");
    return -1;
}

char *get_task_group_position_end(char *string, int current_task_frequency) {
    char *task_group_position;
    switch (current_task_frequency) {
        case 0: task_group_position = strstr(string, "One and Done:") + 13;
            break;
        case 1: task_group_position = strstr(string, "Daily:") + 6;
            break;
        case 2: task_group_position = strstr(string, "Weekly:") + 7;
            break;
        case 3: task_group_position = strstr(string, "Monthly:") + 8;
            break;
        case 4: task_group_position = strstr(string, "Yearly:") + 7;
            break;
        default: printf(
                "Error in 'get_task_group_position' - function: 'task_group_position' not a valid option\n");
            return NULL;
    }
    return task_group_position;
}


char *strcasestr(const char *str, const char *pattern) {
    size_t i;
    unsigned char c0 = *pattern, c1, c2;

    if (c0 == '\0')
        return (char *) str;

    c0 = toupper(c0);
    for (; (c1 = *str) != '\0'; str++) {
        if (toupper(c1) == c0) {
            for (i = 1;; i++) {
                c2 = pattern[i];
                if (c2 != '\0')
                    return (char *) str;
                c1 = str[i];
                if (toupper(c1) != toupper(c2))
                    break;
            }
        }
    }
    return NULL;
}

char *str_word(char *haystack, const char *needle) {
    char *from = haystack;
    size_t length = strlen(needle);
    char *found;
    while ((found = strstr(from, needle)) != NULL) {
        if (found > haystack && isalpha((unsigned char) found[-1]))
            from += length;
        else if (isalpha((unsigned char) found[length]))
            from += length;
        else
            return found;
    }
    return NULL;
}

void fprint_frequency(FILE *file, int current_frequency) {
    switch (current_frequency) {
        case 0: fprintf(file, "OneTime:\n");
            break;
        case 1: fprintf(file, "Daily:\n");
            break;
        case 2: fprintf(file, "Weekly:\n");
            break;
        case 3: fprintf(file, "Monthly:\n");
            break;
        case 4: fprintf(file, "Yearly:\n");
            break;
        default: printf("Error in 'fprint_frequency' - function: 'current_frequency' is not a valid option\n");
    }
}

//END - for bookmark
