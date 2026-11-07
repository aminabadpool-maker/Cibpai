#ifndef CIBPAI_H
#define CIBPAI_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char *video_path;
    char *output_dir;
    int target_fps;
    char *api_key;
} CibpaiConfig;

int cibpai_init(CibpaiConfig *config);
int cibpai_process_frames(CibpaiConfig *config);
int cibpai_send_frame_to_ai(const char *frame_path, const char *api_key);

#endif // CIBPAI_H
