// main.c - main

#include "swg.h"
#include "args.h"
#include "io.h"
#include "generator.h"

char   *words[MAX_WORDS];
size_t  word_count     = 0;
char   *outbuf         = NULL;
size_t  outbuf_used    = 0;
size_t  max_depth_opt  = 0;

const char *ifile     = NULL;
const char *ofile     = NULL;
const char *words_opt = NULL;
bool        quiet     = false;
bool        dry_run   = false;
const char *VERSION   = "0.2.7b";

int main(int argc, char *argv[]) {
    parse_args(argc, argv);

    // Loading the wordlist (either with -w or -f):
    if (words_opt) { load_string(words_opt); } else { load_file(ifile); }

    // Create output (stdout or file):
    FILE *out = stdout;
    if (ofile) {
        out = fopen(ofile, "w");
        if (!out) {
            fprintf(stderr, "Error creating output file: %s\n", strerror(errno)); // <- report why output failed (stuff like missing perms could result in this), ref: https://en.wikipedia.org/wiki/Errno.h
            exit(EX_CANTCREAT); // <- return "can't create (user) output file" exit code, ref: https://man.freebsd.org/cgi/man.cgi?query=sysexits
        }
        if (!quiet) { printf("Output: '%s'\n", ofile); }
    } else if (!quiet) { 
        printf("Output: STDOUT\n"); 
    }

    // Evaluate max depth:
    size_t max_depth;
    if (max_depth_opt == 0) {
        max_depth = word_count;
        if (!quiet) { printf("Max depth: %zu (same as word count)\n", word_count); } // <- if user didn't specify the custom depth, just use amount of words as depth
    } else if (max_depth_opt > word_count) {
        if (!quiet) { fprintf(stderr, "Warning: requested depth (%zu) is bigger than total word count (%zu); capping to %zu\n", max_depth_opt, word_count, word_count); }
        max_depth = word_count;
        if (!quiet) { printf("Max depth: %zu (capped)\n", max_depth); }
    } else {
        max_depth = max_depth_opt;
        if (!quiet) { printf("Max depth: %zu\n", max_depth); }
    }

    // Calculate rough estimate for lines:
    double est = 0;
    for (size_t d = 1; d <= max_depth; d++) {
        est += pow((double)word_count, (double)d);
    }
    if (!quiet) { printf("Rough estimate for total amount of lines: %.0f\n", est); }
    if (dry_run) { printf("Dry run detected, exiting...\n"); exit(EX_OK); } // <- exit if just estimating lines

    if (est > PROMPT_THRESHOLD) { // <- if the amount of lines is >500 million, warn the user
        if (!prompt_user()) {
            fprintf(stderr, "Aborted by user.\n");
            exit(EX_OK);
        }
    }

    // Pre-allocate an 8 MiB write buffer, fwrite() calls will be batched to minimize syscalls:
    outbuf = malloc(OUTPUT_BUF_SIZE);
    if (!outbuf) {
        fprintf(stderr, "Error: malloc failed to allocate %zu bytes!\n"
        "Please report this at https://github.com/sleepymincy/swg/issues/new\n"
        "Exiting...\n", OUTPUT_BUF_SIZE);
        exit(EX_OSERR);
    }

    // Generate combinations and write them out:
    create_wordlist(out, max_depth);
    flush_outbuf(out);
    if (!quiet) { printf("Wordlist generation complete.\n"); }

    // Cleanup:
    free(outbuf);
    for (size_t i = 0; i < word_count; i++) { free(words[i]); }
    if (fclose(out) == EOF) { fprintf(stderr, "Error: failed to close output file '%s': %s\n", ofile, strerror(errno)); exit(EX_IOERR); }
    return EX_OK;
}