/*******************************************************************************
 *
 * File Name: GIAglobalsDefs.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i6a 4-Apr-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: GIA specific version of shared globals (replace if compiling other projects)
 *
 *******************************************************************************/

#ifndef HEADER_GIA_GLOBAL_DEFS
#define HEADER_GIA_GLOBAL_DEFS

#include "SHAREDglobalDefs.h"



//#define GIA_DRAW_DEBUG
//#define GIA_DATABASE_DEBUG
//#define GIA_TRANSLATOR_DEBUG
//#define GIA_QUERY_DEBUG
//#define GIA_SEMANTIC_NET_XML_DEBUG
#define GIA_NLP_DEBUG

//~External NLP Parser
#define GIA_USE_RELEX
#define GIA_USE_STANFORD_CORENLP
#define GIA_USE_STANFORD_PARSER
#ifdef GIA_USE_RELEX
	#define GIA_RELEX_EXECUTABLE_NAME "execute-relex.sh"
	#define GIA_USE_RELEX_1.4.0	//default: enabled (otherwise use Relex 1.3.0)
	#ifdef GIA_USE_RELEX_1.4.0
		#define GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS		//BAI paragraph tag support has not yet been added to Relex 1.3.0
	#endif
#endif
#ifdef GIA_USE_STANFORD_CORENLP		//a more advanced implementation of stanford parser (with lemma, entity name detection, etc: Stanford CoreNLP integrates all our NLP tools for the English language, including the part-of-speech (POS) tagger, the named entity recognizer (NER), the parser, and the coreference resolution system)
	#define GIA_USE_STANFORD
	#define GIA_STANFORD_NLP_EXECUTABLE_NAME "execute-stanfordCoreNLP.sh"
#endif
#ifdef GIA_USE_STANFORD_PARSER
	#define GIA_USE_STANFORD
	#define GIA_STANFORD_PARSER_EXECUTABLE_NAME "execute-stanford.sh"
#endif
#define GIA_NLP_PARSER_RELEX (1)
#define GIA_NLP_PARSER_STANFORD_CORENLP (2)
#define GIA_NLP_PARSER_STANFORD_PARSER (3)
#ifdef GIA_USE_RELEX
	#define GIA_DEFAULT_NLP_PARSER (GIA_NLP_PARSER_RELEX)
#elif defined GIA_USE_STANFORD_CORENLP
	#define GIA_DEFAULT_NLP_PARSER (GIA_NLP_PARSER_STANFORD_CORENLP)
#elif defined GIA_USE_STANFORD_PARSER
	#define GIA_DEFAULT_NLP_PARSER (GIA_NLP_PARSER_STANFORD_PARSER)
#endif

//~GIATranslator
#define GIA_USE_CONCEPT_ENTITY_NODE_MAP_NOT_VECTOR	//this is required (the current set of code has had the alternative case removed - see GIATranslator.cpp.copyWithDataStructureOptions for an example set of code that supports disabling this feature)
#define REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE "_$qVar"
#define GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY
#define GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED
#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED
	#define GIA_PERFORM_RELATION_FUNCTION_ARGUMENT_SWITCHING_WHERE_NECESSARY (0)
	#define GIA_PERFORM_RELATION_FUNCTION_ARGUMENT_SWITCHING_ONLY_WHEN_REQUIRED (0)
#else
	#define GIA_PERFORM_RELATION_FUNCTION_ARGUMENT_SWITCHING_WHERE_NECESSARY (1)
	#define GIA_PERFORM_RELATION_FUNCTION_ARGUMENT_SWITCHING_ONLY_WHEN_REQUIRED (1)
#endif

//~GIAmain
#define GIA_COMPILE_FOR_BAI_APP_SERVER_RELEASE
//#define GIA_DO_NOT_PRINT_RESULTS
#ifdef USE_CE
	#define GIA_WITH_CE_DERIVE_SUBCLAIM_PREPEND
#endif

//~GIAquery
//#define GIA_QUERY_SUPPORT_NON_EXACT_QUERIES	//default: disabled (NB non-exact queries are not required for the text/query 'the dog is red' / 'is the dog red?'). non-exact queries used to be required for which questions, but not anymore [with GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES]	
	//these preprocessor have been moved to GIAglobalsDefs.h as they are used in both GIAtranslator.cpp and GIAquery.cpp
#define GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
	#define GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES_ADVANCED	//allows for verification that answer node is of type identified by the which query eg verifies that the answer 'dog' [from text: 'the dog eats the mud. dogs are animals.'], is an animal with the query: 'which animal eats the mud?'
#endif
#define FEATURE_QUERY_WORD_ACCEPTED_BY_ALTERNATE_METHOD_NUMBER_OF_TYPES (1)


//~GIAdraw
//#define GIA_DRAW_USE_PATENT
//#define TRIAL_DATA_CXL_CONVERSION_ONLY
#ifdef TRIAL_DATA_CXL_CONVERSION_ONLY
	#define GIA_CMAP_CONVERSION_SANITISED 	//use format akin to Cmap Tools / not GIA formatted. linking-phrase-list -> actions + conditions. concept-list -> concepts or properties
#endif


#endif
