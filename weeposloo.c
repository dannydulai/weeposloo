/*
 * Generate Nirva-style names
 * nate@root.org v1.1 9/28/99
 *
 * aaronmb@mit.edu v2.0 4/12/2000
 * - Trimmed suffixes, added prefixes, split vowels into long and short,
 *   added biases for long vowels and against repeats, code generated
 *   "ishiboo" randomly.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


char *prefixes[] = { "b", "d", "f", "l", "l", "m", "n", "n", "p", "s",
    "t", "w", "z", "sk", "sl", "skr", "h", "ish", "ish"};
#define NUM_PREFIXES 19
char *long_vowels[] = { "oo", "ee" };
#define NUM_LONGVOWELS 2
char *short_vowels[] = { "o", "a", "u", "i" };    
#define NUM_SHORTVOWELS 4
char *suffixes[] = { "s", "m", "t" };
#define NUM_SUFFIXES 3


#define DEFAULT_LONGBIAS 55
#define DEFAULT_NOREPEAT_PERCENTAGE 90
#define DEFAULT_MAX_SYLLABLES 3


int 
main(int argc, char **argv)
{   
    extern char *optarg;
    char *ootpoot, *cp, *tp;
    int i, end, numlong, longbias, norep;

#if defined(__FreeBSD__)
    srandomdev();
#else 
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srandom(time(NULL) ^ getpid() ^ (tv.tv_sec << 16) ^ tv.tv_usec);
#endif

    longbias = DEFAULT_LONGBIAS;
    norep = DEFAULT_NOREPEAT_PERCENTAGE;
    end = 0;
    ootpoot = NULL;

    if (argc > 1) {
        while ((i = getopt(argc, argv, "l:r:s:h")) != EOF) {
            switch (i) {
            case 'l':
                longbias = atoi(optarg);
                if ((longbias < 0) || (longbias > 100)) {
                    fprintf(stderr, "longbias must be between 0 and 100.\n");
                    return -1;
                }
                break;
            case 'r':
                norep = atoi(optarg); 
                if ((norep < 0) || (norep > 100)) {
                    fprintf(stderr, "Repeat percentage must be between 0 and 100.\n");
                    return -1;
                }
                break;
	    case 's':
		end = atoi(optarg);
                break;
            case 'h':  /*FALLTHRU*/
            default:
                fprintf(stderr, "Usage: %s [-l longbias] [-r norep] [-s numsym] [-h]\n",
                        argv[0]);
                fprintf(stderr, "longbias - Bias towards long vowels\n");
                fprintf(stderr, "norep    - Percentage of times repeated long vowels will be rejected\n");
                fprintf(stderr, "numsym   - Number of syllables\n");
                return -1;
            }
        }
    }

    if (!end) {                        
    	/* Loop for less than DEFAULT_MAX_SYLLABLES
         * and more than 1 syllable */
    	end = (random() % DEFAULT_MAX_SYLLABLES) + 1;
    	if (end < 2)
            end = 2;
    }

    ootpoot = (char *)malloc(end * 6);
    if (!ootpoot) { 
        perror("malloc");
        return -1;
    } 

    numlong = 0;
    cp = ootpoot;

    for (i = 0 ; i < end ; i++) {
        /* Choose a random prefix and vowel */
        do {
            tp = prefixes[random() % NUM_PREFIXES];

        /* Don't allow repeats of the long prefixes */
        } while ((strlen(tp) > 1) && strstr(ootpoot, tp));
        strcpy(cp, tp); cp += strlen(tp);

	/* Alternate between long and short, bias slightly towards long */
	if ((numlong < 2) && ((random()%100) < longbias)) {
            numlong++;
            do {
                tp = long_vowels[random() % NUM_LONGVOWELS];

            /* Don't allow repeats most of the time */
            } while (strstr(ootpoot, tp) && (random()%100 < norep));

            strcpy(cp, tp); cp += strlen(tp);
                        
	} else {
            tp = short_vowels[random() % NUM_SHORTVOWELS];
            strcpy(cp, tp); cp += strlen(tp);
	}
    }

    printf("%s", ootpoot);

    /* Half the time, do a suffix */
    if (random() & 0x1)
        printf("%s", suffixes[random() % NUM_SUFFIXES]);
    printf("\n");

    return 0;
}
