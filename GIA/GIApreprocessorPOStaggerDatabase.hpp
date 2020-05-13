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
 * File Name: GIApreprocessorPOStagger.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k14b 12-May-2020
 * Requirements: requires plain text file
 * Description: Preprocessor POS tagger database
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_PREPROCESSOR_POS_TAGGER_DATABASE
#define HEADER_GIA_PREPROCESSOR_POS_TAGGER_DATABASE

#include "GIAglobalDefs.hpp"
#include "GIAdatabase.hpp"	//required for checkIfFolderExistsAndIfNotMakeAndSetAsCurrent
#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL
#include "ANNformation.hpp"
#include "ANNalgorithmBackpropagationTraining.hpp"
#include "ANNxmlConversion.hpp"
#endif
#ifdef USE_ANN
#include "ANNexperienceClass.hpp"
#endif

#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_NUMBER_OF_TYPES (GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES+1)
#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_INDEX_OUT_OF_SENTENCE_BOUNDS (GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_NUMBER_OF_TYPES-1)
	
#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_PERSISTENT
	
	#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME ((string)"GIAPOStaggerDatabase")
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
		#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_DEFAULT_XML_FILE_NAME ((string)"GIAPOStaggerNeuralNetwork.xml")
	#endif
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM
		//key: l = left word POS, r = right word POS, c = centre word POS
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_AND_MAP_USE_6BIT_INDICES
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS (2) 	//eg hex (eg lllll/rrrrr/lllllrrrrr.txt
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_SUBDIRECTORY_INDEX_NUMBER_OF_WORDS_PER_LEVEL (5) 	//hex (eg fffff)
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_BYTE_CODED_START_POS (0)
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_BYTE_CODED_LENGTH (1)
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_NUMBER_OF_INSTANCES_INT_START_POS (2)
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_NUMBER_OF_INSTANCES_INT_LENGTH (10)
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_BYTE_CODED_BASE (64)
			static char GIApreprocessorDatabaseBase64encodingCharacters[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '_', '-'};	//GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM: these have to be supported as valid characters for OS file system directory names
			//e.g. Z 0000000001
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_BYTE_CODED_BASE_INDEX_0 0
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_BYTE_CODED_BASE_INDEX_9 9
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_BYTE_CODED_BASE_INDEX_A 10
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_BYTE_CODED_BASE_INDEX_Z 35
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_BYTE_CODED_BASE_INDEX_a 36
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_BYTE_CODED_BASE_INDEX_z 61
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_BYTE_CODED_BASE_INDEX_DASH 62
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_BYTE_CODED_BASE_INDEX_UNDERSCORE 63
		#else
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS (5) 	//eg hex (eg llll/llll/llrr/rrrr/rrrr/llllllllllrrrrrrrrrr.txt
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_SUBDIRECTORY_INDEX_NUMBER_OF_WORDS_PER_LEVEL (2) 	//hex (eg ffff)	
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_BYTE_CODED_START_POS (0)
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_BYTE_CODED_LENGTH (2)
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_NUMBER_OF_INSTANCES_INT_START_POS (3)
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_NUMBER_OF_INSTANCES_INT_LENGTH (10)
			#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_BYTE_CODED_BASE (16)
			//e.g. cc 0000000001
		#endif

		#define ASCII_TABLE_INDEX_OF_a (97)
		#define ASCII_TABLE_NUMBER_OF_LETTERS_IN_ALPHABET (26)
		#define ASCII_TABLE_INDEX_OF_z (ASCII_TABLE_INDEX_OF_a + ASCII_TABLE_NUMBER_OF_LETTERS_IN_ALPHABET)

		#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_FILE_NAME_PARTA "POSpermutation"
		#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_FILE_NAME_EXTENSION ".pos"
	#endif
#endif

class GIApreprocessorPOStaggerDatabaseClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIAdatabaseClass GIAdatabase;
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL
	private: ANNformationClass ANNformation;
	private: ANNalgorithmBackpropagationTrainingClass ANNalgorithmBackpropagationTraining;	
	private: ANNxmlConversionClass ANNxmlConversion;	
	#endif
		
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_PERSISTENT
	public: void initialisePOStaggerDatabase(const string newGIAposTaggerDatabaseFolderName);
	#endif

	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_MAP
	private: bool findInstancePOStaggerMap(const string POSambiguityInfoPermutation, unsigned char centreWordPOSambiguityInfo, int* numberOfInstances, const bool incrementIfFound);
	private: void insertInstanceInPOStaggerMap(const string POSambiguityInfoPermutation, const unsigned char centreWordPOSambiguityInfo, const int numberOfInstances);
	private: multimap<string, pair<unsigned char, int>>* getPOStaggerMap();
	#endif
	public: string convertPOSambiguityInfoPermutationToPOSambiguityInfoPermutationIndexString(vector<uint64_t>* POSambiguityInfoPermutation);
	
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE
	public: void feedNeuralNetworkWithExperience(ANNexperience* currentExperience);
	public: bool writeDatabaseNeuralNetwork();
	#endif
	public: bool calculateIdealClassTargetOfInputExperience(ANNexperience* experience, int* idealClassTarget, double* experienceBackPropagationPassError);
	#endif
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE
	public: bool externalANNgenerateBatchTrainData(ANNexperience* firstExperienceInList, const int batchIndex);
		public: string externalANNgenerateBatchFileName(const string fileNamePrepend, const int batchIndex);
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_TRAIN_EXECUTE_FEED
		public: bool externalANNgenerateBatchTrainDataExecuteFeed();
	public: bool externalANNform();
	public: bool externalANNtrain();
	public: bool externalANNtrainEpoch();
	public: bool externalANNtrainEpochBatch();
	public: bool externalANNtest();
	#endif
	#endif
	public: bool externalANNpredict(ANNexperience* firstExperienceInList);
		private: bool externalANNgenerateBatchDataExperiences(ANNexperience* firstExperienceInList, vector<string>* batchDataInput, vector<string>* batchDataOutput);
			public: string externalANNgenerateBatchDataExperienceInput(ANNexperience* currentExperienceInList);
			public: string externalANNgenerateBatchDataExperienceOutput(ANNexperience* currentExperienceInList);
		private: bool externalANNexecuteScript(string scriptName);
	#endif
	#endif

	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM
	private: string DBgenerateFileName(vector<uint64_t>* POSambiguityInfoPermutation);
		private: string DBgenerateSubFolderName(vector<uint64_t>* POSambiguityInfoPermutation, const int level, const int numberOfWordsPerLevel);
	public: bool DBreadPOSpermutationEstimates(vector<uint64_t>* POSambiguityInfoPermutation, vector<string>* centreWordPOSambiguityInfoList);
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE
	public: bool DBwritePOSpermutationEstimate(vector<uint64_t>* POSambiguityInfoPermutation, const uint64_t centreWordPOSambiguityInfo);
	#endif
	#endif
	
	public: string generateIntFormatString(int numberOfCharacters);
	public: string DBconvertByteToBinaryString(uint64_t byte);
	
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_PREDICTION_VERIFICATION
	public: bool verifyPOStaggerDatabasePredictionAgainstPOSambiguityInfo(const unsigned char centreWordPOSindexPrediction, const unsigned int centreWordPOSambiguityInfo, unsigned char* centreWordPOSvalueFirstAmbiguousPrediction);
	#endif
	
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_AND_MAP_USE_6BIT_INDICES
	private: char DBconvertByteToBase64(unsigned char byte);
	public: unsigned char DBconvertBase64ToByte(char base64char);
	#else
	private: string DBconvertByteToHex(const unsigned char byte);
	public: unsigned char DBconvertHexToByte(string hexString);
	#endif
	
	public: unsigned char convertPOSambiguityInfoToIndex(uint64_t POSambiguityInfo);
		public: bool determinePOSambiguityInfoIsAmbiguous(const uint64_t POSambiguityInfo, unsigned char* unambiguousPOSinfoIndex, const bool treatWordAsAmbiguousIfNullPOSvalue);

	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL
	public: string externalANNgenerateExperienceInputStandalone(ANNexperience* currentExperienceInList);
	#endif
};

#endif



