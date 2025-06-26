// generator.c - increment_counter, create_wordlist

#include "generator.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// https://en.wikipedia.org/wiki/Cartesian_product#Cartesian_products_of_several_sets
static int increment_counter(size_t idx[], size_t depth, size_t base) {
    // Start at the least significant digit:
    for (int i = (int)depth - 1; i >= 0; i--) {
        if (++idx[i] < base) return 1; // <- no carry beyond this position
        idx[i] = 0; // <- we wrapped this position, carry on to next
    }
    return 0; // <- if we get here, we’ve carried past the most significant digit
}

// Wordlist generator logic here! We're generating all combinations of words up to max_depth,
// and then writing them into 'out' with flush_outbuf() (refer to ./src/io.c to see that function)
void create_wordlist(FILE *out, size_t max_depth) {
    if (!quiet) { printf("Creating wordlist...\n"); }
    if (word_count == 0) return; // <- if no words is sourced, nothing to do 
    
    size_t W = word_count;

    // Allocate a scratch buffer large enough to hold max_depth * longest word (and +1 for new line):
    size_t scratch_cap = max_depth * MAX_WORD_LEN + 1;
    char *scratch = malloc(scratch_cap);
    if (!scratch) {
        fprintf(stderr, "Error: malloc failed to allocate %zu bytes!\n"
        "Please report this at https://github.com/sleepymincy/swg/issues/new\n"
        "Exiting...\n", scratch_cap);
        exit(EX_OSERR);
    }

    // idx array holds “digits” 0..W-1 for each position in the depth:
    size_t *idx = calloc(max_depth, sizeof *idx);
    if (!idx) {
        fprintf(stderr, "Error: calloc error\n");
        exit(EX_OSERR);
    }

    // For each target depth 1..max_depth ...
    for (size_t depth = 1; depth <= max_depth; depth++) {
        memset(idx, 0, depth * sizeof *idx); // <- ...zero out counters for this depth...
        
        // ...then generate all W^depth combinations:
        do {
            // Build the line by concatenating words[idx[0..depth-1]]:
            size_t pos = 0;
            for (size_t d = 0; d < depth; d++) {
                const char *w   = words[idx[d]];
                size_t       len = strlen(w);
                memcpy(scratch + pos, w, len);
                pos += len;
            }
            scratch[pos++] = '\n';

            if (outbuf_used + pos > OUTPUT_BUF_SIZE) { flush_outbuf(out); } // <- if we're close to filling the buffer, flush it
            memcpy(outbuf + outbuf_used, scratch, pos); // <- copy this line into the global output buffer
            outbuf_used += pos;

        } while (increment_counter(idx, depth, W)); // <- continue until wrap
    }

    // Cleanup:
    free(idx);
    free(scratch);
}
