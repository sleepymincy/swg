// args.c - parse_args, show_help, show_version

#include "args.h"

// Args parsing logic here:
void parse_args(int argc, char *argv[]) {
    if (argc == 1) { show_help(argv[0]); exit(EX_OK); } // <- if no args given, print out help page

    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];

        // This entire if {} else if {} else ... thing is too yandere-dev coded, sooooo....
        // TODO: FIX THIS:
        if (!strcmp(arg, "-h") || !strcmp(arg, "--help")) {
            show_help(argv[0]);
            exit(EX_OK);
        } else if (!strcmp(arg, "-v") || !strcmp(arg, "--version")) {
            show_version();
            exit(EX_OK);
        } else if (!strcmp(arg, "-w") || !strcmp(arg, "--words")) {
            // If user specifies -w or --words without any input, throw an error:
            if (i + 1 >= argc || argv[i+1][0] == '-') { fprintf(stderr, "Error: '%s' requires a comma-separated list of words\n", arg); exit(EX_USAGE); }
            words_opt = argv[++i];
        } else if (!strcmp(arg, "-f") || !strcmp(arg, "--file")) {
            // If user specifies -f or --file without any input, throw an error:
            if (i + 1 >= argc || argv[i+1][0] == '-') { fprintf(stderr, "Error: '%s' requires a path to a file\n", arg); exit(EX_USAGE); }
            ifile = argv[++i];
        } else if (!strcmp(arg, "-o") || !strcmp(arg, "--output")) {
            // If user specifies -o or --output without any input, throw an error:
            if (i + 1 >= argc || argv[i+1][0] == '-') { fprintf(stderr, "Error: '%s' requires an output path\n", arg); exit(EX_USAGE); }
            ofile = argv[++i];
        } else if (!strcmp(arg, "-d") || !strcmp(arg, "--depth")) {
            // If user specifies -d or --depth without any input, throw an error:
            if (i+1 >= argc || argv[i+1][0]=='-') { fprintf(stderr, "Error: '%s' requires a depth\n", arg); exit(EX_USAGE); }
            long d = strtol(argv[++i], NULL, 10);
            if (d < 1) { fprintf(stderr, "Error: invalid depth\n"); exit(EX_USAGE); }
            max_depth_opt = (size_t)d;
        } else if (!strcmp(arg, "-q") || !strcmp(arg, "--quiet")) {
            quiet = true;
        } else if (!strcmp(arg, "-e") || !strcmp(arg, "--estimate")) {
            dry_run = true;
        } else {
            // Handle unknown args and point user to read the manual:
            fprintf(stderr, "Unknown option: %s\nType '%s -h' for help\n", arg, argv[0]);
            exit(EX_USAGE);
        }
    }

    // In case of user error of specifying both -w and -f, tell them that it's not gonna work out:
    if (words_opt && ifile) { fprintf(stderr, "Error: -w and -f are mutually exclusive\n"); exit(EX_USAGE); }

    // Same if they forget to specify the words entirely:
    if (!words_opt && !ifile) { fprintf(stderr, "Error: you must supply either -w or -f\n"); exit(EX_USAGE); }
}

// -h or --help logic here:
void show_help(const char *bin_name) {
    printf("\e[1mSWG - Simple Wordlist Generator\nThis is a 🔥blazing fast🔥 and (possibly, most likely) 🔥NOT memory safe🔥 wordlist generator tool!\e[m\n\n");
    printf("Usage: %s [args]\n\n", bin_name);
    printf("Required arguments (either/or, but not both):\n"
    "  -w, --words    WORD1,WORD2     Read comma-separated list of words\n"
    "  -f, --file     PATH            Read one word per line from file\n\n"
    "Optional arguments:\n"
    "  -o, --output   PATH            Write the wordlist to specified PATH, otherwise will output to STDOUT\n"
    "  -d, --depth    NUM             Custom depth of a wordlist (how long the generated passwords should be)\n"
    "  -q, --quiet                    Aside from critical errors, informational messages won't be printed\n"
    "  -e, --estimate                 Evaluate the amount of lines you'll generate, without running the code\n"
    "  -h, --help                     Show this help message\n"
    "  -v, --version                  Show current version\n\n"
    "Examples:\n"
    "  swg -w 1,2,3,4,5 -o /usr/share/wordlists/all-five-digit.txt\n"
    "  swg -w foo,bar,baz,qux,quux --depth 3 --estimate\n"
    "  swg -f /tmp/cewl_output.txt -o /usr/share/wordlists/cewl-wordlist.txt\n");
}

// -v or --version logic here:
void show_version(void) {
    printf("%s\n", VERSION);
}