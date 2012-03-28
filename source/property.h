///////////////////////////////////////////////////////////
/// @file property.h
/// This file defines a simple hierarchy of Properties,
/// which hold data for SBOLObjects. Although they don't
/// do much so far, this is where you would add restraints
/// on the getting and setting of specific values. For
/// example, you could add a NucleotidesProperty that
/// only accepts strings of a, c, t, g, and sometimes n.
///////////////////////////////////////////////////////////

/// @todo enums for all the magic numbers...

#ifndef SBOL_PROPERTY_HEADER
#define SBOL_PROPERTY_HEADER

/************
 * Property
 ************/

/// Generic Property that forms the base struct for all the others.
/// Besides making the inheritance diagrams look nice, this provides
/// a hook for doing something whenever a Property is read or written.
/// It shouln't be used directly though because there's no flag for
/// telling whether it holds an int or a char *.
union Property {
	int   number;
	char *letters;
};

union Property *createProperty();
void deleteProperty(union Property *pro);

void setNumber(union Property *pro, int num);
int  getNumber(const union Property *pro);

void  setLetters(union Property *pro, char *str);
char *getLetters(const union Property *pro);

int compareNumbers(const union Property *pro1, const union Property *pro2);
int compareLetters(const union Property *pro1, const union Property *pro2);

/****************
 * TextProperty
 ****************/

/// Base struct for all text-containing Properties.
typedef struct _TextProperty {
	union Property *text;
} TextProperty;

TextProperty* createTextProperty();
void deleteTextProperty(TextProperty* pro);
void setTextProperty(TextProperty* pro, const char* text);
char* getTextProperty(const TextProperty* pro);
int compareTextProperty(const TextProperty* pro1, const TextProperty* pro2);
void printTextProperty(const TextProperty* pro);

/***************
 * URIProperty
 ***************/

/// A TextProperty that only allows creation
/// of values that aren't already in use as the
/// uri of an SBOLObject.
typedef struct _URIProperty {
	struct _TextProperty* uri;
} URIProperty;

URIProperty* createURIProperty();
void deleteURIProperty(URIProperty* pro);
void setURIProperty(URIProperty* pro, const char* uri);
char* getURIProperty(const URIProperty* pro);
int compareURIProperty(const URIProperty* pro1, const URIProperty* pro2);
void printURIProperty(const URIProperty* pro);

/***********************
 * NucleotidesProperty
 ***********************/

typedef struct _NucleotidesProperty {
	struct _TextProperty *nucleotides;
} NucleotidesProperty;

NucleotidesProperty *createNucleotidesProperty();
void deleteNucleotidesProperty(NucleotidesProperty *pro);
void  setNucleotidesProperty(NucleotidesProperty *pro, const char *nucleotides);
char *getNucleotidesProperty(const NucleotidesProperty *pro);
int compareNucleotidesProperty(const NucleotidesProperty *pro1, const NucleotidesProperty *pro2);
void printNucleotidesProperty(const NucleotidesProperty *pro);

/********************
 * PositionProperty
 ********************/

/// IntProperty that only allows non-negative values.
/// There should also be other restrictions, but they
/// only make sense for groups, not individual PositionProperties.
/// For example, SequenceAnnotation->bioEnd should be at least
/// as large as SequenceAnnotation->bioStart.
typedef struct _PositionProperty {
	union Property* position;
} PositionProperty;

PositionProperty* createPositionProperty();
void deletePositionProperty(PositionProperty* pro);
void setPositionProperty(PositionProperty* pro, const int value);
int getPositionProperty(const PositionProperty* pro);
int comparePositionProperty(const PositionProperty* pro1, const PositionProperty* pro2);
void printPositionProperty(const PositionProperty* pro);

/********************
 * PolarityProperty
 ********************/

/// @todo rename to StrandOrientation?
enum StrandPolarity {
	STRAND_FORWARD,
	STRAND_BIDIRECTIONAL,
	STRAND_REVERSE
};

/// IntProperty that only allows StrandPolarity values.
/// 1 indicates the positive strand relative to the parent
/// component, and 0 indicates the reverse complement.
/// @todo rename to StrandProperty? or OrientationProperty?
typedef struct _PolarityProperty {
	union Property* polarity;
} PolarityProperty;

PolarityProperty* createPolarityProperty();
void deletePolarityProperty(PolarityProperty* pro);
void setPolarityProperty(PolarityProperty* pro, const int value);
int getPolarityProperty(const PolarityProperty* pro);
int comparePolarityProperty(const PolarityProperty* pro1, const PolarityProperty* pro2);
void printPolarityProperty(const PolarityProperty* pro);

#endif
