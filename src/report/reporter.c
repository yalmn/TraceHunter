#include "reporter.h"
#include <stdio.h>

void generate_html_report(const ReportEntry *entries, size_t count, const char *output_file) {
    FILE *f = fopen(output_file, "w");
    if (!f) {
        perror("[!] Fehler beim Erstellen des Reports");
        return;
    }

    fprintf(f,
        "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n"
        "<title>TraceHunter Report</title>\n"
        "<style>\n"
        "table { border-collapse: collapse; width: 100%%; }\n"
        "th, td { border: 1px solid #ccc; padding: 8px; text-align: left; }\n"
        "th { background-color: #f2f2f2; }\n"
        "</style>\n</head>\n<body>\n"
        "<h1>TraceHunter Analysebericht</h1>\n"
        "<p>Gefundene Dateien: %zu</p>\n"
        "<table>\n<tr><th>Pfad im Image</th><th>Inode</th><th>Größe</th><th>SHA256</th><th>Extrahiert nach</th></tr>\n",
        count
    );

    for (size_t i = 0; i < count; i++) {
        fprintf(f,
            "<tr><td>%s</td><td>%d</td><td>%ld</td><td>%s</td><td>%s</td></tr>\n",
            entries[i].image_path,
            entries[i].inode,
            entries[i].size_bytes,
            entries[i].sha256[0] ? entries[i].sha256 : "-",
            entries[i].extracted_path
        );
    }

    fprintf(f, "</table>\n</body>\n</html>\n");
    fclose(f);

    printf("[+] HTML-Report erfolgreich erstellt: %s\n", output_file);
}
