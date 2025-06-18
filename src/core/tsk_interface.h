#ifndef TSK_INTERFACE_H
#define TSK_INTERFACE_H

#include "analyzer.h"
#include <stddef.h>     // Für size_t
#include <stdbool.h>

typedef int (*th_ai_callback)(const char *file_path, const char *prompt);

void tsk_set_ai_callback(th_ai_callback cb);

bool is_filetype_allowed(const char *filename, const Scope *scope);
bool is_path_allowed(const char *path, const Scope *scope);
bool contains_keywords(const char *filepath, const Scope *scope);
bool is_within_date_range(const char *filepath, const Scope *scope);
bool is_within_size_limit(const char *filepath, const Scope *scope);
void compute_sha256(const char *filepath, char *output_hash, size_t hash_size);
void analyze_partition(const char *image_path, const Scope *scope);
int find_ntfs_offset(const char *image_path);
void list_all_files(const char *image_path, int offset, const Scope *scope);


#endif
