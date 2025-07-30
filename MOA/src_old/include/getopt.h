#ifndef _GETOPT_
#define _GETOPT_

int getopt(int argc, char **argv, char *optstring);

#ifndef XPDATA
#define XPDATA extern
#endif
XPDATA char *optarg;		// returned arg to go with this option
XPDATA int optind;		// index to next argv element to process
XPDATA int opterr;		// should error messages be printed?
XPDATA int optopt;		//

#define BADCH ('?')

#endif // _GETOPT
