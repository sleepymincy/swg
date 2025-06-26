// io.c - load_file, load_string, flush_outbuf, prompt_user

#include "io.h"

// -f or --file logic here:
void load_file(const char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) { // <- if file does not exist or isn't accessible, tell user, then exit
        fprintf(stderr, "Error: cannot open '%s': %s\n", path, strerror(errno)); 
        exit(EX_NOINPUT); 
    } else if (!quiet) { // <- otherwise continue
        printf("Input: '%s'\n", path); 
    }

    char *line = NULL;
    size_t  bufsize = 0;
    ssize_t linelen;
    size_t  linenum = 0;

    while ((linelen = getline(&line, &bufsize, fp)) != -1) {
        linenum++;
        while (linelen > 0 && (line[linelen-1] == '\n' || line[linelen-1] == '\r')) { line[--linelen] = '\0'; } // <- strip trailing newline and CR
        if (linelen == 0) continue; // <- skip empty lines

        // If line is above character limit, skip it completely:
        if (linelen >= MAX_WORD_LEN) {
            if (!quiet) { fprintf(stderr, "Warning: line %zu in '%s' is %zd chars (>= %d); skipping\n", linenum, path, linelen, MAX_WORD_LEN); }
            continue;
        }

        // Enforce word limit:
        if (word_count >= MAX_WORDS) {
            if (!quiet) { fprintf(stderr, "Error: too many words (max %d); stopping read\n", MAX_WORDS); }
            break;
        }

        char *dup = strdup(line);
        if (!dup) { fprintf(stderr, "Error: strdup error\n" "Exiting...\n"); exit(EX_OSERR); } // <- in case strdup returns NULL, ref: https://pubs.opengroup.org/onlinepubs/9699919799/functions/strdup.html
        words[word_count++] = dup;
    }

    // Cleanup:
    free(line);
    fclose(fp);
}

// -w or --words logic here:
void load_string(const char *word) {
    // Make modifiable copy so strtok can insert NULs:
    char *copy = strdup(word);
    if (!copy) { fprintf(stderr, "Error: strdup error\n" "Exiting...\n"); exit(EX_OSERR); }
    if (!quiet) { printf("Input: '%s'\n", copy); }

    char *tok = strtok(copy, ",");
    size_t tok_index = 0;
    while (tok) {
        tok_index++;

        // Strip leading/trailing spaces or CR/LF:
        tok[strcspn(tok, " \r\n")] = '\0';
        size_t len = strlen(tok);

        if (len == 0) {
            if (!quiet) { fprintf(stderr, "Warning: word #%zu is empty; skipping\n", tok_index); }
        } else if (len >= MAX_WORD_LEN) {
            if (!quiet) { fprintf(stderr, "Warning: word #%zu ('%.8s...') exceeds %d characters; skipping\n", tok_index, tok, MAX_WORD_LEN); }
        } else if (word_count >= MAX_WORDS) {
            fprintf(stderr, "Error: too many words (max %d); stopping read\n", MAX_WORDS);
            break;
        } else {
            char *dup = strdup(tok);
            if (!dup) { fprintf(stderr, "Error: strdup error\n" "Exiting...\n"); exit(EX_OSERR); }
            words[word_count++] = dup;
        }
        tok = strtok(NULL, ",");
    }

    // Cleanup:
    free(copy);
}

// If output estimate is too big, make user question their decisions:
int prompt_user() {
    char buf[8];
    printf(
      "Warning: estimated wordlist size is way too big!\n"
      "This may fill your disk or take a very long time.\n"
      "Proceed anyway? (y/N): "
    );
    if (!fgets(buf, sizeof(buf), stdin)) return 0; // <- on input error or EOF, treat as “no”
    return (buf[0]=='y' || buf[0]=='Y');
}

// Flush to disk or STDOUT:
void flush_outbuf(FILE *fp) {
    if (outbuf_used == 0) return; // <- if buffer is empty, nothing to do
    size_t written = fwrite(outbuf, 1, outbuf_used, fp);
    if (written < outbuf_used) {
        fprintf(stderr, "Error: write failed: %s\n", strerror(errno)); // <- if write fails or only wrote partially, report why
        exit(EX_IOERR); // <- return "input/output error" exit code
    }
    outbuf_used = 0; // <- reset buffer counter
}