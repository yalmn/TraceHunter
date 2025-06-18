#ifndef REPORTER_H
#define REPORTER_H

#include <stddef.h>

/**
 * Struktur für einen Dateitreffer im Report.
 */
typedef struct {
    char image_path[512];         // Pfad im Image (z. B. /Users/.../file.docx)
    int inode;                    // Inode-Nummer im Image
    long size_bytes;              // Dateigröße in Bytes
    char extracted_path[512];     // Zielpfad der extrahierten Datei
    char sha256[65];              // SHA256-Hash (64 Hex-Zeichen + Nullterminator)
    int keyword_match;            // 1 = Treffer bei Keyword-Suche, 0 = kein Treffer
    char matched_keywords[512];   // Kommagetrennte Liste der gefundenen Stichwörter
} ReportEntry;

/**
 * Erzeugt einen HTML-Report aus allen übergebenen Report-Einträgen.
 */
void generate_html_report(const ReportEntry *entries, size_t count, const char *output_file);

#endif
