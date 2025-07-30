#ifndef DUMPER_H
#define DUMPER_H

#ifdef _NFILE		/* if stdio.h included */
extern void cdsprint(HANDLE df, void *record);
	/* prints a formated list of record for debugging dumps from client */
extern void dumpbulk(FILE *df, void *ptr); /* only in fat version */
void dmpbulk(HANDLE hf, void *ptr); /* only in fat version */
extern char	*type_name(char type);
#endif
#endif
