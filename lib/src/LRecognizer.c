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

// Loading data from files
void recognizer_load_data(LRecognizer *recog)
{
/*
	float average, standardDeviation;
	FILE *fp;
	fp=fopen("geomomentsO.txt", "r");
	if( fp ){
		for(int p = 1; p <= MAX_GEOMETRIC_ORDER; ++p){
			for(int q = 1; q <= MAX_GEOMETRIC_ORDER; ++q){
				fscanf(fp, "%f %f\n", &average, &standardDeviation);
				recog->loadedCharacters['O'].geometricMoments[p-1][q-1] = average;
				recog->loadedCharacters['O'].geometricDeviations[p-1][q-1] = standardDeviation;
				printf("O: p,q,avg,sigma = %d,%d,%f,%f\n",p,q,average,standardDeviation);
			}
		}
		fclose(fp);
	}
	fp=fopen("geomomentsL.txt", "r");
	if( fp ){
		for(int p = 1; p <= MAX_GEOMETRIC_ORDER; ++p){
			for(int q = 1; q <= MAX_GEOMETRIC_ORDER; ++q){
				fscanf(fp, "%f %f\n", &average, &standardDeviation);
				recog->loadedCharacters['L'].geometricMoments[p-1][q-1] = average;
				recog->loadedCharacters['L'].geometricDeviations[p-1][q-1] = standardDeviation;
				printf("L: p,q,avg,sigma = %d,%d,%f,%f\n",p,q,average,standardDeviation);
			}
		}
		fclose(fp);
	}
*/
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
    recognizer_score_against(recog, recog->charSet);
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

///////////////////////////////////////////////////////////////////////////////

typedef LFeatureSet* PLFeatureSet;
PLFeatureSet featuresetList[20];
int currentSample = 0;

void recognizer_showMoments(LRecognizer *recog)
{
	if( currentSample == 20 ){
		for(int i = 0; i < 20; ++i) free(featuresetList[i]);
		currentSample = 0;
	}
	featuresetList[currentSample] = (PLFeatureSet)malloc(sizeof(LFeatureSet));
	image_moments(recog->source_image, featuresetList[currentSample]);
	++currentSample;

	
	FILE *fp;
	fp=fopen("geomoments.txt", "w+");
	
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
			fprintf(fp, "%f %f\n", average, standardDeviation);
		}
	}
	
	fclose(fp);
}

///////////////////////////////////////////////////////////////////////////////

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
    image_connected_components(recog->source_image);
    image_points_curvature(recog->source_image);
    image_line_type(recog->source_image);
    recognizer_showMoments(recog);
    // Need to copy data and delete original stuff
    recog->listener.source_image(recog->source_image, recog->listener.obj);
}

// Matching
void recognizer_score_against(LRecognizer *recog, LCharacterSet charSet)
{
/*
	float scoreO = 0.0f;
	float scoreL = 0.0f;
	for(int p = 1; p <= MAX_GEOMETRIC_ORDER; ++p){
		for(int q = 1; q <= MAX_GEOMETRIC_ORDER; ++q){
			float avgO = recog->loadedCharacters['O'].geometricMoments[p-1][q-1];
			float SDO = recog->loadedCharacters['O'].geometricDeviations[p-1][q-1];
			float avgL = recog->loadedCharacters['L'].geometricMoments[p-1][q-1];
			float SDL = recog->loadedCharacters['L'].geometricDeviations[p-1][q-1];
			float measure = featuresetList[currentSample-1]->geometricMoments[p-1][q-1];
			scoreO += (measure - avgO)*(measure - avgO) / (SDO*SDO);
			scoreL += (measure - avgL)*(measure - avgL) / (SDL*SDL);
		}
	}
	
	//printf("Score O = %f \t Score L = %f\n", scoreO, scoreL);
	if( scoreO > scoreL ) printf("L");
	else printf("O");
	fflush(stdout);
*/

//    LResultSet* result = malloc(sizeof(LResultSet));
//    list_init(result, free);
//
//    LCharacterFeatures* feat = malloc(sizeof(LCharacterFeatures));
//    list_init(feat, free);
//    
//    switch (charSet) 
//    {
//        case CharacterSetSlashes:
//            // fill features with certain characterFeature
//            LCharacterFeature* feature = malloc(sizeof(LCharacterFeature));
//            feature->feature_name = "moment";
//            feature->score = 1.0;
//            
//            list_insert_next(feat, feat->tail, *feature);
//            break;
//            
//        default:
//            // fill features with certain characterFeature
//            LCharacterFeature* feature = malloc(sizeof(LCharacterFeature));
//            feature->feature_name = "moment";
//            feature->score = 1.0;
//            
//            list_insert_next(feat, feat->tail, *feature);
//            break;
//    }
//    
//    ListElement* element = feat->head;
//    do
//    {
//        charFeature= (LCharacterFeature *)element->data;
//        
//        LFeatureSet* featureSet = image_feature_set_make(LImage* image);
//        float score = recognizer_compare(recog, featureSet, charFeature->features)
//
//        LMatchData* matchData = malloc(sizeof(LMatchData));
//        matchData->score = score;
//        matchData->character = charFeature->character;
//
//        // add score to result and set
//        list_insert_next(result, result->tail, matchData);
//        element = element->next; 
//    } while(element);
//       
//    recog->results = result;
//    
//    free(feat);
    
    recog->results = (void*)0;
    //recognizer_gather_results(recog);
}

float recognizer_compare(LRecognizer *recog, LFeatureSet* source, LFeatureSet* test)
{
    // weighted average of features
    return 0;
}

///////////////////////////////////////////////////////////////////////////////

void recognizer_gather_results(LRecognizer *recog)
{
    float best_score = 0.0;
    char best_char = '?';
    
    if(!recog->results)
        recognizer_report(recog);
    
    ListElement* element = recog->results->head;
    do
    {
        LMatchData *matchData = element->data;
        
        if(matchData->score > best_score)
        {
            best_score = matchData->score;
            best_char = matchData->character;
        }
                
    element = element->next; 
} while(element);
    
    recog->listener.char_found(best_char, recog->listener.obj); 
    recognizer_report(recog);
}

void recognizer_report(LRecognizer *recog)
{
    if(!recog->results)
        return;
       
    recog->listener.result_set(recog->results, recog->listener.obj);
}
