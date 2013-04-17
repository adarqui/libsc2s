/*
 * Simple little test code..
 * 
 * The bottom line is:
 *
 *  sc = sc2s_init(...)
 *  scl = sc2s_fill(sc, NULL, "name_of_.h.t")
 *  str = sc2s_get(scl, <syscall num>)
 *
 * That's it. :F
 */

#include "sc2s.h"

int main(int argc, char * argv[]) {
	sc_t * sc;
	scl_t * scl;
	int i;

	puts("Testing.");


	sc = sc2s_init(NULL);
	if(!sc) {
		err(0, "sc2s_init()");
	}

	puts("\nAvailable syscall tables:");
	for(scl = sc->head, i = 0; scl != NULL; scl = scl->next, i++) {
		printf("\t%i. %s\n", i, scl->name);
	}

	scl = sc2s_fill(sc, scl, "linux2.6.32_x32.h.t");
	if(!scl) {
		err(0, "sc2s_fill");
	}

	printf("\nAnalyzing: %s/%s\n", sc->dir, "linux2.6.32_x32.h.t");
	printf("\tscl->cnt = %i\n", scl->cnt);
	
	printf("\texecve: %i %s\n\n", scl->table[11], (scl->strings + scl->table[11]));

	for(i=0;i<scl->cnt;i++) {
		printf("\t\tsyscall[%i] = %s\n", i, sc2s_get(scl, i));
	}


	puts("\nTesting every .h.t file");

	for(scl = sc->head, i = 0; scl != NULL; scl = scl->next, i++) {
		int j;
		scl_t * t;

		printf("\t%s:\n", scl->name);
		t = sc2s_fill(sc, scl, scl->name);

		for(j=0;j<t->cnt;j++) {
			printf("\t\t%i = %s\n", j, sc2s_get(t, j));
		}

	}

	sc2s_fini(sc);

	return 0;
}
