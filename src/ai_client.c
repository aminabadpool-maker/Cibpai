#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "cibpai.h"

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    (void)userp;
    printf("%.*s", (int)realsize, (char *)contents);
    return realsize;
}

// جدول انکودینگ استاندارد Base64
static const char base64_chars[] = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

// تابع Base64 Encoder مستقل و ایمن در C
static char *encode_to_base64(const unsigned char *data, size_t input_length, size_t *output_length) {
    size_t encoded_length = 4 * ((input_length + 2) / 3);
    char *encoded_data = malloc(encoded_length + 1);
    if (!encoded_data) return NULL;

    size_t i, j;
    for (i = 0, j = 0; i < input_length;) {
        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = base64_chars[(triple >> 18) & 0x3F];
        encoded_data[j++] = base64_chars[(triple >> 12) & 0x3F];
        encoded_data[j++] = (i > input_length + 1) ? '=' : base64_chars[(triple >> 6) & 0x3F];
        encoded_data[j++] = (i > input_length) ? '=' : base64_chars[triple & 0x3F];
    }

    encoded_data[j] = '\0';
    if (output_length) *output_length = encoded_length;
    return encoded_data;
}

static char *encode_file_to_base64(const char *filename, size_t *out_len) {
    FILE *f = fopen(filename, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    unsigned char *buffer = malloc(fsize);
    if (!buffer) {
        fclose(f);
        return NULL;
    }
    size_t read_bytes = fread(buffer, 1, fsize, f);
    fclose(f);

    if (read_bytes != (size_t)fsize) {
        free(buffer);
        return NULL;
    }

    char *b64 = encode_to_base64(buffer, fsize, out_len);
    free(buffer);
    return b64;
}

int cibpai_send_frame_to_ai(const char *frame_path, const char *api_key) {
    CURL *curl;
    CURLcode res;
    long response_code = 0;

    if (!api_key) {
        fprintf(stderr, "[-] Error: AI_API_KEY environment variable is not set.\n");
        return -1;
    }

    size_t b64_len = 0;
    char *b64_image = encode_file_to_base64(frame_path, &b64_len);
    if (!b64_image) {
        fprintf(stderr, "[-] Error: Failed to encode image to base64: %s\n", frame_path);
        return -1;
    }

    size_t json_size = b64_len + 1024;
    char *json_payload = malloc(json_size);
    if (!json_payload) {
        free(b64_image);
        return -1;
    }

    snprintf(json_payload, json_size,
        "{\n"
        "  \"contents\": [\n"
        "    {\n"
        "      \"parts\": [\n"
        "        {\"text\": \"Describe this video frame concisely:\"},\n"
        "        {\n"
        "          \"inline_data\": {\n"
        "            \"mime_type\": \"image/png\",\n"
        "            \"data\": \"%s\"\n"
        "          }\n"
        "        }\n"
        "      ]\n"
        "    }\n"
        "  ]\n"
        "}", b64_image);

    free(b64_image);

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if(curl) {
        struct curl_slist *headerlist = NULL;
        char url[512];

        snprintf(url, sizeof(url), "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.5-flash:generateContent?key=%s", api_key);

        headerlist = curl_slist_append(headerlist, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        printf("[*] Sending frame %s to Gemini API (JSON Payload)...\n", frame_path);
        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            fprintf(stderr, "[-] curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            free(json_payload);
            curl_slist_free_all(headerlist);
            return -1;
        }

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        printf("\n[+] Frame processed. HTTP Status: %ld\n", response_code);

        curl_easy_cleanup(curl);
        curl_slist_free_all(headerlist);
    }

    free(json_payload);
    curl_global_cleanup();
    return 0;
}
