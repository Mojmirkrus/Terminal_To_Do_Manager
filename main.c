#include "common/used_functions.c"

int main(void) {
    setbuf(stdout, 0); //just to be able to see prints in terminal when debugging
    /*----Check for time, save current time and remove completion
        if past the time period while also saving the time to file----*/
    //load old time//
    struct tm *time_info_old = get_and_print_old_time();
    //get new current time//
    struct tm *time_info = get_new_time();
    print_time(time_info);
    rewrite_time_in_file(time_info);
    //save current time to file//

    //Checking if each time period passed//  //'struct tm time_info_old' compared to 'struct tm time_info'//
    print_if_times_passed(time_info, time_info_old);
    //freeing memory allocated with malloc and NULL-ing pointers to this memory//
    free(time_info_old);
    time_info_old = NULL;
    free(time_info);
    time_info = NULL;

    //---TASK PART---//
    // printf("\nCommands:\n"); //Available options
    int OneAndDone_quant = 0;
    int Daily_quant = 0;
    int Weekly_quant = 0;
    int Monthly_quant = 0;
    int Yearly_quant = 0; 

    FILE *saved_task_data = fopen(
        "C:/Users/mojmi/CLionProjects/Mojmir/Learning/Terminal_To_Do_Manager/saved_task_data", "r+");

    int task_order = ask_task_info_all(saved_task_data);

    //--WRITING THE TO DO LIST--//
    FILE *To_Do_List = fopen("C:/Users/mojmi/CLionProjects/Mojmir/Learning/Terminal_To_Do_Manager/ToDoList", "w+");
    int To_Do_List_file_size = get_file_size(To_Do_List);
    int saved_task_data_size = get_file_size(saved_task_data);
    char *To_Do_List_String = malloc(To_Do_List_file_size + saved_task_data_size);

    if (saved_task_data) {
        fread(To_Do_List_String, 1, saved_task_data_size, saved_task_data);
    }
    
    fclose(saved_task_data);
    for (int current_frequency_order = 0; current_frequency_order <= 4; current_frequency_order++) {
        if (check_frequency(To_Do_List_String, current_frequency_order)) {
            fprint_frequency(To_Do_List, current_frequency_order);
            for (int i = 0; i <= task_order; i++) {
                write_task_to_file(To_Do_List_String, i, To_Do_List, &current_frequency_order);
            }
            fprintf(To_Do_List, "\n");
        }
    }
    free(To_Do_List_String);
    To_Do_List_String = NULL;
    fclose(To_Do_List);
    return 0;
}

