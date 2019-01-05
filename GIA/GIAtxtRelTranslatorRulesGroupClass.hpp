/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAtxtRelTranslatorRulesGroupClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3g9b 03-January-2019
 * Requirements: requires plain text file
 * Description: Textual Relation Translator Rules
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TXT_REL_TRANSLATOR_RULES_GROUP_CLASS
#define HEADER_GIA_TXT_REL_TRANSLATOR_RULES_GROUP_CLASS

#include "GIAglobalDefs.hpp"
#include "GIApreprocessorWordClass.hpp"
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#endif



#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_GENERATED (1)	//type 1

#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PROPAGATED (3)	//type 2
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PREVIOUS_POS_WORD_TYPE_TESTED (4)	//type 2

#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_COMPONENT_CONNECTION_ACTIVE (5)	//type 3
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF (6)	//type 3
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED (9)	//type 3
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_SAVE (10)	//type 3
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_RESET (11)	//type 3

#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED (7)	//type 4
//#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_SOLIDIFY (8)	//type 4



//TODO: rename COMPONENT to GROUP TYPE:
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_UNKNOWN (0)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_LOGICREFERENCESET (1)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_REFERENCESET (2)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SUBREFERENCESET (3)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SENTENCE (4)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_NUMBER_OF_TYPES (5)
static string GIAtxtRelTranslatorRulesGroupsComponentReferenceSetTypes[GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_NUMBER_OF_TYPES] = {"unknown", "logicReferenceSet", "referenceSet", "subReferenceSet", "sentence"};


#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_UNKNOWN (0)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_LOGICREFERENCESETS (1)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_REFERENCESETS (2)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBREFERENCESETS (3)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBREFERENCESETPARTS (4)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBREFERENCESETAPPENDS (5)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_LOGICREFERENCESETSOPTIONAL (6)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_STATEMENTS (7)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_QUESTIONS (8)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBJECTS (9)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_NUMBER_OF_TYPES (10)	//this is just a selection of prominent groupTypes (it doesn't include all of them)
static string GIAtxtRelTranslatorRulesGroupsTypes[GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_NUMBER_OF_TYPES] = {"unknown", "logicReferenceSets", "referenceSets", "subReferenceSets", "subReferenceSetsPart", "subReferenceSetsAppend", "logicReferenceSetsOptional", "statements", "questions", "subjects"};




class GIAtxtRelTranslatorDebug
{
public:
	GIAtxtRelTranslatorDebug(void);
	~GIAtxtRelTranslatorDebug(void);
	
	int activationMemoryGroupArrayIndex;
	int activationMemoryGroupArraySize;
	bool firstComponentActive;
	bool secondComponentActive;
};



class GIAtxtRelTranslatorParserForwardPropogationSignalData
{
public:
	GIAtxtRelTranslatorParserForwardPropogationSignalData(void);
	~GIAtxtRelTranslatorParserForwardPropogationSignalData(void);
	
	GIAentityNode* semanticRelationReturnEntity;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RECORD_AND_CONNECT
	GIAentityNode* semanticRelationReturnEntitySubject;
	GIAentityNode* semanticRelationReturnEntityDelimiter;
	string semanticRelationReturnFunctionNameRecord;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME
	vector<GIAentityNode*> semanticRelationReturnFunctionEntityArray;
	#endif
};

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
class GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData
{
public:
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData(void);
	~GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData(void);
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	int wordNounVariantType;
	int wordVerbVariantType;
	#endif	
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
	bool foundPreviousActiveWord;
	#endif
};

class GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData
{
public:
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData(void);
	~GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData(void);
		
	 //word specific variables:
	GIApreprocessorPlainTextWord* wordReference;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
	int wordPOStype;	
	#endif
	
	int w;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC
	bool expectToSeeConnectionWithPreviousWordTrace;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
	vector<GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData*> previousWordConnections;
	#endif
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC
	bool foundPreviousActiveWord;	//debug only
	#endif
};
#endif

class GIAtxtRelTranslatorRulesComponent;

class GIAtxtRelTranslatorRulesGroup
{
public:
	GIAtxtRelTranslatorRulesGroup(void);
	~GIAtxtRelTranslatorRulesGroup(void);

	vector<string> semanticRelationFunctionName;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_NEW_CONDITIONS
	string semanticRelationFunctionConditionNewName;
	#endif
	string groupName;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_GROUP_PREVIOUS_WORD_POS_TYPE
	string previousWordPOStype;
	string existsPreceedingWordPOStype;
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	int wordVerbVariantTypeDerived;
	int wordNounVariantTypeDerived;	
	#endif
		
	string groupTypeNameBackup;
	int groupTypeReferenceSetTypeBackup;
	
	int numberOfConsecutiveTimesPreviousGroupType;
								
	vector<GIAtxtRelTranslatorRulesComponent*> components;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
	int numberWordsInSet;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData forwardPropogationSignalData;
	#endif
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData forwardPropogationWordData;
	int layer;
	bool inputGroupString;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
	GIAtxtRelTranslatorRulesGroup* groupOrig;	//for memoryActivationGroups only
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
	bool pointArrayGenerated;
	bool firstLevelActivationPointAdded;
	vector<GIAtxtRelTranslatorRulesGroup*> activationPathWordFirstActivationMemoryGroupActivationPointArray;
	vector<GIAtxtRelTranslatorRulesGroup*> activationPathWordFirstParseTreeGroupActivationPointArray;	
	#endif
	bool wordGroupNeuron;
		
	vector<GIAtxtRelTranslatorRulesGroup*> ANNfrontGroupConnectionList;	//for IO only
	vector<GIAtxtRelTranslatorRulesGroup*> ANNbackGroupConnectionList;	//for IO only
	vector<GIAtxtRelTranslatorRulesComponent*> ANNfrontComponentConnectionList;
	GIAtxtRelTranslatorRulesGroup* next;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
	ANNneuron* neuronReference;
	#endif
	
	string GIAtokenLayerName;
	string GIAtokenLayerClassName;
	string GIAtokenLayerClassTypeName;
	string GIAtokenLayerClassTypeInstanceName;
	
	bool neuronGenerated;
	bool neuronPropagated;
	bool neuronPropagatedSave;
	bool neuronPreviousWordPOStypeTested;
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData semanticRelationReturnEntityForwardPropogationSignalData;
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//temporary	//only update semanticRelationReturnEntityForwardPropogationSignalData upon complete/group activation (prevents ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData from being invalidated by partial reactivations of a group; when accessed by an unrelated instance of GIAtxtRelTranslatorNeuralNetworkParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwor) 
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_UNOPTIMISED
	vector<GIAtxtRelTranslatorRulesGroup*> activationMemoryGroupArray;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
	int lastWordIndexActivated;
	#endif
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_UNOPTIMISED
	vector<GIAtxtRelTranslatorRulesGroup*> currentParseTreeGroupArray;
	#endif
	//parse tree variables:
	GIAtxtRelTranslatorRulesGroup* parseTreeGroupRefReverse;
	int parseTreeGroupRefReverseComponentIndex;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
	bool solidified;
	#endif
	#endif
	bool neuronTraced;
	bool neuronActive;	//interpretation: all components are active (unless they are optional / unused or cases)
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP
	//bool neuronSolidified;
	#endif
	#endif

	

};


class GIAtxtRelTranslatorRulesGroupType
{
public:
	GIAtxtRelTranslatorRulesGroupType(void);
	~GIAtxtRelTranslatorRulesGroupType(void);

	string groupTypeName;
	int referenceSetType;
		
	vector<GIAtxtRelTranslatorRulesGroup*> groups;
};



#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
	
class GIAtxtRelTranslatorNeuralNetworkForwardPropogationActivationPointData
{
public:
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationActivationPointData(void);
	~GIAtxtRelTranslatorNeuralNetworkForwardPropogationActivationPointData(void);
	
	bool generateActivationPointArray;
	bool connectToPreviousActivationGroup;
	vector<GIAtxtRelTranslatorRulesGroup*>* activationPathWordFirstActivationMemoryGroupActivationPointArray;
	vector<GIAtxtRelTranslatorRulesGroup*>* activationPathWordFirstParseTreeGroupActivationPointArray;
	int layer;	
	int activationPathW;
};

class GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData
{
public:
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData(void);
	~GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData(void);
		
	//sentence specific variables:
	bool toplevelGroupActivationFound;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	bool parserEnabled;
	int performance;
	#endif
	vector<GIApreprocessorPlainTextWord*>* sentenceContents;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_QUERIES
	bool isQuery;
	#endif
	//#ifdef GIA_TXT_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
	bool parseIsolatedSubreferenceSets;
	//#endif
	
	bool finishedPassingSentenceWords;
	
	vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes;

	vector<GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData*> forwardPropogationWordDataArray;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_UNOPTIMISED
	vector<GIAtxtRelTranslatorRulesGroup*> activationPathWordFirstParseTreeGroupArray;
	#else
	vector<vector<GIAtxtRelTranslatorRulesGroup*>> activationPathWordFirstActivationMemoryGroupArray;
	vector<vector<GIAtxtRelTranslatorRulesGroup*>> activationPathWordFirstParseTreeGroupArray;
	#endif
	
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationActivationPointData* forwardPropogationActivationPointData;
};

#endif


class GIAtxtRelTranslatorRulesGroupClass
{
	public: void copyParseGroupInfo(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroupTemp, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup);
	public: bool isTopLevelGroupType(const string groupTypeName, const int groupTypeReferenceSetType, const bool isQuery, const bool parseIsolatedSubreferenceSets);	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_QUERIES
	public: bool determineIsQuery(vector<GIApreprocessorPlainTextWord*>* sentenceContents);
	#endif
};





#endif
