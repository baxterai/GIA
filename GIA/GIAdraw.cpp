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
 * File Name: GIAdraw.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3a6b 05-April-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Draws GIA nodes in GIA network/tree
 *
 *******************************************************************************/


#include "GIAdraw.hpp"


#include <GL/freeglut.h>	//need to use freeglut as it contains extensions functions which glut does not have; glutMainLoopEvent()

int maxXAtAParticularY[MAX_GIA_TREE_DEPTH];

void GIAdrawClass::printGIAnetworkNodes(vector<GIAentityNode*>* entityNodesActiveListComplete, int width, const int height, const string outputFileNameLDR, const string outputFileNameSVG, const string outputFileNamePPM, const bool display, const bool useOutputLDRfile, const bool useOutputPPMfile, const bool useOutputSVGfile, int maxNumberSentences)
{//most of this is copied from CSexecFlow.cpp
	bool result = true;

	XMLparserTag* firstTagInSVGFile = new XMLparserTag();

	bool printType[3];
	printType[GIA_DRAW_CREATE_LDR_REFERENCES] = false;
	printType[GIA_DRAW_CREATE_SVG_REFERENCES] = false;
	printType[GIA_DRAW_CREATE_LDR_OR_SVG_REFERENCES] = false;
	if(useOutputLDRfile || display)
	{// || useOutputPPMfile - implied
		printType[GIA_DRAW_CREATE_LDR_REFERENCES] = true;
		printType[GIA_DRAW_CREATE_LDR_OR_SVG_REFERENCES] = true;
	}
	if(useOutputSVGfile)
	{
		printType[GIA_DRAW_CREATE_SVG_REFERENCES] = true;
		printType[GIA_DRAW_CREATE_LDR_OR_SVG_REFERENCES] = true;
	}

	LDreference* firstReferenceInPrintList = new LDreference();
	this->determineBasicPrintPositionsOfAllNodes(entityNodesActiveListComplete, printType, firstReferenceInPrintList, firstTagInSVGFile, maxNumberSentences);

	if(useOutputSVGfile)
	{
		if(!LDsvg.writeSVGfile(outputFileNameSVG, firstTagInSVGFile))
		{
			result = false;
		}
		delete firstTagInSVGFile;
	}

	if(printType[GIA_DRAW_CREATE_LDR_REFERENCES] == true)
	{
		LDreferenceManipulation.writeReferencesToFile(outputFileNameLDR, firstReferenceInPrintList);
	}

	if(display)
	{

		//re-parse, then re-write to create a collapsed referencelist file...
		//method1:
		string topLevelSceneFileName = outputFileNameLDR;
		string topLevelSceneFileNameCollapsed = "sceneCollapsedForOpenGLDisplay.ldr";
		LDreference* initialReferenceInSceneFile = new LDreference();
		LDreference* topLevelReferenceInSceneFile = new LDreference(topLevelSceneFileName, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
		if(!LDparser.parseFile(topLevelSceneFileName, initialReferenceInSceneFile, topLevelReferenceInSceneFile, true))
		{//file does not exist
			cout << "The file: " << topLevelSceneFileName << " does not exist in the directory" << endl;
			exit(EXIT_ERROR);
		}
		LDreferenceManipulation.write2DreferenceListCollapsedTo1DtoFile(topLevelSceneFileNameCollapsed, initialReferenceInSceneFile);
		/* method2: why doesnt this work - "invalid dat file for conversion to rgb"
		char* topLevelSceneFileNameCollapsed = "sceneCollapsedForRaytracing.ldr";
		LDreferenceManipulation.write2DreferenceListCollapsedTo1DtoFile(topLevelSceneFileNameCollapsed, firstReferenceInPrintList);
		*/

		delete initialReferenceInSceneFile;
		delete topLevelReferenceInSceneFile;


		unsigned char* rgbMap = new unsigned char[width*height*RGB_NUM];

		LDopengl.setViewPort3Dortho(-100.0, width-100, height-100, -100.0, 1.0, -1.0);	//-100 is used to display the most left semantic network nodes

		//now reparse file
		LDreference* initialReferenceInCollapsedSceneFile = new LDreference();
		LDreference* topLevelReferenceInCollapsedSceneFile = new LDreference(topLevelSceneFileNameCollapsed, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
		if(!LDparser.parseFile(topLevelSceneFileNameCollapsed, initialReferenceInCollapsedSceneFile, topLevelReferenceInCollapsedSceneFile, true))
		{//file does not exist
			cout << "The file: " << topLevelSceneFileNameCollapsed << " does not exist in the directory" << endl;
			exit(EXIT_ERROR);
		}

		LDopengl.drawPrimitivesReferenceListToOpenGLandCreateRGBmapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
		LDopengl.drawPrimitivesReferenceListToOpenGLandCreateRGBmapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
			//due to opengl code bug, need to execute this function twice.

		//delete initialReferenceInCollapsedSceneFile;	//this cannot be deleted because it is still used by glutDisplayFunc
		delete topLevelReferenceInCollapsedSceneFile;

		if(useOutputPPMfile)
		{
			RTpixelMaps.generatePixmapFromRGBmap(outputFileNamePPM, width, height, rgbMap);
		}

		delete rgbMap;

	}
	else
	{
		//must use an external program to view the .ldr file (Eg LDView)
	}

	delete firstReferenceInPrintList;
}

void GIAdrawClass::initiateMaxXAtParticularY()
{
	//now print based upon above lists;
	for(int i=0; i<MAX_GIA_TREE_DEPTH; i++)
	{
		maxXAtAParticularY[i] = 0;
	}
}


bool GIAdrawClass::determineBasicPrintPositionsOfAllNodes(vector<GIAentityNode*>* entityNodesActiveListComplete, bool printType[], LDreference* firstReferenceInPrintList, XMLparserTag* firstTagInSVGFile, int maxNumberSentences)
{
	bool result = true;
	
	this->initiateMaxXAtParticularY();
	int xInitial = DRAW_X_INITIAL_OFFSET;
	int yInitial = DRAW_Y_INITIAL_OFFSET;
	maxXAtAParticularY[yInitial] = xInitial;
	//first pass; determine maxXAtAParticularY	[and use these to centre each row {at a given y} respectively]

	#ifndef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX
	maxNumberSentences = 1;
	#endif
	bool printFromXMLcoordinatesAlreadyDefined = false;
	if((*(entityNodesActiveListComplete->begin()))->printCoordsAlreadyDefined)
	{
		printFromXMLcoordinatesAlreadyDefined = true;
		maxNumberSentences = 1;
	}
	for(int sentenceIndex=GIA_NLP_START_SENTENCE_INDEX; sentenceIndex <= maxNumberSentences; sentenceIndex++)
	{
		for(vector<GIAentityNode*>::iterator entityIter = entityNodesActiveListComplete->begin(); entityIter != entityNodesActiveListComplete->end(); entityIter++)
		{
			//this->initiateMaxXAtParticularY();
			xInitial = maxXAtAParticularY[yInitial];
			//xInitial = sentenceIndex*100;	//compact

			bool thisIsDefinitionAndPreviousNodeWasInstance = false;

			this->initialiseEntityNodeForPrinting((*entityIter), yInitial, xInitial, printType, &firstReferenceInPrintList, &firstTagInSVGFile, sentenceIndex, thisIsDefinitionAndPreviousNodeWasInstance);
		}
	}
	
	return result;
}

bool GIAdrawClass::initialiseEntityConnectionForPrinting(vec* pos1, GIAentityConnection* entityConnection, bool printType[], string connectionName, int entityConnectionColour, LDreference** currentReferenceInPrintList, XMLparserTag** currentTag)
{
	bool result = true;
	GIAentityNode* entityNodeToConnect = entityConnection->entity;


	#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX_ADVANCED
	if((entityNodeToConnect->initialisedForPrinting) && !(entityConnection->initialisedForPrinting) && !(entityNodeToConnect->disabled))
	//added condition 31 August 2013: only print connection if node has been printed
	#else
	if(!(entityNodeToConnect->disabled))
	#endif
	{
		#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX_ADVANCED
		entityConnection->initialisedForPrinting = true;
		#endif

		//may accidentially overwrite adjacent nodes that have already been printed here; be careful...
		vec pos2;
		pos2.x = entityNodeToConnect->printX;
		pos2.y = entityNodeToConnect->printY;
		pos2.z = GIA_DRAW_CONNECTION_Z;

		//connectionName = connectionName + convertIntToString(entityConnection->sentenceIndexTemp);

		this->createReferenceConnectionWithText(pos1, &pos2, entityConnectionColour, currentReferenceInPrintList, currentTag, connectionName, printType);
	}

	return result;
}


bool GIAdrawClass::initialiseEntityNodeForPrinting(GIAentityNode* entityNode, int y, int x, bool printType[], LDreference** currentReferenceInPrintList, XMLparserTag** currentTag, int sentenceIndex, bool thisIsDefinitionAndPreviousNodeWasInstance)
{
	bool result = true;
	
	#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	if((entityNode->sentenceIndexTemp == sentenceIndex) || (entityNode->wasReference) || thisIsDefinitionAndPreviousNodeWasInstance || entityNode->printCoordsAlreadyDefined)	//condition (entityNode->wasReference) added 12 October 2012 1q3b
	#else
	if((entityNode->sentenceIndexTemp == sentenceIndex) || thisIsDefinitionAndPreviousNodeWasInstance || entityNode->printCoordsAlreadyDefined)
	#endif
	{
	#endif
		//if(!(entityNode->initialisedForPrinting) || (entityNode->printY < y))
		if(!(entityNode->initialisedForPrinting) && !(entityNode->disabled))
		{
			entityNode->initialisedForPrinting = true;

			maxXAtAParticularY[y] = maxXAtAParticularY[y] + GIA_DRAW_X_SPACE_BETWEEN_ENTITIES;	//only used, for indepdendent network visualisation (eg, when rendering next sentence)

			if(entityNode->printCoordsAlreadyDefined)
			{
				x = entityNode->printX;
				y = entityNode->printY;
			}
			else
			{
				entityNode->printX = x;
				entityNode->printY = y;
			}

			int q, r;

			vec pos1;

			pos1.x = entityNode->printX;
			pos1.y = entityNode->printY;
			pos1.z = GIA_DRAW_CONNECTION_Z;


			int entityDefinitionConnectionColour = GIA_DRAW_CONNECTION_DEFINING_INSTANCE_COLOUR;

			for(int connectionType=0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
			{
				int q = entityVectorConnectionDrawPosYinitialArray[connectionType];
				int r = entityVectorConnectionDrawPosXinitialArray[connectionType];
				int qSpacing = entityVectorConnectionDrawPosYspacingArray[connectionType];
				
				if(GIAentityNodeClass.entityIsRelationship(entityNode))
				{
					if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT)
					{
						q = -relationshipEntityVectorConnectionDrawPosYinitialArray[GIAentityNodeClass.getRelationshipEntityRelativeTypeIndex(entityNode)];
						r = -relationshipEntityVectorConnectionDrawPosXinitialArray[GIAentityNodeClass.getRelationshipEntityRelativeTypeIndex(entityNode)];
						qSpacing = -relationshipEntityVectorConnectionDrawPosYspacingArray[GIAentityNodeClass.getRelationshipEntityRelativeTypeIndex(entityNode)];
					}
					else if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT)
					{
						q = relationshipEntityVectorConnectionDrawPosYinitialArray[GIAentityNodeClass.getRelationshipEntityRelativeTypeIndex(entityNode)];
						r = relationshipEntityVectorConnectionDrawPosXinitialArray[GIAentityNodeClass.getRelationshipEntityRelativeTypeIndex(entityNode)];
						qSpacing = relationshipEntityVectorConnectionDrawPosYspacingArray[GIAentityNodeClass.getRelationshipEntityRelativeTypeIndex(entityNode)];
					}
				}

				for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entityNode->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
				{
					GIAentityConnection* connection = *connectionIter;
					
					bool thisIsDefinitionAndPreviousNodeWasInstance = false;
					#ifdef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
					if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE)
					{
						thisIsDefinitionAndPreviousNodeWasInstance = true;
					}
					#endif
					this->initialiseEntityNodeForPrinting(connection->entity, y+q, x+r, printType, currentReferenceInPrintList, currentTag, sentenceIndex, thisIsDefinitionAndPreviousNodeWasInstance);

					bool pass = true;
					int entityConnectionColour = entityVectorConnectionDrawColourNameArray[connectionType];

					/*
					//this shouldn't be necessary:
					#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX_ADVANCED
					if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE)
					{
						if(GIAentityNodeClass.entityIsRelationship(connection->entity))
						{
							pass = false;
						}
					}	
					#endif				
					*/
					
					string connectionName = entityVectorConnectionDrawConnectionNameArray[connectionType];
					#ifdef GIA_DRAW_ONLY_PRINT_CONNECTIONS_IN_ONE_DIRECTION
					if(!entityVectorConnectionDrawPrintConnectionArray[connectionType])
					{
						//pass = false;	//not compatible with GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX
						
						int connectionTypeGenerated;
						if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT)
						{
							connectionTypeGenerated = GIAtranslatorOperations.generateConnectionType(entityNode->entityType);
						}
						else if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT)
						{
							connectionTypeGenerated = GIAtranslatorOperations.generateConnectionTypeReverse(entityNode->entityType);
						}
						else
						{
							connectionTypeGenerated = inverseVectorConnectionsArray[connectionType];	
						}
						
						connectionName = entityVectorConnectionDrawConnectionNameArray[connectionTypeGenerated];
						
					}
					#endif
					#ifdef GIA_DRAW_PRINT_CONNECTION_SENTENCE_INDEX
					connectionName = string("s") + SHAREDvars.convertIntToString(connection->sentenceIndexTemp) + connectionName;
					#endif
					
					#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
					if(GIAtranslatorOperations.connectionIsAlias(connection))
					{
						entityConnectionColour = GIA_DRAW_DEFINITION_MARK_AS_ALIAS_CONNECTION_COLOUR;
					}
					#endif
					if(pass)
					{
						if(entityVectorConnectionDrawConnectionArray[connectionType])
						{
							#ifdef GIA_DRAW_VERBOSE_HIGHLIGHT_REFERENCE_SET_CONNECTIONS_WITH_COLOURS
							if(connection->sameReferenceSet)
							{
								entityConnectionColour = DAT_FILE_COLOUR_GREEN;
							}
							else
							{
								entityConnectionColour = DAT_FILE_COLOUR_RED;
							}
							#endif
							
							this->initialiseEntityConnectionForPrinting(&pos1, connection, printType, connectionName, entityConnectionColour, currentReferenceInPrintList, currentTag);
						}
					}

					q = q + qSpacing;
				}
			}

			q = GIA_DRAW_Y_SPACE_BETWEEN_CONDITION_NODES;
			r = GIA_DRAW_X_SPACE_BETWEEN_CONDITION_NODES;
			if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)
			{
				int timeConditionNodePrintX = x+r;
				int timeConditionNodePrintY = y+q;
				this->initialiseTimeConditionNodeForPrinting(entityNode->timeConditionNode, timeConditionNodePrintY, timeConditionNodePrintX, printType, currentReferenceInPrintList, currentTag);

				q = q+GIA_DRAW_Y_SPACE_BETWEEN_CONDITIONS_OF_SAME_NODE;

				//may accidentially overwrite adjacent nodes that have already been printed here; be careful...
				vec pos2;
				pos2.x = timeConditionNodePrintX;
				pos2.y = timeConditionNodePrintY;
				pos2.z = GIA_DRAW_CONNECTION_Z;
				this->createReferenceConnectionWithText(&pos1, &pos2, GIA_DRAW_CONDITION_TIME_CONNECTION_COLOUR, currentReferenceInPrintList, currentTag, "time", printType);

			}


			if(printType[GIA_DRAW_CREATE_LDR_OR_SVG_REFERENCES] == true)
			{
				//may accidentially overwrite adjacent nodes that have already been printed here; be careful...

				int boxThickness = GIA_DRAW_THICKNESS_NORMAL;

				int entityColour;
				entityColour = entityNodeTypeColourArray[entityNode->entityType];
				
				//special entity cases;
				if(entityNode->isQuery)
				{
					entityColour = GIA_DRAW_QUERY_QUESTION_NODE_COLOUR;
				}
				else if(entityNode->isAnswerToQuery)
				{
					entityColour = GIA_DRAW_QUERY_ANSWER_NODE_COLOUR;
				}
				#ifdef GIA_DRAW_DISPLAY_ANSWER_CONTEXTS
				else if(entityNode->queryAnswerContext)
				{
					entityColour = GIA_DRAW_QUERY_ANSWER_CONTEXT_NODE_COLOUR;
				}
				#endif
				else if(entityNode->entityType == GIA_ENTITY_TYPE_SUBSTANCE)
				{
					if(entityNode->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL)
					{
						boxThickness = GIA_DRAW_THICKNESS_THICK;
					}
					if(entityNode->hasMeasure)
					{
						entityColour = GIA_DRAW_SUBSTANCE_MEASURE_NODE_COLOUR;
					}
					else if(entityNode->hasQuantity)
					{
						entityColour = GIA_DRAW_SUBSTANCE_QUANTITY_NODE_COLOUR;
					}
				}
				else if(entityNode->entityType == GIA_ENTITY_TYPE_ACTION)
				{
					if(entityNode->isActionConcept)
					{
						entityColour = GIA_DRAW_ACTION_CONCEPT_NODE_COLOUR;
					}
				}
				/*
				else if(entityNode->hasAssociatedTime)
				{
					entityColour = GIA_DRAW_CONDITION_DEFINITION_TIME_NODE_COLOUR;	//clear identify a time node
				}
				*/

				#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
				if(entityNode->isLogicReferenceEntity)
				{
					entityColour = GIA_DRAW_LOGIC_REFERENCE_ENTITY_COLOUR;
				}
				#endif
					
				//first, print this action node.
				string nameOfBox = "";
				if(entityNode->hasQuantity)
				{
					string quantityNumberStringTemp;
					if(entityNode->isQuery)
					{
						quantityNumberStringTemp = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
					}
					else
					{
						quantityNumberStringTemp = GIAentityNodeClass.printQuantityNumberString(entityNode);
					}
					nameOfBox = nameOfBox + quantityNumberStringTemp + " " + entityNode->entityName;

				}
				else if(entityNode->negative)
				{
					nameOfBox = nameOfBox + "!" + entityNode->entityName;
				}
				else
				{
					nameOfBox = entityNode->entityName;
				}
				
				#ifdef GIA_DRAW_PRINT_ENTITY_INSTANCE_ID
				//nameOfBox = SHAREDvars.convertIntToString(entityNode->referenceSetID);
				nameOfBox = nameOfBox + SHAREDvars.convertIntToString(entityNode->idInstance);
				#endif
				#ifdef GIA_DRAW_PRINT_ENTITY_ACTIVELIST_ID
				nameOfBox = nameOfBox + SHAREDvars.convertIntToString(entityNode->idActiveList);
				#endif
				#ifdef GIA_DRAW_PRINT_ENTITY_SENTENCE_INDEX
				nameOfBox = string("s") + SHAREDvars.convertIntToString(entityNode->sentenceIndexTemp) + nameOfBox;
				#endif
				#ifdef GIA_DRAW_PRINT_ENTITY_ENTITY_INDEX
				nameOfBox = nameOfBox + SHAREDvars.convertIntToString(entityNode->entityIndexTemp);
				#endif

				//nameOfBox = nameOfBox + SHAREDvars.convertIntToString(entityNode->grammaticalDefiniteTemp);

				this->createBox(&pos1, GIA_DRAW_ACTION_NODE_WIDTH, GIA_DRAW_ACTION_NODE_HEIGHT, entityColour, &nameOfBox, currentReferenceInPrintList, currentTag, boxThickness, printType);
			}



		}
	#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX
	}
	#endif

	return result;

}




bool GIAdrawClass::initialiseTimeConditionNodeForPrinting(GIAtimeConditionNode* timeConditionNode, const int y, const int x, bool printType[], LDreference** currentReferenceInPrintList, XMLparserTag** currentTag)
{
	bool result = true;
	
	int timeConditionNodePrintX = x;
	int timeConditionNodePrintY = y;

	timeConditionNode->printX = timeConditionNodePrintX;
	timeConditionNode->printY = timeConditionNodePrintY;

	vec pos1;
	vec pos2;
	vec pos3;

	pos1.x = timeConditionNodePrintX;
	pos1.y = timeConditionNodePrintY;
	pos1.z = GIA_DRAW_CONNECTION_Z;

	//may accidentially overwrite adjacent nodes/connections that have already been printed here; be careful...


	int boxThickness = GIA_DRAW_THICKNESS_NORMAL;
	if(timeConditionNode->isProgressive)
	{
		boxThickness = GIA_DRAW_THICKNESS_THICK;
	}
	this->createBox(&pos1, GIA_DRAW_CONDITION_NODE_WIDTH, GIA_DRAW_CONDITION_NODE_HEIGHT, GIA_DRAW_CONDITION_TIME_NODE_COLOUR, &(timeConditionNode->conditionName), currentReferenceInPrintList, currentTag, boxThickness, printType);

	/*
	int timeConditionNodeColour = GIA_DRAW_CONDITION_TIME_NODE_COLOUR;
	if(timeConditionNode->isProgressive)
	{
		timeConditionNodeColour = GIA_DRAW_CONDITION_TIME_STATE_NODE_COLOUR;
	}
	this->createBox(currentReferenceInPrintList, &pos1, GIA_DRAW_CONDITION_NODE_WIDTH, GIA_DRAW_CONDITION_NODE_HEIGHT, timeConditionNodeColour, &(timeConditionNode->conditionName), currentReferenceInPrintList, currentTag, GIA_DRAW_THICKNESS_NORMAL, printType);
	*/


	return result;
}


bool GIAdrawClass::createReferenceConnectionWithText(vec* pos1, vec* pos2, int colour, LDreference** currentReferenceInPrintList, XMLparserTag** currentTag, string connectionTypeName, bool printType[])
{
	bool result = true;
	
	this->createReferenceConnection(pos1, pos2, colour, currentReferenceInPrintList, currentTag, printType);

	if(GIA_DRAW_USE_CONNECTION_TYPE_NAME_TEXT)
	{
		vec vect;
		vect.x = (pos1->x + pos2->x)/2;
		vect.y = (pos1->y + pos2->y)/2;
		vect.z = (pos1->z + pos2->z)/2;

		if(printType[GIA_DRAW_CREATE_LDR_REFERENCES] == true)
		{
			int numSpritesAdded;	//not used
			vec positionLDR;
			positionLDR.x = vect.x - GIA_DRAW_BASICENTITY_NODE_WIDTH/4;
			positionLDR.y = vect.y - GIA_DRAW_BASICENTITY_NODE_HEIGHT/4;
			positionLDR.z = vect.z - GIA_OUTPUT_Z_POSITION_CONNECTIONS;
			*currentReferenceInPrintList = LDsprite.LDaddBasicTextualSpriteStringToReferenceList(connectionTypeName, *currentReferenceInPrintList, &positionLDR, &numSpritesAdded, false, DAT_FILE_COLOUR_BLACK, 0.3);	//add sprite text within box
		}
		if(printType[GIA_DRAW_CREATE_SVG_REFERENCES] == true)
		{
			vec positionSVG;
			positionSVG.x = vect.x - GIA_DRAW_BASICENTITY_NODE_WIDTH/3;
			positionSVG.y = vect.y - GIA_DRAW_BASICENTITY_NODE_HEIGHT/4;
			positionSVG.z = GIA_OUTPUT_Z_POSITION_CONNECTIONS;
			LDsvg.writeSVGtext(currentTag, connectionTypeName, &positionSVG, GIA_SVG_SCALE_FACTOR*GIA_SVG_TEXT_SCALE_FACTOR, DAT_FILE_COLOUR_BLACK);
		}
	}

	return result;
}

bool GIAdrawClass::createReferenceConnection(vec* pos1, vec* pos2, int colour, LDreference** currentReferenceInPrintList, XMLparserTag** currentTag, const bool printType[])
{
	bool result = true;
	
	if(printType[GIA_DRAW_CREATE_LDR_REFERENCES] == true)
	{

		(*currentReferenceInPrintList)->type = REFERENCE_TYPE_LINE;
		(*currentReferenceInPrintList)->colour = colour;

		(*currentReferenceInPrintList)->vertex1relativePosition.x = pos1->x;
		(*currentReferenceInPrintList)->vertex1relativePosition.y = pos1->y;
		(*currentReferenceInPrintList)->vertex1relativePosition.z = pos1->z;


		(*currentReferenceInPrintList)->vertex2relativePosition.x = pos2->x;
		(*currentReferenceInPrintList)->vertex2relativePosition.y = pos2->y;
		(*currentReferenceInPrintList)->vertex2relativePosition.z = pos2->z;


		LDreference* newDispayReference = new LDreference();
		(*currentReferenceInPrintList)->next = newDispayReference;
		(*currentReferenceInPrintList) = (*currentReferenceInPrintList)->next;
	}

	if(printType[GIA_DRAW_CREATE_SVG_REFERENCES] == true)
	{
		pos1->z = GIA_OUTPUT_Z_POSITION_CONNECTIONS;
		pos2->z = GIA_OUTPUT_Z_POSITION_CONNECTIONS;
		LDsvg.writeSVGline(currentTag, pos1, pos2, colour);
	}

	return result;
}



//consider using elipse instead; <ellipse cx="240" cy="100" rx="220" ry="30">

bool GIAdrawClass::createBox(vec* vect, const double width, const double height, int colour, string* text, LDreference** currentReferenceInPrintList, XMLparserTag** currentTag, const int thickness, const bool printType[])
{
	bool result = true;
	
	if(printType[GIA_DRAW_CREATE_LDR_REFERENCES] == true)
	{

		(*currentReferenceInPrintList)->type = REFERENCE_TYPE_QUAD;
		(*currentReferenceInPrintList)->colour = colour;

		(*currentReferenceInPrintList)->vertex1relativePosition.x = vect->x - width/2.0;
		(*currentReferenceInPrintList)->vertex1relativePosition.y = vect->y + height/2.0;
		(*currentReferenceInPrintList)->vertex1relativePosition.z = vect->z;

		(*currentReferenceInPrintList)->vertex2relativePosition.x = vect->x + width/2.0;
		(*currentReferenceInPrintList)->vertex2relativePosition.y = vect->y + height/2.0;
		(*currentReferenceInPrintList)->vertex2relativePosition.z = vect->z;

		(*currentReferenceInPrintList)->vertex3relativePosition.x = vect->x + width/2.0;
		(*currentReferenceInPrintList)->vertex3relativePosition.y = vect->y - height/2.0;
		(*currentReferenceInPrintList)->vertex3relativePosition.z = vect->z;

		(*currentReferenceInPrintList)->vertex4relativePosition.x = vect->x - width/2.0;
		(*currentReferenceInPrintList)->vertex4relativePosition.y = vect->y - height/2.0;
		(*currentReferenceInPrintList)->vertex4relativePosition.z = vect->z;


		LDreference* newDispayReference;

		newDispayReference = new LDreference();
		(*currentReferenceInPrintList)->next = newDispayReference;
		(*currentReferenceInPrintList) = (*currentReferenceInPrintList)->next;

		(*currentReferenceInPrintList)->type = REFERENCE_TYPE_LINE;
		(*currentReferenceInPrintList)->colour = DAT_FILE_COLOUR_BLACK;

		(*currentReferenceInPrintList)->vertex1relativePosition.x = vect->x - width/2.0;
		(*currentReferenceInPrintList)->vertex1relativePosition.y = vect->y + height/2.0;
		(*currentReferenceInPrintList)->vertex1relativePosition.z = vect->z;

		(*currentReferenceInPrintList)->vertex2relativePosition.x = vect->x + width/2.0;
		(*currentReferenceInPrintList)->vertex2relativePosition.y = vect->y + height/2.0;
		(*currentReferenceInPrintList)->vertex2relativePosition.z = vect->z;

		newDispayReference = new LDreference();
		(*currentReferenceInPrintList)->next = newDispayReference;
		(*currentReferenceInPrintList) = (*currentReferenceInPrintList)->next;

		(*currentReferenceInPrintList)->type = REFERENCE_TYPE_LINE;
		(*currentReferenceInPrintList)->colour = DAT_FILE_COLOUR_BLACK;

		(*currentReferenceInPrintList)->vertex1relativePosition.x = vect->x + width/2.0;
		(*currentReferenceInPrintList)->vertex1relativePosition.y = vect->y + height/2.0;
		(*currentReferenceInPrintList)->vertex1relativePosition.z = vect->z;

		(*currentReferenceInPrintList)->vertex2relativePosition.x = vect->x + width/2.0;
		(*currentReferenceInPrintList)->vertex2relativePosition.y = vect->y - height/2.0;
		(*currentReferenceInPrintList)->vertex2relativePosition.z = vect->z;

		newDispayReference = new LDreference();
		(*currentReferenceInPrintList)->next = newDispayReference;
		(*currentReferenceInPrintList) = (*currentReferenceInPrintList)->next;

		(*currentReferenceInPrintList)->type = REFERENCE_TYPE_LINE;
		(*currentReferenceInPrintList)->colour = DAT_FILE_COLOUR_BLACK;

		(*currentReferenceInPrintList)->vertex1relativePosition.x = vect->x + width/2.0;
		(*currentReferenceInPrintList)->vertex1relativePosition.y = vect->y - height/2.0;
		(*currentReferenceInPrintList)->vertex1relativePosition.z = vect->z;

		(*currentReferenceInPrintList)->vertex2relativePosition.x = vect->x - width/2.0;
		(*currentReferenceInPrintList)->vertex2relativePosition.y = vect->y - height/2.0;
		(*currentReferenceInPrintList)->vertex2relativePosition.z = vect->z;

		newDispayReference = new LDreference();
		(*currentReferenceInPrintList)->next = newDispayReference;
		(*currentReferenceInPrintList) = (*currentReferenceInPrintList)->next;

		(*currentReferenceInPrintList)->type = REFERENCE_TYPE_LINE;
		(*currentReferenceInPrintList)->colour = DAT_FILE_COLOUR_BLACK;

		(*currentReferenceInPrintList)->vertex1relativePosition.x = vect->x - width/2.0;
		(*currentReferenceInPrintList)->vertex1relativePosition.y = vect->y + height/2.0;
		(*currentReferenceInPrintList)->vertex1relativePosition.z = vect->z;

		(*currentReferenceInPrintList)->vertex2relativePosition.x = vect->x - width/2.0;
		(*currentReferenceInPrintList)->vertex2relativePosition.y = vect->y - height/2.0;
		(*currentReferenceInPrintList)->vertex2relativePosition.z = vect->z;


		newDispayReference = new LDreference();
		(*currentReferenceInPrintList)->next = newDispayReference;
		(*currentReferenceInPrintList) = (*currentReferenceInPrintList)->next;

		int numSpritesAdded;	//not used
		vec positionLDR;
		positionLDR.x = vect->x - GIA_DRAW_BASICENTITY_NODE_WIDTH/4;
		positionLDR.y = vect->y - GIA_DRAW_BASICENTITY_NODE_HEIGHT/4;
		positionLDR.z = vect->z - GIA_OUTPUT_Z_POSITION_NODES;
		(*currentReferenceInPrintList) = LDsprite.LDaddBasicTextualSpriteStringToReferenceList(*text, (*currentReferenceInPrintList), &positionLDR, &numSpritesAdded, false, DAT_FILE_COLOUR_BLACK, 0.3);	//add sprite text within box
	}

	if(printType[GIA_DRAW_CREATE_SVG_REFERENCES] == true)
	{
		vec positionSVG;
		positionSVG.x = vect->x + GIA_DRAW_BASICENTITY_NODE_WIDTH/2;
		positionSVG.y = vect->y;
		positionSVG.z = GIA_OUTPUT_Z_POSITION_NODES;
		LDsvg.writeSVGbox(currentTag, &positionSVG, width, height, colour, GIA_FILE_TEXT_BOX_OUTLINE_WIDTH_SVG*thickness, GIA_SVG_ELLIPTICAL_BOXES);
		positionSVG.x = vect->x - GIA_DRAW_BASICENTITY_NODE_WIDTH/3;
		positionSVG.y = vect->y - GIA_DRAW_BASICENTITY_NODE_HEIGHT/4;
		positionSVG.z = GIA_OUTPUT_Z_POSITION_TEXT;
		LDsvg.writeSVGtext(currentTag,* text, &positionSVG, GIA_SVG_SCALE_FACTOR*GIA_SVG_TEXT_SCALE_FACTOR, DAT_FILE_COLOUR_BLACK);
	}

	return result;
}








