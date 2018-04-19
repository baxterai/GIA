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
 * File Name: GIAtxtRelTranslatorRulesComponentClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f1f 22-February-2018
 * Requirements: requires plain text file
 * Description: Textual Relation Translator Rules
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorRulesComponentClass.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_RULES


GIAtxtRelTranslatorRulesComponent::GIAtxtRelTranslatorRulesComponent(void)
{
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID
	referenceSetTypeHybrid = GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_REFERENCE_SET_TYPE_HYBRID_UNKNOWN;
	#endif
	
	componentType = GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_UNKNOWN;
	
	groupTypeRefName = "";
	groupRefName = "";
	
	stringType = GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_UNKNOWN;
	word = "";
	wordPOStype = "";
	tokenLayer = "";
	tokenClass = "";
	tokenType = "";
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_MISSING
	missing = false;
	#endif
	
	semanticRelationIndexType.resize(GIA_TXT_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP);
	for(int i=0; i<GIA_TXT_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP; i++)
	{
		semanticRelationIndexType[i] = GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_UNKNOWN;
	}
	semanticRelationReturnEntity = false;

	semanticRelationReturnFunctionName = "";
	semanticRelationReturnFunctionNameIndexType = GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_UNKNOWN;
	
	semanticRelationRecord = false;
	semanticRelationConnect = false;
	
	optional = false;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_QUERY
	queryComparisonVariable = false;
	isAuxiliaryQuery = false;
	isWhichOrEquivalentWhatQuery = false;
	#endif
	#ifdef GIA_ALIASES
	isNameQuery = false;
	isName = false;
	#endif
	#ifdef GIA_TRANSLATOR_NUMBER_OF
	isNumberOf = false;
	#endif
	
	
	groupTypeRef = NULL;
	groupRef = NULL;
	
	//subComponents = NULL;
	
	candidateStringMatch = NULL;
	parseTreeGroupRef = NULL;

}
GIAtxtRelTranslatorRulesComponent::~GIAtxtRelTranslatorRulesComponent(void)
{
}

#endif


