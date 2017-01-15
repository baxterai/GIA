/*******************************************************************************
 * 
 * This file is part of BAIPROJECT.
 * 
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * File Name: GIATranslatorDefineProperties.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1o1a 09-August-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors entityNodesActiveListConcepts/conceptEntityNamesList with a map, and replace vectors GIATimeConditionNode/timeConditionNumbersActiveList with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIATranslatorDefineProperties.h"
#include "GIATranslatorOperations.h"
#include "GIAdatabase.h"





/*
void collapseRedundantRelationAndMakeNegative(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], int NLPdependencyRelationsType)
{
	#ifdef GIA_USE_RELEX
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		collapseRedundantRelationAndMakeNegativeRelex(currentSentenceInList, GIAEntityNodeArray);
	}
	#endif
	#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		collapseRedundantRelationAndMakeNegativeStanford(currentSentenceInList, GIAEntityNodeArray);
	}
	#endif
}
*/

#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
void collapseRedundantRelationAndMakeNegativeStanford(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_NEGATIVE)
			{
				//eg The chicken has not eaten a pie.: neg(eaten-5, not-4)

				currentRelationInList->disabled = true;
				GIAEntityNodeArray[currentRelationInList->relationGovernorIndex]->negative = true;
				disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList->relationDependentIndex]);
			}

		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}
#endif

#ifdef GIA_USE_RELEX
void collapseRedundantRelationAndMakeNegativeRelex(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	//_subj(not[5], by[4]), _subj(have[6], not[5])

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT)
			{

				bool passed = false;
				for(int j=0; j<RELATION_TYPE_NEGATIVE_CONTEXT_NUMBER_OF_TYPES; j++)
				{
					if(GIAEntityNodeArray[currentRelationInList->relationGovernorIndex]->entityName == relationContextNegativeNameArray[j])
					{
						passed = true;
					}
				}
				if(passed)
				{
					Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
					while(currentRelationInList2->next != NULL)
					{
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
						if(!(currentRelationInList2->disabled))
						{
						#endif
							//cout << "here1" << endl;
							//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

							if(currentRelationInList2->relationType == RELATION_TYPE_SUBJECT)
							{

								bool passed2 = false;
								for(int j=0; j<RELATION_TYPE_NEGATIVE_CONTEXT_NUMBER_OF_TYPES; j++)
								{
									if(GIAEntityNodeArray[currentRelationInList2->relationDependentIndex]->entityName == relationContextNegativeNameArray[j])
									{
										passed2 = true;
									}
								}
								if(passed2)
								{
									disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList2->relationGovernorIndex]);
									disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList2->relationDependentIndex]);

									GIAEntityNodeArray[currentRelationInList2->relationDependentIndex] = GIAEntityNodeArray[currentRelationInList->relationDependentIndex];
									GIAEntityNodeArray[currentRelationInList2->relationGovernorIndex]->negative = true;
									currentRelationInList->disabled = true;
								}
							}
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
						}
						#endif
						currentRelationInList2 = currentRelationInList2->next;
					}
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}
#endif

void definePropertiesObjectsAndSubjectsWithProperties(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], GIAEntityNode * GIAFeatureTempEntityNodeArray[])
{
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAEntityNodeArrayFilled[i])
		{
			if(((GIAFeatureTempEntityNodeArray[i]->isObjectTemp) && (GIAFeatureTempEntityNodeArray[i]->hasPropertyTemp)) || ((GIAFeatureTempEntityNodeArray[i]->isSubjectTemp) && (GIAFeatureTempEntityNodeArray[i]->hasPropertyTemp)))
			{
				GIAEntityNodeArray[i] = addPropertyToPropertyDefinition(GIAEntityNodeArray[i]);
			}
		}
	}
}

void definePropertiesDefiniteNouns(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], Feature * featureArrayTemp[])
{
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAEntityNodeArrayFilled[i])
		{ //condition required as GIAEntityNodeArrayFilled is not always true. With grammaticalIsDefinite, eg "Mr" of "Mr Smith" will still be interpreted as a definite
			if(!(featureArrayTemp[i]->isPronounReference))
			{//do not define properties based upon references (as the grammatical information is no longer correct, and it has already been done previously if necessary to the referenced entity)
				#ifndef GIA_ASSIGN_INSTANCE_PROPERTY_TO_PROPER_NOUNS
				if(!(featureArrayTemp[i]->grammaticalIsProperNoun))	//&& !GIAEntityNodeIsDateOrTime[i]
				{
				#endif
					if(featureArrayTemp[i]->grammaticalIsDefinite)
					{
						//cout << "as0" << endl;
						//cout << "GIAEntityNodeArray[i]->entityName = " << GIAEntityNodeArray[i]->entityName << endl;
						GIAEntityNodeArray[i] = addPropertyToPropertyDefinition(GIAEntityNodeArray[i]);
					}
				#ifndef GIA_ASSIGN_INSTANCE_PROPERTY_TO_PROPER_NOUNS	
				}
				#endif
			}
		}
	}
}

#ifdef GIA_DEFINE_PROPERTIES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
//Added 09 August 2012 [INCOMPLETE]
void definePropertiesBasedOnDeterminatesOfDefinitionEntities(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[],  int referenceTypeHasDeterminateCrossReferenceNumberArray[], Feature * featureArrayTemp[])
{
	/* Added 09 August 2012
	The bat is an animal - (definition connection, bat = property, animal = concept)	GIA_DEFINE_PROPERTIES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_1_GOVERNOR_DEFINITE_DEPENDENT_INDEFINITE [assumes definePropertiesDefiniteNouns() executed]
		eg2; The red bat is a zenomorph. The blue bat is a non-zenomorph.
		NO: definite implies animal is an adjective/quality [check NLP]
	Bats are animals.  (definition connection, bat = concept, animal = concept)		GIA_DEFINE_PROPERTIES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_2_GOVERNOR_PLURAL_DEPENDENT_PLURAL
	A book is an animal (definition connection, bat = concept, animal = concept)		GIA_DEFINE_PROPERTIES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_3_GOVERNOR_INDEFINITE_DEPENDENT_INDEFINITE
	Bat is an animal. (definition connection, bat = concept or property [if GIA_ASSIGN_INSTANCE_PROPERTY_TO_PROPER_NOUNS] with isName==true, animal = concept)	GIA_DEFINE_PROPERTIES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_4_GOVERNOR_NAME_DEPENDENT_INDEFINITE
	The bat is Max. (double definition connection, bat = property, animal = concept or property [if GIA_ASSIGN_INSTANCE_PROPERTY_TO_PROPER_NOUNS] with isName==true)		GIA_DEFINE_PROPERTIES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_5_GOVERNOR_DEFINITE_DEPENDENT_NAME	[assumes definePropertiesDefiniteNouns() executed]
	Bat is the animal. (double definition connection, bat = concept or property [if GIA_ASSIGN_INSTANCE_PROPERTY_TO_PROPER_NOUNS] with isName==true, animal = property)	GIA_DEFINE_PROPERTIES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_6_GOVERNOR_NAME_DEPENDENT_DEFINITE
	*/
					
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_APPOSITIVE_OF_NOUN)
			{
				//cout << "definePropertiesBasedOnDeterminatesOfDefinitionEntities(): RELATION_TYPE_APPOSITIVE_OF_NOUN" << endl;
				
				int thingIndex = currentRelationInList->relationGovernorIndex;
				int definitionIndex = currentRelationInList->relationDependentIndex;
				
				bool thingFeatureHasDeterminate = false;
				for(int j=0; j<GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES; j++)
				{
					if(featureArrayTemp[thingIndex]->grammaticalNumber == referenceTypeHasDeterminateCrossReferenceNumberArray[j])	//changed from GIAConceptNodeArray to featureArrayTemp 14 July 2012b
					{
						thingFeatureHasDeterminate = true;
					}
				}
				bool definitionFeatureHasDeterminate = false;
				for(int j=0; j<GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES; j++)
				{
					if(featureArrayTemp[definitionIndex]->grammaticalNumber == referenceTypeHasDeterminateCrossReferenceNumberArray[j])	//changed from GIAConceptNodeArray to featureArrayTemp 14 July 2012b
					{
						definitionFeatureHasDeterminate = true;
					}
				}	
				
				bool thingFeatureIsProperNoun = featureArrayTemp[thingIndex]->grammaticalIsProperNoun;	
				bool definitionFeatureIsProperNoun = featureArrayTemp[definitionIndex]->grammaticalIsProperNoun;								
				
				bool thingIsDefinite =  featureArrayTemp[thingIndex]->grammaticalIsDefinite;	
				bool definitionIsDefinite = featureArrayTemp[definitionIndex]->grammaticalIsDefinite;	
										
				GIAEntityNode * thingEntity = GIAEntityNodeArray[thingIndex];
				GIAEntityNode * definitionEntity = GIAEntityNodeArray[definitionIndex];
				featureArrayTemp[thingIndex]->alreadyAssignedPropertiesBasedOnDeterminatesOfDefinitionEntitiesTemp = true;
				featureArrayTemp[definitionIndex]->alreadyAssignedPropertiesBasedOnDeterminatesOfDefinitionEntitiesTemp = true;
				
				/*
				cout << "thingName = " << thingEntity->entityName << endl;
				cout << "\tthingFeatureHasDeterminate = " << thingFeatureHasDeterminate << endl;
				cout << "\tthingIsDefinite = " << thingIsDefinite << endl;
				cout << "\tthingFeatureIsProperNoun = " << thingFeatureIsProperNoun << endl;
				cout << "definitionName = " << definitionEntity->entityName << endl;
				cout << "\tdefinitionFeatureHasDeterminate = " << definitionFeatureHasDeterminate << endl;
				cout << "\tdefinitionIsDefinite = " << definitionIsDefinite << endl;
				cout << "\tdefinitionFeatureIsProperNoun = " << definitionFeatureIsProperNoun << endl;
				*/
				
				if((thingIsDefinite) && (definitionFeatureHasDeterminate && !definitionIsDefinite && !definitionFeatureIsProperNoun))
				{
					//cout << "GIA_DEFINE_PROPERTIES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_1_GOVERNOR_DEFINITE_DEPENDENT_INDEFINITE" << endl;	
					//property already assigned to thing  [assumes definePropertiesDefiniteNouns() executed]
					//property will not be assigned to definition
				}
				else if((!thingFeatureHasDeterminate && !thingIsDefinite && !thingFeatureIsProperNoun) && (!definitionFeatureHasDeterminate && !definitionIsDefinite && !definitionFeatureIsProperNoun))
				{
					//cout << "GIA_DEFINE_PROPERTIES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_2_GOVERNOR_PLURAL_DEPENDENT_PLURAL" << endl;	
					//property will not be assigned to thing
					//property will not be assigned to definition
				}
				else if((thingFeatureHasDeterminate && !thingIsDefinite && !thingFeatureIsProperNoun) && (definitionFeatureHasDeterminate && !definitionIsDefinite && !definitionFeatureIsProperNoun))
				{
					//cout << "GIA_DEFINE_PROPERTIES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_3_GOVERNOR_INDEFINITE_DEPENDENT_INDEFINITE" << endl;	
					//property will not be assigned to thing
					//property will not be assigned to definition				
				}
				else if((thingFeatureHasDeterminate && !thingIsDefinite && thingFeatureIsProperNoun) && (definitionFeatureHasDeterminate && !definitionIsDefinite && !definitionFeatureIsProperNoun))		//NB thingFeatureHasDeterminate will be true for proper nouns [as haveDeterminate is regenerated based upon GRAMMATICAL_NUMBER_SINGULAR], even though proper nouns do not actually have a determinate (eg a/the) assigned
				{
					//cout << "GIA_DEFINE_PROPERTIES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_4_GOVERNOR_NAME_DEPENDENT_INDEFINITE" << endl;	
					//property will be assigned to thing [only if GIA_ASSIGN_INSTANCE_PROPERTY_TO_PROPER_NOUNS]
					featureArrayTemp[thingIndex]->alreadyAssignedPropertiesBasedOnDeterminatesOfDefinitionEntitiesTemp = false;	
					//property will not be assigned to definition
				}
				else if((thingFeatureHasDeterminate && thingIsDefinite && !thingFeatureIsProperNoun) && (definitionFeatureHasDeterminate && !definitionIsDefinite && definitionFeatureIsProperNoun))
				{
					//cout << "GIA_DEFINE_PROPERTIES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_5_GOVERNOR_DEFINITE_DEPENDENT_NAME" << endl;	
					//property already assigned to thing  [assumes definePropertiesDefiniteNouns() executed]
					//property will be assigned to definition [only if GIA_ASSIGN_INSTANCE_PROPERTY_TO_PROPER_NOUNS]
					featureArrayTemp[definitionIndex]->alreadyAssignedPropertiesBasedOnDeterminatesOfDefinitionEntitiesTemp = false;	
				}												
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}
#endif

void definePropertiesNounsWithDeterminates(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], int referenceTypeHasDeterminateCrossReferenceNumberArray[], Feature * featureArrayTemp[])
{
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAEntityNodeArrayFilled[i])
		{
			if(!(featureArrayTemp[i]->isPronounReference))
			{//do not define properties based upon references (as the grammatical information is no longer correct, and it has already been done previously if necessary to the referenced entity)
				#ifndef GIA_ASSIGN_INSTANCE_PROPERTY_TO_PROPER_NOUNS
				if(!(featureArrayTemp[i]->grammaticalIsProperNoun))	//&& !GIAEntityNodeIsDateOrTime[i]
				{
				#endif
					bool passed = false;
					for(int j=0; j<GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES; j++)
					{
						if(featureArrayTemp[i]->grammaticalNumber == referenceTypeHasDeterminateCrossReferenceNumberArray[j])	//changed from GIAConceptNodeArray to featureArrayTemp 14 July 2012b
						{
							passed = true;
						}
					}
					if(passed)
					{
						#ifdef GIA_DEFINE_PROPERTIES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
						if(!(featureArrayTemp[i]->alreadyAssignedPropertiesBasedOnDeterminatesOfDefinitionEntitiesTemp))
						{
						#endif
							//cout << "GIAEntityNodeArray[i]->entityName = " << GIAEntityNodeArray[i]->entityName << endl;
							//cout << "as1" << endl;
							GIAEntityNodeArray[i] = addPropertyToPropertyDefinition(GIAEntityNodeArray[i]);
							//cout << "as2" << endl;
						#ifdef GIA_DEFINE_PROPERTIES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
						}
						#endif								
					}
				#ifndef GIA_ASSIGN_INSTANCE_PROPERTY_TO_PROPER_NOUNS	
				}
				#endif
			}
		}
	}
}

void definePropertiesNounsWithAdjectives(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], int NLPdependencyRelationsType)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool passed = false;
			for(int i=0; i<RELATION_TYPE_ADJECTIVE_WHICH_IMPLY_ENTITY_INSTANCE_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeAdjectiveWhichImplyEntityInstanceNameArray[i])
				{
					passed = true;
				}
			}
			if(passed)
			{
				bool passed3 = isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe(currentRelationInList, GIAEntityNodeArray, currentRelationInList->relationGovernorIndex, NLPdependencyRelationsType);

				if(passed3)
				{
					//create a new property for the entity and assign a property definition entity if not already created
					string thingName = currentRelationInList->relationGovernor;
					string propertyName = currentRelationInList->relationDependent;
					int thingIndex = currentRelationInList->relationGovernorIndex;
					int propertyIndex = currentRelationInList->relationDependentIndex;


					GIAEntityNode * thingEntity = GIAEntityNodeArray[thingIndex];
					GIAEntityNode * propertyEntity = GIAEntityNodeArray[propertyIndex];

					//cout << "as2" << endl;

					GIAEntityNodeArray[thingIndex] = addPropertyToPropertyDefinition(thingEntity);
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}

void definePropertiesQuantitiesAndMeasures(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool passed = false;
			for(int i=0; i<RELATION_TYPE_QUANTITY_OR_MEASURE_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeQuantityOrMeasureNameArray[i])
				{
					passed = true;
				}
			}
			if(passed)
			{
				//create a new property for the entity and assign a property definition entity if not already created
				int propertyIndex = currentRelationInList->relationGovernorIndex;
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[propertyIndex];

				//cout << "as3" << endl;

				GIAEntityNodeArray[propertyIndex] = addPropertyToPropertyDefinition(propertyEntity);
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
}

void definePropertiesQuantityModifiers(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool passed = false;
			for(int i=0; i<RELATION_TYPE_QUANTITY_OR_MEASURE_SWITCHED_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeQuantityOrMeasureSwitchedNameArray[i])
				{
					passed = true;
				}
			}
			if(passed)
			{
				//create a new property for the entity and assign a property definition entity if not already created
				int propertyIndex = currentRelationInList->relationDependentIndex;
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[propertyIndex];

				//cout << "as3" << endl;
				GIAEntityNodeArray[propertyIndex] = addPropertyToPropertyDefinition(propertyEntity);
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
}

void definePropertiesExpletives(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	//eg There is 	_expl(be[2], there[1]) [Relex]	/ expl(is-2, There-1) [stanford]
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool passed = false;
			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT_EXPLETIVE)
			{
				//create property definition
				int propertyIndex = currentRelationInList->relationDependentIndex;
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[propertyIndex];

				#ifdef GIA_INTERPRET_EXPLETIVE_AS_SUBJECT_OF_ACTION
				//cout << "as4" << endl;
				GIAEntityNodeArray[propertyIndex] = addPropertyToPropertyDefinition(propertyEntity);
				#else
				disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(propertyEntity);
				#endif
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}

void definePropertiesPronouns(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], Feature * featureArrayTemp[])
{
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAEntityNodeArrayFilled[i])
		{
			if(!(featureArrayTemp[i]->isPronounReference))
			{//do not define properties based upon references (as the grammatical information is no longer correct, and it has already been done previously if necessary to the referenced entity)

				if(featureArrayTemp[i]->grammaticalIsPronoun == GRAMMATICAL_PRONOUN)
				{
					//cout << "as5" << endl;
					//cout << "asd" << endl;
					//cout << "GIAEntityNodeArray[i]->entityName = " << GIAEntityNodeArray[i]->entityName << endl;
					GIAEntityNodeArray[i] = addPropertyToPropertyDefinition(GIAEntityNodeArray[i]);
				}
			}
		}
	}
}

#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1F_RELATIONS_TREAT_THAT_AS_A_PRONOUN_IE_PROPERTY
void definePropertiesNonExplicitPronouns(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAEntityNodeArrayFilled[i])
		{
			bool passed = false;
			for(int j=0; j<RELATION_TYPE_TREAT_AS_PRONOUN_IE_PROPERTY_NUMBER_OF_TYPES; j++)
			{
				//cout << "GIAEntityNodeArray[i]->entityName = " << GIAEntityNodeArray[i]->entityName << endl;
				//cout << "relationTypeTreatAsPronounIeProperty[j] = " << relationTypeTreatAsPronounIeProperty[j] << endl;

				if(GIAEntityNodeArray[i]->entityName == relationTypeTreatAsPronounIeProperty[j])
				{
					passed = true;
				}
			}

			if(passed)
			{
				GIAEntityNodeArray[i] = addPropertyToPropertyDefinition(GIAEntityNodeArray[i]);
			}
		}
	}
}
#endif


void definePropertiesToBe(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_BE)
			{

				//create a new property for the entity and assign a property definition entity if not already created
				int propertyIndex = currentRelationInList->relationDependentIndex;
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[propertyIndex];

				GIAEntityNodeArray[propertyIndex] = addPropertyToPropertyDefinition(propertyEntity);
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif

		//cout << "as3" << endl;

		currentRelationInList = currentRelationInList->next;
	}
}

void definePropertiesToDo(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	//cout << "0i pass; define properties (to_do);" << endl;
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_DO)
			{

				//create a new property for the entity and assign a property definition entity if not already created
				int actionIndex = currentRelationInList->relationDependentIndex;
				GIAEntityNode * actionEntity = GIAEntityNodeArray[actionIndex];

				GIAEntityNodeArray[actionIndex] = addActionToActionDefinitionDefineProperties(actionEntity);
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		//cout << "as3" << endl;

		currentRelationInList = currentRelationInList->next;
	}
}

void definePropertiesHasTime(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], Feature * featureArrayTemp[])
{
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		//cout << "w = " << w << endl;

		if(GIAEntityNodeArrayFilled[w])
		{
			if(featureArrayTemp[w]->grammaticalIsDateOrTime)
			{
				GIAEntityNode * currentGIAEntityNode = GIAEntityNodeArray[w];
				//cout << "hastime; currentGIAEntityNode->entityName = " << currentGIAEntityNode->entityName << endl;

				GIAEntityNodeArray[w] = addPropertyToPropertyDefinition(currentGIAEntityNode);
			}
		}
	}
}

void definePropertiesIndirectObjects(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		//cout << "here1" << endl;
		//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_INDIRECT_OBJECT)
			{
				//now find the associated object..
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					if(!(currentRelationInList2->disabled))
					{
					#endif
						bool partnerTypeRequiredFound = false;
						for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
						{
							if(currentRelationInList2->relationType == relationTypeObjectNameArray[i])
							{
								partnerTypeRequiredFound = true;
							}
						}
						if(partnerTypeRequiredFound)
						{

							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
							{//found a matching object-indirectobject relationship
								//cout << "partnerTypeRequiredFound: currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;

								int propertyIndex = currentRelationInList2->relationDependentIndex;
								int thingIndex = currentRelationInList->relationDependentIndex;

								GIAEntityNode * propertyEntity = GIAEntityNodeArray[propertyIndex];
								GIAEntityNode * thingEntity = GIAEntityNodeArray[thingIndex];

								GIAEntityNodeArray[propertyIndex] = addPropertyToPropertyDefinition(propertyEntity);
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					}
					#endif

					currentRelationInList2 = currentRelationInList2->next;
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}
