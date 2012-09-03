// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#define NEW_STROKE_OFFSET_T 0.1

#include <stdio.h>
#include <math.h>
#include "../include/LRecognizer.h"
#include "../include/LDatabase.h"

//Callback
float recognizer_score_symbol(void* _recog, void* symbolhandle);

/* Convenience methods */

LPoint* points_center(LPointData* pointData)
{
    float x_min = 10000.0, x_max = 0.0, y_min = x_min, y_max = x_max;

    ListElement* element = pointData->head;
    do
    {
        LPoint* point = (LPoint *)element->data;
        if (point->x < x_min)
            x_min = point->x;
        if (point->x > x_max) 
            x_max = point->x;
        if (point->y < y_min) 
            y_min = point->y;
        if (point->y > y_max) 
            y_max = point->y;
    element = element->next; } while(element);

    return LPointMake(x_min + (x_max - x_min) / 2, 
                      y_min + (y_max - y_min) / 2, 0);
}

// Incoming data
void recognizer_set_data(LRecognizer *recog, LPointData* pointData)
{
    recog->image_size = 65;

    if(recog->source_points != 0)
    {
        list_destroy(recog->source_points);
        free(recog->source_points);
    }

    recog->source_points = pointData;

    recognizer_normalize_data(recog);
    recognizer_connect_data(recog);
    recognizer_create_image(recog);

    char* best_symbol = database_do_scores(&recognizer_score_symbol, recog);
    if( best_symbol ){
	    recog->listener.char_found(best_symbol[0], recog->listener.obj); 
    }
    //recog->listener.result_set(recog->results, recog->listener.obj);
}

// Preprocessing
void recognizer_normalize_data(LRecognizer *recog)
{
    // calculate the center of the data
    LPoint* center = points_center(recog->source_points);

    ListElement* element = recog->source_points->head;
    do
    {
        LPoint* point = (LPoint *)element->data;
        point->x -= center->x;
        point->y -= center->y;        
        element = element->next; 
    } while(element);
    
    float x_min = 1.0, x_max = -1.0, y_min = 1.0, y_max = -1.0;

    element = recog->source_points->head;
    do
    {
        LPoint* point = (LPoint *)element->data;
        if (point->x < x_min)
            x_min = point->x;
        if (point->x > x_max) 
            x_max = point->x;
        if (point->y < y_min) 
            y_min = point->y;
        if (point->y > y_max) 
            y_max = point->y;
        element = element->next; 
    } while(element);
    
    
    float max = 0.0;
    if (x_max > y_max) { max = x_max; } 
    else { max = y_max; }
    
    element = recog->source_points->head;
    do
    {
        LPoint* point = (LPoint *)element->data;
        point->x = point->x / max;
        point->y = point->y / max;
        element = element->next; 
    } while(element);
            
    free(center);
}

void recognizer_connect_data(LRecognizer *recog)
{
    // if the distance between two subsequent points is greater
    // than the interval (width of rects in grid) then we need
    // to add points between the two points till it isn't.
    float b = 2.0f / (recog->image_size);
    
    ListElement* element = recog->source_points->head;
    do
    {
        if(element->next == NULL)
            break;
            
        LPoint* point = (LPoint *)element->data;
        LPoint* next_point = (LPoint *)element->next->data;
        
        if((next_point->t - point->t) > NEW_STROKE_OFFSET_T)
        {
            element = element->next;
            continue;
        }
        
        float d = LPointDistance(point, next_point);
        int steps = floorf(d/b) + 1;
        float ival = b/d;
        
        for(int i = 0; i < steps; ++i)
        {
            // better to malloc in chunks
            LPoint* point_new = malloc(sizeof(LPoint));
            
            float t = i * ival;
            point_new->x = t * point->x + (1 - t) * next_point->x;
            point_new->y = t * point->y + (1 - t) * next_point->y;
            
            list_insert_next(recog->source_points, element, point_new);
            element = element->next;
        }
        element = element->next; 
    } while(element);
}

void recognizer_create_image(LRecognizer *recog)
{    
    // DEBUG: arbitrary number for now
    int n = recog->image_size;
    
    LImage* image = malloc(sizeof(LImage));
    
    //Note that we allocate n*n + 1 gridpoints because we use the extra grid point
    //at get_neighbour at the edges: when requesting a neighbour that it outside
    //the field, it returns this extra dummy square which is not enabled so we
    //get exactly the right result.
    image->size = n;
    image->grid = malloc(sizeof(LGridPoint) * (n * n + 1));
    memset(image->grid, 0, sizeof(LGridPoint) * (n * n + 1));
    
    float interval = 2 / (float)n;

    LRect* rect = LRectMake(0.0, 0.0, 0.0, 0.0);
    
    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            ListElement* element = recog->source_points->head;
            do
            {
                LPoint* point = (LPoint *)element->data;
                                
                LRectSet(rect,  -1 + interval * j, 
                                -1 + interval * (j + 1), 
                                -1 + interval * i, 
                                -1 + interval * (i + 1));
                
                if(LPointInRect(*point, *rect))
                    image->grid[i*n + j].enabled = 1;
                
                        element = element->next; 
            } while(element);
        }
    }

	recog->source_image = image;
	image_thin(recog->source_image);
	image_branch_points(recog->source_image);
	image_end_points(recog->source_image);
	//    image_connected_components(recog->source_image);
	//    image_points_curvature(recog->source_image);
	//    image_line_type(recog->source_image);
	image_moments(recog->source_image, &recog->currentFeatures);
	// Need to copy data and delete original stuff
	recog->listener.source_image(recog->source_image, recog->listener.obj);
}


// Matching

// This is a callback, called by LDatabase
float recognizer_score_symbol(void* _recog, void* symbolhandle)
{
	LRecognizer* recog = (LRecognizer*)_recog;

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

///////////////////////////////////////////////////////////////////////////////
