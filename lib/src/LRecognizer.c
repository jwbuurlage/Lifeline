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
    // NOTE:  Make _gridView (100, 100) 
    int n = 25;
    
    int stroke_width = 2;

    LImage* image = malloc(sizeof(LImage));
    
    image->size = n;
    image->grid = malloc(sizeof(char) * n * n);
    memset(image->grid, '0', sizeof(char) * n * n);
    
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
                    image->grid[i*n + j] = '1';
                
                element = element->next;
            } while (element);
        }
    }
    
    recog->source_image = image;
        
    // need to copy data and delete original stuff
    recog->listener.source_image(recog->source_image, recog->listener.obj);
    //recog->listener.source_image(image_A, recog->listener.obj);

}

// Matching
void recognizer_score_against(LRecognizer *recog, LCharacterSet charSet)
{
    if(charSet == CharacterSetSlashes)
    {
        LImage* image_A = malloc(sizeof(LImage));
        image_A->size = 25;
        image_A->grid = (char*)A_char;
        
        LImage* image_B = malloc(sizeof(LImage));
        image_B->size = 25;
        image_B->grid = (char*)B_char;

        LImage* image_L = malloc(sizeof(LImage));
        image_L->size = 25;
        image_L->grid = (char*)L_char;

        float A, L, B;
        
        A = recognizer_compare(recog, recog->source_image, image_A);
        B = recognizer_compare(recog, recog->source_image, image_B);
        L = recognizer_compare(recog, recog->source_image, image_L);
        
        char match;
        if(A > L && A > B)
            match = 'A';
        else if (B > A && B > L)
            match = 'B';
        else if (L > A && L > B)
            match = 'L';
                
        recog->listener.char_found(match, recog->listener.obj);
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
        if((test->grid)[i] == '0')
            continue;
        
        ++test_count;
        if((source->grid)[i] == '1')
            ++matches;
    }
    
    return matches / (float)test_count;
}

// Postprocessing & reporting
void recognizer_gather_results(LRecognizer *recog)
{
    
}

void recognizer_report(LRecognizer *recog)
{
    
}
