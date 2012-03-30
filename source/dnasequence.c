#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "property.h"
#include "array.h"
#include "dnasequence.h"
#include "object.h"

static PointerArray* allDNASequences;

void lazyCreateAllDNASequences() {
	if (!allDNASequences)
		allDNASequences = createPointerArray();
}

void registerDNASequence(DNASequence* seq) {
	lazyCreateAllDNASequences();
	insertPointerIntoArray(allDNASequences, seq);
}

// TODO constrain to actg and sometimes n?
DNASequence* createDNASequence(char* uri) {
	if (!uri || isSBOLObjectURI(uri))
	    return NULL;
	DNASequence* seq = malloc(sizeof(DNASequence));
	seq->base        = createSBOLObject(uri);
	//seq->nucleotides = createTextProperty();
	seq->nucleotides = createNucleotidesProperty();
	registerDNASequence(seq);
	return seq;
}

void setDNASequenceURI(DNASequence* seq, const char* uri) {
	if (seq)
		setSBOLObjectURI(seq->base, uri);
}

void setNucleotides(DNASequence* seq, const char* nucleotides) {
	if (seq)
		setNucleotidesProperty(seq->nucleotides, nucleotides);
}

// TODO generalize this
void removeDNASequence(DNASequence* seq) {
	if (seq && allDNASequences) {
		int index = indexOfPointerInArray(allDNASequences, seq);
		if (index >= 0)
			removePointerFromArray(allDNASequences, index);
	}
}

void deleteDNASequence(DNASequence* seq) {
	if (seq) {
		if (seq->base) {
			deleteSBOLObject(seq->base);
			seq->base = NULL;
		}
		if (seq->nucleotides) {
			deleteNucleotidesProperty(seq->nucleotides);
			seq->nucleotides = NULL;
		}
		removeDNASequence(seq);
		free(seq);
		seq = NULL;
	}
}

void cleanupDNASequences() {
	if (allDNASequences) {
		int n;
		DNASequence* seq;
		for (n=getNumDNASequences()-1; n>=0; n--) {
			seq = getNthDNASequence(n);
			deleteDNASequence(seq);
			seq = NULL;
		}
		deletePointerArray(allDNASequences);
		allDNASequences = NULL;
	}
}

int getNumDNASequences() {
	if (allDNASequences)
	    return getNumPointersInArray(allDNASequences);
	else
	    return 0;
}

DNASequence* getDNASequence(const char* uri) {
	lazyCreateAllDNASequences();
	if (!uri)
		return NULL;
	int n;
	char* candidate;
	DNASequence* seq;
	for (n=0; n < getNumDNASequences(); n++) {
		seq = getNthDNASequence(n);
		candidate = getSBOLObjectURI(seq->base);
		if (candidate && strcmp(candidate, uri) == 0)
			return seq;
	}
	return NULL;
}

char* getDNASequenceURI(const DNASequence* seq) {
	if (seq)
		return getSBOLObjectURI(seq->base);
	else
		return NULL;
}

int isDNASequenceURI(const char* uri) {
	lazyCreateAllDNASequences();
	if (!uri)
		return 0;
	int n;
	char* candidate;
	DNASequence* seq;
	for (n=0; n < getNumDNASequences(); n++) {
		seq = getNthDNASequence(n);
		candidate = getSBOLObjectURI(seq->base);
		if (candidate && strcmp(candidate, uri) == 0)
			return 1;
	}
	return 0;
}

char* getNucleotides(const DNASequence* seq) {
	if (seq)
	    return getNucleotidesProperty(seq->nucleotides);
}

// TODO generalize this further?
DNASequence* getNthDNASequence(int n) {
	if (getNumDNASequences() > n && n >= 0)
		return (DNASequence*) getNthPointerInArray(allDNASequences, n);
	else
		return NULL;
}

