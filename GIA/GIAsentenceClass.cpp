/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
 *
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAsentenceClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2h6a 18-January-2015
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 *
 *******************************************************************************/



#include "GIAsentenceClass.h"




#ifdef GIA_USE_STANFORD_CORENLP


StanfordCoreNLPmention::StanfordCoreNLPmention(void)
{
	representative = false;
	sentence = INT_DEFAULT_VALUE;
	start = INT_DEFAULT_VALUE;
	end = INT_DEFAULT_VALUE;
	head = INT_DEFAULT_VALUE;

	next = NULL;
}

StanfordCoreNLPmention::~StanfordCoreNLPmention(void)
{
	if(next != NULL)
	{
		delete next;
	}
}

StanfordCoreNLPcoreference::StanfordCoreNLPcoreference(void)
{
	firstMentionInList = new StanfordCoreNLPmention();

	next = NULL;
}

StanfordCoreNLPcoreference::~StanfordCoreNLPcoreference(void)
{
	if(firstMentionInList != NULL)
	{
		delete firstMentionInList;
	}

	if(next != NULL)
	{
		delete next;
	}
}
#endif

#ifdef GIA_USE_ADVANCED_REFERENCING
GIAMention::GIAMention(void)
{
	representative = false;
	idActiveList = INT_DEFAULT_VALUE;
	entityIndex = INT_DEFAULT_VALUE;	//ie, "head"
	entityName = "";
	intrasentenceReference = false;		//only applies to representative/source mentions (specifies whether reference source is contained within current sentence)

	next = NULL;
}

GIAMention::~GIAMention(void)
{
	if(next != NULL)
	{
		delete next;
	}
}

GIACoreference::GIACoreference(void)
{
	firstMentionInList = new GIAMention();

	next = NULL;
}

GIACoreference::~GIACoreference(void)
{
	#ifdef GIA_FREE_MEMORY_DEBUG
	//cout << "delete GIACoreference " << endl;
	#endif
	if(firstMentionInList != NULL)
	{
		delete firstMentionInList;
	}

	if(next != NULL)
	{
		delete next;
	}
}

#endif





Relation::Relation(void)
{
	relationType = "";
	#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW
	relationTypeIndex = INT_DEFAULT_VALUE;
	#endif
	relationDependent = "";
	relationDependentIndex = INT_DEFAULT_VALUE;
	relationGovernor = "";
	relationGovernorIndex = INT_DEFAULT_VALUE;

	disabled = false;
	//#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	disabledDuringLink = false;
	//#endif

	#ifdef GIA_USE_RELEX
	subjObjRelationAlreadyAdded = false;
	#endif

	#ifdef GIA_USE_STANFORD_CORENLP
	prepositionCombinationAlreadyCreatedTemp = false;
	#endif

	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	auxiliaryIndicatesDifferentReferenceSet = false;
	rcmodIndicatesSameReferenceSet = false;
	#endif

	#ifdef GIA_USE_CORPUS_DATABASE
	sameReferenceSet = false;
	#ifdef GIA2_SUPPORT_QUERIES
	corpusSpecialRelationGovernorIsQuery = "";
	corpusSpecialRelationDependentIsQuery = "";
	#endif
	#endif
	
	#ifdef GIA_LRP_NORMALISE_PREPOSITIONS
	#ifdef GIA_LRP_NORMALISE_INVERSE_PREPOSITIONS
	inverseRelation = false;
	#endif
	#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS
	relationTwoWay = false;
	#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
	inverseRelationTwoWay = false;
	#endif
	#endif
	#endif
	
	next = NULL;
}

Relation::~Relation(void)
{
	if(next != NULL)
	{
		delete next;
	}

}

Feature::Feature(void)
{
	entityIndex = 0;
	word = "";
	lemma = "";
	#ifdef GIA_USE_LRP
	wordWithLRPforNLPonly = "";
	#endif

	#ifdef GIA_USE_RELEX
	type = "";
	grammar = "";
	#endif

	NER = FEATURE_NER_UNDEFINED;
	#ifdef GIA_USE_STANFORD_CORENLP
	CharacterOffsetBegin = INT_DEFAULT_VALUE;
	CharacterOffsetEnd = INT_DEFAULT_VALUE;
	stanfordPOS = "";
	NormalizedNER = "";
	Timex = "";
	#endif

	//derived variables:
	grammaticalIsDateOrTime = false;
	grammaticalTense = GRAMMATICAL_TENSE_UNDEFINED;
	for(int q=0; q<GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES;q++)
	{
		grammaticalTenseModifierArray[q] = false;
	}
	grammaticalNumber = GRAMMATICAL_NUMBER_UNDEFINED;
	grammaticalIsDefinite = false;
	grammaticalIsIndefinitePlural = false;
	grammaticalIsProperNoun = false;
	grammaticalGender = GRAMMATICAL_GENDER_UNDEFINED;
	grammaticalIsPronoun = false;
	grammaticalWordType = GRAMMATICAL_WORD_TYPE_UNDEFINED;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	grammaticalIndexOfDeterminer = GIA_ENTITY_INDEX_UNDEFINED;
	#endif
	previousWordInSentenceIsTo = false;

	#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
	alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = false;		//#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
	mustSetIsSubstanceConceptBasedOnApposRelation = false;
	isPronounReference = false;
	#endif

	entityDisabled = false;

	#ifdef GIA_USE_CORPUS_DATABASE
	GIAconnectionistNetworkPOStype = 0;	//ie GIA_CONNECTIONIST_NETWORK_POS_TYPE_UNDEFINED
	#endif

	next = NULL;
	previous = NULL;
}

Feature::~Feature(void)
{
	if(next != NULL)
	{
		delete next;
	}

}


Sentence::Sentence(void)
{
	#ifdef GIA_USE_RELEX
	sentenceText = "";
	constituentsText = "";
	featuresText = "";
	relationsText = "";
	linksText = "";
	#endif

	sentenceIndex = GIA_SENTENCE_INDEX_UNDEFINED;
	#ifdef GIA_USE_STANFORD_CORENLP
	firstCoreferenceInList = new StanfordCoreNLPcoreference();
	#endif

	maxNumberOfWordsInSentence = 0;

	firstRelationInList = new Relation();	//auto constructor execution added 23 Feb 2012
	firstFeatureInList = new Feature();	//auto constructor execution added 23 Feb 2012

	next = NULL;
	previous = NULL;

	isQuestion = false;

	corpusLookupSuccessful = false;
	
	conditionEntityArtificialIndexCurrent = MAX_NUMBER_OF_WORDS_PER_SENTENCE - 2;	//NB REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_DEPENDENT_INDEX = (MAX_NUMBER_OF_WORDS_PER_SENTENCE-1)
}

Sentence::~Sentence(void)
{
	if(firstRelationInList != NULL)
	{
		delete firstRelationInList;
	}

	if(firstFeatureInList != NULL)	//added 23 Feb 2012
	{
		delete firstFeatureInList;
	}

	#ifdef GIA_FREE_MEMORY1
	#ifdef GIA_USE_STANFORD_CORENLP
	if(firstCoreferenceInList != NULL)	//added 21 Sept 2012
	{
		delete firstCoreferenceInList;
	}
	#endif
	#endif

	if(next != NULL)
	{
		delete next;
	}

}


Paragraph::Paragraph(void)
{
	firstSentenceInList = new Sentence();

	next = NULL;
	previous = NULL;
}

Paragraph::~Paragraph(void)
{
	if(firstSentenceInList != NULL)
	{
		delete firstSentenceInList;
	}

	if(next != NULL)
	{
		delete next;
	}
}

void copySentences(Sentence * sentenceToCopy, Sentence * newSentence)
{
	newSentence->sentenceIndex = sentenceToCopy->sentenceIndex;

	#ifdef GIA_USE_RELEX
	newSentence->sentenceText = sentenceToCopy->sentenceText;
	newSentence->constituentsText = sentenceToCopy->constituentsText;
	newSentence->featuresText = sentenceToCopy->featuresText;
	newSentence->relationsText = sentenceToCopy->relationsText;
	newSentence->linksText = sentenceToCopy->linksText;
	#endif

	#ifdef GIA_USE_STANFORD_CORENLP
	copyStanfordCoreferences(sentenceToCopy->firstCoreferenceInList, newSentence->firstCoreferenceInList);	//changed 21 Sept 2012
	//newSentence->firstCoreferenceInList = sentenceToCopy->firstCoreferenceInList;
	#endif

	newSentence->maxNumberOfWordsInSentence = sentenceToCopy->maxNumberOfWordsInSentence;

	copyRelations(sentenceToCopy->firstRelationInList, newSentence->firstRelationInList);
	copyFeatures(sentenceToCopy->firstFeatureInList, newSentence->firstFeatureInList);

	newSentence->next = NULL;
	newSentence->previous = NULL;

	newSentence->isQuestion = sentenceToCopy->isQuestion;
}


void copyRelations(Relation * firstRelationInListToCopy, Relation * firstRelationInList)
{
	Relation * currentRelationToCopy = firstRelationInListToCopy;
	Relation * currentRelation = firstRelationInList;
	while(currentRelationToCopy->next != NULL)
	{

		currentRelation->relationType = currentRelationToCopy->relationType;
		#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW
		currentRelation->relationTypeIndex = currentRelationToCopy->relationTypeIndex;
		#endif
		currentRelation->relationDependent = currentRelationToCopy->relationDependent;
		currentRelation->relationDependentIndex = currentRelationToCopy->relationDependentIndex;
		currentRelation->relationGovernor = currentRelationToCopy->relationGovernor;
		currentRelation->relationGovernorIndex = currentRelationToCopy->relationGovernorIndex;

		#ifdef GIA_ADVANCED_REFERENCING_DEBUG
		//cout << "copy relation:" << endl;
		//cout << currentRelation->relationType << "(" << currentRelation->relationGovernor << ", " << currentRelation->relationDependent << ")" << endl;
		#endif

		#ifdef GIA_USE_CORPUS_DATABASE
		#ifdef GIA2_SUPPORT_QUERIES
		currentRelation->corpusSpecialRelationGovernorIsQuery = currentRelationToCopy->corpusSpecialRelationGovernorIsQuery;
		currentRelation->corpusSpecialRelationDependentIsQuery = currentRelationToCopy->corpusSpecialRelationDependentIsQuery;
		#endif
		#endif

		Relation * newRelation = new Relation();
		//newRelation->previous = currentRelation;
		currentRelation->next = newRelation;

		currentRelationToCopy = currentRelationToCopy->next;
		currentRelation = currentRelation->next;
	}
}

void copyFeatures(Feature * firstFeatureInListToCopy, Feature * firstFeatureInList)
{
	Feature * currentFeatureToCopy = firstFeatureInListToCopy;
	Feature * currentFeature = firstFeatureInList;
	while(currentFeatureToCopy->next != NULL)
	{

		currentFeature->entityIndex = currentFeatureToCopy->entityIndex;
		currentFeature->word = currentFeatureToCopy->word;
		currentFeature->lemma = currentFeatureToCopy->lemma;

		#ifdef GIA_USE_RELEX
		currentFeature->type = currentFeatureToCopy->type;
		currentFeature->grammar = currentFeatureToCopy->grammar;
		#endif

		currentFeature->NER = currentFeatureToCopy->NER;
		#ifdef GIA_USE_STANFORD_CORENLP
		currentFeature->CharacterOffsetBegin = currentFeatureToCopy->CharacterOffsetBegin;
		currentFeature->CharacterOffsetEnd = currentFeatureToCopy->CharacterOffsetEnd;
		currentFeature->stanfordPOS = currentFeatureToCopy->stanfordPOS;
		currentFeature->NormalizedNER = currentFeatureToCopy->NormalizedNER;
		currentFeature->Timex = currentFeatureToCopy->Timex;
		#endif

		#ifdef GIA_ADVANCED_REFERENCING_DEBUG
		//cout << "copy feature:" << endl;
		//cout << currentFeature->lemma << endl;
		#endif

		Feature * newFeature = new Feature();
		newFeature->previous = currentFeature;
		currentFeature->next = newFeature;

		currentFeatureToCopy = currentFeatureToCopy->next;
		currentFeature = currentFeature->next;
	}
}


#ifdef GIA_USE_STANFORD_CORENLP
void copyStanfordCoreferences(StanfordCoreNLPcoreference * firstCoreferenceInListToCopy, StanfordCoreNLPcoreference * firstCoreferenceInList)
{
	StanfordCoreNLPcoreference * currentCoreferenceInListToCopy = firstCoreferenceInListToCopy;
	StanfordCoreNLPcoreference * currentCoreferenceInList = firstCoreferenceInList;
	while(currentCoreferenceInListToCopy->next != NULL)
	{
		currentCoreferenceInList->firstMentionInList = new StanfordCoreNLPmention();
		copyStanfordMention(currentCoreferenceInListToCopy->firstMentionInList, currentCoreferenceInList->firstMentionInList);

		#ifdef GIA_ADVANCED_REFERENCING_DEBUG
		//cout << "copy Stanford coreference:" << endl;
		//cout << currentCoreferenceInList->head << endl;
		#endif

		StanfordCoreNLPcoreference * newCoreference = new StanfordCoreNLPcoreference();
		currentCoreferenceInList->next = newCoreference;

		currentCoreferenceInListToCopy = currentCoreferenceInListToCopy->next;
		currentCoreferenceInList = currentCoreferenceInList->next;
	}
}

void copyStanfordMention(StanfordCoreNLPmention * firstMentionInListToCopy, StanfordCoreNLPmention * firstMentionInList)
{
	StanfordCoreNLPmention * currentMentionInListToCopy = firstMentionInListToCopy;
	StanfordCoreNLPmention * currentMentionInList = firstMentionInList;
	while(currentMentionInListToCopy->next != NULL)
	{
		currentMentionInList->representative = currentMentionInListToCopy->representative;
		currentMentionInList->sentence = currentMentionInListToCopy->sentence;
		currentMentionInList->start = currentMentionInListToCopy->start;
		currentMentionInList->end = currentMentionInListToCopy->end;
		currentMentionInList->head = currentMentionInListToCopy->head;

		#ifdef GIA_ADVANCED_REFERENCING_DEBUG
		//cout << "copy Stanford Mention:" << endl;
		//cout << currentMentionInList->head << endl;
		#endif

		StanfordCoreNLPmention * newMention = new StanfordCoreNLPmention();
		currentMentionInList->next = newMention;

		currentMentionInListToCopy = currentMentionInListToCopy->next;
		currentMentionInList = currentMentionInList->next;
	}
}

#endif





