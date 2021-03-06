#ifndef SBOL_PROTOTYPES_HEADER
#define SBOL_PROTOTYPES_HEADER

////////////////////////////////////////////////////////////
/// @file
/// 
/// Declares all the major structs, unions, and enums
/// so the compiler will recognize them when reading
/// through the rest of the code. This was necessary
/// because even though the SBOL core is split up into 
/// files for easier maintenance, a lot of them are
/// interdependent.
////////////////////////////////////////////////////////////

typedef enum   StrandPolarity       StrandPolarity;

typedef union  _Property            Property;
typedef struct _TextProperty        TextProperty;
typedef struct _URIProperty         URIProperty;
typedef struct _NucleotidesProperty NucleotidesProperty;
typedef struct _PositionProperty    PositionProperty;
typedef struct _PolarityProperty    PolarityProperty;
typedef struct _TypeProperty        TypeProperty;

typedef struct _PointerArray        PointerArray;

typedef struct _SBOLObject          SBOLObject;
typedef struct _SBOLCompoundObject  SBOLCompoundObject;
typedef struct _DNASequence         DNASequence;
typedef struct _SequenceAnnotation  SequenceAnnotation;
typedef struct _DNAComponent        DNAComponent;
typedef struct _Collection          Collection;

typedef struct _Document            Document;

#endif
