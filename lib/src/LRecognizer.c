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
void insert_char_into_list(List* list, char character, unsigned int* grid)
{
    LImage* image = malloc(sizeof(LImage));
    image->size = 25;
    image->grid = grid;
    
    LCharacterImage *cimage = malloc(sizeof(LCharacterImage));
    cimage->character = character;
    cimage->image = image;
    
    list_insert_next(list, list->tail, cimage);
}


// Incoming data
void recognizer_set_data(LRecognizer *recog, LPointData* pointData)
{
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
    
    // ugly
    float x_min = 10000.0, x_max = 0.0, y_min = 10000.0, y_max = 0.0;
    
    ListElement* element = recog->source_points->points.head;
    while(element->next)
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
    }
        
    element = recog->source_points->points.head;
    while(element->next)
    {
        LPoint* point = (LPoint *)element->data;
        point->x = (point->x - x_min) / (x_max - x_min);
        point->y = (point->y - y_min) / (y_max - y_min);
        element = element->next;
    }
}

void recognizer_create_image(LRecognizer *recog)
{    
    // DEBUG: arbitrary number for now
    int n = 25;
    
    int stroke_width = 1;

    LImage* image = malloc(sizeof(LImage));
    
    image->size = n;
    image->grid = malloc(sizeof(unsigned int) * n * n);
    memset(image->grid, 0, sizeof(unsigned int) * n * n);
    
    float interval = 1 / (float)n;

    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            ListElement* element = recog->source_points->points.head;
            do
            {
                LPoint* point = (LPoint *)element->data;
                LRect* rect = LRectMake(interval*(j - stroke_width), 
                                        interval*(j+1+stroke_width), 
                                        interval*(i - stroke_width), 
                                        interval*(i+1+stroke_width));
                if(LPointInRect(*point, *rect))
                    image->grid[i*n + j] = 1;
                
                element = element->next;
            } while (element);
        }
    }
    
    recog->source_image = image;
        
    
    // need to copy data and delete original stuff
    recog->listener.source_image(recog->source_image, recog->listener.obj);

}

// Matching
void recognizer_score_against(LRecognizer *recog, LCharacterSet charSet)
{
    if(charSet == CharacterSetSlashes)
    {
        LImageSet* imageSet = malloc(sizeof(LImageSet));
        list_init(&(imageSet->images), free);
        
        insert_char_into_list(&(imageSet->images), 'A', (unsigned int*)A_char);
        insert_char_into_list(&(imageSet->images), 'B', (unsigned int*)B_char);
        insert_char_into_list(&(imageSet->images), 'H', (unsigned int*)H_char);
        insert_char_into_list(&(imageSet->images), 'L', (unsigned int*)L_char);
        insert_char_into_list(&(imageSet->images), 'O', (unsigned int*)O_char);
        insert_char_into_list(&(imageSet->images), 'Q', (unsigned int*)Q_char);
        insert_char_into_list(&(imageSet->images), 'Z', (unsigned int*)Z_char);

        // now loop through list and make proper result set
        LResultSet* result = malloc(sizeof(LResultSet));
        list_init(&(result->matchData), free);
        
        ListElement* element = imageSet->images.head;
        do
        {
                        
            LCharacterImage* charImage = (LCharacterImage *)element->data;
            float score = recognizer_compare(recog, recog->source_image, charImage->image);
            float character = charImage->character;
            
            LMatchData* matchData = malloc(sizeof(LMatchData));
            matchData->score = score;
            matchData->character = character;
            
            list_insert_next(&(result->matchData), result->matchData.tail, matchData);
            
            element = element->next;
        } while(element);
        
        recog->results = result;
        
        recognizer_gather_results(recog);
    }
}

float recognizer_compare(LRecognizer *recog, LImage* source, LImage* test)
{
    if(source->size != test->size)
        return -1.0; //ERROR: Can't compare images of different sizes
    
    int n, test_count, matches;
    n = source->size;
    test_count = 0;
    matches = 0;
    
    for(int i = 0; i < n * n; ++i)
    {
        if((test->grid)[i] == 0)
            continue;
        
        ++test_count;
        if((source->grid)[i] == 1)
            ++matches;
    }
    
    return matches / (float)test_count;
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
