#include "tracehunter_api.h"
#include "../scope/scope_parser.h"
#include "../core/tsk_interface.h"
#include <stdio.h>

static th_ai_callback g_ai_cb = NULL;

void th_set_ai_callback(th_ai_callback cb) {
    g_ai_cb = cb;
    tsk_set_ai_callback(cb); /* propagate to tsk module */
}

int th_run_analysis(const char *image_path, const char *scope_json) {
    Scope scope;
    if (!load_scope(scope_json, &scope)) {
        fprintf(stderr, "[!] Failed to load scope from %s\n", scope_json);
        return 0;
    }

    int offset = find_ntfs_offset(image_path);
    if (offset < 0) {
        fprintf(stderr, "[!] Could not determine NTFS offset\n");
        return 0;
    }

    list_all_files(image_path, offset, &scope);
    return 1;
}
