#include "sbol.h"

// This only creates a Document in memory;
// you would then want to write it out with writeSBOLCore(doc, filename)
// and free it with deleteDocument(doc).
// You can see the output with sbol_run_tests, which writes
// xml files for all the CreateValid* examples.

Document* CreateValid11() {
	Document* doc = createDocument();
	// collection
	Collection *col = createCollection(doc, "http://example.com/collection1");
	setCollectionDisplayID(col, "Coll1");
	setCollectionName(col, "Collection1");
	setCollectionDescription(col, "A collection may contain multiple components");
	// components
	DNAComponent *dc1 = createDNAComponent(doc, "http://example.com/dc1");
	DNAComponent *dc2 = createDNAComponent(doc, "http://example.com/dc2");
	setDNAComponentDisplayID(dc1, "DC1");
	setDNAComponentDisplayID(dc2, "DC2");
	setDNAComponentName(dc1, "DnaComponent1");
	setDNAComponentName(dc2, "DnaComponent2");
	addDNAComponentToCollection(col, dc1);
	addDNAComponentToCollection(col, dc2);
	// sequence
	DNASequence *ds1 = createDNASequence(doc, "http://example.com/ds1");
	setDNASequenceNucleotides(ds1, "tccctatcagtgat");
	setDNAComponentSequence(dc1, ds1);
	return doc;
}

