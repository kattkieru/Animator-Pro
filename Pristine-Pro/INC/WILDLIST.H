#ifndef WILDLIST_H
#define WILDLIST_H

#ifndef STDTYPES_H
	#include "stdtypes.h"
#endif

#ifndef LINKLIST_H
	#include "linklist.h"
#endif



/* all is hunky dory as long as this structure is contiguaous as one
 * buffer and can be freed by calling freemem(&we->hdr) free_wildlist()
 * will call freemem() for each node in the list */

typedef struct wild_entry {
	Names hdr;
	char name_buf[1];
} Wild_entry;

/* nameload is responsible for setting dst->hdr.name and filling it with
 * data in input name */

typedef void (*Nameload)(Wild_entry *dst,char *name);

void free_wild_list(Names **pwild_list);

Errcode alloc_wild_list(Names **pwild_list, char *pat,Boolean get_dirs,
						int min_alloc_size, Nameload load_name );

Errcode build_wild_list(Names **pwild_list, char *pat, Boolean get_dirs);

Errcode build_dir_list(Names **pwild_list, char *pat, 
					   Boolean get_dirs, char *dir);

Names *merge_wild_lists(Names *l1, Names *l2); /* merges two sorted lists */

int name_is_wild(char *name);

#endif /* WILDLIST_H */
