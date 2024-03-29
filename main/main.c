#include <stdio.h>
#include "backprop_tools_adapter.h"
#include <stdbool.h>
#include "esp_timer.h"


#define BACKPROP_TOOLS_OUTPUT_DIM 4
#define BACKPROP_TOOLS_BATCH_SIZE 1
float output_mem[BACKPROP_TOOLS_BATCH_SIZE * BACKPROP_TOOLS_OUTPUT_DIM];
void app_main(void)
{
    float abs_diff = backprop_tools_run_single_sample_check_output((float*)output_mem);
    for(int batch_i = 0; batch_i < BACKPROP_TOOLS_BATCH_SIZE; batch_i++){
        for(int col_i = 0; col_i < BACKPROP_TOOLS_OUTPUT_DIM; col_i++){
            printf("%+5.5f ", output_mem[batch_i * BACKPROP_TOOLS_OUTPUT_DIM + col_i]);
        }
        printf("\n");
    }
    printf("Absolute difference: %f\n", abs_diff);

    // to verify the accuracy of the internal timer:
    {
        int64_t start = esp_timer_get_time();
        int interval = 10000000;
        int current_interval = 0;
        while(true){
            int64_t current = esp_timer_get_time();
            if (current_interval * interval < current - start) {
                current_interval++;
                printf("Time taken: %lld us\n", current - start);
            }
        }
    }

    int num_runs = 1000;
    while(true){
        printf("Starting %d runs\n", num_runs);
        int64_t start = esp_timer_get_time();
        for(int i = 0; i < num_runs; i++){
            backprop_tools_run_single_sample((float*)output_mem);
        }
        int64_t end = esp_timer_get_time();
        printf("Time taken: %lld us\n", end - start);
        printf("Frequency: %f Hz\n", num_runs / ((end - start) / 1000000.0));
    }
}