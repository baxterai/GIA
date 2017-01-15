/*******************************************************************************
 *
 * File Name: GIATranslator.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1b10b 28-Sept-11
 * Requirements: requires text parsed by RelEx (available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors conceptEntityNodesList/conceptEntityNamesList with a map, and replace vectors GIATimeConditionNode/timeConditionNumbersList with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIATranslator.h"
#include "GIAdatabase.h"

string relationTypePropositionTimeNameArray[RELATION_TYPE_PREPOSITION_TIME_NUMBER_OF_TYPES] = {"in", "at", "on", "after", "ago", "before", "between", "by", "during", "for", "to", "till", "until", "past", "since", "up_to", "within", REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN};
	//http://www.englisch-hilfen.de/en/grammar/preposition_time.htm + is [time is] etc
string relationTypePropositionLocationNameArray[RELATION_TYPE_PREPOSITION_LOCATION_NUMBER_OF_TYPES] = {"in", "on", "at", "by", "near", "nearby", "above", "below", "over", "under", "around", "through", "inside", "inside_of", "outside", "between", "beside", "beyond", "in_front_of", "in_front", "in_back_of", "behind", "next_to", "on_top_of", "within", "beneath", "underneath", "among", "along", "against", "before", "after", "behind", "to", REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE};		
	//http://www.eslgold.com/grammar/prepositions_location.html + before, after, behind, to, etc
string relationTypePropositionReasonOrCircumstanceNameArray[RELATION_TYPE_PREPOSITION_REASON_OR_CIRCUMSTANCE_NUMBER_OF_TYPES] = {"because", "on_account_of", "for", "out_of", "when",  REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHY};
	//http://vlc.polyu.edu.hk/vlc/GrammarCourse/Lesson2_Preposition/CausePurpose.htm
string relationContextPropositionTimeNameArray[REFERENCE_TYPE_QUESTION_WHEN_CONTEXT_NUMBER_OF_TYPES] = {"time", "period", "era", "millenia", "decade", "day", "month", "year", "hour", "minute", "second", "millisecond", "microsecond", "nanosecond", "picosecond"};
string relationContextPropositionLocationNameArray[REFERENCE_TYPE_QUESTION_WHERE_CONTEXT_NUMBER_OF_TYPES] = {"location", "place", "position"};	//coordinates?
string relationContextPropositionReasonNameArray[REFERENCE_TYPE_QUESTION_WHY_CONTEXT_NUMBER_OF_TYPES] = {"reason", "basis", "argument"};
	
string relationTypeObjectNameArray[RELATION_TYPE_OBJECT_NUMBER_OF_TYPES] = {RELATION_TYPE_OBJECT, RELATION_TYPE_OBJECT_THAT};
string relationTypeSubjectNameArray[RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES] = {RELATION_TYPE_SUBJECT, RELATION_TYPE_SUBJECT_EXPLETIVE};
string relationTypeAdjectiveNameArray[RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_1, RELATION_TYPE_ADJECTIVE_2, RELATION_TYPE_ADJECTIVE_3};
string relationTypePossessiveNameArray[RELATION_TYPE_POSSESSIVE_NUMBER_OF_TYPES] = {RELATION_TYPE_POSSESSIVE, RELATION_TYPE_PRENOMIAL_MODIFIER};

string relationFunctionCompositionNameArray[RELATION_FUNCTION_COMPOSITION_NUMBER_OF_TYPES] = {RELATION_FUNCTION_COMPOSITION_1, RELATION_FUNCTION_COMPOSITION_2, RELATION_FUNCTION_COMPOSITION_3, RELATION_FUNCTION_COMPOSITION_4};
string relationFunctionDefinitionNameArray[RELATION_FUNCTION_DEFINITION_NUMBER_OF_TYPES] = {RELATION_FUNCTION_DEFINITION_1};

string relationTypeObjectSpecialConditionNameArray[RELATION_TYPE_OBJECT_SPECIAL_CONDITION_NUMBER_OF_TYPES] = {RELATION_TYPE_MEASURE_DISTANCE};

string relationTypeComplementsNameArray[RELATION_TYPE_COMPLEMENTS_NUMBER_OF_TYPES] = {RELATION_TYPE_COMPLIMENT_TO_BE, RELATION_TYPE_COMPLIMENT_TO_DO};

int referenceTypeHasDeterminateCrossReferenceNumberArray[GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES] = {GRAMMATICAL_NUMBER_SINGULAR};
string relationTypeAdjectiveWhichImplyEntityInstanceNameArray[RELATION_TYPE_ADJECTIVE_WHICH_IMPLY_ENTITY_INSTANCE_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_1, RELATION_TYPE_ADJECTIVE_3};
string relationTypeRequireSwitchingNameArray[RELATION_TYPE_REQUIRE_SWITCHING_NUMBER_OF_TYPES] = {RELATION_TYPE_OBJECT_THAT};

string relationTypeQuantityOrMeasureNameArray[RELATION_TYPE_QUANTITY_OR_MEASURE_NUMBER_OF_TYPES] = {RELATION_TYPE_QUANTITY, RELATION_TYPE_MEASURE_DISTANCE, RELATION_TYPE_MEASURE_SIZE, RELATION_TYPE_MEASURE_TIME};
string relationTypeQuantityOrMeasureSwitchedNameArray[RELATION_TYPE_QUANTITY_OR_MEASURE_SWITCHED_NUMBER_OF_TYPES] = {RELATION_TYPE_QUANTITY_MOD, RELATION_TYPE_MEASURE_PER};
string relationTypeMeasureNameArray[RELATION_TYPE_MEASURE_NUMBER_OF_TYPES] = {RELATION_TYPE_MEASURE_DISTANCE, RELATION_TYPE_MEASURE_PER, RELATION_TYPE_MEASURE_SIZE, RELATION_TYPE_MEASURE_TIME};
int relationTypeMeasureNameTypeIndexArray[RELATION_TYPE_MEASURE_NUMBER_OF_TYPES] = {MEASURE_TYPE_DISTANCE, MEASURE_TYPE_PER, MEASURE_TYPE_SIZE, MEASURE_TYPE_TIME};

string relationTypeQuantityArgumentImplyMeasurePerNameArray[RELATION_TYPE_QUANTITY_ARGUMENT_IMPLY_MEASURE_PER_NUMBER_OF_TYPES] = {"every"};

//int timeMonthIntArray[TIME_MONTH_NUMBER_OF_TYPES] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
string timeMonthStringArray[TIME_MONTH_NUMBER_OF_TYPES] = {TIME_MONTH_JANUARY, TIME_MONTH_FEBRUARY, TIME_MONTH_MARCH, TIME_MONTH_APRIL, TIME_MONTH_MAY, TIME_MONTH_JUNE, TIME_MONTH_JULY, TIME_MONTH_AUGUST, TIME_MONTH_SEPTEMBER, TIME_MONTH_OCTOBER, TIME_MONTH_NOVEMBER, TIME_MONTH_DECEMBER};
	

string referenceTypePossessiveNameArray[REFERENCE_TYPE_POSSESSIVE_NUMBER_OF_TYPES] = {"undefined", "his", "her", "them", "its"};
//int referenceTypePossessiveNameLengthsArray[REFERENCE_TYPE_POSSESSIVE_NUMBER_OF_TYPES] = {9, 3, 3, 4, 3};
string referenceTypePersonNameArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {"undefined", "he", "she", "they", "it"};
//int referenceTypePersonNameLengthsArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {9, 2, 3, 4, 2};

int referenceTypePersonCrossReferenceNumberArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {GRAMMATICAL_NUMBER_UNDEFINED, GRAMMATICAL_NUMBER_SINGULAR, GRAMMATICAL_NUMBER_SINGULAR, GRAMMATICAL_NUMBER_PLURAL, GRAMMATICAL_NUMBER_SINGULAR};
int referenceTypePersonCrossReferenceGenderArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {GRAMMATICAL_GENDER_UNDEFINED, GRAMMATICAL_GENDER_MASCULINE, GRAMMATICAL_GENDER_FEMININE, GRAMMATICAL_GENDER_UNDEFINED, GRAMMATICAL_GENDER_UNDEFINED};
bool referenceTypePersonCrossReferenceDefiniteArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {false, true, true, true, true};
bool referenceTypePersonCrossReferencePersonArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {GRAMMATICAL_PERSON_UNDEFINED, GRAMMATICAL_PERSON, GRAMMATICAL_PERSON, GRAMMATICAL_PERSON_UNDEFINED, GRAMMATICAL_PERSON_UNDEFINED};

static long currentEntityNodeIDInCompleteList;
static vector<GIAEntityNode*> * entityNodesCompleteList;
static long currentEntityNodeIDInConceptEntityNodesList;
//static vector<GIAEntityNode*> * conceptEntityNodesList;
static long currentEntityNodeIDInPropertyEntityNodesList;
static vector<GIAEntityNode*> * propertyEntityNodesList;
static long currentEntityNodeIDInActionEntityNodesList;
static vector<GIAEntityNode*> * actionEntityNodesList;

static bool foundComparisonVariable;
static GIAEntityNode* comparisonVariableNode;


void setTranslatorEntityNodesCompleteList(vector<GIAEntityNode*> * newEntityNodesCompleteList)
{
	entityNodesCompleteList = newEntityNodesCompleteList;
}
/*
void setTranslatorConceptEntityNodesList(vector<GIAEntityNode*> * newConceptEntityNodesList)
{
	conceptEntityNodesList = newConceptEntityNodesList;
}
*/
void setTranslatorPropertyEntityNodesList(vector<GIAEntityNode*> * newPropertyEntityNodesList)
{
	propertyEntityNodesList = newPropertyEntityNodesList;
}
void setTranslatorActionEntityNodesList(vector<GIAEntityNode*> * newActionEntityNodesList)
{
	actionEntityNodesList = newActionEntityNodesList;
}

	
bool getFoundComparisonVariable()
{
	return foundComparisonVariable;
}
GIAEntityNode* getComparisonVariableNode()
{
	return comparisonVariableNode;
}


	
GIAEntityNode * addProperty(GIAEntityNode * propertyEntity)
{
	//configure property node
	GIAEntityNode * newProperty = new GIAEntityNode();
	newProperty->id = currentEntityNodeIDInCompleteList;
	newProperty->idSecondary = currentEntityNodeIDInPropertyEntityNodesList;

	entityNodesCompleteList->push_back(newProperty);
	currentEntityNodeIDInCompleteList++;
	propertyEntityNodesList->push_back(newProperty);
	currentEntityNodeIDInPropertyEntityNodesList++;

	newProperty->entityName = propertyEntity->entityName;
	newProperty->isProperty = true;
	newProperty->entityNodeContainingThisProperty = NULL;
	newProperty->entityNodeDefiningThisPropertyOrAction = propertyEntity;
	propertyEntity->hasAssociatedProperty = true;
	propertyEntity->hasAssociatedPropertyTemp = true;	////temporary: used for GIA translator only - overwritten every time a new sentence is parsed

	//if(propertyEntity->grammaticalNumberTemp > GRAMMATICAL_NUMBER_SINGULAR)
	//{
	newProperty->grammaticalNumber = propertyEntity->grammaticalNumberTemp;
	//}

	if(propertyEntity->grammaticalTenseTemp > GRAMMATICAL_TENSE_PRESENT)
	{//ie, tense = GRAMMATICAL_TENSE_FUTURE/GRAMMATICAL_TENSE_PAST
		addTenseOnlyTimeConditionToProperty(newProperty, propertyEntity->grammaticalTenseTemp);
	}

	//configure property definition node
	propertyEntity->AssociatedPropertyNodeList.push_back(newProperty);

	propertyEntity->entityAlreadyDeclaredInThisContext = true;	//temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed
		
}

void addOrConnectPropertyToEntity(GIAEntityNode * thingEntity, GIAEntityNode * propertyEntity)
{
	if(propertyEntity->entityAlreadyDeclaredInThisContext)
	{
		if(!(propertyEntity->hasAssociatedPropertyTemp))
		{
			propertyEntity->hasAssociatedPropertyTemp = true;
		}	
	}
	
	if(propertyEntity->hasAssociatedPropertyTemp)
	{
		GIAEntityNode * existingProperty  = propertyEntity->AssociatedPropertyNodeList.back();	//added 4 May 11a

		if(thingEntity->entityAlreadyDeclaredInThisContext)
		{
			if(!(thingEntity->hasAssociatedPropertyTemp))
			{
				thingEntity->hasAssociatedPropertyTemp = true;
			}	
		}
		
		if(thingEntity->hasAssociatedPropertyTemp)
		{		
			thingEntity = thingEntity->AssociatedPropertyNodeList.back();	//added 4 May 11a
		}

		//configure entity node containing this property
		thingEntity->PropertyNodeList.push_back(existingProperty);	

		thingEntity->hasPropertyTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

		existingProperty->entityNodeContainingThisProperty = thingEntity;		//added 26 Aug 11a	
		
	}
	else
	{
		if(thingEntity->entityAlreadyDeclaredInThisContext)
		{
			if(!(thingEntity->hasAssociatedPropertyTemp))
			{
				thingEntity->hasAssociatedPropertyTemp = true;
			}	
		}
		
		if(thingEntity->hasAssociatedPropertyTemp)
		{		
			thingEntity = thingEntity->AssociatedPropertyNodeList.back();	//added 4 May 11a
		}
	
		GIAEntityNode * newProperty = addProperty(propertyEntity);		
		newProperty->entityNodeContainingThisProperty = thingEntity;
		
		//configure entity node containing this property
		thingEntity->PropertyNodeList.push_back(newProperty);		

		thingEntity->hasPropertyTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
	}
	
}

void addPropertyToPropertyDefinition(GIAEntityNode * propertyEntity)
{
	if(propertyEntity->entityAlreadyDeclaredInThisContext)
	{
		if(!(propertyEntity->hasAssociatedPropertyTemp))
		{
			propertyEntity->hasAssociatedPropertyTemp = true;
		}	
	}
	
	if((propertyEntity->hasAssociatedPropertyTemp) || (propertyEntity->entityAlreadyDeclaredInThisContext))
	{	
		//cout << "break; propertyEntity->entityName = " << propertyEntity->entityName << endl;
		propertyEntity = propertyEntity->AssociatedPropertyNodeList.back();	//added 4 May 11a
		
	}
	else
	{	
		GIAEntityNode * newProperty = addProperty(propertyEntity);
	}	
}


#ifdef GIA_TRANSLATOR_ACTION_DEFINITION_CODE_SIMPLIFICATION
void addActionToActionDefinition(GIAEntityNode * actionEntity)
{
	GIAEntityNode * newOrExistingAction;
	newOrExistingAction = addAction(actionEntity);
}
#else
void addActionToActionDefinition(GIAEntityNode * actionEntity)
{
	if((actionEntity->hasAssociatedPropertyTemp) || (actionEntity->entityAlreadyDeclaredInThisContext))
	{//CHECK THIS; need to convert to action node here also? ie hasAssociatedPropertyIsAction->true? [must look at generated semanticNet.xml and see if any propertyNodeContainers contain action nodes..., or if any actionNodeContainers do not contain property nodes...]
		if(!(actionEntity->hasAssociatedPropertyTemp))
		{
			actionEntity->hasAssociatedPropertyTemp = true;
		}
			
		//cout << "break; actionEntity->entityName = " << actionEntity->entityName << endl;
		actionEntity = actionEntity->AssociatedPropertyNodeList.back();	//added 4 May 11a
	}
	else
	{	
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "addPropertyToPropertyDefinition: actionEntity->entityName = " << actionEntity->entityName << endl;
		#endif
		//configure property node
		GIAEntityNode * newAction = new GIAEntityNode();
		newAction->id = currentEntityNodeIDInCompleteList;
		newAction->idSecondary = currentEntityNodeIDInActionEntityNodesList;
		
		entityNodesCompleteList->push_back(newAction);
		currentEntityNodeIDInCompleteList++;
		actionEntityNodesList->push_back(newAction);
		currentEntityNodeIDInActionEntityNodesList++;
				
		newAction->entityName = actionEntity->entityName;
		newAction->isAction = true;
		newAction->entityNodeContainingThisProperty = NULL;
		newAction->entityNodeDefiningThisPropertyOrAction = actionEntity;
		actionEntity->hasAssociatedProperty = true;
		actionEntity->hasAssociatedPropertyIsAction = true;
		actionEntity->hasAssociatedPropertyTemp = true;

		//if(actionEntity->grammaticalNumberTemp > GRAMMATICAL_NUMBER_SINGULAR)
		//{
		newAction->grammaticalNumber = actionEntity->grammaticalNumberTemp;
		//}				

		if(actionEntity->grammaticalTenseTemp > GRAMMATICAL_TENSE_PRESENT)
		{//ie, tense = GRAMMATICAL_TENSE_FUTURE/GRAMMATICAL_TENSE_PAST
			addTenseOnlyTimeConditionToProperty(newAction, actionEntity->grammaticalTenseTemp);
		}

		//configure property definition node
		actionEntity->AssociatedPropertyNodeList.push_back(newAction);	
		
		actionEntity->entityAlreadyDeclaredInThisContext = true;	//temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed
	}	
}
#endif


	//conditions required to be added [eg when, where, how, why]
GIAEntityNode * addAction(GIAEntityNode * actionEntity)
{		
	bool newAction = false;
	
	#ifdef GIA_TRANSLATOR_ACTION_DEFINITION_CODE_SIMPLIFICATION
	if(actionEntity->entityAlreadyDeclaredInThisContext)
	{//CHECK THIS; need to convert to action node here also? ie hasAssociatedPropertyIsAction->true? [must look at generated semanticNet.xml and see if any propertyNodeContainers contain action nodes..., or if any actionNodeContainers do not contain property nodes...]
		if(!(actionEntity->hasAssociatedPropertyTemp))
		{
			actionEntity->hasAssociatedPropertyTemp = true;
		}
	}
	#endif
			
	//configure action node	
	GIAEntityNode * newOrExistingAction;
	if(actionEntity->hasAssociatedPropertyTemp)
	{
		newOrExistingAction = actionEntity->AssociatedPropertyNodeList.back();	
		
		if(actionEntity->hasAssociatedPropertyIsAction == false)
		{//upgrade associated property to action
			//CHECK THIS; must remove from property list, and add to action list 
			actionEntity->hasAssociatedPropertyIsAction = true;
			newOrExistingAction->isProperty = false;
			newOrExistingAction->isAction = true;
			
			vector<GIAEntityNode*>::iterator propertyEntityNodesListIterator = propertyEntityNodesList->begin();
			advance(propertyEntityNodesListIterator,newOrExistingAction->idSecondary);
			propertyEntityNodesList->erase(propertyEntityNodesListIterator);
			currentEntityNodeIDInPropertyEntityNodesList--;
			
			actionEntityNodesList->push_back(newOrExistingAction);
			currentEntityNodeIDInActionEntityNodesList++;
		}
	}
	else
	{		
		newOrExistingAction = new GIAEntityNode();
		newOrExistingAction->id = currentEntityNodeIDInCompleteList;
		newOrExistingAction->idSecondary = currentEntityNodeIDInActionEntityNodesList;
		
		entityNodesCompleteList->push_back(newOrExistingAction);
		currentEntityNodeIDInCompleteList++;
		actionEntityNodesList->push_back(newOrExistingAction);
		currentEntityNodeIDInActionEntityNodesList++;
				
		newOrExistingAction->entityName = actionEntity->entityName;		
		newOrExistingAction->entityNodeDefiningThisPropertyOrAction = actionEntity;
		
		actionEntity->AssociatedPropertyNodeList.push_back(newOrExistingAction);
		actionEntity->hasAssociatedProperty = true;
		actionEntity->hasAssociatedPropertyIsAction = true;
		actionEntity->hasAssociatedPropertyTemp = true;
		newOrExistingAction->isAction = true;
		//WHY WOULD THIS EVER BE REQURIED?; newAction->entityNodeContainingThisProperty = NULL;

		//cout << "actionEntity->grammaticalTenseTemp = " << actionEntity->grammaticalTenseTemp << endl;
		//cout << "actionEntity->entityName = " << actionEntity->entityName << endl;

		if(actionEntity->grammaticalTenseTemp > GRAMMATICAL_TENSE_PRESENT)
		{//ie, tense = GRAMMATICAL_TENSE_FUTURE/GRAMMATICAL_TENSE_PAST
			//cout << "hello" << endl;
			//exit(0);
			addTenseOnlyTimeConditionToProperty(newOrExistingAction, actionEntity->grammaticalTenseTemp);
		}	
		
		#ifdef GIA_TRANSLATOR_ACTION_DEFINITION_CODE_SIMPLIFICATION
		actionEntity->entityAlreadyDeclaredInThisContext = true;	//temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed
		#endif	
	}

	return newOrExistingAction;
}




/*
#ifdef GIA_USE_TIME_NODE_INDEXING
void addTenseOnlyTimeConditionToProperty(GIAEntityNode * propertyNode, int tense, vector<GIATimeConditionNode*> *timeConditionNodesList, vector<long> *timeConditionNumbersList)
#else
void addTenseOnlyTimeConditionToProperty(GIAEntityNode * propertyNode, int tense)
#endif
*/
void addTenseOnlyTimeConditionToProperty(GIAEntityNode * propertyNode, int tense)
{
	propertyNode->conditionType = CONDITION_NODE_TYPE_TIME;
	
	/*
	#ifdef GIA_USE_TIME_NODE_INDEXING
	int timeConditionEntityIndex = -1;
	bool argumentEntityAlreadyExistant = false;
	long timeConditionTotalTimeInSeconds = 0; //cannot assign absolute time to an event which occurs in the past.... //calculateTotalTimeInSeconds();
	GIATimeConditionNode * newTimeCondition = findOrAddTimeNodeByNumber(timeConditionNodesList, conceptEntityNamesList, timeConditionAbsoluteTimeValue, &argumentEntityAlreadyExistant, &timeConditionEntityIndex, true);
	#else
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	#endif
	*/
	
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	
	newTimeCondition->tense = tense;
	newTimeCondition->conditionName = grammaticalTenseNameArray[tense];
	propertyNode->timeConditionNode = newTimeCondition;
}


void addDefinitionToEntity(GIAEntityNode * thingEntity, GIAEntityNode * definitionEntity)
{
	if(thingEntity->hasAssociatedPropertyTemp)
	{
		thingEntity = thingEntity->AssociatedPropertyNodeList.back();	//added 4 May 11a
	}
	
	if(definitionEntity->hasAssociatedPropertyTemp)
	{
		definitionEntity = definitionEntity->AssociatedPropertyNodeList.back();	//added 4 May 11a
	}
	
	//configure entity node
	thingEntity->EntityNodeDefinitionList.push_back(definitionEntity);
	
	//configure entity definition node
	definitionEntity->EntityNodeDefinitionReverseList.push_back(thingEntity);		
}

	//conditions required to be added [eg when, where, how, why]
	//replace action if already existant
void addActionToEntity(GIAEntityNode * subjectEntity, GIAEntityNode * objectEntity, GIAEntityNode * actionEntity)
{
	GIAEntityNode * newOrExistingAction;

	addActionToSubject(subjectEntity, actionEntity);

	addActionToObject(objectEntity, actionEntity);

}


void addActionToSubject(GIAEntityNode * subjectEntity, GIAEntityNode * actionEntity)
{	
	GIAEntityNode * newOrExistingAction;
	newOrExistingAction = addAction(actionEntity);

	if(subjectEntity->hasAssociatedPropertyTemp)
	{
		subjectEntity = subjectEntity->AssociatedPropertyNodeList.back();
	}
	
	//configure subject entity node
	subjectEntity->ActionNodeList.push_back(newOrExistingAction);
	
	newOrExistingAction->actionSubjectEntity = subjectEntity;
	//cout << "BUG SHOULD BE JOE; subjectEntity->entityName = " << subjectEntity->entityName << endl;
	
	subjectEntity->isSubjectTemp = true; 	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
	
}

void addActionToObject(GIAEntityNode * objectEntity, GIAEntityNode * actionEntity)
{		
	GIAEntityNode * newOrExistingAction;
	newOrExistingAction = addAction(actionEntity);
	
	if(objectEntity->hasAssociatedPropertyTemp)
	{
		objectEntity = objectEntity->AssociatedPropertyNodeList.back();
	}
		
	//configure object entity node
	objectEntity->IncomingActionNodeList.push_back(newOrExistingAction);
	
	newOrExistingAction->actionObjectEntity = objectEntity;
	
	objectEntity->isObjectTemp = true; 	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
}

/*
#ifdef GIA_USE_TIME_NODE_INDEXING
void addTenseOnlyTimeConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * timeConditionEntity, string conditionName, vector<GIATimeConditionNode*> *timeConditionNodesList, vector<long> *timeConditionNumbersList)
#else
void addTimeConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * timeConditionEntity, string conditionName)
#endif
*/
void addTimeConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * timeConditionEntity, string conditionName)
{	
	timeConditionEntity->conditionType = CONDITION_NODE_TYPE_TIME;
	
	/*
	#ifdef GIA_USE_TIME_NODE_INDEXING
	int timeConditionEntityIndex = -1;
	bool argumentEntityAlreadyExistant = false;
	long timeConditionTotalTimeInSeconds = calculateTotalTimeInSeconds(timeConditionEntity->entityName);
	GIATimeConditionNode * newTimeCondition = findOrAddTimeNodeByNumber(timeConditionNodesList, conceptEntityNamesList, timeConditionAbsoluteTimeValue, &argumentEntityAlreadyExistant, &timeConditionEntityIndex, true);
	#else	
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	#endif
	*/
	
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	
	timeConditionEntity->timeConditionNode = newTimeCondition;	
	
	propertyNode->ConditionNodeList.push_back(timeConditionEntity);
	propertyNode->ConditionNodeTypeList.push_back(conditionName);
	timeConditionEntity->ConditionNodeReverseList.push_back(propertyNode);
	timeConditionEntity->ConditionNodeTypeReverseList.push_back(conditionName);
}

void addLocationConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * locationConditionEntity, string conditionName)
{	
	locationConditionEntity->conditionType = CONDITION_NODE_TYPE_LOCATION;
	
	propertyNode->ConditionNodeList.push_back(locationConditionEntity);
	propertyNode->ConditionNodeTypeList.push_back(conditionName);	
	locationConditionEntity->ConditionNodeReverseList.push_back(propertyNode);
	locationConditionEntity->ConditionNodeTypeReverseList.push_back(conditionName);
}

void addReasonConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * reasonConditionEntity, string conditionName)
{	
	reasonConditionEntity->conditionType = CONDITION_NODE_TYPE_REASON;
	
	propertyNode->ConditionNodeList.push_back(reasonConditionEntity);
	propertyNode->ConditionNodeTypeList.push_back(conditionName);	
	reasonConditionEntity->ConditionNodeReverseList.push_back(propertyNode);
	reasonConditionEntity->ConditionNodeTypeReverseList.push_back(conditionName);
}


void addPropertyConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * propertyConditionEntity, string conditionName)
{
	//timeConditionEntity->conditionType = CONDITION_NODE_TYPE_UNDEFINED;
	
	propertyNode->ConditionNodeList.push_back(propertyConditionEntity);
	propertyNode->ConditionNodeTypeList.push_back(conditionName);	
	propertyConditionEntity->ConditionNodeReverseList.push_back(propertyNode);
	propertyConditionEntity->ConditionNodeTypeReverseList.push_back(conditionName);
}

void addOrConnectPropertyConditionToEntity(GIAEntityNode * entityNode, GIAEntityNode * conditionEntityNode, string conditionName)
{
	if(entityNode->hasAssociatedPropertyTemp)
	{
		entityNode = entityNode->AssociatedPropertyNodeList.back();
	}
	if(conditionEntityNode->hasAssociatedPropertyTemp)
	{
		conditionEntityNode = conditionEntityNode->AssociatedPropertyNodeList.back();
	}										
	addPropertyConditionToProperty(entityNode, conditionEntityNode, conditionName);
}


void convertSentenceRelationsIntoGIAnetworkNodes(vector<GIAEntityNode*> *conceptEntityNodesList, vector<string> *conceptEntityNamesList, vector<GIATimeConditionNode*> *timeConditionNodesList, vector<long> *timeConditionNumbersList, Sentence * firstSentenceInList)
{
	
	currentEntityNodeIDInCompleteList = 0;
	currentEntityNodeIDInConceptEntityNodesList = 0;
	currentEntityNodeIDInPropertyEntityNodesList = 0;
	currentEntityNodeIDInActionEntityNodesList = 0;
	
	vector<GIAEntityNode*>::iterator indexOfEntityNodesIterator;
	vector<string*>::iterator indexOfEntityNamesIterator;
	vector<GIATimeConditionNode*>::iterator indexOfTimeNodesIterator;
	vector<long*>::iterator indexOfTimeNumbersIterator;	


	/*
	//initialise conceptEntityNodesList;	[should be moved elsewhere]
	string firstEntityNameInNetwork = "universe";
	GIAEntityNode * firstEntityNodeInNetwork = new GIAEntityNode();
	firstEntityNodeInNetwork->entityName = firstEntityNameInNetwork;
	conceptEntityNodesList->push_back(firstEntityNodeInNetwork);
	conceptEntityNamesList->push_back(firstEntityNameInNetwork);
	
	//DEBUG
	//conceptEntityNamesList->push_back("za");
	//conceptEntityNamesList->push_back("zb");
	//conceptEntityNamesList->push_back("zc");
	//conceptEntityNamesList->push_back("zd");
	//conceptEntityNamesList->push_back("ze");
	//conceptEntityNamesList->push_back("zf");
	//conceptEntityNamesList->push_back("zg");
	//conceptEntityNamesList->push_back("zh");
	//conceptEntityNamesList->push_back("zi");
	//conceptEntityNamesList->push_back("zj");
	
	
	//initialise timeConditionNodesList;		[should be moved elsewhere]	
	long firstTimeInNetwork = -14*(10^9)*SECONDS_IN_YEAR;
	string firstTimeNameInNetwork = "beginning";
	GIATimeConditionNode * firstTimeNodeInNetwork = new GIATimeConditionNode();
	firstTimeNodeInNetwork->conditionName = firstTimeNameInNetwork;
	firstTimeNodeInNetwork->totalTimeInSeconds = firstTimeInNetwork;
	timeConditionNodesList->push_back(firstTimeNodeInNetwork);
	timeConditionNumbersList->push_back(firstTimeInNetwork);		
	
	
	//DEBUG	
	//bool resultTemp = true;
	//string tempName = "zh";
	//long findIndex = -1;
	//GIAEntityNode * tempEntity = findOrAddEntityNodeByName(conceptEntityNodesList, conceptEntityNamesList, &tempName, &resultTemp, &findIndex);
	//if(resultTemp)
	//{
	//	cout << "tempEntity->entityName = " << tempEntity->entityName << endl;
	//}
	//exit(0);
	*/

	
	Sentence * currentSentenceInList = firstSentenceInList;
	
	while(currentSentenceInList->next != NULL)
	{
		Relation * currentRelationInList;
		
		
		bool GIAEntityNodeIsDate[MAX_NUMBER_OF_WORDS_PER_SENTENCE];	//not properly implemented yet
		int GIAEntityNodeGrammaticalTenseArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
		bool GIAEntityNodeGrammaticalTenseModifierArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE][GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES];
		int GIAEntityNodeGrammaticalNumberArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
		bool GIAEntityNodeGrammaticalIsDefiniteArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
		bool GIAEntityNodeGrammaticalIsPersonArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
		int GIAEntityNodeGrammaticalGenderArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
		//bool GIAEntityNodeGrammaticalHasCountArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
		bool GIAEntityNodeGrammaticalIsPronounArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
		
		bool GIAEntityNodeArrayFilled[MAX_NUMBER_OF_WORDS_PER_SENTENCE];		//NB could also use currentSentence->maxNumberOfWordsInSentence
		GIAEntityNode * GIAEntityNodeArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];

		//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
		//bool GIAEntityNodeArrayHasAssociatedProperty[MAX_NUMBER_OF_WORDS_PER_SENTENCE];

		bool GIAEntityNodeIsAReference[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
	
		for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
		{					
			GIAEntityNodeIsDate[w] = false;
			GIAEntityNodeGrammaticalTenseArray[w] = GRAMMATICAL_TENSE_UNDEFINED;
			GIAEntityNodeGrammaticalNumberArray[w] = GRAMMATICAL_NUMBER_UNDEFINED;
			GIAEntityNodeGrammaticalIsDefiniteArray[w] = false;
			GIAEntityNodeGrammaticalIsPersonArray[w] = false;
			GIAEntityNodeGrammaticalGenderArray[w] = GRAMMATICAL_NUMBER_UNDEFINED;
			//GIAEntityNodeGrammaticalHasCountArray[w] = GRAMMATICAL_NUMBER_UNDEFINED;
			GIAEntityNodeGrammaticalIsPronounArray[w] = GRAMMATICAL_PRONOUN_UNDEFINED;
			for(int q=0; q<GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES;q++)
			{
				GIAEntityNodeGrammaticalTenseModifierArray[w][q] = false;
			}
			
			GIAEntityNodeIsAReference[w] = false;
			
			GIAEntityNodeArrayFilled[w] = false;

			//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
			//GIAEntityNodeArrayHasAssociatedProperty[w] = false;
			
			//GIAActionNodeArrayFilled[w] = false;
		}
		Feature * currentFeatureInList = currentSentenceInList->firstFeatureInList;
		while(currentFeatureInList->next != NULL)
		{
			//cout << "currentFeatureInList->grammar = " << currentFeatureInList->grammar << endl;
			
			//this date code probably requires an update [there appear to be multiple ways in which dates are defined in relex...
			if((currentFeatureInList->grammar).find(FEATURE_GRAMMATICAL_TENSE_DATE) != -1)
			{
				GIAEntityNodeIsDate[currentFeatureInList->entityIndex] = true;
				//cout << "isDate currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
			}
			
			for(int grammaticalTenseIndex = 0; grammaticalTenseIndex < GRAMMATICAL_TENSE_NUMBER_OF_TYPES; grammaticalTenseIndex++)
			{
				//NB only the first characters of currentFeatureInList->grammar contain the tense type name 
				if((currentFeatureInList->grammar).find(grammaticalTenseNameArray[grammaticalTenseIndex]) != -1) 
				//if((currentFeatureInList->grammar).substr(0, grammaticalTenseNameLengthsArray[grammaticalTenseIndex]) == grammaticalTenseNameArray[grammaticalTenseIndex]) 
				{
					GIAEntityNodeGrammaticalTenseArray[currentFeatureInList->entityIndex] = grammaticalTenseIndex;
					//cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex grammaticalTenseIndex = " << grammaticalTenseNameArray[grammaticalTenseIndex] << endl;
				}			
			}
			for(int grammaticalTenseModifierIndex = 0; grammaticalTenseModifierIndex < GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES; grammaticalTenseModifierIndex++)
			{
				//NB only the first characters of currentFeatureInList->grammar contain the tense type name 
				if((currentFeatureInList->grammar).find(grammaticalTenseModifierNameArray[grammaticalTenseModifierIndex]) != -1) 
				{
					GIAEntityNodeGrammaticalTenseModifierArray[currentFeatureInList->entityIndex][grammaticalTenseModifierIndex] = true;
					//cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex grammaticalTenseModifierIndex true = " << grammaticalTenseModifierNameArray[grammaticalTenseModifierIndex] << endl;
				}			
			}
						
			for(int grammaticalNumberIndex = 0; grammaticalNumberIndex < GRAMMATICAL_NUMBER_NUMBER_OF_TYPES; grammaticalNumberIndex++)
			{
				//NB only the first characters of currentFeatureInList->grammar contain the grammatical number type name 
				if((currentFeatureInList->grammar).find(grammaticalNumberNameArray[grammaticalNumberIndex]) != -1) 				
				{
					GIAEntityNodeGrammaticalNumberArray[currentFeatureInList->entityIndex] = grammaticalNumberIndex;
					//cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex grammaticalNumberIndex = " << grammaticalNumberNameArray[grammaticalNumberIndex] << endl;
				}			
			}
			if((currentFeatureInList->grammar).find(GRAMMATICAL_DEFINITE_NAME) != -1)
			{
				GIAEntityNodeGrammaticalIsDefiniteArray[currentFeatureInList->entityIndex] = GRAMMATICAL_DEFINITE;
				//cout << "isDefinite currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
				
			}			

			if((currentFeatureInList->grammar).find(GRAMMATICAL_PERSON_NAME) != -1)
			{
				GIAEntityNodeGrammaticalIsPersonArray[currentFeatureInList->entityIndex] = GRAMMATICAL_PERSON;
				//cout << "isPerson currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
				
			}
			for(int grammaticalGenderIndex = 0; grammaticalGenderIndex < GRAMMATICAL_GENDER_NUMBER_OF_TYPES; grammaticalGenderIndex++)
			{
				//NB only the first characters of currentFeatureInList->grammar contain the grammatical Gender type name 
				if((currentFeatureInList->grammar).find(grammaticalGenderNameArray[grammaticalGenderIndex]) != -1) 				
				{
					//NB it will always find "person" in relex grammar string if "person" is existant, but this will be overwritten by "feminine" or "masculine" if this is specified (not possible for bigender names like "joe")
					GIAEntityNodeGrammaticalGenderArray[currentFeatureInList->entityIndex] = grammaticalGenderIndex;
					//cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex grammaticalGenderIndex = " << grammaticalGenderNameArray[grammaticalGenderIndex] << endl;
				}			
			}
			
			/*
			//this date code probably requires an update [there appear to be multiple ways in which dates are defined in relex...
			if((currentFeatureInList->grammar).find(FEATURE_GRAMMATICAL_COUNT) != -1)
			{
				GIAEntityNodeGrammaticalHasCountArray[currentFeatureInList->entityIndex] = true;
				//cout << "hasCount currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
			}	
			*/
			
			if((currentFeatureInList->grammar).find(GRAMMATICAL_PRONOUN_NAME) != -1)
			{
				GIAEntityNodeGrammaticalIsPronounArray[currentFeatureInList->entityIndex] = GRAMMATICAL_PRONOUN;
				//cout << "isDefinite currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;	
			}					
												 
			currentFeatureInList = currentFeatureInList->next;
		}
		//exit(0);
		
		//cout << "pass A;" << endl;
		//cout << "pass 1; locate/add all entities" << endl;
		currentRelationInList = currentSentenceInList->firstRelationInList;
 		while(currentRelationInList->next != NULL)
		{			

			string functionName = currentRelationInList->relationFunction;
			string argumentName = currentRelationInList->relationArgument; 
			int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
			int relationArgumentIndex = currentRelationInList->relationArgumentIndex;
			//cout << "functionName = " << functionName << endl;
			//cout << "argumentName = " << argumentName << endl;

			long functionEntityIndex = -1;
			long argumentEntityIndex = -1;
			bool functionEntityAlreadyExistant = false;
			bool argumentEntityAlreadyExistant = false;

			//cout << "relationFunctionIndex = " << relationFunctionIndex << endl;
			//cout << "relationArgumentIndex = " << relationArgumentIndex << endl;

			bool argumentIsQuery = false;
			if(argumentName == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
			{//modify relation index [to prevent overlapping of comparison variable indicies with other indicies]
				relationArgumentIndex = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_ARGUMENT_INDEX;
				currentRelationInList->relationArgumentIndex = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_ARGUMENT_INDEX;
				argumentIsQuery = true;
			}
				
			if(!GIAEntityNodeArrayFilled[relationFunctionIndex])
			{
				//cout << "wf2" <<endl;
				GIAEntityNode * functionEntity = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, conceptEntityNamesList, &functionName, &functionEntityAlreadyExistant, &functionEntityIndex, true, &currentEntityNodeIDInCompleteList, &currentEntityNodeIDInConceptEntityNodesList);
				//cout << "wf3" <<endl;
				GIAEntityNodeArrayFilled[relationFunctionIndex] = true;
				GIAEntityNodeArray[relationFunctionIndex] = functionEntity;
				functionEntity->hasAssociatedTime = GIAEntityNodeIsDate[relationFunctionIndex]; 
				//cout << "functionEntity->hasAssociatedTime = " << functionEntity->hasAssociatedTime << endl;
				//cout << "relationFunctionIndex = " << relationFunctionIndex << endl;	
				
				GIAEntityNodeArray[relationFunctionIndex]->grammaticalTenseTemp = GIAEntityNodeGrammaticalTenseArray[relationFunctionIndex];
				GIAEntityNodeArray[relationFunctionIndex]->grammaticalNumberTemp = GIAEntityNodeGrammaticalNumberArray[relationFunctionIndex];
				GIAEntityNodeArray[relationFunctionIndex]->grammaticalDefiniteTemp = GIAEntityNodeGrammaticalIsDefiniteArray[relationFunctionIndex];
				GIAEntityNodeArray[relationFunctionIndex]->grammaticalPersonTemp = GIAEntityNodeGrammaticalIsPersonArray[relationFunctionIndex];
				GIAEntityNodeArray[relationFunctionIndex]->grammaticalGenderTemp = GIAEntityNodeGrammaticalGenderArray[relationFunctionIndex];
				GIAEntityNodeArray[relationFunctionIndex]->grammaticalPronounTemp = GIAEntityNodeGrammaticalIsPronounArray[relationFunctionIndex];
				
				 
				GIAEntityNodeArray[relationFunctionIndex]->hasAssociatedPropertyTemp = false;
				
			}
			if(!GIAEntityNodeArrayFilled[relationArgumentIndex])
			{

				GIAEntityNode * argumentEntity = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, conceptEntityNamesList, &argumentName, &argumentEntityAlreadyExistant, &argumentEntityIndex, true, &currentEntityNodeIDInCompleteList, &currentEntityNodeIDInConceptEntityNodesList);
				GIAEntityNodeArrayFilled[relationArgumentIndex] = true;
				GIAEntityNodeArray[relationArgumentIndex] = argumentEntity;				
				argumentEntity->hasAssociatedTime = GIAEntityNodeIsDate[relationArgumentIndex]; 
				//cout << "argumentEntity->hasAssociatedTime = " << argumentEntity->hasAssociatedTime << endl;	
				//cout << "relationArgumentIndex = " << relationArgumentIndex << endl;
			
				GIAEntityNodeArray[relationArgumentIndex]->grammaticalTenseTemp = GIAEntityNodeGrammaticalTenseArray[relationArgumentIndex];
				GIAEntityNodeArray[relationArgumentIndex]->grammaticalNumberTemp = GIAEntityNodeGrammaticalNumberArray[relationArgumentIndex];
				GIAEntityNodeArray[relationArgumentIndex]->grammaticalDefiniteTemp = GIAEntityNodeGrammaticalIsDefiniteArray[relationArgumentIndex];
				GIAEntityNodeArray[relationArgumentIndex]->grammaticalPersonTemp = GIAEntityNodeGrammaticalIsPersonArray[relationArgumentIndex];
				GIAEntityNodeArray[relationArgumentIndex]->grammaticalGenderTemp = GIAEntityNodeGrammaticalGenderArray[relationArgumentIndex];
				GIAEntityNodeArray[relationArgumentIndex]->grammaticalPronounTemp = GIAEntityNodeGrammaticalIsPronounArray[relationArgumentIndex];
								
				GIAEntityNodeArray[relationArgumentIndex]->hasAssociatedPropertyTemp = false;	
				
				if(argumentIsQuery)
				{
					GIAEntityNodeArray[relationArgumentIndex]->isQuery = true;
					foundComparisonVariable = true;
					comparisonVariableNode = GIAEntityNodeArray[relationArgumentIndex];					
				}
			}
			
			currentRelationInList = currentRelationInList->next;
		}
		//cout << "as2" <<endl;
	
		//cout << "pass 1b; switch argument/functions where necessary" << endl;
		if(GIA_PERFORM_RELATION_FUNCTION_ARGUMENT_SWITCHING_WHERE_NECESSARY)
		{
			currentRelationInList = currentSentenceInList->firstRelationInList;
			while(currentRelationInList->next != NULL)
			{	
				//cout << "here1" << endl;
				//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

				bool passed = false;
				for(int i=0; i<RELATION_TYPE_REQUIRE_SWITCHING_NUMBER_OF_TYPES; i++)
				{
					//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

					if(currentRelationInList->relationType == relationTypeRequireSwitchingNameArray[i])
					{
						passed = true;
					}
				}
				if(passed)
				{	
					bool passed2 = false;
									
					if(GIA_PERFORM_RELATION_FUNCTION_ARGUMENT_SWITCHING_ONLY_WHEN_REQUIRED)
					{						
						//now find the associated object..
 						Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
						while(currentRelationInList2->next != NULL)
						{	
							
							/*
							bool partnerTypeRequiredFoundObj = false;
							for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
							{
								if(currentRelationInList2->relationType == relationTypeObjectNameArray[i])
								{
									partnerTypeRequiredFoundObj = true;
								}
							}
							*/
							bool partnerTypeRequiredFoundSubj = false;
							for(int i=0; i<RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
							{
								if(currentRelationInList2->relationType == relationTypeSubjectNameArray[i])
								{
									partnerTypeRequiredFoundSubj = true;
								}
							}								
							if(partnerTypeRequiredFoundSubj)
							{
								if(currentRelationInList2->relationFunctionIndex == currentRelationInList->relationArgumentIndex)
								{//found a matching subject-that[obj] relationship that requires function/argument switching

									//cout << "found a matching subject-that[obj] relationship that requires function/argument switching" << endl;
									passed2 = true;
									//cout << "partnerTypeRequiredFound: currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;
								}
							}

							currentRelationInList2 = currentRelationInList2->next;
						}
					}
					else
					{
						passed2 = true;
					}
					if(passed2)
					{
						string tempString = currentRelationInList->relationArgument;
						int tempIndex = currentRelationInList->relationArgumentIndex;
						currentRelationInList->relationArgument = currentRelationInList->relationFunction;				
						currentRelationInList->relationFunction = tempString;
						currentRelationInList->relationArgumentIndex = currentRelationInList->relationFunctionIndex;				
						currentRelationInList->relationFunctionIndex = tempIndex;						
					}
				}
				currentRelationInList = currentRelationInList->next;
			}					
		}
				
		//cout << "pass 2; identify entity types [define entities as objects, subjects, and being possessive of properties];" << endl;
		currentRelationInList = currentSentenceInList->firstRelationInList;
		while(currentRelationInList->next != NULL)
		{
			//possessive of properties [NB plural/singular indicates definite noun - eg a robin, the robin, the robins - which is therefore a property (entity instance):
								
			//possessive of properties:
			
			bool passed = false;
			for(int i=0; i<RELATION_TYPE_POSSESSIVE_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypePossessiveNameArray[i])
				{
					passed = true;
				}
			}			
			//if(currentRelationInList->relationType == RELATION_TYPE_POSSESSIVE)
			if(passed)
			{
				int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
				int relationArgumentIndex = currentRelationInList->relationArgumentIndex;				
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationFunctionIndex];
				GIAEntityNode * ownerEntity = GIAEntityNodeArray[relationArgumentIndex];
				ownerEntity->hasPropertyTemp = true;
			}
			
			//possessive of properties:
			
			passed = false;
			for(int i=0; i<RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeAdjectiveNameArray[i])
				{
					passed = true;
				}
			}						
			//if((currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_1) || (currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_2) || (currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_3))
			if(passed)
			{
				int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
				int relationArgumentIndex = currentRelationInList->relationArgumentIndex;				
				GIAEntityNode * thingEntity = GIAEntityNodeArray[relationFunctionIndex];
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationArgumentIndex];
				thingEntity->hasPropertyTemp = true;
			}
			
			//has subject:
				
			passed = false;
			for(int i=0; i<RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeSubjectNameArray[i])
				{
					passed = true;
				}
			}														
			//if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT || (currentRelationInList->relationType == RELATION_TYPE_SUBJECT_EXPLETIVE))
			if(passed)
			{
				int relationArgumentIndex = currentRelationInList->relationArgumentIndex;	
				GIAEntityNode * subjectEntity = GIAEntityNodeArray[relationArgumentIndex];
				subjectEntity->isSubjectTemp = true;
			}
			
			//has object:
			
			passed = false;
			for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeObjectNameArray[i])
				{
					passed = true;
				}
			}
			//if((currentRelationInList->relationType == RELATION_TYPE_OBJECT) || (currentRelationInList->relationType == RELATION_TYPE_OBJECT_TO))
			if(passed)	
			{
				int relationArgumentIndex = currentRelationInList->relationArgumentIndex;
				GIAEntityNode * objectEntity = GIAEntityNodeArray[relationArgumentIndex];
				objectEntity->isObjectTemp = true; 
			}
						
			currentRelationInList = currentRelationInList->next;
		}
		
		#ifdef GIA_ENABLE_REFERENCE_LINKING_BASED_UPON_PRONOUNS
		//cout << "pass 3; link references (eg his/her with joe/emily)" << endl;
		for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
		{	
			//cout << "w = " << w << endl;
				
			if(GIAEntityNodeArrayFilled[w])
			{
				GIAEntityNode * currentGIAEntityNode = GIAEntityNodeArray[w];
				//cout << "currentGIAEntityNode->entityName = " << currentGIAEntityNode->entityName << endl;

				for(int i=0; i< REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES; i++)
				{
					//cout << "i = " << i << endl;

					if(((currentGIAEntityNode->entityName == referenceTypePossessiveNameArray[i]) || (currentGIAEntityNode->entityName == referenceTypePersonNameArray[i])) && (currentGIAEntityNode->grammaticalPronounTemp == GRAMMATICAL_PRONOUN))
					{//pronoun required for currentGIAEntityNode
						//cout << "currentGIAEntityNode->entityName = " << currentGIAEntityNode->entityName << endl;
						//now go for a search in tree for given / this sentence + previous sentence until find candidate reference source

						GIAEntityNode * referenceSource = NULL;
						bool referenceSourceHasBeenFound = false;
						bool stillSentencesToSearch = true;
						Sentence * currentSentenceInWhichReferenceSourceIsBeingSearchedFor = currentSentenceInList;
						int s2 = 0;
						while(!referenceSourceHasBeenFound && stillSentencesToSearch) 
						{
							//cout << "s2 = " << s2 << endl;

							Relation * currentRelationInWhichReferenceSourceIsBeingSearchedFor = currentSentenceInWhichReferenceSourceIsBeingSearchedFor->firstRelationInList;
							int maxWordLimit = 999999;
							if(s2 == 0)
							{
								maxWordLimit = w;
							}
							int w2 = 0;
							while((currentRelationInWhichReferenceSourceIsBeingSearchedFor->next != NULL) && (w2 < maxWordLimit))
							{
								//cout << "w2 = " << w2 << endl;

								long entityIndex = -1;
								bool entityAlreadyExistant = false;

								string entityName = currentRelationInWhichReferenceSourceIsBeingSearchedFor->relationArgument;
											
								//cout << "currentRelationInWhichReferenceSourceIsBeingSearchedFor = " << entityName << endl;
											
								if(entityName != "")
								{
								
									GIAEntityNode * currentEntityInWhichReferenceSourceIsBeingSearchedFor = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, conceptEntityNamesList, &entityName, &entityAlreadyExistant, &entityIndex, false, &currentEntityNodeIDInCompleteList, &currentEntityNodeIDInConceptEntityNodesList);

									if(entityAlreadyExistant)
									{
										//cout << "candidateReferenceSourceEntityName = " << entityName << endl;
										
										bool entityPassesGrammaticalTestsForReference = true;
										

																	
										//if(referenceTypePersonCrossReferenceNumberArray[i] != GRAMMATICAL_CATEGORY_UNDEFINED) 
										//if(!((referenceTypePersonCrossReferenceNumberArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED) && (referenceTypePersonCrossReferenceNumberArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED)))
										//{
											if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalNumberTemp != referenceTypePersonCrossReferenceNumberArray[i])
											{
												//cout << "a1" << endl;
												entityPassesGrammaticalTestsForReference = false;
											}
										//}
										//if(referenceTypePersonCrossReferenceGenderArray[i] != GRAMMATICAL_CATEGORY_UNDEFINED) 
										//if(!((referenceTypePersonCrossReferenceGenderArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED) && (referenceTypePersonCrossReferenceGenderArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED)))
										//{
											if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalGenderTemp != referenceTypePersonCrossReferenceGenderArray[i])
											{
												//cout << "a2" << endl;
												entityPassesGrammaticalTestsForReference = false;
											}
										//}
										//if(referenceTypePersonCrossReferenceDefiniteArray[i] != GRAMMATICAL_CATEGORY_UNDEFINED) 
										//if(!((referenceTypePersonCrossReferenceDefiniteArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED) && (referenceTypePersonCrossReferenceDefiniteArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED)))
										//{
											if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalDefiniteTemp != referenceTypePersonCrossReferenceDefiniteArray[i])
											{
												//cout << "a3" << endl;
												entityPassesGrammaticalTestsForReference = false;
											}
										//}
										//if(referenceTypePersonCrossReferencePersonArray[i] != GRAMMATICAL_CATEGORY_UNDEFINED) 
										//if(!((referenceTypePersonCrossReferencePersonArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED) && (referenceTypePersonCrossReferencePersonArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED)))
										//{
											if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalPersonTemp != referenceTypePersonCrossReferencePersonArray[i])
											{
												//cout << "a4" << endl;
												entityPassesGrammaticalTestsForReference = false;
											}
											
											
											if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalPronounTemp == GRAMMATICAL_PRONOUN)
											{
												//cout << "a5" << endl;
												entityPassesGrammaticalTestsForReference = false;
											}
											
											
																						
										//}
										if(entityPassesGrammaticalTestsForReference)
										{	
											#ifdef GIA_TRANSLATOR_DEBUG
											cout << "entityPassesGrammaticalTestsForReference" << endl;
											#endif
											
											//cout << "referenceTypePersonCrossReferenceNumberArray[i] = " << referenceTypePersonCrossReferenceNumberArray[i] << endl;
											//cout << "referenceTypePersonCrossReferenceGenderArray[i] = " << referenceTypePersonCrossReferenceGenderArray[i] << endl;
											//cout << "referenceTypePersonCrossReferenceDefiniteArray[i] = " << referenceTypePersonCrossReferenceDefiniteArray[i] << endl;
											//cout << "referenceTypePersonCrossReferencePersonArray[i] = " << referenceTypePersonCrossReferencePersonArray[i] << endl;
											//cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalNumberTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalNumberTemp << endl;
											//cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalGenderTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalGenderTemp << endl;
											//cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalDefiniteTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalDefiniteTemp << endl;
											//cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalPersonTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalPersonTemp << endl;
											
																																										
											if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->isSubjectTemp)
											{
												//cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->isSubjectTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->isSubjectTemp << endl;
												referenceSourceHasBeenFound = true;
												referenceSource = currentEntityInWhichReferenceSourceIsBeingSearchedFor;
											}
											else if((currentEntityInWhichReferenceSourceIsBeingSearchedFor->isObjectTemp) && (s2 > 0))
											{
												//cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->isObjectTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->isObjectTemp << endl;
												referenceSourceHasBeenFound = true;
												referenceSource = currentEntityInWhichReferenceSourceIsBeingSearchedFor;
											}
											else if((currentEntityInWhichReferenceSourceIsBeingSearchedFor->hasPropertyTemp) && (s2 > 0))
											{
												//cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->hasPropertyTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->hasPropertyTemp << endl;
												referenceSourceHasBeenFound = true;
												referenceSource = currentEntityInWhichReferenceSourceIsBeingSearchedFor;
											}
										}

									}
									else
									{
										//cout << "error: !entityAlreadyExistant" << endl;	//will be non-existant in the case of intermediary words like "the"
										//exit(0);
									}
								}
								w2++;
								
								currentRelationInWhichReferenceSourceIsBeingSearchedFor = currentRelationInWhichReferenceSourceIsBeingSearchedFor->next;
							}

							if(currentSentenceInWhichReferenceSourceIsBeingSearchedFor->previous != NULL)
							{	
								currentSentenceInWhichReferenceSourceIsBeingSearchedFor = currentSentenceInWhichReferenceSourceIsBeingSearchedFor->previous;
							}
							else
							{
								stillSentencesToSearch = false;
							}
							s2++;

						}

						if(referenceSourceHasBeenFound)
						{//remap entity; eg He to John 
							#ifdef GIA_TRANSLATOR_DEBUG
							cout << "referenceSourceHasBeenFound: assigning " << GIAEntityNodeArray[w]->entityName << " to " << referenceSource->entityName << "." << endl;
							#endif
							//referenceSource->isReferenceEntityInThisSentence = true;
							GIAEntityNodeArray[w] =	referenceSource;
							GIAEntityNodeIsAReference[w] = true;
						}			
					}
				}
			}
		}
		#endif
		
				
		//cout << "pass B;" << endl;
			
		//cout << "0z pass; define properties (objects/subjects with properties; eg "Truffles which are picked are tasty." - Truffle must be an instance/property in this case);" << endl;
		for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
		{	
			if(GIAEntityNodeArrayFilled[i])
			{
				if(((GIAEntityNodeArray[i]->isObjectTemp) && (GIAEntityNodeArray[i]->hasPropertyTemp)) || ((GIAEntityNodeArray[i]->isSubjectTemp) && (GIAEntityNodeArray[i]->hasPropertyTemp)))
				{
					addPropertyToPropertyDefinition(GIAEntityNodeArray[i]);	
				}
			}
		}		

			
		//cout << "0a pass; define properties (definite nouns); eg the house" << endl;
		if(GIA_ASSIGN_INSTANCE_PROPERTY_TO_ALL_DEFINITIVE_NOUNS == 1)
		{
			for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
			{
				if(GIAEntityNodeArrayFilled[i])
				{ //condition required as GIAEntityNodeArrayFilled is not always true. With GRAMMATICAL_DEFINITE, eg "Mr" of "Mr Smith" will still be interpreted as a definite
					if(!GIAEntityNodeIsAReference[i])
					{//do not define properties based upon references (as the grammatical information is no longer correct, and it has already been done previously if necessary to the referenced entity)
						if(!GIA_DO_NOT_ASSIGN_INSTANCE_PROPERTY_TO_PERSONS_OR_DATES || (!GIAEntityNodeGrammaticalIsPersonArray[i] & !GIAEntityNodeIsDate[i]))
						{
							if(GIAEntityNodeGrammaticalIsDefiniteArray[i] == GRAMMATICAL_DEFINITE)
							{
								//cout << "as0" << endl;
								//cout << "GIAEntityNodeArray[i]->entityName = " << GIAEntityNodeArray[i]->entityName << endl;			
								addPropertyToPropertyDefinition(GIAEntityNodeArray[i]);			
							}
						}
					}
				}
			}
		}
		//cout << "0b pass; define properties (nouns with determinates); eg a house, the house, the houses [all nouns with singular/plural are assumed to have determintes, and are therefore properties]" << endl;
		if(GIA_ASSIGN_INSTANCE_PROPERTY_TO_ALL_NOUNS_WITH_DETERMINATES == 1)
		{
			for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
			{	
				if(GIAEntityNodeArrayFilled[i])
				{
					if(!GIAEntityNodeIsAReference[i])
					{//do not define properties based upon references (as the grammatical information is no longer correct, and it has already been done previously if necessary to the referenced entity)
						if(!GIA_DO_NOT_ASSIGN_INSTANCE_PROPERTY_TO_PERSONS_OR_DATES || (!GIAEntityNodeGrammaticalIsPersonArray[i] && !GIAEntityNodeIsDate[i]))
						{
							bool passed = false;
							for(int j=0; j<GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES; j++)
							{
								if(GIAEntityNodeArray[i]->grammaticalNumberTemp == referenceTypeHasDeterminateCrossReferenceNumberArray[j])
								{
									passed = true;
								}
							}
							if(passed)
							{
								//cout << "as1" << endl;
								addPropertyToPropertyDefinition(GIAEntityNodeArray[i]);
							}
						}
					}
				}
			}
		}
		
		//cout << "0c pass; define properties (nouns with adjectives); _amod; eg locked door, _advmod; eg cheetahs run quickly [NOT and c) _predadj; eg giants are red / joe is late]" << endl;
		currentRelationInList = currentSentenceInList->firstRelationInList;
 		while(currentRelationInList->next != NULL)
		{	
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
				//create a new property for the entity and assign a property definition entity if not already created
				string thingName = currentRelationInList->relationFunction;
				string propertyName = currentRelationInList->relationArgument; 
				int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
				int relationArgumentIndex = currentRelationInList->relationArgumentIndex;				
				GIAEntityNode * thingEntity = GIAEntityNodeArray[relationFunctionIndex];
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationArgumentIndex];
				
				//cout << "as2" << endl;
				
				addPropertyToPropertyDefinition(thingEntity);					
			}			
			currentRelationInList = currentRelationInList->next;
		}				
		
		//cout << "0d pass; define properties (quantities [not quantity mods/multipiers, not measure pers] and measures);" << endl;
		currentRelationInList = currentSentenceInList->firstRelationInList;
 		while(currentRelationInList->next != NULL)
		{			
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
				int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationFunctionIndex];
				
				//cout << "as3" << endl;
				
				addPropertyToPropertyDefinition(propertyEntity);					
			}
						
			currentRelationInList = currentRelationInList->next;
		}
		
		//cout << "0e pass; define properties (quantity mods);" << endl;
		currentRelationInList = currentSentenceInList->firstRelationInList;
 		while(currentRelationInList->next != NULL)
		{						
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
				int relationArgumentIndex = currentRelationInList->relationArgumentIndex;
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationArgumentIndex];
				
				//cout << "as3" << endl;
				addPropertyToPropertyDefinition(propertyEntity);					
			}
						
			currentRelationInList = currentRelationInList->next;
		}
								
		
		//cout << "0f pass; define properties (expletives eg "there" in "there is a place");" << endl;
		currentRelationInList = currentSentenceInList->firstRelationInList;
 		while(currentRelationInList->next != NULL)
		{			
			bool passed = false;
			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT_EXPLETIVE)
			{
				//create property definition
				int relationArgumentIndex = currentRelationInList->relationArgumentIndex;
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationArgumentIndex];
				
				//cout << "as4" << endl;
				
				addPropertyToPropertyDefinition(propertyEntity);					
			}			
			currentRelationInList = currentRelationInList->next;
		}

		//cout << "0g pass; define properties (pronouns eg "we"/"I");" << endl;
		if(GIA_ASSIGN_INSTANCE_PROPERTY_TO_ALL_PRONOUNS == 1)
		{
			for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
			{
				if(GIAEntityNodeArrayFilled[i])
				{
					if(!GIAEntityNodeIsAReference[i])
					{//do not define properties based upon references (as the grammatical information is no longer correct, and it has already been done previously if necessary to the referenced entity)
				
						if(GIAEntityNodeGrammaticalIsPronounArray[i] == GRAMMATICAL_PRONOUN)
						{
							//cout << "as5" << endl;
							//cout << "asd" << endl;
							//cout << "GIAEntityNodeArray[i]->entityName = " << GIAEntityNodeArray[i]->entityName << endl;			
							addPropertyToPropertyDefinition(GIAEntityNodeArray[i]);			
						}
					}
				}
			}
		}
		
		//cout << "0h pass; define properties (to_be);" << endl;
		currentRelationInList = currentSentenceInList->firstRelationInList;
 		while(currentRelationInList->next != NULL)
		{								
			if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_BE)
			{
				//create a new property for the entity and assign a property definition entity if not already created
				int relationArgumentIndex = currentRelationInList->relationArgumentIndex;
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationArgumentIndex];

				addPropertyToPropertyDefinition(propertyEntity);
			}

			//cout << "as3" << endl;
						
			currentRelationInList = currentRelationInList->next;
		}	
		
		//cout << "0i pass; define properties (to_do);" << endl;
		currentRelationInList = currentSentenceInList->firstRelationInList;
 		while(currentRelationInList->next != NULL)
		{						
			if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_DO)
			{
				//create a new property for the entity and assign a property definition entity if not already created
				int relationArgumentIndex = currentRelationInList->relationArgumentIndex;
				GIAEntityNode * actionEntity = GIAEntityNodeArray[relationArgumentIndex];

				addActionToActionDefinition(actionEntity);
			}
			//cout << "as3" << endl;
		
			currentRelationInList = currentRelationInList->next;
		}			
									

		
										
		//cout << "1 pass; link properties (possessive relationships); eg joe's bike" << endl;
		currentRelationInList = currentSentenceInList->firstRelationInList;
 		while(currentRelationInList->next != NULL)
		{
			bool passed = false;
			for(int i=0; i<RELATION_TYPE_POSSESSIVE_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypePossessiveNameArray[i])
				{
					passed = true;
				}
			}	
			//if(currentRelationInList->relationType == RELATION_TYPE_POSSESSIVE)
			if(passed)
			{
				//cout << "RELATION_TYPE_POSSESSIVE" << endl;
				
				string propertyName = currentRelationInList->relationFunction; 
				string ownerName = currentRelationInList->relationArgument; 
				int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
				int relationArgumentIndex = currentRelationInList->relationArgumentIndex;				

				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationFunctionIndex];
				GIAEntityNode * ownerEntity = GIAEntityNodeArray[relationArgumentIndex];
				//cout << "propertyName = " << propertyEntity->entityName << endl;
				//cout << "ownerName = " << ownerEntity->entityName << endl;
													
				addOrConnectPropertyToEntity(ownerEntity, propertyEntity);
			}			
			currentRelationInList = currentRelationInList->next;
		}
	
		//cout << "2 pass; link properties (descriptive relationships); eg joe is happy" << endl;
		currentRelationInList = currentSentenceInList->firstRelationInList;
 		while(currentRelationInList->next != NULL)
		{	
			bool passed = false;
			for(int i=0; i<RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeAdjectiveNameArray[i])
				{
					passed = true;
				}
			}						
			//if((currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_1) || (currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_2) || (currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_3))
			if(passed)
			{
				//create a new property for the entity and assign a property definition entity if not already created
				string thingName = currentRelationInList->relationFunction;
				string propertyName = currentRelationInList->relationArgument; 
				int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
				int relationArgumentIndex = currentRelationInList->relationArgumentIndex;				
				GIAEntityNode * thingEntity = GIAEntityNodeArray[relationFunctionIndex];
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationArgumentIndex];
				
				#ifdef GIA_TRANSLATOR_DEBUG
				cout << "thingEntity = " << thingEntity->entityName << endl;
				cout << "propertyEntity = " << propertyEntity->entityName << endl;
				#endif
												
				addOrConnectPropertyToEntity(thingEntity, propertyEntity);					
			}			
			currentRelationInList = currentRelationInList->next;
		}					

		//cout << "2b pass; link entity definitions (appositive of nouns only)" << endl;
		currentRelationInList = currentSentenceInList->firstRelationInList;
 		while(currentRelationInList->next != NULL)
		{
			if(currentRelationInList->relationType == RELATION_TYPE_APPOSITIVE_OF_NOUN)
			{
				string propertyName = currentRelationInList->relationFunction; 
				string entityName = currentRelationInList->relationArgument; 
				int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
				int relationArgumentIndex = currentRelationInList->relationArgumentIndex;				

				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationFunctionIndex];
				GIAEntityNode * definitionEntity = GIAEntityNodeArray[relationArgumentIndex];
				
				#ifdef GIA_TRANSLATOR_DEBUG
				cout << "propertyName = " << propertyEntity->entityName << endl;
				cout << "entityName = " << definitionEntity->entityName << endl;
				#endif
								
				addDefinitionToEntity(propertyEntity, definitionEntity);									
			}			
			currentRelationInList = currentRelationInList->next;
		}
					
		
 		//cout <<"3 pass; define dependent subject-object definition/composition/action relationships and independent subject/object action relationships" << endl;
 		currentRelationInList = currentSentenceInList->firstRelationInList;
		bool subjectObjectRelationshipAlreadyAdded[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
		for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE;i++)
		{
			subjectObjectRelationshipAlreadyAdded[i] = false;
		}
		while(currentRelationInList->next != NULL)
		{	
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
				
			int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
			int relationArgumentIndex = currentRelationInList->relationArgumentIndex;
			
			bool passed = false;
			for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeObjectNameArray[i])
				{
					passed = true;
				}
			}
			for(int i=0; i<RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeSubjectNameArray[i])
				{
					passed = true;
				}
			}												
			//if((currentRelationInList->relationType == RELATION_TYPE_SUBJECT) || ((currentRelationInList->relationType == RELATION_TYPE_OBJECT) || (currentRelationInList->relationType == RELATION_TYPE_OBJECT_TO) || (currentRelationInList->relationType == RELATION_TYPE_SUBJECT_EXPLETIVE)))
			if(passed)
			{
				if(subjectObjectRelationshipAlreadyAdded[relationFunctionIndex] != true)
				{
					subjectObjectRelationshipAlreadyAdded[relationFunctionIndex] = true;

					bool foundPartner = false;			
					string partnerTypeRequired;
					string subjectObjectName[2];
					GIAEntityNode * subjectObjectEntityArray[2];
					GIAEntityNode * subjectObjectFunctionEntityArray[2];
					string partnerTypeRequiredArray[2];
					partnerTypeRequiredArray[SUBJECT_INDEX] = RELATION_TYPE_SUBJECT;
					partnerTypeRequiredArray[OBJECT_INDEX] = RELATION_TYPE_OBJECT;
					for(int i=0; i<2; i++)
					{
						subjectObjectEntityArray[i] = NULL;
						subjectObjectFunctionEntityArray[i] = NULL;
					}
					int firstIndex;
					int secondIndex;
					
					bool passsubject = false;
					bool passobject = false;
					for(int i=0; i<RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
					{
						if(currentRelationInList->relationType == relationTypeSubjectNameArray[i])
						{
							passsubject = true;
						}
					}														
					for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
					{
						if(currentRelationInList->relationType == relationTypeObjectNameArray[i])
						{
							passobject = true;
						}
					}			
					//if((currentRelationInList->relationType == RELATION_TYPE_SUBJECT) || (currentRelationInList->relationType == RELATION_TYPE_SUBJECT_EXPLETIVE))
					if(passsubject)
					{
						firstIndex = SUBJECT_INDEX;
						secondIndex = OBJECT_INDEX;			
					}
					//else if((currentRelationInList->relationType == RELATION_TYPE_OBJECT) || (currentRelationInList->relationType == RELATION_TYPE_OBJECT_TO))
					else if(passobject)
					{
						firstIndex = OBJECT_INDEX;
						secondIndex = SUBJECT_INDEX;									

					}
					partnerTypeRequired = partnerTypeRequiredArray[secondIndex];
					subjectObjectName[firstIndex] = currentRelationInList->relationArgument;
					#ifdef GIA_TRANSLATOR_DEBUG
					cout << partnerTypeRequiredArray[firstIndex] << " name = " << subjectObjectName[firstIndex] << endl;
					#endif
					subjectObjectEntityArray[firstIndex] = GIAEntityNodeArray[relationArgumentIndex];
					subjectObjectFunctionEntityArray[firstIndex] = GIAEntityNodeArray[relationFunctionIndex]; 					
					//cout << "subjectObjectEntityArray[firstIndex]->entityName = " << subjectObjectEntityArray[firstIndex]->entityName << endl;	


					bool passcomposition = false;
					for(int i=0; i<RELATION_FUNCTION_COMPOSITION_NUMBER_OF_TYPES; i++)
					{
						if(currentRelationInList->relationFunction == relationFunctionCompositionNameArray[i])
						{
							passcomposition = true;
						}
					}

					bool passdefinition = false;
					for(int i=0; i<RELATION_FUNCTION_DEFINITION_NUMBER_OF_TYPES; i++)
					{
						if(currentRelationInList->relationFunction == relationFunctionDefinitionNameArray[i])
						{
							passdefinition = true;
						}
					}					
								
					//now find the associated object..
 					Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
					while(currentRelationInList2->next != NULL)
					{	
						//cout << "currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;

						bool partnerTypeRequiredFound = false;
						bool partnerTypeObjectSpecialConditionFound = false;
						
						if(partnerTypeRequired == RELATION_TYPE_SUBJECT)
						{							
							for(int i=0; i<RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
							{
								if(currentRelationInList2->relationType == relationTypeSubjectNameArray[i])
								{
									partnerTypeRequiredFound = true;
								}
							}
						}
						else if(partnerTypeRequired == RELATION_TYPE_OBJECT)														
						{
							for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
							{
								if(currentRelationInList2->relationType == relationTypeObjectNameArray[i])
								{
									partnerTypeRequiredFound = true;
								}
							}
							
							for(int i=0; i<RELATION_TYPE_OBJECT_SPECIAL_CONDITION_NUMBER_OF_TYPES; i++)
							{
								if(currentRelationInList2->relationType == relationTypeObjectSpecialConditionNameArray[i])
								{
									partnerTypeRequiredFound = true;
									partnerTypeObjectSpecialConditionFound = true;
								}
							}							
						}
						int relationFunctionIndex2 = currentRelationInList2->relationFunctionIndex;
						int relationArgumentIndex2 = currentRelationInList2->relationArgumentIndex;
						subjectObjectName[secondIndex] = currentRelationInList2->relationArgument;
						subjectObjectEntityArray[secondIndex] = GIAEntityNodeArray[relationArgumentIndex2];
						subjectObjectFunctionEntityArray[secondIndex] = GIAEntityNodeArray[relationFunctionIndex2]; 						
												
						if(partnerTypeRequiredFound)
						{
							if(relationFunctionIndex == relationFunctionIndex2)
							{//found a matching object-subject relationship
								
								//cout << "\n\n relationFunctionIndex2 == relationFunctionIndex2 " << endl;
			
								#ifdef GIA_TRANSLATOR_DEBUG
								cout << partnerTypeRequiredArray[secondIndex] << " name = " << subjectObjectName[secondIndex] << endl;
								#endif
								//cout << "subjectObjectEntityArray[secondIndex]->entityName = " << subjectObjectEntityArray[secondIndex]->entityName << endl;	

								//cout << "subjectObjectEntityArray[SUBJECT_INDEX]->entityName = " << subjectObjectEntityArray[SUBJECT_INDEX]->entityName << endl;	
								//cout << "subjectObjectEntityArray[OBJECT_INDEX]->entityName = " << subjectObjectEntityArray[OBJECT_INDEX]->entityName << endl;

								//if(currentRelationInList->relationFunction == RELATION_FUNCTION_DEFINITION_1) 
								if(passdefinition)
								{//expected subject-object relationship is a definition "is"
									
									//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
										//NB definitions are only assigned to entities, not properties (instances of entities)
									
									if(subjectObjectEntityArray[SUBJECT_INDEX]->entityName == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
									{
									#ifdef GIA_TRANSLATOR_DISABLE_OBJ_SUB_QVARIABLE_ANOMALY
										//switch object/subject variables [transform question into answer form]
										addDefinitionToEntity(subjectObjectEntityArray[OBJECT_INDEX], subjectObjectEntityArray[SUBJECT_INDEX]);
									#else
										//added 20 October 2011 [what is the time?]
										string relationType = currentRelationInList->relationFunction;
										GIAEntityNode * actionOrPropertyEntity = subjectObjectEntityArray[OBJECT_INDEX];				
										GIAEntityNode * actionOrPropertyConditionEntity = subjectObjectEntityArray[SUBJECT_INDEX];	
										createConditionBasedUponPreposition(actionOrPropertyEntity, actionOrPropertyConditionEntity, relationType);
									#endif	
									}
									else
									{
										
										addDefinitionToEntity(subjectObjectEntityArray[SUBJECT_INDEX], subjectObjectEntityArray[OBJECT_INDEX]);
									}
								}
								//else if((currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_1) || (currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_2) || (currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_3))
								else if(passcomposition)
								{//subject-object relationship is a composition [property]
									addOrConnectPropertyToEntity(subjectObjectEntityArray[SUBJECT_INDEX], subjectObjectEntityArray[OBJECT_INDEX]);
										//check can use properties for composition/comprises ; ie, does "tom is happy" = "tom comprises happiness" ?
								}
								else if(partnerTypeObjectSpecialConditionFound)
								{
									GIAEntityNode * subjectEntityOrProperty = subjectObjectEntityArray[SUBJECT_INDEX];
									GIAEntityNode * specialConditionNode = GIAEntityNodeArray[relationFunctionIndex2];
									//cout << "subjectEntityOrProperty->entityName = " << subjectEntityOrProperty->entityName << endl;
									//cout << "specialConditionNode->entityName = " << specialConditionNode->entityName << endl;			
									addOrConnectPropertyConditionToEntity(subjectEntityOrProperty, specialConditionNode, specialConditionNode->entityName);
								}
								else
								{//assume that the subject-object relationships is an action
									string actionName = currentRelationInList->relationFunction;
									//cout << "1 actionName = " << actionName << endl;
									GIAEntityNode * actionEntity = GIAEntityNodeArray[relationFunctionIndex];
									
									//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
									GIAEntityNode * subjectEntityTemp = subjectObjectEntityArray[SUBJECT_INDEX];
									GIAEntityNode * objectEntityTemp = subjectObjectEntityArray[OBJECT_INDEX];

									/*
									cout << "SUBJECT_INDEX = " << SUBJECT_INDEX << endl;
									cout << "OBJECT_INDEX = " << OBJECT_INDEX << endl;									
									cout << "firstIndex = " << firstIndex << endl;
									cout << "secondIndex = " << secondIndex << endl;
									cout << "subjectObjectName[SUBJECT_INDEX] = " << subjectObjectName[SUBJECT_INDEX] << endl;
									cout << "subjectObjectName[OBJECT_INDEX] = " << subjectObjectName[OBJECT_INDEX] << endl;
									cout << "subjectObjectEntityArray[SUBJECT_INDEX]->entityName = " << subjectObjectEntityArray[SUBJECT_INDEX]->entityName << endl;																	
									cout << "subjectObjectEntityArray[OBJECT_INDEX]->entityName = " << subjectObjectEntityArray[OBJECT_INDEX]->entityName << endl;
									cout << "relationArgumentIndex = " << relationArgumentIndex << endl;
									cout << "relationArgumentIndex2 = " << relationArgumentIndex2 << endl;
									*/
									
									addActionToEntity(subjectEntityTemp, objectEntityTemp, actionEntity);
								}
								foundPartner = true;	
								
												
							}
							else
							{//do not find matching object-subject relationship [search for intermediary {ie redundant} relations, and if so create a condition link between subject and object] 
								
								if(!foundPartner)
								{//do not overwrite usage of subj/obj if a direct link [ie action] has been found (this condition probably mau not be required)
								
									if(subjectObjectFunctionEntityArray[SUBJECT_INDEX]->entityName == RELATION_FUNCTION_DEFINITION_1)
									{
										//cout << "a" << endl;
										Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
										while(currentRelationInList3->next != NULL)
										{
											if(currentRelationInList3->relationFunction == RELATION_FUNCTION_DEFINITION_1)
											{											
												//cout << "a2" << endl;
												if((currentRelationInList3->relationType == RELATION_TYPE_ADJECTIVE_3))	//OR if (currentRelationInList3->relationType == subjectObjectFunctionEntityArray[OBJECT_INDEX]->entityName)
												{
													//cout << "b" << endl;
													if(currentRelationInList3->relationArgument == subjectObjectFunctionEntityArray[OBJECT_INDEX]->entityName)
													{		
														//cout << "c" << endl;
														/*
														Eg;									
														_obj(on[6], _$qVar[1])
														_advmod(be[2], on[6])
														_subj(be[2], claim[5])
														and/or;
														_obj(on[6], _$qVar[1])
														on(be[2], on[6])
														_subj(be[2], claim[5])													
														*/

														//create a condition link between the object and subject, based upon RELATION_FUNCTION_DEFINITION_1

														GIAEntityNode * subjectEntityOrProperty = subjectObjectEntityArray[SUBJECT_INDEX];
														GIAEntityNode * specialConditionNode = subjectObjectEntityArray[OBJECT_INDEX];
														//cout << "subjectEntityOrProperty->entityName = " << subjectEntityOrProperty->entityName << endl;
														//cout << "specialConditionNode->entityName = " << specialConditionNode->entityName << endl;			
														addOrConnectPropertyConditionToEntity(subjectEntityOrProperty, specialConditionNode, currentRelationInList3->relationArgument);

														foundPartner = true;
													}
												}
											}

											//cout << "currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;
											currentRelationInList3 = currentRelationInList3->next;											
										}
									}

								}
							}
						}
						currentRelationInList2 = currentRelationInList2->next;
					}

					if(!foundPartner)
					{//add independent action if appropriate
						if(passdefinition)
						//if(currentRelationInList->relationFunction == RELATION_FUNCTION_DEFINITION_1) 
						{
						}
						//else if((currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_1) || (currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_2) || (currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_3))
						else if(passcomposition)
						{
						}
						else
						{//assume that the subject-object relationships is an action
							string actionName = currentRelationInList->relationFunction;
							//cout << "2 actionName = " << actionName << endl;
							GIAEntityNode * actionEntity = GIAEntityNodeArray[relationFunctionIndex];

							//addAction(actionEntity);	//WHY WAS THIS HERE????
							if(firstIndex == SUBJECT_INDEX)
							{//fired by joe..???? [is there a possible example of this?]
							
								//find out if the subject is connected to an _advmod, if so assign it as an action condition instead of a subject+action
								bool subjectIsConnectedToAnAdvMod = false;
								Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
								while(currentRelationInList3->next != NULL)
								{
									if(currentRelationInList3->relationType == RELATION_TYPE_ADJECTIVE_3)
									{
										if(subjectObjectEntityArray[SUBJECT_INDEX]->entityName == currentRelationInList3->relationArgument)
										{//subject is connected to an _advmod
											subjectIsConnectedToAnAdvMod = true;

											//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
											GIAEntityNode * actionOrPropertyConditionEntity = subjectObjectFunctionEntityArray[SUBJECT_INDEX];
											GIAEntityNode * actionOrPropertyEntity = GIAEntityNodeArray[currentRelationInList3->relationFunctionIndex];
											string relationType = subjectObjectEntityArray[SUBJECT_INDEX]->entityName;
											
											addActionToActionDefinition(actionOrPropertyConditionEntity);
											createConditionBasedUponPreposition(actionOrPropertyEntity, actionOrPropertyConditionEntity, relationType);								
										}
									}
									currentRelationInList3 = currentRelationInList3->next;
								}	
															
								if(!subjectIsConnectedToAnAdvMod)
								{
									//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
									GIAEntityNode * subjectEntityTemp = subjectObjectEntityArray[SUBJECT_INDEX];

									addActionToSubject(subjectEntityTemp, actionEntity);	
								}					
							}
							else if(firstIndex == OBJECT_INDEX)
							{//eg the bow was fired
							
								//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
								GIAEntityNode * objectEntityTemp = subjectObjectEntityArray[OBJECT_INDEX];
															
								addActionToObject(objectEntityTemp, actionEntity);
							}	

						}
					}

					//now depending upon the function, do something different; create an action (if 'does' etc), create a definition (if 'is' etc)
				}
			}
		
			currentRelationInList = currentRelationInList->next;
		}
		
		
		 //cout << "3b pass; define indirect objects" << endl;
		 currentRelationInList = currentSentenceInList->firstRelationInList;
		while(currentRelationInList->next != NULL)
		{	
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
														
			if(currentRelationInList->relationType == RELATION_TYPE_INDIRECT_OBJECT)
			{					
				//now find the associated object..
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{	
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
							
						if(currentRelationInList2->relationFunctionIndex == currentRelationInList->relationFunctionIndex)
						{//found a matching object-indirectobject relationship
							//cout << "partnerTypeRequiredFound: currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;
						
							
							GIAEntityNode * propertyEntity = GIAEntityNodeArray[currentRelationInList2->relationArgumentIndex];
							GIAEntityNode * thingEntity = GIAEntityNodeArray[currentRelationInList->relationArgumentIndex];
				
							addOrConnectPropertyToEntity(thingEntity, propertyEntity);
						}
					}
					
					currentRelationInList2 = currentRelationInList2->next;
				}
			}
			currentRelationInList = currentRelationInList->next;
		}			

		 //cout << "3c pass; define object/subject of preposition" << endl;
		 currentRelationInList = currentSentenceInList->firstRelationInList;
		while(currentRelationInList->next != NULL)
		{	
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
														
			if(currentRelationInList->relationType == RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION)
			{					
				//now find the associated object..
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{	
					bool partnerTypeRequiredFound = false;					
					if(currentRelationInList2->relationType == RELATION_TYPE_PREPOSITION_SUBJECT_OF_PREPOSITION)
					{
						partnerTypeRequiredFound = true;
					}
						
					if(partnerTypeRequiredFound)
					{		
						if(currentRelationInList2->relationFunctionIndex == currentRelationInList->relationFunctionIndex)
						{//found a matching preposition of object-subject relationship
							//cout << "partnerTypeRequiredFound: currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;
						
							GIAEntityNode * entityNode = GIAEntityNodeArray[currentRelationInList2->relationArgumentIndex];
							GIAEntityNode * conditionEntityNode = GIAEntityNodeArray[currentRelationInList->relationArgumentIndex];
							string conditionName = GIAEntityNodeArray[currentRelationInList2->relationFunctionIndex]->entityName;
							
							addOrConnectPropertyConditionToEntity(entityNode, conditionEntityNode, conditionName);
						}
					}
					
					currentRelationInList2 = currentRelationInList2->next;
				}
			}
			currentRelationInList = currentRelationInList->next;
		}
		
		
		//cout << "4a pass; define action/property conditions" << endl;
		currentRelationInList = currentSentenceInList->firstRelationInList;
		while(currentRelationInList->next != NULL)
		{	
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

			int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
			int relationArgumentIndex = currentRelationInList->relationArgumentIndex;
			string relationType = currentRelationInList->relationType;
			GIAEntityNode * actionOrPropertyEntity = GIAEntityNodeArray[relationFunctionIndex];				
			GIAEntityNode * actionOrPropertyConditionEntity = GIAEntityNodeArray[relationArgumentIndex];	
			createConditionBasedUponPreposition(actionOrPropertyEntity, actionOrPropertyConditionEntity, relationType);
						
			currentRelationInList = currentRelationInList->next;
		}
	
		
		//cout <<"4b pass; extract dates" << endl;	//[this could be implemented/"shifted" to an earlier execution stage with some additional configuration]
		for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
		{
			if(GIAEntityNodeArrayFilled[i])
			{
				GIAEntityNode * currentEntity = GIAEntityNodeArray[i];
				if(currentEntity->hasAssociatedTime)
				{
					if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)	
					{
						if(currentEntity->timeConditionNode != NULL)
						{
												
							GIAEntityNode * timeConditionEntity = currentEntity;
							//cout << "as1" << endl;

							string monthString = timeConditionEntity->entityName;
							int monthInt = TIME_MONTH_UNDEFINED;
							for(int i=0; i<TIME_MONTH_NUMBER_OF_TYPES; i++)
							{
								if(monthString == timeMonthStringArray[i])
								{
									monthInt = i+1;
								}
							}
							timeConditionEntity->timeConditionNode->month = monthInt;

							//update/regenerate timeConditionName
							timeConditionEntity->timeConditionNode->conditionName = generateDateTimeConditionName(timeConditionEntity->timeConditionNode->dayOfMonth, timeConditionEntity->timeConditionNode->month, timeConditionEntity->timeConditionNode->year);
								//OLD; timeConditionEntity->entityName
						}
						else
						{
							#ifdef GIA_TRANSLATOR_DEBUG
							cout << "error: isolated date node found (not declared as a time condition)" << endl;
							#else
							cout << "error: [confidential]" << endl;	
							#endif
							exit(0);	//remove this later
						}
					}
					else
					{
						#ifdef GIA_TRANSLATOR_DEBUG
						cout << "error: isolated date node found (not declared as a time condition)" << endl;
						#else
						cout << "error: [confidential]" << endl;
						#endif
						exit(0);	//remove this later						
					}

				}
			}
		}	
		currentRelationInList = currentSentenceInList->firstRelationInList;
		while(currentRelationInList->next != NULL)
		{	
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
														
			if((currentRelationInList->relationType == RELATION_TYPE_DATE_DAY) || (currentRelationInList->relationType == RELATION_TYPE_DATE_YEAR))
			{	
				//now locate and fill corresponding time condition node;	
				for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
				{
					if(GIAEntityNodeArrayFilled[i])
					{
						GIAEntityNode * currentEntity = GIAEntityNodeArray[i];
						if(currentEntity->hasAssociatedTime)
						{			
							GIAEntityNode * timeConditionEntity = currentEntity;

							if(timeConditionEntity->entityName == currentRelationInList->relationFunction)
							{	
								if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)	
								{
									if(currentEntity->timeConditionNode != NULL)
									{									
										if(currentRelationInList->relationType == RELATION_TYPE_DATE_DAY)
										{
											//http://www.cplusplus.com/reference/clibrary/cstdlib/atoi/
												//The string can contain additional characters after those that form the integral number, which are ignored and have no effect on the behavior of this function.	[eg "3rd" -> 3]
											string dayOfMonthString = currentRelationInList->relationArgument;
											char * dayOfMonthStringcharstar = const_cast<char*>(dayOfMonthString.c_str());
											int dayOfMonthInt = atoi(dayOfMonthStringcharstar);
											timeConditionEntity->timeConditionNode->dayOfMonth = dayOfMonthInt;
											
											#ifdef GIA_TRANSLATOR_DEBUG
											cout << "adding day of month: " << dayOfMonthInt << endl;
											#endif
											
											//update/regenerate timeConditionName
											timeConditionEntity->timeConditionNode->conditionName = generateDateTimeConditionName(timeConditionEntity->timeConditionNode->dayOfMonth, timeConditionEntity->timeConditionNode->month, timeConditionEntity->timeConditionNode->year);
											//timeConditionEntity->entityName  =
										}
										if(currentRelationInList->relationType == RELATION_TYPE_DATE_YEAR)
										{
											string yearString = currentRelationInList->relationArgument;
											char * yearStringcharstar = const_cast<char*>(yearString.c_str());
											int yearInt = atoi(yearStringcharstar);
											timeConditionEntity->timeConditionNode->year = yearInt;
											
											#ifdef GIA_TRANSLATOR_DEBUG
											cout << "adding year: " << yearInt << endl;
											#endif
											
											//update/regenerate timeConditionName
											timeConditionEntity->timeConditionNode->conditionName = generateDateTimeConditionName(timeConditionEntity->timeConditionNode->dayOfMonth, timeConditionEntity->timeConditionNode->month, timeConditionEntity->timeConditionNode->year);
											//tempTimeCondition->entityName
										}
									}
									else
									{
										cout << "error: isolated date node found (not declared as a time condition)" << endl;
										exit(0);	//remove this later
									}										
								}
								else
								{
									#ifdef GIA_TRANSLATOR_DEBUG
									cout << "error: isolated date node found (not declared as a time condition)" << endl;
									#else
									cout << "error: [confidential]" << endl;
									#endif
									exit(0);	//remove this later						
								}
							}
						}
					}
				}										
			}
			currentRelationInList = currentRelationInList->next;		
		}
		#ifdef GIA_USE_TIME_NODE_INDEXING
		//cout <<"4b2 pass; add time condition nodes to index [for fast lookup by time]" << endl;
		for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
		{
			if(GIAEntityNodeArrayFilled[i])
			{
				GIAEntityNode * currentEntity = GIAEntityNodeArray[i];
				if(currentEntity->hasAssociatedTime)
				{
					if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)	
					{
						if(currentEntity->timeConditionNode != NULL)
						{
							//replace current entity time condition node with generated time condition node.
							
							int timeConditionEntityIndex = -1;
							bool argumentEntityAlreadyExistant = false;
							long timeConditionTotalTimeInSeconds = calculateTotalTimeInSeconds(timeConditionEntity->timeConditionNode->dayOfMonth, timeConditionEntity->timeConditionNode->month, timeConditionEntity->timeConditionNode->year);
							currentEntity->timeConditionNode = findOrAddTimeNodeByNumber(timeConditionNodesList, conceptEntityNamesList, timeConditionTotalTimeInSeconds, &argumentEntityAlreadyExistant, &timeConditionEntityIndex, true, currentEntity->timeConditionNode);
						
						}
						else
						{
							#ifdef GIA_TRANSLATOR_DEBUG
							cout << "error: isolated date node found (not declared as a time condition)" << endl;
							#else
							cout << "error: [confidential]" << endl;
							#endif
							exit(0);	//remove this later
						}
					}
					else
					{
						#ifdef GIA_TRANSLATOR_DEBUG
						cout << "error: isolated date node found (not declared as a time condition)" << endl;
						#else
						cout << "error: [confidential]" << endl;
						#endif
						exit(0);	//remove this later						
					}

				}
			}
		}	
		#endif	
		
		//cout << "4c pass; extract quantities" << endl;	
		currentRelationInList = currentSentenceInList->firstRelationInList;
		while(currentRelationInList->next != NULL)
		{	
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
														
			if(currentRelationInList->relationType == RELATION_TYPE_QUANTITY)
			{
				GIAEntityNode * quantityEntity = GIAEntityNodeArray[currentRelationInList->relationFunctionIndex];
				if(quantityEntity->AssociatedPropertyNodeList.size() >= 1)
				//if(quantityEntity->AssociatedPropertyNodeList.back() != NULL) - this is dangerous/impossible to use; it will not return NULL if pop_back() has been executed on the vector				
				{
					GIAEntityNode * quantityProperty = quantityEntity->AssociatedPropertyNodeList.back();
					quantityProperty->hasQuantity = true;
					int quantityNumberInt = calculateQuantityNumberInt(currentRelationInList->relationArgument);
					quantityProperty->quantityNumber = quantityNumberInt;
					
					if(currentRelationInList->relationArgument == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
					{//update comparison variable (set it to the quantity)	
						quantityProperty->isQuery = true;
						GIAEntityNodeArray[currentRelationInList->relationArgumentIndex]->isQuery = false;
						comparisonVariableNode = quantityProperty;		
					}
					
					//now locate quantity modifiers and multiplicators
					Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
					while(currentRelationInList2->next != NULL)
					{	
						//cout << "here1" << endl;
						//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

						if(currentRelationInList2->relationType == RELATION_TYPE_QUANTITY_MOD)
						{	
							if(currentRelationInList2->relationFunction == currentRelationInList->relationFunction)
							{
								//cout << "AAAA" << endl;
								
								/*
								int quantityModifierInt = calculateQuantityModifierInt(currentRelationInList2->relationArgument);
								quantityProperty->quantityModifier = quantityModifierInt;
								*/
								quantityProperty->quantityModifierString = currentRelationInList2->relationArgument;
							
							
								//added 12 Oct 11; add quantity modifiers as conditions (eg "almost" lost)	
								GIAEntityNode * entityNode = quantityProperty;
								GIAEntityNode * conditionEntityNode = GIAEntityNodeArray[currentRelationInList2->relationArgumentIndex];
								string conditionName = quantityProperty->quantityModifierString;

								addOrConnectPropertyConditionToEntity(entityNode, conditionEntityNode, "quantityModifier");
															
							}

						}	
						if(currentRelationInList2->relationType == RELATION_TYPE_QUANTITY_MULT)
						{
							if(currentRelationInList2->relationFunction == currentRelationInList->relationArgument)
							{
								int quantityMultiplierInt = calculateQuantityMultiplierInt(currentRelationInList2->relationArgument);
								quantityProperty->quantityNumber = quantityProperty->quantityNumber * quantityMultiplierInt;
							}						
						}						

						currentRelationInList2 = currentRelationInList2->next;
					}	
					
					
					bool relationTypeQuantityArgumentImplyMeasurePer = false;
					for(int i=0; i<RELATION_TYPE_QUANTITY_ARGUMENT_IMPLY_MEASURE_PER_NUMBER_OF_TYPES; i++)
					{
						if(currentRelationInList->relationArgument == relationTypeQuantityArgumentImplyMeasurePerNameArray[i])
						{
							relationTypeQuantityArgumentImplyMeasurePer = true;
						}
					}																		
					if(relationTypeQuantityArgumentImplyMeasurePer)
					{//eg "every hour" or "every day" - convert to measure_per system
						
						GIAEntityNode * entityToConnectMeasurePerEntity;
						bool foundQuantityOwner = false;
						Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
						while(currentRelationInList2->next != NULL)
						{	
							if(currentRelationInList2->relationArgument == currentRelationInList->relationFunction)
							{		
								entityToConnectMeasurePerEntity = GIAEntityNodeArray[currentRelationInList2->relationFunctionIndex];	//eg row
								foundQuantityOwner = true;
							}	
												
							currentRelationInList2 = currentRelationInList2->next;
						}	
						
						if(foundQuantityOwner)
						{
							//disconnect quantity node from existing connections (not including definitions) - NOT YET CODED.
							disconnectNodeFromAllButDefinitions(quantityProperty);
							
							GIAEntityNode * measureProperty = quantityProperty;	//convert quantity property to measure property
							measureProperty->hasQuantity = false;
							measureProperty->hasMeasure = true;
							measureProperty->measureType = MEASURE_TYPE_PER;						

							GIAEntityNode * newQuantityTimesEntity = new GIAEntityNode();
							newQuantityTimesEntity->id = currentEntityNodeIDInCompleteList;
							newQuantityTimesEntity->idSecondary = currentEntityNodeIDInPropertyEntityNodesList;

							entityNodesCompleteList->push_back(newQuantityTimesEntity);
							currentEntityNodeIDInCompleteList++;
							propertyEntityNodesList->push_back(newQuantityTimesEntity);
							currentEntityNodeIDInPropertyEntityNodesList++;
									
							newQuantityTimesEntity->entityName = "times";
																				
							//reconnect refreshed quanity (times) node;
							addOrConnectPropertyToEntity(entityToConnectMeasurePerEntity, newQuantityTimesEntity);

							if(newQuantityTimesEntity->hasAssociatedPropertyTemp)
							{//assumed true since its property was just explicitly created
								newQuantityTimesEntity = newQuantityTimesEntity->AssociatedPropertyNodeList.back();
							}
							newQuantityTimesEntity->hasQuantity = true;
							newQuantityTimesEntity->quantityNumber = 1;
							
							//now add measure_per condition node
							addOrConnectPropertyConditionToEntity(newQuantityTimesEntity, measureProperty, RELATION_TYPE_MEASURE_PER);
							
						}
						
					}
											
				}								
			}
			currentRelationInList = currentRelationInList->next;		
		}
		
		//cout << "4d pass; extract measures" << endl;
		currentRelationInList = currentSentenceInList->firstRelationInList;
		while(currentRelationInList->next != NULL)
		{	
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
			bool pass = false;
			int measureTypeIndex = MEASURE_TYPE_UNDEFINED;
			for(int i=0; i<RELATION_TYPE_MEASURE_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeMeasureNameArray[i])
				{
					measureTypeIndex = i;
					pass = true;
				}
			}																		
			if(pass)
			{
				int relationQuantityIndex = 0;
				int relationMeasureIndex = 0;
				if(currentRelationInList->relationType == RELATION_TYPE_MEASURE_PER)
				{
					relationQuantityIndex = currentRelationInList->relationFunctionIndex;
					relationMeasureIndex = currentRelationInList->relationArgumentIndex;			
				}
				else
				{
	
					relationQuantityIndex = currentRelationInList->relationArgumentIndex;
					relationMeasureIndex = currentRelationInList->relationFunctionIndex;										
				}
				
				GIAEntityNode * measureEntity = GIAEntityNodeArray[relationMeasureIndex];
				GIAEntityNode * quantityEntity = GIAEntityNodeArray[relationQuantityIndex];
				if(measureEntity->AssociatedPropertyNodeList.size() >= 1)
				//if(measureEntity->AssociatedPropertyNodeList.back() != NULL) - this is dangerous/impossible to use; it will not return NULL if pop_back() has been executed on the vector
				{
					GIAEntityNode * measurePropertyEntity = measureEntity->AssociatedPropertyNodeList.back();
					measurePropertyEntity->hasMeasure = true;
					measurePropertyEntity->measureType = measureTypeIndex;
					
					#ifdef GIA_TRANSLATOR_DEBUG									
					cout << "measurePropertyName = " << measurePropertyEntity->entityName << endl;
					cout << "quantityEntityName = " << quantityEntity->entityName << endl;
					#endif
					
					if(currentRelationInList->relationType == RELATION_TYPE_MEASURE_PER)
					{
						addOrConnectPropertyConditionToEntity(quantityEntity, measurePropertyEntity, relationTypeMeasureNameArray[measureTypeIndex]);
					}
					else
					{
						addOrConnectPropertyConditionToEntity(measurePropertyEntity, quantityEntity, relationTypeMeasureNameArray[measureTypeIndex]);

					}
				}								
			}
			currentRelationInList = currentRelationInList->next;		
		}
		//cout << "4d2 pass; extract measures" << endl;


		 //cout << "4e/4f pass; define to_be/to_do conditions" << endl;
		 currentRelationInList = currentSentenceInList->firstRelationInList;
		while(currentRelationInList->next != NULL)
		{	
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

			if((currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_BE) || (currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_DO))
			{					
				GIAEntityNode * entityNode = GIAEntityNodeArray[currentRelationInList->relationFunctionIndex];
				GIAEntityNode * conditionEntityNode = GIAEntityNodeArray[currentRelationInList->relationArgumentIndex];
				string conditionName = currentRelationInList->relationType;

				addOrConnectPropertyConditionToEntity(entityNode, conditionEntityNode, conditionName);
			}
			currentRelationInList = currentRelationInList->next;
		}
		


		//cout << "5a pass; parse questions" << endl;	
		/*
		currentRelationInList = currentSentenceInList->firstRelationInList;
		while(currentRelationInList->next != NULL)
		{	
			//parse specific relex questions: replace with standard prepositions and add question flag "isQuery"
			if(currentRelationInList->relationType == REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN)
			{
				passedPropositionTime = true;
			}
			else if(currentRelationInList->relationType == REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE)
			{
				passedPropositionLocation = true;
			}
			else if(currentRelationInList->relationType == REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHY)
			{
				passedPropositionReason = true;
			}	
		}
		*/
		
		
				
		#ifdef GIA_ENABLE_REFERENCE_LINKING_BASED_UPON_PRONOUNS_CLEAR_REFERENCES_EVERY_SENTENCE	
		//restore critical variables: used for GIA translator reference paser only - cleared every time a new sentence is parsed (Clearing should actually be applied to each paragraph/manuscript instead)
		long vectorSize = conceptEntityNamesList->size();
		for(int entityIndex=0; entityIndex<vectorSize; entityIndex++)
		{	
			GIAEntityNode * entityNode = conceptEntityNodesList->at(entityIndex);
			entityNode->entityAlreadyDeclaredInThisContext = false;
		}
		#endif
							
		currentSentenceInList = currentSentenceInList->next;
	}

	#ifndef GIA_ENABLE_REFERENCE_LINKING_BASED_UPON_PRONOUNS_CLEAR_REFERENCES_EVERY_SENTENCE	
	//restore critical variables: used for GIA translator reference paser only - cleared every time a new sentence is parsed (Clearing should actually be applied to each paragraph/manuscript instead)
	long vectorSize = conceptEntityNamesList->size();
	for(int entityIndex=0; entityIndex<vectorSize; entityIndex++)
	{	
		GIAEntityNode * entityNode = conceptEntityNodesList->at(entityIndex);
		entityNode->entityAlreadyDeclaredInThisContext = false;
	}
	#endif

}

void createConditionBasedUponPreposition(GIAEntityNode * actionOrPropertyEntity, GIAEntityNode * actionOrPropertyConditionEntity, string relationType)
{			
	bool passedPropositionTime = false;	
	bool passedPropositionLocation = false;
	bool passedPropositionReasonOrCircumstances = false;
	bool passedPropositionUnknown = false;

	//parse specific relex questions: not required, as these have been added to the relationTypePreposition_NameArrays
	/*
	if(currentRelationInList->relationType == REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN)
	{
		passedPropositionTime = true;
	}
	else if(currentRelationInList->relationType == REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE)
	{
		passedPropositionLocation = true;
	}
	else if(currentRelationInList->relationType == REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHY)
	{
		passedPropositionReason = true;
	}
	*/

	for(int i=0; i<RELATION_TYPE_PREPOSITION_TIME_NUMBER_OF_TYPES; i++)
	{
		if(relationType == relationTypePropositionTimeNameArray[i])
		{
			if(actionOrPropertyConditionEntity->hasAssociatedTime)
			{
				passedPropositionTime = true;
			}
			else if(relationType == REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN)
			{//NB time query entities obviously do not have associated times (time grammar flags generated by relex)
				passedPropositionTime = true;
			}
		}
	}			
	for(int i=0; i<RELATION_TYPE_PREPOSITION_LOCATION_NUMBER_OF_TYPES; i++)
	{
		if(relationType == relationTypePropositionLocationNameArray[i])
		{
			if(!actionOrPropertyConditionEntity->hasAssociatedTime)	
			{//NB "at" and "on" are shared for location and time prepositions
				passedPropositionLocation = true;
			}
		}
	}
	for(int i=0; i<RELATION_TYPE_PREPOSITION_REASON_OR_CIRCUMSTANCE_NUMBER_OF_TYPES; i++)
	{
		if(relationType == relationTypePropositionReasonOrCircumstanceNameArray[i])
		{
			if(!actionOrPropertyConditionEntity->hasAssociatedTime)	
			{//NB in case "_" and "_" are shared for reason and time prepositions				
				passedPropositionReasonOrCircumstances = true;
			}
		}
	}

	if(relationType[0] != RELATION_TYPE_PREPOSITION_FIRST_CHARACTER)
	{//not valid for REFERENCE_TYPE_QUESTION_QUERY_VARIABLEs... [but this is not a problem because passedPropositionUnknown is processed last in the if/else switch below]
		passedPropositionUnknown = true;
	}

	//CHECK THIS; check order - either select action or property first; NB there should not be both an associated action and an associated property in a given "Temp" context
	if(actionOrPropertyEntity->hasAssociatedPropertyTemp)
	{
		actionOrPropertyEntity = actionOrPropertyEntity->AssociatedPropertyNodeList.back();	
	}				

	//CHECK THIS; check order - either select action or property first; NB there should not be both an associated action and an associated property in a given "Temp" context
	if(actionOrPropertyConditionEntity->hasAssociatedPropertyTemp)
	{
		actionOrPropertyConditionEntity = actionOrPropertyConditionEntity->AssociatedPropertyNodeList.back();	//added 4 May 11a
	}

	if(passedPropositionTime)
	{
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "actionOrPropertyEntity->entityName = " << actionOrPropertyEntity->entityName << endl;
		cout << "timeConditionName = " << actionOrPropertyConditionEntity->entityName << endl;
		#endif

		addTimeConditionToProperty(actionOrPropertyEntity, actionOrPropertyConditionEntity, relationType);				
	}
	else if(passedPropositionLocation)
	{
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "actionOrPropertyEntity->entityName = " << actionOrPropertyEntity->entityName << endl;
		cout << "locationConditionName = " << actionOrPropertyConditionEntity->entityName << endl;
		#endif

		addLocationConditionToProperty(actionOrPropertyEntity, actionOrPropertyConditionEntity, relationType);	
	}		
	else if(passedPropositionReasonOrCircumstances)
	{
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "actionOrPropertyEntity->entityName = " << actionOrPropertyEntity->entityName << endl;
		cout << "reasonConditionName = " << actionOrPropertyConditionEntity->entityName << endl;
		#endif

		addReasonConditionToProperty(actionOrPropertyEntity, actionOrPropertyConditionEntity, relationType);		
	}
	else if(passedPropositionUnknown)
	{
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "actionOrPropertyEntity->entityName = " << actionOrPropertyEntity->entityName << endl;
		cout << "actionOrPropertyConditionName = " << actionOrPropertyConditionEntity->entityName << endl;
		#endif

		addPropertyConditionToProperty(actionOrPropertyEntity, actionOrPropertyConditionEntity, relationType);				
		//currentRelationInList->relationType.substr(1, currentRelationInList->relationType.length()-1)
	}
}





