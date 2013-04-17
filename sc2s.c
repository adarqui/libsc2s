/* syscall 2 string -- adarqui (github.com/adarqui && adarq.org */

#include "sc2s.h"

sc_t * sc2s_init(char * base_dir) {
	/* Initialize the syscall 2 string base structure */

	DIR * dir;
	struct dirent * de;
	sc_t * sc;

	sc = (sc_t *) calloc(1,sizeof(sc_t));
	if(!sc) return NULL;

	if(!base_dir) {
		/* If no dir specified, use default dir */
		base_dir = SC_DIR;
	}

	sc->dir = strdup(base_dir); 

	dir = opendir(SC_DIR);
	if(!dir) {
		goto fail;
	}

	while((de = readdir(dir)) != NULL) {
		scl_t * scl;

		/* We only want .h.t files. These are "parsed" syscall tables */
		if(strstr(de->d_name, ".h.t") == NULL) continue;
		
		scl = sc2s_link_add(sc, de->d_name);
		if(!scl) goto fail;
		
	}

	return sc;


	/* Fail city. */
	fail:
		if(sc) {
			free(sc);
			sc = NULL;
		}	

	return NULL;
}

scl_t * sc2s_fill(sc_t * sc, scl_t * scl, char * name) {
	/* Fill an scl_t's strings & table members. This is what we reference when using sc2s_get() */

	FILE * fp=NULL;
	struct stat stb;
	int tok_index;
	char file[255+1], buf[255+1], *strings_ptr, *tok;

	if(!name || (!sc && !sc->head)) return NULL;

	if(scl == NULL) {
		for(scl = sc->head ; scl != NULL ; scl = scl->next) {
			if(!strcasecmp(scl->name, name)) break;
		}
	}

	if(!scl) return NULL;

	/*
	 * Fill the syscalls
	 */
	if(scl->strings) {
		/* Already configured. Free the 'body' of the scl */
		sc2s_free_body(scl);
	}

	snprintf(file,sizeof(file)-1,"%s/%s",sc->dir,name);
	scl->file = strdup(file);

	if(stat(scl->file, &stb) < 0) {
		return NULL;
	}

	scl->table = (int *) calloc(SC_MAX+1, sizeof(int));
	if(!scl->table) goto fail;

	fp = fopen(scl->file, "r");
	if(!fp) {
		goto fail;
	}

	scl->strings = (char *) calloc(1,stb.st_size+1);
	if(!scl) goto fail;

	strings_ptr = scl->strings;
	while(1) {
		memset(buf,0,sizeof(buf));
		if(fgets(buf,sizeof(buf)-1,fp) == NULL) break;

		tok = strtok(buf, " ");
		if(!tok) continue;

		tok_index = atoi(tok);
		if(tok_index >= SC_MAX) break;

		if(tok_index > scl->cnt) {
			scl->cnt = tok_index+1;
		}

		tok = strtok(NULL, "");
		if(!tok) continue;
		
		/* Remove newline */
		tok[strlen(tok)-1] = '\0';
		strcat(strings_ptr, tok);
		
		/* offset into string table */
		scl->table[tok_index] = strings_ptr - scl->strings;

		strings_ptr += strlen(tok) + 1;
	}


	fail:
		if(fp) fclose(fp);

	return scl;
}


char * sc2s_get(scl_t * scl, int idx) {
	/* This lib was built for this func: return a string based on syscall number */
	char * s;

	if(!scl || idx < 0 || idx >= scl->cnt) return NULL;

	s = (scl->strings + scl->table[idx]);
	return s;
}


void sc2s_fini(sc_t *sc) {
	/* Finish everything. Nobody cares though really. */
	scl_t * scl, * prev;
	if(!sc) return;
	
	if(sc) {
		for( scl = sc->head ; scl != NULL ; ) {
			prev = scl->next;
			sc2s_free(scl);
			scl = prev;
		}
	}

	if(sc->dir) free(sc->dir);
	memset(sc, 0, sizeof(sc_t));

	free(sc);
	return;
}


void sc2s_free(scl_t *scl) {
	if(!scl) return;

	sc2s_free_body(scl);

	/* Keep name, for re-enterant calls to sc2s_Fill */
	if(scl->name) free(scl->name);

	memset(scl,0,sizeof(scl_t));
	free(scl);
}

void sc2s_free_body(scl_t *scl) {

	if(!scl) return;

	if(scl->strings) free(scl->strings);
	if(scl->table) free(scl->table);
	if(scl->file) free(scl->file);

	return;
}


scl_t * sc2s_link_add(sc_t * sc, char * name) {
	scl_t * scl, *scl_prev;

	if(!name || !sc) return NULL;

	scl = (scl_t *) calloc(1,sizeof(scl_t));
	if(!scl) return NULL;

	
	scl_prev = sc->head;
	if(!scl_prev) {
		sc->head = scl;
	} else {
		sc->head = scl;
		scl->next = scl_prev;
	}

	scl->name = strdup(name);

	return scl;
}
