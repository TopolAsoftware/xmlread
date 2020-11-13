/*
 *  Copyright (c) 2005-2015 Oleg Vlasenko <vop@unity.net>
 *  All Rights Reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <inttypes.h>
#include <locale.h>

#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <libxml/parser.h>

#include "v2_iconv.h"

#define STAT_BUFF_SIZE 65000

#define TREE__IDENT 4

/* ======================================================== */
int main(int argc, char *argv[], char *argp[]) {
    xmlDocPtr doc;
    xmlNodePtr sub;
    xmlAttrPtr prop;
    xmlNsPtr ns;

    uint8_t x_buff[STAT_BUFF_SIZE];
    int x_size=0;

    char *locale=NULL;
    char strtmp[10000]; // Output string
    char *in_file=NULL;
    int lvl=0;
    int res=0;

    if(argc == 1) {
	if(isatty(fileno(stdin))) {
	    fprintf(stderr, "Usage:\n\t%s [file.xml|-]\n", argv[0]);
	    return(0);
	} else {
            in_file="-";
	}
    } else {
	in_file=argv[1];
    }

    if(in_file[0] == '-' && in_file[1] == '\0') { // This is stdin - read it to the buffer

	if(!(x_size=fread(x_buff, 1, STAT_BUFF_SIZE, stdin))) {
	    printf("Can't read stream from stdin.\n");
	    return(1);
	}


	if(!(doc=xmlParseMemory((const char *)x_buff, x_size))) {
	    printf("Can't parse input stream.\n");
	    return(1);
	}
    } else {
	if(!(doc=xmlParseFile(in_file))) {
	    printf("Can't parse file: %s\n", in_file);
	    return(1);
	}
    }

    locale=getenv("LC_ALL");

    //if(!(sub=xmlDocGetRootElement(doc))) return(esy_prn_respons(-203, "Incorrect Request XML (empty)"));
    sub=doc->children;

    while(sub) {
	res=0;
	if(sub->type == XML_ELEMENT_NODE) {
	    if(lvl) res+=sprintf(strtmp+res, "%*c|", lvl*4, ' ');
            if(sub->ns && sub->ns->prefix) res+=sprintf(strtmp+res, "%s:", sub->ns->prefix);
	    res+=sprintf(strtmp+res, "%s", sub->name);
            if(sub->children && sub->children->content) {
                register int x;
                int iss=0;
                for(x=0; sub->children->content[x]; x++) {
                  if(sub->children->content[x] == 0x0a) continue;
                  if(sub->children->content[x] == 0x0d) continue;
                  if(sub->children->content[x] == ' ') continue;
                  if(sub->children->content[x] == '\t') continue;
                  iss=1;
                }
                if(iss) res+=sprintf(strtmp+res, " = %s", sub->children->content);
	    }
	    res+=sprintf(strtmp+res, "\n");

	    for(ns=sub->nsDef; ns; ns=ns->next) {
		if(lvl) res+=sprintf(strtmp+res, "%*c", lvl*TREE__IDENT, ' ');
		res+=sprintf(strtmp+res, " xmlns");
		if(ns->prefix) {
		    res+=sprintf(strtmp+res, ":%s", (char *)ns->prefix);
		}
                if(ns->href) res+=sprintf(strtmp+res, "=\"%s\"", ns->href);
		res+=sprintf(strtmp+res, "\n");
            }

            for(prop=sub->properties; prop; prop=prop->next) {
                res+=sprintf(strtmp+res, "%*c+%s", lvl*TREE__IDENT, ' ', prop->name);
                if(prop->children && prop->children->content) {
		    res+=sprintf(strtmp+res, " = %s", prop->children->content);
                }
                res+=sprintf(strtmp+res, "\n");
            }
        }

        if(sub->children) {
            sub=sub->children;
            lvl++;
        } else if(sub->next) {
            sub=sub->next;
        } else {
            while(sub && !sub->next) {
                if(sub == doc->children) { // Alredy head
                    sub=NULL;
                } else {
                    sub=sub->parent;
                    lvl--;
                }
            }
            if(sub) sub=sub->next;
       }

	if(!res) continue;

	if(locale) {
	    printf("%s", v2_iconv("UTF-8", locale, strtmp));
	} else {
	    printf("%s", strtmp);
	}

    }

    xmlFreeDoc(doc);

    return(0);
}
/* ======================================================== */
