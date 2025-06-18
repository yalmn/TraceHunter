#include "tsk_interface.h"
#include <tsk/libtsk.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>


bool is_filetype_allowed(const char *filename, const Scope *scope) {
    for (int i = 0; i < scope->filetype_count; i++) {
        const char *ext = strrchr(filename, '.');
        if (ext && strcmp(ext, scope->filetypes[i]) == 0)
            return true;
    }
    return false;
}

bool is_path_allowed(const char *path, const Scope *scope) {
    for (int i = 0; i < scope->target_path_count; i++) {
        if (strstr(path, scope->target_paths[i]) != NULL)
            return true;
    }
    return false;
}

bool contains_keywords(const char *filepath, const Scope *scope) {
    FILE *f = fopen(filepath, "r");
    if (!f) return false;

    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        for (int i = 0; i < scope->keyword_count; i++) {
            if (strstr(line, scope->keywords[i]) != NULL) {
                fclose(f);
                return true;
            }
        }
    }
    fclose(f);
    return false;
}

static time_t parse_date(const char *date_str) {
    struct tm tm = {0};
    strptime(date_str, "%Y-%m-%d", &tm);
    return mktime(&tm);
}

bool is_within_date_range(const char *filepath, const Scope *scope) {
    struct stat st;
    if (stat(filepath, &st) != 0) return false;

    time_t after = parse_date(scope->date_range.created_after);
    time_t before = parse_date(scope->date_range.created_before);

    return (st.st_ctime >= after && st.st_ctime <= before);
}

bool is_within_size_limit(const char *filepath, const Scope *scope) {
    struct stat st;
    if (stat(filepath, &st) != 0) return false;

    return (st.st_size <= scope->max_file_size_mb * 1024 * 1024);
}

void compute_sha256(const char *filepath, char *output_hash, size_t hash_size) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        strncpy(output_hash, "FILE_OPEN_ERROR", hash_size);
        return;
    }

    SHA256_CTX ctx;
    SHA256_Init(&ctx);

    unsigned char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), file)) != 0) {
        SHA256_Update(&ctx, buffer, bytes);
    }

    fclose(file);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &ctx);

    for (int i = 0; i < SHA256_DIGEST_LENGTH && i*2+1 < hash_size; i++) {
        sprintf(output_hash + i*2, "%02x", hash[i]);
    }
}

void analyze_partition(const char *image_path, const Scope *scope) {
    printf("[i] Analyse von: %s (Dummy-Funktion: implementiere TSK-Laufwerke)\n", image_path);
    // Hier folgt später die Logik mit TSK: Öffne Image, iteriere durch Filesystem, prüfe Regeln.
}

// Platzhalterfunktionen, damit das Projekt kompilierbar bleibt
int find_ntfs_offset(const char *image_path) {
    (void)image_path;
    printf("[w] find_ntfs_offset: Dummy-Implementierung gibt 0 zurück.\n");
    return 0;
}

void list_all_files(const char *image_path, int offset, const Scope *scope) {
    (void)scope;
    printf("[w] list_all_files: Dummy-Implementierung (%s @ %d).\n", image_path, offset);
    // Hier könnte TSK verwendet werden, um Dateien aufzulisten
}
