#include "analyzer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

int load_scope_from_json(const char *json_path, Scope *scope) {
    FILE *f = fopen(json_path, "r");
    if (!f) {
        perror("Fehler beim Öffnen von scope.json");
        return 0;
    }

    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *data = malloc(len + 1);
    if (!data) {
        fclose(f);
        return 0;
    }

    fread(data, 1, len, f);
    data[len] = '\0';
    fclose(f);

    cJSON *root = cJSON_Parse(data);
    free(data);
    if (!root) {
        fprintf(stderr, "Fehler beim Parsen von JSON\n");
        return 0;
    }

    cJSON *case_id = cJSON_GetObjectItem(root, "case_id");
    if (case_id && cJSON_IsString(case_id)) {
        strncpy(scope->case_id, case_id->valuestring, sizeof(scope->case_id));
    }

    cJSON *filters = cJSON_GetObjectItem(root, "filters");
    if (filters) {
        cJSON *filetypes = cJSON_GetObjectItem(filters, "filetypes");
        scope->filetype_count = 0;
        if (filetypes && cJSON_IsArray(filetypes)) {
            int count = cJSON_GetArraySize(filetypes);
            for (int i = 0; i < count && i < MAX_FILETYPES; i++) {
                cJSON *item = cJSON_GetArrayItem(filetypes, i);
                if (cJSON_IsString(item)) {
                    strncpy(scope->filetypes[i], item->valuestring, 15);
                    scope->filetype_count++;
                }
            }
        }

        cJSON *keywords = cJSON_GetObjectItem(filters, "keywords");
        scope->keyword_count = 0;
        if (keywords && cJSON_IsArray(keywords)) {
            int count = cJSON_GetArraySize(keywords);
            for (int i = 0; i < count && i < MAX_KEYWORDS; i++) {
                cJSON *item = cJSON_GetArrayItem(keywords, i);
                if (cJSON_IsString(item)) {
                    strncpy(scope->keywords[i], item->valuestring, 63);
                    scope->keyword_count++;
                }
            }
        }

        cJSON *date_range = cJSON_GetObjectItem(filters, "date_range");
        if (date_range) {
            cJSON *after = cJSON_GetObjectItem(date_range, "created_after");
            cJSON *before = cJSON_GetObjectItem(date_range, "created_before");
            if (after && cJSON_IsString(after)) {
                strncpy(scope->date_range.created_after, after->valuestring, 15);
            }
            if (before && cJSON_IsString(before)) {
                strncpy(scope->date_range.created_before, before->valuestring, 15);
            }
        }

        cJSON *max_size = cJSON_GetObjectItem(filters, "max_file_size_mb");
        if (max_size && cJSON_IsNumber(max_size)) {
            scope->max_file_size_mb = max_size->valueint;
        }

        cJSON *paths = cJSON_GetObjectItem(filters, "target_paths");
        scope->target_path_count = 0;
        if (paths && cJSON_IsArray(paths)) {
            int count = cJSON_GetArraySize(paths);
            for (int i = 0; i < count && i < MAX_PATHS; i++) {
                cJSON *item = cJSON_GetArrayItem(paths, i);
                if (cJSON_IsString(item)) {
                    strncpy(scope->target_paths[i], item->valuestring, 255);
                    scope->target_path_count++;
                }
            }
        }
    }

    cJSON *output = cJSON_GetObjectItem(root, "output");
    if (output) {
        cJSON *format = cJSON_GetObjectItem(output, "report_format");
        if (format && cJSON_IsString(format)) {
            strncpy(scope->report_format, format->valuestring, 15);
        }

        cJSON *hashes = cJSON_GetObjectItem(output, "include_hashes");
        scope->include_hashes = hashes && cJSON_IsTrue(hashes);

        cJSON *store = cJSON_GetObjectItem(output, "store_matched_files");
        scope->store_matched_files = store && cJSON_IsTrue(store);
    }

    cJSON *ai_prompt = cJSON_GetObjectItem(root, "ai_prompt");
    if (ai_prompt && cJSON_IsString(ai_prompt)) {
        strncpy(scope->ai_prompt, ai_prompt->valuestring, sizeof(scope->ai_prompt));
    }

    cJSON_Delete(root);
    return 1;
}

void print_scope(const Scope *scope) {
    printf("Fallnummer: %s\n", scope->case_id);
    printf("Dateitypen:\n");
    for (int i = 0; i < scope->filetype_count; i++) {
        printf("  - %s\n", scope->filetypes[i]);
    }
    printf("Stichwörter:\n");
    for (int i = 0; i < scope->keyword_count; i++) {
        printf("  - %s\n", scope->keywords[i]);
    }
    printf("Zeitraum: %s bis %s\n", scope->date_range.created_after, scope->date_range.created_before);
    printf("Max. Dateigröße: %d MB\n", scope->max_file_size_mb);
    printf("Pfad(e):\n");
    for (int i = 0; i < scope->target_path_count; i++) {
        printf("  - %s\n", scope->target_paths[i]);
    }
    printf("Reportformat: %s\n", scope->report_format);
    printf("Hashes: %s\n", scope->include_hashes ? "ja" : "nein");
    printf("Dateien speichern: %s\n", scope->store_matched_files ? "ja" : "nein");
    if (strlen(scope->ai_prompt) > 0)
        printf("KI-Prompt: %s\n", scope->ai_prompt);
}
