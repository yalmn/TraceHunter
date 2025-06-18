#ifndef SCOPE_PARSER_H
#define SCOPE_PARSER_H

#include "../core/analyzer.h"

int generate_scope_json(const char *txt_input_path, const char *json_output_path);
int load_scope(const char *json_path, Scope *scope);

#endif
