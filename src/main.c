#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "cibpai.h"

int cibpai_process_directory(const char *output_dir, const char *api_key) {
    DIR *dir;
    struct dirent *entry;
    char filepath[1024];

    dir = opendir(output_dir);
    if (!dir) {
        fprintf(stderr, "[-] Error: Could not open output directory %s\n", output_dir);
        return -1;
    }

    printf("[*] Starting AI processing pipeline for frames in %s...\n", output_dir);

    while ((entry = readdir(dir)) != NULL) {
        // فقط فایل‌های PNG مربوط به فریم‌ها را انتخاب می‌کنیم
        if (strstr(entry->d_name, ".png") != NULL) {
            snprintf(filepath, sizeof(filepath), "%s/%s", output_dir, entry->d_name);
            
            if (cibpai_send_frame_to_ai(filepath, api_key) != 0) {
                fprintf(stderr, "[-] Warning: Failed to process frame: %s\n", filepath);
            }
        }
    }

    closedir(dir);
    return 0;
}

int main(int argc, char *argv[]) {
    printf("=======================================\n");
    printf("       Cibpai: C is Best for AI        \n");
    printf("=======================================\n");

    if (argc < 3) {
        printf("Usage: %s <input_video> <output_directory>\n", argv[0]);
        return 1;
    }

    CibpaiConfig config = {
        .video_path = argv[1],
        .output_dir = argv[2],
        .target_fps = 24,
        .api_key = getenv("AI_API_KEY")
    };

    if (cibpai_process_frames(&config) == 0) {
        printf("[+] Frame extraction completed. Handing over to AI module...\n");
        cibpai_process_directory(config.output_dir, config.api_key);
        printf("[+] Cibpai full pipeline executed successfully.\n");
    } else {
        printf("[-] Error executing frame extraction phase.\n");
        return 1;
    }

    return 0;
}
