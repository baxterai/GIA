/*******************************************************************************
 *
 * File Name: GIATranslatorDefineProperties.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i6a 4-Apr-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_DEFINE_PROPERTIES
#define HEADER_GIA_TRANSLATOR_DEFINE_PROPERTIES

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
#include <vector>
using namespace std;


#include "GIAglobalDefs.h"					
#include "GIASentenceClass.h"
#include "GIAEntityNodeClass.h"
#include "GIAConditionNodeClass.h"

//Pass B
//0	
void collapseRedundantRelationAndMakeNegative(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);								//Stanford Compatible	
	#ifdef GIA_USE_RELEX
	void collapseRedundantRelationAndMakeNegativeRelex(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);
	#endif
	#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	void collapseRedundantRelationAndMakeNegativeStanford(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);		//Stanford Specific		
	#endif			
void definePropertiesObjectsAndSubjectsWithProperties(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);							//~Stanford Compatible [requires isObjectTemp, hasPropertyTemp, isSubjectTemp to be filled]
void definePropertiesDefiniteNouns(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsPersonArray[], bool GIAEntityNodeIsDate[], bool GIAEntityNodeIsAReference[]);				//~Stanford Compatible [requires GIAEntityNodeGrammaticalIsPersonArray[] + GIAEntityNodeIsDate[] + GIAEntityNodeIsAReferencep[] to be filled]
void definePropertiesNounsWithDeterminates(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], int referenceTypeHasDeterminateCrossReferenceNumberArray[], bool GIAEntityNodeGrammaticalIsPersonArray[], bool GIAEntityNodeIsDate[], bool GIAEntityNodeIsAReference[]);	//~Stanford Compatible [requires GIAEntityNodeGrammaticalIsPersonArray[] + GIAEntityNodeIsDate[] + GIAEntityNodeIsAReferencep[] to be filled]
void definePropertiesNounsWithAdjectives(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);								//Stanford Compatible
void definePropertiesQuantitiesAndMeasures(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);								//~Stanford Compatible [measures not supported]
void definePropertiesQuantityModifiers(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);									//~Stanford Compatible [measures not supported]
void definePropertiesExpletives(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);									//Stanford Compatible
void definePropertiesPronouns(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], bool GIAEntityNodeIsAReference[]);	//~Stanford Compatible {requires GRAMMATICAL_PRONOUN to be define + GIAEntityNodeIsAReferencep[] to be filled}
void definePropertiesToBe(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);										//Stanford Compatible
void definePropertiesToDo(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);										//Stanford Compatible
bool isAdjectiveNotConnectedToObjectOrSubject(Sentence * currentSentenceInList, Relation * currentRelationInList);								//Stanford Compatible
void definePropertiesHasTime(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);										//~Stanford Compatible {requires hasAssociatedTime to be filled}
#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1F_RELATIONS_TREAT_THAT_AS_A_PRONOUN_IE_PROPERTY											
void definePropertiesNonExplicitPronouns(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);								//Stanford Compatible
#endif
			
#endif
