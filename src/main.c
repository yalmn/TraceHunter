#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core/analyzer.h"
#include "core/tsk_interface.h"
#include "core/fileutils.h"
#include "scope/scope_parser.h"


void print_usage(const char *prog) {
    printf("Usage:\n");
    printf("  %s <image.dd> <scope_input.txt>\n", prog);
    printf("Beispiel:\n");
    printf("  %s input/image.dd input/scope_input.txt\n", prog);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        print_usage(argv[0]);
        return 1;
    }

    const char *image_path = argv[1];
    const char *scope_input_txt = argv[2];
    const char *scope_output_json = "input/scope.json";

    printf("[*] Starte TraceHunter Analyse\n");
    printf("[*] Disk-Image: %s\n", image_path);
    printf("[*] Scope-Datei: %s\n", scope_input_txt);

    // 1. Scope generieren (TXT → JSON)
    if (!generate_scope_json(scope_input_txt, scope_output_json)) {
        fprintf(stderr, "[!] Fehler beim Parsen von %s\n", scope_input_txt);
        return 2;
    }

    // 2. JSON laden
    Scope scope;
    if (!load_scope(scope_output_json, &scope)) {
        fprintf(stderr, "[!] Fehler beim Laden des Scopes aus JSON\n");
        return 3;
    }

    // 3. Offset ermitteln
    int offset = find_ntfs_offset(image_path);
    if (offset < 0) {
        fprintf(stderr, "[!] Kein gültiger Offset gefunden – Abbruch.\n");
        return 4;
    }

    // 4. Dateianalyse starten
    list_all_files(image_path, offset, &scope);

    printf("[✓] Analyse abgeschlossen.\n");
    printf("[→] Bericht unter: output/tracehunter_report.html\n");

    return 0;
}
