/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * File Name: GIAtranslatorLinkEntitiesDynamic.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3d4a 18-July-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_LINK_ENTITIES_DYNAMIC
#define HEADER_GIA_TRANSLATOR_LINK_ENTITIES_DYNAMIC

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAtranslatorOperations.hpp"
#include "GIAdatabase.hpp"
#ifdef GIA_SEMANTIC_PARSER_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
#include "GIAsemanticParserOperations.hpp"
#endif

#ifndef GIA_DYNAMICALLY_LINK_FROM_CONDITIONS_GENERALISE_ACTION_TYPES
#define GIA_DYNAMICALLY_LINK_FROM_CONDITIONS_ACTION_TYPES_NUMBER_OF_TYPES 2
static string dynamicallyLinkFromConditionsActionTypesArray[GIA_DYNAMICALLY_LINK_FROM_CONDITIONS_ACTION_TYPES_NUMBER_OF_TYPES] = {"move", "copy"};	//must be synced with !GIA_STANFORD_PARSER_CONNECT_PREPOSITIONS_TO_ACTION_RATHER_THAN_ACTION_OBJECT_GENERALISE_ACTION_TYPES:redistributeStanfordRelationsConnectPrepositionsToActionRatherThanActionObject xml code
#endif

class GIAtranslatorLinkEntitiesDynamicClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAsentenceClassClass GIAsentenceClass;
	#ifdef GIA_SEMANTIC_PARSER_READ_SEMANTIC_RELATIONS
	private: GIAsemanticParserOperationsClass GIAsemanticParserOperations;
	#endif
	private: GIAentityNodeClassClass GIAentityNodeClass;
	public: void linkEntitiesDynamic(GIAtranslatorVariablesClass* translatorVariables);
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_OR_DEFINITIONS_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS
		private: void linkEntitiesDynamicPrenominalModifierOfNoun(GIAtranslatorVariablesClass* translatorVariables);
			private: bool linkEntitiesDynamicPrenominalModifierOfNounDirection(const GIArelation* currentRelationInList, GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entity1, GIAentityNode* entity2, int entity1Index, int entity2Index, int relationTypeIndex, const bool direction);
	#endif
	#ifdef GIA_DYNAMICALLY_LINK_FROM_CONDITIONS
		private: void linkEntitiesDynamicFromConditions(GIAtranslatorVariablesClass* translatorVariables);
	#endif
			private: bool findPreviousRelationship(GIAentityNode* instanceEntity, const GIAentityNode* comparisonEntity, GIAentityNode** targetEntityFound, bool* previousDefinitionRelationshipFound, bool* previousPropertyRelationshipFound, bool* previousConditionRelationshipFound);
			private: void connectDefinitionToEntityFull(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entity1, GIAentityNode* entity2, int entity1Index, int entity2Index, bool sameReferenceSet);
			private: void connectPropertyToEntityFull(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entity1, GIAentityNode* entity2, int entity1Index, int entity2Index, bool sameReferenceSet);
			private: int connectConditionToEntityFull(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entity1, GIAentityNode* entity2, int entity1Index, int entity2Index, const string conditionEntityName, int conditionIndex, bool sameReferenceSet);	//generic function (belongs in GIAtranslatorOperations.cpp)
			private: int getEntityIndex(GIAtranslatorVariablesClass* translatorVariables, const GIAentityNode* instanceEntity);
};		//generic function (belongs in GIAtranslatorOperations.cpp) //this function is required because entityIndexTemp reflects that of the referenceSource for references. Consider updating entityIndexTemp of referenceSource each time it is referenced (for current sentence)

#endif
