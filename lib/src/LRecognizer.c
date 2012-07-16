// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#include <stdio.h>
#include "../include/LRecognizer.h"
#include "../include/LTestCharacters.h"

/* Convenience methods */

LPoint* points_center(LPointData* pointData)
{
    float x_min = 10000.0, x_max = 0.0, y_min = x_min, y_max = x_max;
    
    ListElement* element = pointData->points.head;
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
    } while((element = element->next));
    
    return LPointMake(x_min + (x_max - x_min) / 2, 
                      y_min + (y_max - y_min) / 2);
}

// Incoming data
void recognizer_set_data(LRecognizer *recog, LPointData* pointData)
{
    recog->image_size = 65;
    
    if(recog->source_points != 0)
    {
        list_destroy(&(recog->source_points->points));
        free(recog->source_points);
    }
    
    recog->source_points = pointData;
    
    recognizer_normalize_data(recog);
    recognizer_create_image(recog);
    recognizer_score_against(recog, recog->charSet);
}

// Preprocessing
void recognizer_normalize_data(LRecognizer *recog)
{
    // calculate the center of the data
    LPoint* center = points_center(recog->source_points);
        
    ListElement* element = recog->source_points->points.head;
    do
    {
        LPoint* point = (LPoint *)element->data;
        point->x -= center->x;
        point->y -= center->y;        
    } while((element = element->next));
    
    float x_min = 1.0, x_max = -1.0, y_min = 1.0, y_max = -1.0;

    element = recog->source_points->points.head;
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
    } while((element = element->next));
    
    
    float max = 0.0;
    if (x_max > y_max) { max = x_max; } 
    else { max = y_max; }
    
    element = recog->source_points->points.head;
    do
    {
        LPoint* point = (LPoint *)element->data;
        point->x = point->x / max;
        point->y = point->y / max;
    } while((element = element->next));
            
    free(center);
}

void recognizer_create_image(LRecognizer *recog)
{    
    // DEBUG: arbitrary number for now
    int n = recog->image_size;
    
    LImage* image = malloc(sizeof(LImage));
    
    image->size = n;
    image->grid = malloc(sizeof(unsigned int) * n * n);
    memset(image->grid, 0, sizeof(unsigned int) * n * n);
    
    float interval = 2 / (float)n;

    LRect* rect = LRectMake(0.0, 0.0, 0.0, 0.0);
    
    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            ListElement* element = recog->source_points->points.head;
            do
            {
                LPoint* point = (LPoint *)element->data;
                
                // printf("(%f, %f)\n", point->x, point->y);
                
                LRectSet(rect,  -1 + interval * j, 
                                -1 + interval * (j + 1), 
                                -1 + interval * i, 
                                -1 + interval * (i + 1));
                
                if(LPointInRect(*point, *rect))
                    image->grid[i*n + j] = 1;
                
                element = element->next;
            } while(element);
        }
    }
    
    recog->source_image = image;
    
    // Need to copy data and delete original stuff
    recog->listener.source_image(recog->source_image, recog->listener.obj);
}

// Matching
void recognizer_score_against(LRecognizer *recog, LCharacterSet charSet)
{    
    LResultSet* result = malloc(sizeof(LResultSet));
    list_init(&(result->matchData), free);

    charFeatures* feat = malloc(sizeof(List));
    list_init(&feat, free);
    
    switch (charSet) {
        case CharacterSetSlashes:
            // fill features with certain characterFeature
            break;
            
        default:
            // fill features with default characterFeature
            break;
    }
    
    ListElement* element = feat->head;
    do
    {
        charFeature= (LCharacterFeature *)element->data;
        
        LFeatureSet* featureSet = image_feature_set(LImage* image);
        float score = recognizer_compare(recog, featureSet, charFeature->features)

        LMatchData* matchData = malloc(sizeof(LMatchData));
        matchData->score = score;
        matchData->character = charFeature->character;

        // add score to result and set
        list_insert_next(&(result->matchData), result->matchData.tail, matchData);
    } while((element = element->next));
       
    recog->results = result;
    recognizer_gather_results(recog);
    
    free(imageSet);
}

float recognizer_compare(LRecognizer *recog, LFeatureSet* source, LFeatureSet* test)
{
    // weighted average of features
    return 0;
}

// Postprocessing & reporting
void recognizer_gather_results(LRecognizer *recog)
{
    float best_score = 0.0;
    char best_char = '?';
    
    ListElement* element = recog->results->matchData.head;
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
    recog->listener.result_set(recog->results, recog->listener.obj);
}
