/* syscall 2 string -- adarqui (github.com/adarqui && adarq.org */

#ifndef SC2S_H
#define SC2S_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SC_VER 0.1
#define SC_DIR "/usr/share/sc2s/"
#define SC_MAX 600 /* Kind of dirty, not really though, especially if you only fill one 'instance' */

typedef struct scl {
	struct scl * next;	
	char *	name;
	char *	file;
	char *	strings;
	int *	table;
	int 	cnt;
} scl_t;

typedef struct sc {
	char *	dir;
	int		cnt;
	scl_t * head;		
} sc_t;
 
sc_t *	sc2s_init(char *);
scl_t *	sc2s_fill(sc_t *, scl_t *, char *);
char *	sc2s_get(scl_t *, int);
void	sc2s_free(scl_t *);
void	sc2s_free_body(scl_t *);
void	sc2s_fini(sc_t *);

scl_t *	sc2s_link_add(sc_t *, char *);

#endif
