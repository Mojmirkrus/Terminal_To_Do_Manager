#ifndef used_functions
#define used_functions

//STRUCTS//
typedef struct {
    char task_create_name[100];
    int task_frequency;
    int task_importance;
    int task_count_till_completion;
    bool task_is_completed;
} task;

//ENUMS//
typedef enum { ONE_AND_DONE = 0, DAILY = 1, WEEKLY = 2, MONTHLY = 3, YEARLY = 4 } frequency;

typedef enum { NOT_IMPORTANT = 0, LOW = 1, DEFAULT = 2, HIGH = 3, EXTREMELY_IMPORTANT = 4 } importance;

//TIME FUNCTIONS//TIME_UTILS//
struct tm *get_and_print_old_time();

struct tm *get_new_time();

void print_time(struct tm *time_info);

void rewrite_time_in_file(struct tm *time_info);

void print_if_times_passed(struct tm *time_info, struct tm *time_info_old);

//ASK FOR INFO FUNCTIONS//
int ask_task_info_all(FILE *saved_task_data);

bool is_name_valid(char *name);

bool is_frequency_valid(char frequency);

bool is_importance_valid(char importance);

bool is_count_valid(char* count);

//GET FILE SIZE FUNCTION//
int get_file_size(FILE *file);

//WRITING TO FILE FUNCTIONS//
void fprint_frequency(FILE *file, int current_frequency);

int check_frequency(char *string, int current_frequency);

void write_task_to_file(char *string, int current_task_order, FILE *file, int *current_frequency_order);

char *get_task_position(char *string, int task_position);

int get_task_name_size(char *current_task_position);

void get_task_name(char *current_task_position, int task_name_size, char *current_task_name);

int get_task_frequency(char *current_task_position);

int get_task_importance(char *current_task_position);

int get_task_count(char *current_task_position);

int get_task_completion(char *current_task_position);

char *get_task_group_position_end(char *string, int current_task_frequency);

//not my functions/from others
char *strcasestr(const char *str, const char *pattern);

char *str_word(char *haystack, const char *needle);

#endif