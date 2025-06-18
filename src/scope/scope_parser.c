#include "scope_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <jansson.h>
#include "analyzer.h"

int generate_scope_json(const char *input_txt, const char *output_json) {
    FILE *in = fopen(input_txt, "r");
    if (!in) {
        perror("Fehler beim Öffnen der Eingabedatei");
        return 0;
    }

    json_t *root = json_object();
    char line[1024], key[256], val[768];

    while (fgets(line, sizeof(line), in)) {
        if (sscanf(line, "%[^=]=%[^\n]", key, val) != 2) continue;

        if (strcmp(key, "case_id") == 0) {
            json_object_set_new(root, "case_id", json_string(val));
        } else if (strcmp(key, "filetypes") == 0) {
            json_t *arr = json_array();
            char *tok = strtok(val, ",");
            while (tok) {
                json_array_append_new(arr, json_string(tok));
                tok = strtok(NULL, ",");
            }
            json_object_set_new(root, "filters", json_object());
            json_object_set_new(json_object_get(root, "filters"), "filetypes", arr);
        } else if (strcmp(key, "keywords") == 0) {
            json_t *arr = json_array();
            char *tok = strtok(val, ",");
            while (tok) {
                json_array_append_new(arr, json_string(tok));
                tok = strtok(NULL, ",");
            }
            json_object_set_new(json_object_get(root, "filters"), "keywords", arr);
        } else if (strcmp(key, "created_after") == 0) {
            json_object_set_new(json_object_get(root, "filters"), "date_range", json_object());
            json_object_set_new(json_object_get(json_object_get(root, "filters"), "date_range"),
                                "created_after", json_string(val));
        } else if (strcmp(key, "created_before") == 0) {
            json_object_set_new(json_object_get(json_object_get(root, "filters"), "date_range"),
                                "created_before", json_string(val));
        } else if (strcmp(key, "max_file_size_mb") == 0) {
            json_object_set_new(json_object_get(root, "filters"), "max_file_size_mb", json_integer(atoi(val)));
        } else if (strcmp(key, "target_paths") == 0) {
            json_t *arr = json_array();
            char *tok = strtok(val, ",");
            while (tok) {
                json_array_append_new(arr, json_string(tok));
                tok = strtok(NULL, ",");
            }
            json_object_set_new(json_object_get(root, "filters"), "target_paths", arr);
        } else if (strcmp(key, "report_format") == 0 ||
                   strcmp(key, "include_hashes") == 0 ||
                   strcmp(key, "store_matched_files") == 0) {
            json_t *output = json_object_get(root, "output");
            if (!output) {
                output = json_object();
                json_object_set_new(root, "output", output);
            }

            if (strcmp(key, "report_format") == 0) {
                json_object_set_new(output, "report_format", json_string(val));
            } else if (strcmp(key, "include_hashes") == 0) {
                json_object_set_new(output, "include_hashes", json_boolean(strcasecmp(val, "ja") == 0));
            } else if (strcmp(key, "store_matched_files") == 0) {
                json_object_set_new(output, "store_matched_files", json_boolean(strcasecmp(val, "ja") == 0));
            }
        } else if (strcmp(key, "ai_prompt") == 0) {
            json_object_set_new(root, "ai_prompt", json_string(val));
        }
    }

    fclose(in);

    FILE *out = fopen(output_json, "w");
    if (!out) {
        perror("Fehler beim Öffnen der Ausgabedatei");
        json_decref(root);
        return 0;
    }

    json_dumpf(root, out, JSON_INDENT(2));
    fclose(out);
    json_decref(root);
    return 1;
}

int load_scope(const char *json_path, Scope *scope) {
    return load_scope_from_json(json_path, scope);
}
