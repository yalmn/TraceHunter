#ifndef ANALYZER_H
#define ANALYZER_H

#include <stdbool.h>

#define MAX_FILETYPES 10
#define MAX_KEYWORDS 20
#define MAX_PATHS 10

typedef struct {
    char created_after[16];
    char created_before[16];
} DateRange;

typedef struct {
    char case_id[64];
    char filetypes[MAX_FILETYPES][16];
    int filetype_count;

    char keywords[MAX_KEYWORDS][64];
    int keyword_count;

    DateRange date_range;

    int max_file_size_mb;

    char target_paths[MAX_PATHS][256];
    int target_path_count;

    char report_format[16];
    bool include_hashes;
    bool store_matched_files;

    char ai_prompt[512];
} Scope;

int load_scope_from_json(const char *json_path, Scope *scope);
void print_scope(const Scope *scope);

#endif
