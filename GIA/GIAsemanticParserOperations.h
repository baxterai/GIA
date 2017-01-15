/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is commercially licensed software. It may not be redistributed,
 * and may only be used by a registered licensee.
 *
 * BAIPROJECT is sold in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * BAI Commercial License for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You will have received a copy of the BAI Commercial License
 * along with BAIPROJECT. For more information, visit <http://www.baxterai.com>
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAsemanticParserOperations.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2p3a 14-January-2017
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_SEMANTIC_PARSER_OPERATIONS
#define HEADER_GIA_SEMANTIC_PARSER_OPERATIONS

#include "GIAglobalDefs.h"
#include "GIAtranslatorDefs.h"

#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER

void GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(constEffective GIAentityNode** GIAentityNodeArray, const GIAsentence* currentSentenceInList, int connectionType, int entityIndex1, int entityIndex2, bool sameReferenceSet);
void GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrainSpecial(constEffective GIAentityNode** GIAentityNodeArray, const GIAsentence* currentSentenceInList, const bool linkPreestablishedReferencesGIA, const int NLPdependencyRelationsType);
	void generateGIA2semanticDependencyRelation(constEffective GIAentityNode** GIAentityNodeArray, int connectionType, int entityIndex1, int entityIndex2, bool sameReferenceSet, const string* GIA2semanticDependencyRelationText, GIArelation* GIA2semanticDependencyRelation);
		#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS
		string generateGIA2semanticDependencyRelationText(const string entityName1, const string entityName2, const string semanticRelation, const int entityIndex1, const int entityIndex2, const bool sameReferenceSet);
			string createSameReferenceSetRecord(const bool sameReferenceSet);
string generateCorpusFileHeaderText(const GIAfeature* firstFeatureInSentence, const bool addPOSinfo);
#endif
void determineGIAconnectionistNetworkPOStypeNames(GIAfeature* firstFeatureInList, const int NLPfeatureParser);
	void determineGIAconnectionistNetworkPOStypeNameStanford(GIAfeature* currentFeatureInSentence);
	void determineGIAconnectionistNetworkPOStypeNameRelex(GIAfeature* currentFeatureInSentence);
#ifdef GIA2_SEMANTIC_PARSER
#ifdef GIA2_SEMANTIC_PARSER_OPTIMISE_BASED_ON_CONJUNCTIONS
GIAfeature* generateOptimisedFeatureSubsetBasedOnContextualConjunctions(GIAfeature* firstFeatureInSentenceSubset, const int centralWord, bool* optimisedBasedOnContextualConjunctions);
#endif
#ifdef GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE
int calculateFirstWordInTupleIndexRelative(const int firstWordInTupleIndex, const int firstWordInSentenceSubsetIndex);
#endif
#endif

#endif

#endif
