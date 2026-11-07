#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cibpai.h"

int cibpai_process_frames(CibpaiConfig *config) {
    char command[512];
    
    // ساخت دستور ffmpeg برای خرد کردن ویدیو به فریم‌های PNG با سرعت بالا
    snprintf(command, sizeof(command),
             "ffmpeg -i \"%s\" -vf fps=%d \"%s/frame_%%04d.png\" -loglevel quiet",
             config->video_path, config->target_fps, config->output_dir);
    
    printf("[*] Running FFmpeg extraction: %s\n", command);
    
    int result = system(command);
    if (result == 0) {
        printf("[+] Frames extracted successfully into: %s\n", config->output_dir);
        return 0;
    } else {
        fprintf(stderr, "[-] Error: Failed to extract frames.\n");
        return -1;
    }
}
