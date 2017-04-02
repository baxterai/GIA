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
 * File Name: GIApreprocessorLogicReference.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Natural Language Compiler (Programming Interface)
 * Project Version: 3a5d 28-March-2017
 * Requirements: requires plain text file
 * Description: Logic Reference preprocessor
 *
 *******************************************************************************/


#include "GIApreprocessorLogicReference.hpp"
#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET
#include "GIApreprocessorReferenceSet.hpp"
#endif
#include "GIAtranslatorDefs.hpp"
#include "XMLrulesClass.hpp"

#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE

bool GIApreprocessorLogicReferenceClass::extractGIApreprocessorLogicReferenceClasses(XMLparserTag** firstLogicReferenceClassTag, vector<string>* logicReferenceInstanceList)
{
	bool result = true;
	XMLparserTag* firstTagInRulesTag = XMLparserClass.parseTagDownALevel(GIAfirstTagInXMLfile, RULES_XML_TAG_rules, &result);
	if(result)
	{
		XMLparserTag* currentTag = firstTagInRulesTag;
		bool foundPreprocessorTag = false;
		XMLparserTag* firstTagInPreprocessorTag = NULL;
		while((currentTag->nextTag != NULL) && !foundPreprocessorTag)
		{
			if(currentTag->name == GIA_PREPROCESSOR_XML_TAG_preprocessor)
			{
				foundPreprocessorTag = true;
				firstTagInPreprocessorTag = XMLparserClass.parseTagDownALevel(currentTag, GIA_PREPROCESSOR_XML_TAG_preprocessor, &result);
			}
			currentTag = currentTag->nextTag;
		}
		if(foundPreprocessorTag)
		{
			
			XMLparserTag* currentTagInPreprocessor = firstTagInPreprocessorTag;
			bool foundLogicReferencePreprocessorTag = false;
			XMLparserTag* firstTagInLogicReferencePreprocessorTag = NULL;
			while((currentTagInPreprocessor->nextTag != NULL) && !foundLogicReferencePreprocessorTag)
			{
				if(currentTagInPreprocessor->name == GIA_PREPROCESSOR_XML_TAG_logicReference)
				{
					foundLogicReferencePreprocessorTag = true;
					firstTagInLogicReferencePreprocessorTag = XMLparserClass.parseTagDownALevel(currentTagInPreprocessor, GIA_PREPROCESSOR_XML_TAG_logicReference, &result);
					*firstLogicReferenceClassTag = firstTagInLogicReferencePreprocessorTag;
				}
				currentTagInPreprocessor = currentTagInPreprocessor->nextTag;
			}
			
			//error checking only:
			if(foundLogicReferencePreprocessorTag)
			{
				
				XMLparserTag* currentTagInLogicReferencePreprocessor = firstTagInLogicReferencePreprocessorTag;
				while(currentTagInLogicReferencePreprocessor->nextTag != NULL)
				{
					if(currentTagInLogicReferencePreprocessor->name == GIA_PREPROCESSOR_XML_TAG_class)
					{	
						
						bool foundClass = true;
						int logicReferenceClass = GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED;
						if(currentTagInLogicReferencePreprocessor->firstAttribute->name == GIA_PREPROCESSOR_XML_ATTRIBUTE_name)
						{
							string className = currentTagInLogicReferencePreprocessor->firstAttribute->value;
							if(className == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_undefined)
							{
								logicReferenceClass = GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED;
							}
							else if(className == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_conjunction)
							{
								logicReferenceClass = GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION;
							}
							else if(className == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_verb)
							{
								logicReferenceClass = GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_VERB;
							}
							else if(className == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_preposition)
							{
								logicReferenceClass = GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION;
							}
							else
							{
								cout << "GIApreprocessor::extractGIApreprocessorLogicReferenceClasses{} error: illegal class name = currentTagInLogicReferencePreprocessor->firstAttribute->value = " << currentTagInLogicReferencePreprocessor->firstAttribute->value << endl;
								exit(EXIT_ERROR);
							}
						}
						else
						{
							cout << "GIApreprocessor::extractGIApreprocessorLogicReferenceClasses{} error: illegal class name = currentTagInLogicReferencePreprocessor->firstAttribute->name = " << currentTagInLogicReferencePreprocessor->firstAttribute->name << ", expected: GIA_PREPROCESSOR_XML_ATTRIBUTE_name" << endl;
							exit(EXIT_ERROR);
						}
						
						if(foundClass)
						{
							XMLparserTag* firstTagInClassTag = XMLparserClass.parseTagDownALevel(currentTagInLogicReferencePreprocessor, GIA_PREPROCESSOR_XML_TAG_class, &result);	
							XMLparserTag* currentTagInClass = firstTagInClassTag;
							while(currentTagInClass->nextTag != NULL)
							{
								if(currentTagInClass->name == GIA_PREPROCESSOR_XML_TAG_type)
								{
									
									if(currentTagInClass->firstAttribute->name == GIA_PREPROCESSOR_XML_ATTRIBUTE_name)
									{

									}
									else
									{
										cout << "GIApreprocessor::extractGIApreprocessorLogicReferenceClasses{} error: illegal type name = currentTagInClass->firstAttribute->name = " << currentTagInClass->firstAttribute->name << ", expected: GIA_PREPROCESSOR_XML_ATTRIBUTE_name" << endl;
										exit(EXIT_ERROR);
									}

									XMLparserTag* firstTagInTypeTag = XMLparserClass.parseTagDownALevel(currentTagInClass, GIA_PREPROCESSOR_XML_TAG_type, &result);
									XMLparserTag* currentTagInType = firstTagInTypeTag;
									while(currentTagInType->nextTag != NULL)
									{
										if(currentTagInType->name == GIA_PREPROCESSOR_XML_TAG_instance)
										{
											
											if(currentTagInType->firstAttribute->name == GIA_PREPROCESSOR_XML_ATTRIBUTE_name)
											{
												string instanceName = currentTagInType->firstAttribute->value;
												logicReferenceInstanceList->push_back(instanceName);
											}
											else
											{
												cout << "GIApreprocessor::extractGIApreprocessorLogicReferenceClasses{} error: illegal instance name = currentTagInType->firstAttribute->name = " << currentTagInType->firstAttribute->name << ", expected: GIA_PREPROCESSOR_XML_ATTRIBUTE_name" << endl;
												exit(EXIT_ERROR);
											}
										}
										else
										{
											cout << "GIApreprocessor::extractGIApreprocessorLogicReferenceClasses{} error: illegal currentTagInType->name = " << currentTagInType->name << ", expected: GIA_PREPROCESSOR_XML_TAG_instance" << endl;
											exit(EXIT_ERROR);
										}
										currentTagInType = currentTagInType->nextTag;
									}
								}
								else
								{
									cout << "GIApreprocessor::extractGIApreprocessorLogicReferenceClasses{} error: illegal currentTagInClass->name = " << currentTagInClass->name << ", expected: GIA_PREPROCESSOR_XML_TAG_type" << endl;
									exit(EXIT_ERROR);
								}
								currentTagInClass = currentTagInClass->nextTag;
							}
						}
					}
					else
					{
						cout << "GIApreprocessor::extractGIApreprocessorLogicReferenceClasses{} error: illegal currentTagInLogicReferencePreprocessor->name = " << currentTagInLogicReferencePreprocessor->name << ", expected: GIA_PREPROCESSOR_XML_TAG_class" << endl;
						exit(EXIT_ERROR);
					}
					currentTagInLogicReferencePreprocessor = currentTagInLogicReferencePreprocessor->nextTag;
				}
			}
		}
	}
	return result;
}

//preconditions: must conform to wordIndex identification in GIApreprocessorMultiwordReductionClass::loadPlainTextFile (which is the same as that used in NLP)
bool GIApreprocessorLogicReferenceClass::executeLogicReferencePreprocessor(const string* sentenceContents, GIApreprocessorSentence* currentGIApreprocessorSentenceInList, XMLparserTag* firstLogicReferenceClassTag)
{
	bool result = true;
	
	//OLD:  Separate out 1. ,/and/or (including doactionA and doactionB),

	int wordIndexSentence = 0;
	string currentWord = "";
	string currentContents = "";
	int logicReferenceVariableNameIndex = 0;
	vector<string> logicReferenceVariableWordList;
	bool expectToFindSubjectAuxObjectLogicReferenceVariable = false;
	bool whiteSpace = false;
	
	GIApreprocessorLogicReference* currentLogicReferenceInList = currentGIApreprocessorSentenceInList->firstLogicReferenceInList;
	
	for(int i=0; i<sentenceContents->length(); i++)
	{
		char currentChar = (*sentenceContents)[i];
		cout << currentChar;
		
		bool whiteSpaceFound = false;
		bool punctuationMarkFound = false;
		if(SHAREDvars.charInCharArray(currentChar, nlpWhitespaceCharacterArray, GIA_NLP_NUMBER_OF_WHITESPACE_CHARACTERS))
		{
			whiteSpaceFound = true;
			if(!whiteSpace)
			{
				cout << "1 [currentWord = " << currentWord << "]" << endl;
				bool foundClassType = false;
				string logicReferenceClassType = "";
				int logicReferenceClass = GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED;
				if(isClassTag(currentWord, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_conjunction, &logicReferenceClassType, firstLogicReferenceClassTag))
				{
					logicReferenceClass = GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION;
					cout << "logicReferenceClass = GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION " << endl;
					foundClassType = true;
				}
				else if(isClassTag(currentWord, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_verb, &logicReferenceClassType, firstLogicReferenceClassTag))
				{
					logicReferenceClass = GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_VERB;
					cout << "logicReferenceClass = GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_VERB " << endl;
					foundClassType = true;
				}
				else if(isClassTag(currentWord, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_preposition, &logicReferenceClassType, firstLogicReferenceClassTag))
				{
					logicReferenceClass = GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION;
					cout << "logicReferenceClass = GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION " << endl;
					foundClassType = true;
				}

				if(foundClassType)
				{
					if(logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION)
					{
						if(currentContents == "")
						{
							if(currentLogicReferenceInList->logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED)
							{
								//x, and
								//eg [The house is blue], and the chicken is happy.
							}
							else
							{
								#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_ABSTRACT_CONJUNCTIONS
								//eg subsequent_to and before (FUTURE: and -> subsequent_to + before)
								if(!fillLogicReferenceVariableAndAddLogicReferenceToList(&currentLogicReferenceInList, &currentContents, &logicReferenceVariableWordList, &logicReferenceVariableNameIndex, false, &expectToFindSubjectAuxObjectLogicReferenceVariable))
								{
									result = false;
								}
								#else
								cout << "GIApreprocessor::executeLogicReferencePreprocessor{} error: GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_ABSTRACT_CONJUNCTIONS not supported; eg subsequent_to and before" << endl;	
								exit(EXIT_ERROR);
								#endif
							}	
						}
						else
						{
							//x and
							//eg [The house is blue] and the chicken is happy.
							//?eg [The house is blue, red] and green.
							logicReferenceVariableWordList.push_back(currentWord);
							currentContents = currentContents + currentWord + currentChar;
							if(!fillLogicReferenceVariableAndAddLogicReferenceToList(&currentLogicReferenceInList, &currentContents, &logicReferenceVariableWordList, &logicReferenceVariableNameIndex, false, &expectToFindSubjectAuxObjectLogicReferenceVariable))
							{
								result = false;
							}
						}
						string logicReferenceContents = generateLogicReferenceContents(logicReferenceClassType);
						if(!initialiseNewLogicReference(currentLogicReferenceInList, logicReferenceContents, logicReferenceClass, logicReferenceClassType, wordIndexSentence))
						{
							result = false;
						}
					}
					else if(logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_VERB)
					{
						GIApreprocessorLogicReference* currentLogicReferenceInListOriginal = currentLogicReferenceInList;						
						if(currentContents != "")
						{
							if((currentLogicReferenceInList->logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED) || (currentLogicReferenceInList->logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION))
							{
								/*
								//eg The apple proposed [that] the house [is blue].
								//FUTURE CONSIDER eg The house is blue, the apple proposed.
								//Tom said A and Jim proposed [that] the house [is blue].
								*/
								if(expectToFindSubjectAuxObjectLogicReferenceVariable != false)
								{
									cout << "GIApreprocessor::extractGIApreprocessorLogicReferenceClasses{} error: expectToFindSubjectAuxObjectLogicReferenceVariable != false" << endl;
									exit(EXIT_ERROR);
								}
								if(!fillLogicReferenceVariableAndAddLogicReferenceToList(&currentLogicReferenceInList, &currentContents, &logicReferenceVariableWordList, &logicReferenceVariableNameIndex, false, &expectToFindSubjectAuxObjectLogicReferenceVariable))
								{
									result = false;
								}
							}
							else
							{
								//eg Subsequent_to proposed that the house is blue.
								cout << "GIApreprocessor::extractGIApreprocessorLogicReferenceClasses{} error: (logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_VERB) && (currentLogicReferenceInList->logicReferenceClass != GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED||GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION)" << endl;	
								exit(EXIT_ERROR);
							}
						}
						else
						{
							if(currentLogicReferenceInList->logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION)
							{
								/*
								//Tom said A and proposed [that] the house [is blue].
								//Tom said A, and proposed [that] the house [is blue].
								*/
							}
							else
							{	
								//eg The house is blue, suggested the apple.
								cout << "GIApreprocessor::extractGIApreprocessorLogicReferenceClasses{} error: (logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_VERB) && (currentContents == \"\")" << endl;	
								exit(EXIT_ERROR);
							}
						}
							
								
						string logicReferenceContents = generateLogicReferenceContents(logicReferenceClassType);
						if(sentenceContents->find(GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_SKIP_APPENDED_THAT_NAME, i+1) == i+1)
						{
							logicReferenceContents = logicReferenceContents + GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_SKIP_APPENDED_THAT_NAME;
							i = i + string(GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_SKIP_APPENDED_THAT_NAME).length();	//skip to end of that
							wordIndexSentence++;
							expectToFindSubjectAuxObjectLogicReferenceVariable = true;
						}
						else
						{
							expectToFindSubjectAuxObjectLogicReferenceVariable = false;	//redundant?
						}

						#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_RECURSION
						//create a new sub logical condition
						currentLogicReferenceInList = currentLogicReferenceInListOriginal;
						if(!initialiseNewSubLogicReference(&currentLogicReferenceInList, logicReferenceContents, logicReferenceClass, logicReferenceClassType, wordIndexSentence))
						{
							result = false;
						}
						#else
						if(!initialiseNewLogicReference(currentLogicReferenceInListOriginal, logicReferenceContents, logicReferenceClass, logicReferenceClassType, wordIndexSentence))
						{
							result = false;
						}
						#endif
					}
					else if(logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION)
					{
						if(currentLogicReferenceInList->logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED) 
						{
							if(currentContents != "")
							{
								/*
								//eg The house is blue, considering the pie.
								//eg Considering the pie, the house is blue.
								//eg The house is blue, considering that the pie is happy.
								//eg Considering that the pie is happy, the house is blue.
								*/

								string logicReferenceContents = generateLogicReferenceContents(logicReferenceClassType);
								#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_SUPPORT_PREPOSITIONS_WITH_THAT
								if(sentenceContents->find(GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_SKIP_APPENDED_THAT_NAME, i+1) == i+1)
								{
									logicReferenceContents = logicReferenceContents + GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_SKIP_APPENDED_THAT_NAME;
									i = i + string(GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_SKIP_APPENDED_THAT_NAME).length();	//skip to end of that
									wordIndexSentence++;
									expectToFindSubjectAuxObjectLogicReferenceVariable = true;
								}
								else
								{
									expectToFindSubjectAuxObjectLogicReferenceVariable = false;	//redundant?
								}
								#endif

								if(!initialiseNewLogicReference(currentLogicReferenceInList, logicReferenceContents, logicReferenceClass, logicReferenceClassType, wordIndexSentence))
								{
									result = false;
								}
							}
							else
							{
								//eg the house is blue considering the pie.
								cout << "GIApreprocessor::extractGIApreprocessorLogicReferenceClasses{} error: (logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION) && (currentContents != "") " << endl;	
								exit(EXIT_ERROR);
							}
							
						}
						else
						{
							//eg the house is blue considering the pie.
							cout << "GIApreprocessor::extractGIApreprocessorLogicReferenceClasses{} error: (logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION) && (currentLogicReferenceInList->logicReferenceClass != GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED) " << endl;	
							exit(EXIT_ERROR);
						}
					}
				}
				else
				{
					logicReferenceVariableWordList.push_back(currentWord);
					currentContents = currentContents + currentWord + currentChar;
				}
				currentWord = "";
				wordIndexSentence++;
			}
			else
			{
				//ignore consecutive white space
				//skip (do not parse) multiple white space/punctuation characters (eg ". "/".."/"  "/" .")
				#ifndef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_REMOVE_DOUBLE_WHITE_SPACE
				currentWord = currentWord + currentChar;
				#endif
			}
		}
		else if(currentChar == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_NEW_LOGIC_REFERENCE_CHAR)
		{	
			logicReferenceVariableWordList.push_back(currentWord);
			currentContents = currentContents + currentWord;
			currentWord = "";
			wordIndexSentence++;		
			if(!fillLogicReferenceVariableAndAddLogicReferenceToList(&currentLogicReferenceInList, &currentContents, &logicReferenceVariableWordList, &logicReferenceVariableNameIndex, true, &expectToFindSubjectAuxObjectLogicReferenceVariable))
			{
				result = false;
			}
		}
		else if(SHAREDvars.charInCharArray(currentChar, nlpPunctionMarkCharacterEndOfSentenceArray, GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS_END_OF_SENTENCE))
		{
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_FILENAMES_WITH_FULLSTOPS_AND_FLOATS_AND_TIMES
			if(GIApreprocessorMultiwordReduction.isIntrawordPunctuationMark(i, sentenceContents))
			{
				currentWord = currentWord + currentChar;
			}
			else
			{
			#endif
				cout << "2 [currentWord = " << currentWord << "]" << endl;
				if(currentContents == "")
				{
					cout << "GIApreprocessor::extractGIApreprocessorLogicReferenceClasses{} error: GIApreprocessorMultiwordReduction.isIntrawordPunctuationMark(currentChar, &sentenceContents) && (currentContents == "")" << endl;
					exit(EXIT_ERROR);	
				}
				else
				{
					logicReferenceVariableWordList.push_back(currentWord);
					currentContents = currentContents + currentWord;
					currentWord = "";	//not required because finished parsing sentence
					wordIndexSentence++;	//not required because finished parsing sentence
					punctuationMarkFound = true;	//not required because finished parsing sentence;
				}

				if(!fillLogicReferenceVariableAndAddLogicReferenceToList(&currentLogicReferenceInList, &currentContents, &logicReferenceVariableWordList, &logicReferenceVariableNameIndex, false, &expectToFindSubjectAuxObjectLogicReferenceVariable))
				{
					result = false;
				}

			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_FILENAMES_WITH_FULLSTOPS_AND_FLOATS_AND_TIMES
			}
			#endif
		}
		else if(SHAREDvars.charInCharArray(currentChar, nlpPunctionMarkCharacterArray, GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS))
		{
			//CHAR_SEMICOLON / CHAR_COLON:

			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_FILENAMES_WITH_FULLSTOPS_AND_FLOATS_AND_TIMES
			if(GIApreprocessorMultiwordReduction.isIntrawordPunctuationMark(i, sentenceContents))
			{
				currentWord = currentWord + currentChar;
			}
			else
			{
			#endif
				logicReferenceVariableWordList.push_back(currentWord);	//CHECKTHIS: assume that logic reference variable will not occur before a punctuation mark (CHAR_SEMICOLON / CHAR_COLON)
				currentContents = currentContents + currentWord + currentChar;	//non-comma punctuation marks are retained within currentContents
				currentWord = "";
				wordIndexSentence++;
				punctuationMarkFound = true;
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_FILENAMES_WITH_FULLSTOPS_AND_FLOATS_AND_TIMES
			}
			#endif	
		}
		else
		{
			currentWord = currentWord + currentChar;
		}

		if(whiteSpaceFound || punctuationMarkFound)
		{
			whiteSpace = true;
		}
		else
		{
			whiteSpace = false;
		}
	}

		
	//FUTURE? always fill in implicit conjunction logic reference sets before implicit preposition logic reference sets	//eg The house is blue and red, considering the pie. 
	if(!identifyImplicitLogicReferenceSets(currentGIApreprocessorSentenceInList->firstLogicReferenceInList))
	{
		result = false;
	}
	
	return result;
}




bool GIApreprocessorLogicReferenceClass::initialiseNewLogicReference(GIApreprocessorLogicReference* currentLogicReferenceInList, const string logicReferenceContents, const int logicReferenceClass, const string logicReferenceClassType, const int wordIndexSentence)
{
	bool result = true;
	
	currentLogicReferenceInList->logicReferenceClass = logicReferenceClass;	
	currentLogicReferenceInList->logicReferenceClassType = logicReferenceClassType;
	currentLogicReferenceInList->logicReferenceContents = logicReferenceContents;		//similiar if not identical to GIApreprocessorLogicReferenceTypes[logicReferenceClassType];
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	currentLogicReferenceInList->logicReferenceVariable->wordIndexSentence = wordIndexSentence+1;
	#endif
	
	return result;
}

#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_RECURSION
bool GIApreprocessorLogicReferenceClass::initialiseNewSubLogicReference(GIApreprocessorLogicReference** currentLogicReferenceInList, const string logicReferenceContents, const int logicReferenceClass, const string logicReferenceClassType, const int wordIndexSentence)
{
	(*currentLogicReferenceInList)->logicReferenceClass = logicReferenceClass;
	(*currentLogicReferenceInList)->logicReferenceClassType = logicReferenceClassType;	
	(*currentLogicReferenceInList)->logicReferenceContents = logicReferenceContents;		//similiar if not identical to GIApreprocessorLogicReferenceTypes[logicReferenceClassType];
	(*currentLogicReferenceInList)->hasSubLogicReference = true;
	(*currentLogicReferenceInList)->firstSubLogicReferenceInList = new GIApreprocessorLogicReference();
	(*currentLogicReferenceInList)->firstSubLogicReferenceInList->isSubLogicReference = true;
	(*currentLogicReferenceInList)->firstSubLogicReferenceInList->lastLogicReferenceInUpperLevel = (*currentLogicReferenceInList);	//so that can return to the current level upon the detection of a comma
	(*currentLogicReferenceInList) = (*currentLogicReferenceInList)->firstSubLogicReferenceInList;
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	(*currentLogicReferenceInList)->logicReferenceVariable->wordIndexSentence = wordIndexSentence+1;
	#endif
}
#endif

bool GIApreprocessorLogicReferenceClass::fillLogicReferenceVariableAndAddLogicReferenceToList(GIApreprocessorLogicReference** currentLogicReferenceInList, string* currentContents, vector<string>* logicReferenceVariableWordList, int* logicReferenceVariableNameIndex, const bool commaDetected, bool* expectToFindSubjectAuxObjectLogicReferenceVariable)
{
	//exit parsing of logical condition variable
	cout << "fillLogicReferenceVariableAndAddLogicReferenceToList{}" << endl;
	cout << "currentContents = " << *currentContents << endl;
	
	bool result = true;
	
	(*currentLogicReferenceInList)->logicReferenceVariable->logicReferenceVariableContents = *currentContents;
	(*currentLogicReferenceInList)->logicReferenceVariable->logicReferenceVariableNameIndex = *logicReferenceVariableNameIndex;
	string logicReferenceVariableName = GIApreprocessorLogicReferenceVariableNames[*logicReferenceVariableNameIndex];
	(*currentLogicReferenceInList)->logicReferenceVariable->logicReferenceVariableName = logicReferenceVariableName;
	*logicReferenceVariableNameIndex = *logicReferenceVariableNameIndex + 1;
	
	#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET
	if(!GIApreprocessorReferenceSet.executeReferenceSetPreprocessor(currentContents, logicReferenceVariableWordList, (*currentLogicReferenceInList)->logicReferenceVariable, *expectToFindSubjectAuxObjectLogicReferenceVariable, (*currentLogicReferenceInList)->logicReferenceVariable->wordIndexSentence))
	{
		result = false;	
	}
	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_SET_CONJUNCTION_AND_OR_TEXT_TO_REFERENCE_SET_OBJECT_BEFORE_SEMANTIC_RELATION_PARSE
	if(islogicReferenceConjunctionAndOr(*currentLogicReferenceInList))
	{
		if(GIApreprocessorReferenceSet.hasReferenceSet((*currentLogicReferenceInList)->logicReferenceVariable->referenceSetSubject))
		{
			if(!GIApreprocessorReferenceSet.hasReferenceSet((*currentLogicReferenceInList)->logicReferenceVariable->referenceSetObject))
			{
				//eg Tom is happy and fast 
				//switch reference set subject/object
				(*currentLogicReferenceInList)->logicReferenceVariable->referenceSetObject = (*currentLogicReferenceInList)->logicReferenceVariable->referenceSetSubject;
				(*currentLogicReferenceInList)->logicReferenceVariable->referenceSetSubject = NULL;
			}
			else
			{
				//eg Tom is happy and Tom is fast / Tom is happy and is fast
			}
		}
		else
		{
			cout << "GIApreprocessor::fillLogicReferenceVariableAndAddLogicReferenceToList{} error: islogicReferenceConjunctionAndOr && isReferenceSetEmpty(referenceSetSubject)" << endl;
			exit(EXIT_ERROR);
		}
	}
	#endif
	#endif
	
	*currentContents = "";
	(*currentLogicReferenceInList)->next = new GIApreprocessorLogicReference();
	(*currentLogicReferenceInList)->next->previous = (*currentLogicReferenceInList);
	*expectToFindSubjectAuxObjectLogicReferenceVariable = false;
	
	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_RECURSION
	bool returnToUpperLevel = false;
	if(commaDetected)
	{
		GIApreprocessorLogicReference* firstLogicReferenceInCurrentLevel = (*currentLogicReferenceInList);
		while(firstLogicReferenceInCurrentLevel->previous != NULL)
		{
			firstLogicReferenceInCurrentLevel = firstLogicReferenceInCurrentLevel->previous;
		}
		if(firstLogicReferenceInCurrentLevel->isSubLogicReference)
		{
			returnToUpperLevel = true;
			(*currentLogicReferenceInList) = firstLogicReferenceInCurrentLevel->lastLogicReferenceInUpperLevel->next;
		}
	}
	if(!returnToUpperLevel)
	{
	#endif
		(*currentLogicReferenceInList) = (*currentLogicReferenceInList)->next;
	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_RECURSION
	}
	#endif

	logicReferenceVariableWordList->clear();
	*currentContents = "";
	
	return result;
}


bool GIApreprocessorLogicReferenceClass::isClassTag(string word, string classNameToFind, string* classTypeNameFound, XMLparserTag* firstLogicReferenceClassTag)
{
	bool classTagFound = false;
	bool result = false;
	
	XMLparserTag* currentTagInLogicReferencePreprocessor = firstLogicReferenceClassTag;

	while(currentTagInLogicReferencePreprocessor->nextTag != NULL)
	{
		if(currentTagInLogicReferencePreprocessor->name == GIA_PREPROCESSOR_XML_TAG_class)
		{	
			bool sameClass = false;
			//vector<string>* logicReferenceClassVector = NULL;
			if(currentTagInLogicReferencePreprocessor->firstAttribute->name == GIA_PREPROCESSOR_XML_ATTRIBUTE_name)
			{
				string className = currentTagInLogicReferencePreprocessor->firstAttribute->value;
				if(className == classNameToFind)
				{
					sameClass = true;	
				}
			}

			if(sameClass)
			{				
				XMLparserTag* firstTagInClass = XMLparserClass.parseTagDownALevel(currentTagInLogicReferencePreprocessor, GIA_PREPROCESSOR_XML_TAG_class, &result);
				XMLparserTag* currentTagInClass = firstTagInClass;
				//cout << "classNameToFind = " << classNameToFind << endl;
				while(currentTagInClass->nextTag != NULL)
				{
					if(currentTagInClass->name == GIA_PREPROCESSOR_XML_TAG_type)
					{
						//cout << "\tGIApreprocessor::extractGIApreprocessorLogicReferenceClasses{} debug: GIA_PREPROCESSOR_XML_TAG_type" << endl;
						string classTypeName = "";
						if(currentTagInClass->firstAttribute->name == GIA_PREPROCESSOR_XML_ATTRIBUTE_name)
						{
							classTypeName = currentTagInClass->firstAttribute->value;
						}
			
			
						XMLparserTag* firstTagInTypeTag = XMLparserClass.parseTagDownALevel(currentTagInClass, GIA_PREPROCESSOR_XML_TAG_type, &result);
						XMLparserTag* currentTagInType = firstTagInTypeTag;
						while(currentTagInType->nextTag != NULL)
						{
							if(currentTagInType->name == GIA_PREPROCESSOR_XML_TAG_instance)
							{
								//cout << "\t\tGIApreprocessor::extractGIApreprocessorLogicReferenceClasses{} debug: GIA_PREPROCESSOR_XML_TAG_instance" << endl;
								if(currentTagInType->firstAttribute->name == GIA_PREPROCESSOR_XML_ATTRIBUTE_name)
								{
									string instanceName = currentTagInType->firstAttribute->value;
									
									if(instanceName == word)
									{
										classTagFound = true;
										*classTypeNameFound = classTypeName;
									}
								}
							}
							currentTagInType = currentTagInType->nextTag;
						}
					}

					currentTagInClass = currentTagInClass->nextTag;
				}
			}
		}

		currentTagInLogicReferencePreprocessor = currentTagInLogicReferencePreprocessor->nextTag;
	}
	
	return classTagFound;
}



bool GIApreprocessorLogicReferenceClass::identifyImplicitLogicReferenceSets(GIApreprocessorLogicReference* firstLogicReferenceInList)
{
	bool result = true;
	GIApreprocessorLogicReference* currentLogicReferenceInList = firstLogicReferenceInList;
	while(currentLogicReferenceInList->next != NULL)
	{
		if(currentLogicReferenceInList->logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION)
		{
			//replace GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNKNOWN before logic reference
			bool multiplePreceedingLogicReferenceSets = islogicReferenceConjunctionAndOr(currentLogicReferenceInList);
			if(multiplePreceedingLogicReferenceSets)
			{
				if(!identifyImplicitConjunctionLogicReferenceSetsBefore(currentLogicReferenceInList, currentLogicReferenceInList->logicReferenceClassType))
				{
					result = false;
				}
			}
			else
			{
				if(!identifyImplicitLogicReferenceSetBefore(currentLogicReferenceInList, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPLEMENT, currentLogicReferenceInList->logicReferenceClassType))
				{
					result = false;
				}			
			}
		}
		else if(currentLogicReferenceInList->logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION)
		{
			//replace GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNKNOWN before or after logic reference
			if(!identifyImplicitLogicReferenceSetBefore(currentLogicReferenceInList, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_COMPLEMENT, currentLogicReferenceInList->logicReferenceClassType))
			{
				if(!identifyImplicitLogicReferenceSetAfter(currentLogicReferenceInList, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_COMPLEMENT, currentLogicReferenceInList->logicReferenceClassType))
				{
					result = false;
				}
			}
		}
		#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_RECURSION
		if(currentLogicReferenceInList->hasSubLogicReference)
		{
			if(!identifyImplicitLogicReferenceSets(currentLogicReferenceInList->firstSubLogicReferenceInList))
			{
				result = false;
			}
		}
		#endif
		currentLogicReferenceInList = currentLogicReferenceInList->next;
	}
	return result;	
}					

bool GIApreprocessorLogicReferenceClass::identifyImplicitLogicReferenceSetBefore(GIApreprocessorLogicReference* logicReferenceInList, const int logicReferenceClass, const string logicReferenceClassType)
{
	bool result = false;
	
	if(logicReferenceInList->previous != NULL)
	{
		GIApreprocessorLogicReference* previousLogicReferenceInList = logicReferenceInList->previous;
		if(previousLogicReferenceInList->logicReferenceClassType == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown)
		{
			previousLogicReferenceInList->logicReferenceClass = logicReferenceClass;
			previousLogicReferenceInList->logicReferenceClassType = logicReferenceClassType;
			result = true;
		}		
	}
	return result;
}

bool GIApreprocessorLogicReferenceClass::identifyImplicitLogicReferenceSetAfter(GIApreprocessorLogicReference* logicReferenceInList, const int logicReferenceClass, const string logicReferenceClassType)
{
	bool result = false;
	
	if(logicReferenceInList->next != NULL)
	{
		GIApreprocessorLogicReference* nextLogicReferenceInList = logicReferenceInList->next;
		if(nextLogicReferenceInList->logicReferenceClassType == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown)
		{
			nextLogicReferenceInList->logicReferenceClass = logicReferenceClass;
			nextLogicReferenceInList->logicReferenceClassType = logicReferenceClassType;
		}
	}
	return result;
}
				
bool GIApreprocessorLogicReferenceClass::identifyImplicitConjunctionLogicReferenceSetsBefore(GIApreprocessorLogicReference* conjunctionLogicReferenceInList, const string logicReferenceClassType)
{
	bool foundImplicitConjunction = false;
	
	//go back and reprocess the referenceSet content of all previous GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_UNKNOWN logical conditions
	if(conjunctionLogicReferenceInList->previous != NULL)
	{
		GIApreprocessorLogicReference* currentLogicReferenceInList = conjunctionLogicReferenceInList->previous;
		bool stillParsing = true;
		while((currentLogicReferenceInList->logicReferenceClassType == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown) && stillParsing)	//OR (currentLogicReferenceInList->logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED)
		{
			foundImplicitConjunction = true;
			currentLogicReferenceInList->logicReferenceClass = GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPLEMENT;
			if(logicReferenceClassType == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_and)
			{
				currentLogicReferenceInList->logicReferenceClassType = GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_and;
			}
			else if(logicReferenceClassType == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_or)
			{
				currentLogicReferenceInList->logicReferenceClassType = GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_or;
			}
			
			if(conjunctionLogicReferenceInList->previous != NULL)
			{
				currentLogicReferenceInList = currentLogicReferenceInList->previous;				
			}
			else
			{
				stillParsing = false;
			}
		}
		
		#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CONJUNCTIONS_ADD_EXPLICIT_SUBJECT_TEXT_FOR_CONJUNCTIONS
		if(foundImplicitConjunction)
		{
			GIApreprocessorLogicReference* firstConjunctionLogicReferenceInList = currentLogicReferenceInList;
			if(!hasReferenceSet(firstConjunctionLogicReferenceInList->logicReferenceVariable->referenceSetSubject) || !hasReferenceSet(firstConjunctionLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter) || !hasReferenceSet(firstConjunctionLogicReferenceInList->logicReferenceVariable->referenceSetObject))
			{
				cout << "GIApreprocessor::identifyConjunctionReferenceSets{} error: firstConjunctionLogicReferenceInList doesn't have a subject, referenceSetDelimiter or object defined" << endl;
				exit(EXIT_ERROR);	
			}

			currentLogicReferenceInList = firstConjunctionLogicReferenceInList->next;
			while(currentLogicReferenceInList->logicReferenceClassType == logicReferenceClassType)
			{
				//GIA translator should take this information into account when parsing the sentence (generating semantic relations), don't do this now:
				if(!hasReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter))
				{
					setReferenceSetText(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, getReferenceSetText(firstConjunctionLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter));
				}
				if(!hasReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject))
				{
					setReferenceSetText(currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject, getReferenceSetText(firstConjunctionLogicReferenceInList->logicReferenceVariable->referenceSetSubject));
				}			

				currentLogicReferenceInList = currentLogicReferenceInList->next;	
			}
		}
		#endif
	}
	else
	{
		cout << "GIApreprocessor::identifyConjunctionReferenceSets{} error: and/or logical condition detected at start of sentence" << endl;
		exit(EXIT_ERROR);	
	}
	
	return foundImplicitConjunction;
}
		

string GIApreprocessorLogicReferenceClass::generateLogicReferenceContents(string logicReferenceClassType)
{
	string logicReferenceContents = logicReferenceClassType;	//this needs to be updated
	return logicReferenceContents;
}

bool GIApreprocessorLogicReferenceClass::islogicReferenceConjunctionAndOr(GIApreprocessorLogicReference* currentLogicReferenceInList)
{
	bool conjunctionAndOr = (currentLogicReferenceInList->logicReferenceClassType == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_and) || (currentLogicReferenceInList->logicReferenceClassType == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_or);
	return conjunctionAndOr;
}


#endif






