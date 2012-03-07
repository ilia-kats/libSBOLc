#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "property.h"
#include "genericarray.h"
#include "dnacomponent.h"
#include "dnasequence.h"
#include "sequenceannotation.h"
#include "collection.h"

static GenericArray* allDNAComponents;

/**************************
	create/destroy
***************************/

void registerDNAComponent(DNAComponent* com) {
	if (!allDNAComponents)
		allDNAComponents = createGenericArray();
	insertIntoGenericArray(allDNAComponents, com);
}

DNAComponent* createDNAComponent(const char* id) {
	DNAComponent* com = malloc(sizeof(DNAComponent));
	com->id          = createProperty();
	com->name        = createProperty();
	com->description = createProperty();
	com->annotations = createGenericArray();
	com->collections = createGenericArray();
	setDNAComponentID(com, id);
	registerDNAComponent(com);
	return com;
}

void removeDNAComponent(DNAComponent* com) {
	if (com && allDNAComponents) {
		int index = indexByPtr(allDNAComponents, com);
		if (index >= 0)
			removeFromGenericArray(allDNAComponents, index);
	}
}

void deleteDNAComponent(DNAComponent* com) {
	if (com) {
		removeDNAComponent(com);
		if (com->id) {
			deleteProperty(com->id);
			com->id = NULL;
		}
		if (com->name) {
			deleteProperty(com->name);
			com->name = NULL;
		}
		if (com->description) {
			deleteProperty(com->description);
			com->description = NULL;
		}
		if (com->annotations) {
			deleteGenericArray(com->annotations);
			com->annotations = NULL;
		}
		if (com->collections) {
			deleteGenericArray(com->collections);
			com->collections = NULL;
		}
		free(com);
	}
}

/**************************
	is... functions
***************************/

int isDNAComponentPtr(const void* ptr) {
	if (!allDNAComponents)
		allDNAComponents = createGenericArray();
	return (int) indexByPtr(allDNAComponents, ptr) >= 0;
}

int isDNAComponentID(const char* id) {
	if (!allDNAComponents || !id)
		return 0;
	int index;
	DNAComponent* com;
	for (index=0; index<allDNAComponents->numInUse; index++) {
		com = (DNAComponent*) allDNAComponents->array[index];
		if (compareProperty(com->id, id) == 0)
			return 1;
	}
	return 0;
}

/**************************
	getNum... functions
***************************/

int getNumDNAComponents() {
	if (allDNAComponents)
		return allDNAComponents->numInUse;
	else
		return 0;
}

int getNumCollectionsFor(const DNAComponent* com) {
	if (com)
		return com->collections->numInUse;
	else
		return -1;
}

int getNumSequenceAnnotationsIn(const DNAComponent* com) {
	if (com)
		return com->annotations->numInUse;
	else
		return -1;
}

/**************************
	getNth... functions
***************************/

DNAComponent* getNthDNAComponent(int n) {
	if (allDNAComponents && allDNAComponents->numInUse > n)
		return allDNAComponents->array[n];
	else
		return NULL;
}

Collection* getNthCollectionFor(const DNAComponent* com, int n) {
	if (com->collections->numInUse >= n)
		return (Collection*) com->collections->array[n];
	else
		return NULL;
}

SequenceAnnotation* getNthSequenceAnnotationIn(const DNAComponent* com, int n) {
	if (com && com->annotations->numInUse >= n)
		return (SequenceAnnotation*) com->annotations->array[n];
	else
		return NULL;
}

/**************************
	get... functions
***************************/

DNAComponent* getDNAComponent(const char* id) {
	if (!allDNAComponents || !id)
		return NULL;
	int index;
	DNAComponent* com;
	for (index=0; index<allDNAComponents->numInUse; index++) {
		com = (DNAComponent*) allDNAComponents->array[index];
		if (compareProperty(com->id, id) == 0)
			return com;
	}
	return NULL;
}

char* getDNAComponentID(const DNAComponent* com) {
    return getProperty(com->id);
}

char* getDNAComponentName(const DNAComponent* com) {
    return getProperty(com->name);
}

char* getDNAComponentDescription(const DNAComponent* com) {
    if (com)
        return getProperty(com->description);
}

/**************************
	set... functions
***************************/

void setDNAComponentID(DNAComponent* com, const char* id) {
    setProperty(com->id, id);
}

void setDNAComponentName(DNAComponent* com, const char* name) {
    if (com)
        setProperty(com->name, name);
}

void setDNAComponentDescription(DNAComponent* com, const char* descr) {
    if (com)
        setProperty(com->description, descr);
}

/**************************
	add annotation
***************************/

void addSequenceAnnotation(DNAComponent* com, SequenceAnnotation* ann) {
	if (com && ann) {
		insertIntoGenericArray(com->annotations, ann);
		ann->annotates = com;
	}
}

void setSubComponent(SequenceAnnotation* ann, DNAComponent* com) {
	if (ann && com)
		ann->subComponent = com;
}

void cleanupDNAComponents() {
	if (allDNAComponents) {
		int n;
		DNAComponent* com;
		for (n=getNumDNAComponents()-1; n>=0; n--) {
			com = getNthDNAComponent(n);
			deleteDNAComponent(com);
	    }
		deleteGenericArray(allDNAComponents);
		allDNAComponents = NULL;
	}
}

void printDNAComponent(const DNAComponent* com, int tabs) {
    if (!com)
        return;
    indent(tabs);   printf("%s\n", getDNAComponentID(com));
    indent(tabs+1); printf("name: %s\n", getDNAComponentName(com));
    indent(tabs+1); printf("description: %s\n", getDNAComponentDescription(com));
    if (com->dnaSequence) {
        printDNASequence(com->dnaSequence, tabs+1);
    } else {
    	indent(tabs+1); printf("no sequence\n");
    }
    int i;
    int num;
    if (com->annotations) {
        SequenceAnnotation* seq;
        num = getNumSequenceAnnotationsIn(com);
        if (num > 0) {
            indent(tabs+1); printf("%i annotations:\n", num);
            for (i=0; i<num; i++) {
                seq = getNthSequenceAnnotationIn(com, i);
                indent(tabs+2); printf("%s\n", getSequenceAnnotationID(seq));
            }
        }
    }
    if (com->collections) {
        Collection* col;
        num = getNumCollectionsFor(com);
        if (num > 0) {
            indent(tabs+1); printf("%i collections:\n", num);
            for (i=0; i<num; i++) {
                col = getNthCollectionFor(com, i);
                indent(tabs+2); printf("%s\n", getCollectionID(col));
            }
        }
    }
}

void printAllDNAComponents() {
    int n;
    int num = getNumDNAComponents();
    if (num > 0) {
        printf("%i components:\n", num);
        for (n=0; n<num; n++)
            printDNAComponent(getNthDNAComponent(n), 1);
    }
}
