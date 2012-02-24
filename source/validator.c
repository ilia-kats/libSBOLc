#include <libxml/parser.h>
#include <libxml/xmlschemas.h>
#include "core.h"
#include "validator.h"

int isValidSBOL(const xmlDocPtr doc)
{
    xmlDocPtr schema_doc = xmlReadFile(SBOL_SCHEMA_FILENAME, NULL, XML_PARSE_NONET);
    if (schema_doc == NULL)
    {
        // the schema cannot be loaded or is not well-formed
        return -1;
    }
    xmlSchemaParserCtxtPtr parser_ctxt = xmlSchemaNewDocParserCtxt(schema_doc);
    if (parser_ctxt == NULL)
    {
        // unable to create a parser context for the schema
        xmlFreeDoc(schema_doc);
        return -2;
    }
    xmlSchemaPtr schema = xmlSchemaParse(parser_ctxt);
    if (schema == NULL)
    {
        // the schema itself is not valid
        xmlSchemaFreeParserCtxt(parser_ctxt);
        xmlFreeDoc(schema_doc);
        return -3;
    }
    xmlSchemaValidCtxtPtr valid_ctxt = xmlSchemaNewValidCtxt(schema);
    if (valid_ctxt == NULL)
    {
        // unable to create a validation context for the schema
        xmlSchemaFree(schema);
        xmlSchemaFreeParserCtxt(parser_ctxt);
        xmlFreeDoc(schema_doc);
        return -4;
    }
    int is_valid = (xmlSchemaValidateDoc(valid_ctxt, doc) == 0);
    xmlSchemaFreeValidCtxt(valid_ctxt);
    xmlSchemaFree(schema);
    xmlSchemaFreeParserCtxt(parser_ctxt);
    xmlFreeDoc(schema_doc);
    // force the return value to be non-negative on success
    return is_valid ? 1 : 0;
}