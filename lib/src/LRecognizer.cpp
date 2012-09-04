// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
//.
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#define NEW_STROKE_OFFSET_T 0.1

#include <stdio.h>
#include <math.h>
#include "../include/LRecognizer.h"
#include "../include/LImageProcessor.h"
#include "../include/LDatabase.h"

namespace Lifeline
{
  Recognizer::Recognizer(int _imageSize)
  {
    imageSize = _imageSize;
    imageProcessor = new ImageProcessor(imageSize);
  }

  Recognizer::~Recognizer()
  {
    delete imageProcessor;
  }

  void Recognizer::loadSymbolsWithData(char* filedata)
  {
    
  }

  void Recognizer::processPoints(const PointData &data, ResultSet &results)
  {
    FeatureVector features;

    imageProcessor->processData(data, features);

    return;
  }
}

#ifdef OLDCODE__OLDCODE__OLDCODE

	LFeatureGeometric* featuresGeo = database_get_symbol_feature(symbolhandle, FeatureTypeGeometric);
	LFeatureZernike* featuresZernike = database_get_symbol_feature(symbolhandle, FeatureTypeZernike);

	if( !featuresGeo || !featuresZernike ) return -1.0f;

	//Geometric
	float geometricScore = 0.0f;
	for(int p = 1; p <= MAX_GEOMETRIC_ORDER; ++p){
		for(int q = 1; q <= MAX_GEOMETRIC_ORDER; ++q){
			float average = featuresGeo->geometricMoments[p-1][q-1];
			float SD = featuresGeo->geometricDeviations[p-1][q-1];
			if( SD <= 0.01f ) continue; //Prevent divide by zero
			float samplevalue = recog->currentFeatures.geometricMoments[p-1][q-1];
			geometricScore += (samplevalue - average)*(samplevalue - average) / (SD * SD);
		}
	}

	//Zernike
	float zernikeScore = 0.0f;
	for(int n = 1; n <= MAX_ZERNIKE_N; ++n){
		for(int m = -n; m <= n; ++m){
			for(int k = 0; k < 2; ++k){ //real and imaginary part
				float average = featuresZernike->zernikeMoments[ZERNIKE_INDEX(n,m)][k];
				float SD = featuresZernike->zernikeDeviations[ZERNIKE_INDEX(n,m)][k];
				if( SD <= 0.001f ) continue;
				float samplevalue = recog->currentFeatures.zernikeMoments[ZERNIKE_INDEX(n,m)][k];
				zernikeScore += (samplevalue - average)*(samplevalue - average) / (SD * SD);
			}
		}
	}

	if( geometricScore != 0.0f || zernikeScore != 0.0f )
		printf("Symbol Score geometric: %f zernike: %f\n", geometricScore, zernikeScore);

	//Total score, some count more than others
	return 0.7f*geometricScore + 1.0f*zernikeScore;
}


///////////////////////////////////////////////////////////////////////////////

//This is for taking samples, updating the symbol database and calculating averages and so on
typedef LFeatureSet* PLFeatureSet;
PLFeatureSet featuresetList[20];
int currentSample = 0;

void recognizer_clear_samples(LRecognizer *recog){
	for(int i = 0; i < currentSample; ++i) free(featuresetList[i]);
	currentSample = 0;
}

void recognizer_take_sample(LRecognizer *recog)
{
	if( currentSample == 20 ){
		for(int i = 0; i < 20; ++i) free(featuresetList[i]);
		currentSample = 0;
	}
	//Save the current sample
	featuresetList[currentSample] = (PLFeatureSet)malloc(sizeof(LFeatureSet));
	memcpy(featuresetList[currentSample], &(recog->currentFeatures), sizeof(LFeatureSet));
	++currentSample;
	
	return;
}

void recognizer_save_samples(LRecognizer *recog, char* symbol){
	void* handle = database_get_symbol_handle(symbol);
	if( handle == 0 ){
		printf("Unable to find symbol '%s' in database.\n", symbol);
		return;
	}

	//Calculate averages
	LFeatureGeometric featureGeometric;
	LFeatureZernike featureZernike;
	
	for(int p = 1; p <= MAX_GEOMETRIC_ORDER; ++p){
		for(int q = 1; q <= MAX_GEOMETRIC_ORDER; ++q){
			float average = 0.0f;
			for(int i = 0; i < currentSample; ++i){
				average += featuresetList[i]->geometricMoments[p-1][q-1];
			}
			average /= (float)currentSample;
			float standardDeviation = 0.0f;
			for(int i = 0; i < currentSample; ++i){
				standardDeviation += (featuresetList[i]->geometricMoments[p-1][q-1] - average)*(featuresetList[i]->geometricMoments[p-1][q-1] - average);
			}
			standardDeviation /= (float)currentSample;
			standardDeviation = sqrt(standardDeviation);

			featureGeometric.geometricMoments[p-1][q-1] = average;
			featureGeometric.geometricDeviations[p-1][q-1] = standardDeviation;
			printf("SAVING: Geometric p=%d q=%d value=%f SD=%f\n",p,q,average,standardDeviation);
		}
	}

	for(int n = 1; n <= MAX_ZERNIKE_N; ++n){
		for(int m = -n; m <= n; ++m){
		for(int k = 0; k < 2; ++k){ //real and imaginary part
			float average = 0.0f;
			for(int i = 0; i < currentSample; ++i){
				average += featuresetList[i]->zernikeMoments[ZERNIKE_INDEX(n,m)][k];
			}
			average /= (float)currentSample;
			float standardDeviation = 0.0f;
			for(int i = 0; i < currentSample; ++i){
				standardDeviation += (featuresetList[i]->zernikeMoments[ZERNIKE_INDEX(n,m)][k] - average)*(featuresetList[i]->zernikeMoments[ZERNIKE_INDEX(n,m)][k] - average);
			}
			standardDeviation /= (float)currentSample;
			standardDeviation = sqrt(standardDeviation);
			featureZernike.zernikeMoments[ZERNIKE_INDEX(n,m)][k] = average;
			featureZernike.zernikeDeviations[ZERNIKE_INDEX(n,m)][k] = standardDeviation;
			printf("SAVING: Zernike n=%d m=%d value=%f SD=%f\n",n,m,average,standardDeviation);
		}
		}
	}
	
	int res = database_update_symbol_feature(handle, FeatureTypeGeometric, &featureGeometric);
	if( res == 0 ) printf("Unable to update geometric moments.\n");
	res = database_update_symbol_feature(handle, FeatureTypeZernike, &featureZernike);
	if( res == 0 ) printf("Unable to update zernike moments.\n");
}

#endif

