// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#define L_DEBUG

#include <stdio.h>
#include "../include/LRecognizer.h"

// Incoming data
void recognizer_set_data(LRecognizer *recog, LPointData* pointData)
{
#ifdef L_DEBUG
    printf("<Recognizer> Set data \n");
#endif
    
    recog->source_points = pointData;
    recognizer_normalize_data(recog);
    recognizer_create_image(recog);
}

// Preprocessing
void recognizer_normalize_data(LRecognizer *recog)
{
#ifdef L_DEBUG
    printf("<Recognizer> Normalizing data \n");
#endif
    
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
    
    printf("Values: %f, %f, %f, %f", x_min, x_max, y_min, y_max);
    
    element = recog->source_points->points.head;
    while(element->next)
    {
        LPoint* point = (LPoint *)element->data;
        point->x = (point->x - x_min) / x_max;
        point->y = (point->y - y_min) / y_max;
        
        element = element->next;
    }
}

void recognizer_create_image(LRecognizer *recog)
{
#ifdef L_DEBUG
    printf("<Recognizer> Creating image \n");
#endif
    
    // DEBUG: arbitrary number for now
    // NOTE:  Make _gridView (100, 100) 
    int n = 25;

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
                LRect* rect = LRectMake(interval*i, interval*(i+1), interval*j, interval*(j+1));
                if(LPointInRect(*point, *rect))
                    image->grid[i*n + j] = '1';
                
                element = element->next;
            } while (element);
        }
    }
    
    recog->source_image = image;
    recog->listener.source_image(recog->source_image, recog->listener.obj);
}

// Matching
void recognizer_score_against(LRecognizer *recog, LCharacterSet charSet)
{

}

float recognizer_compare(LRecognizer *recog, LImage source, LImage test)
{
    return 0.1;
}

// Postprocessing & reporting
void recognizer_gather_results(LRecognizer *recog)
{
    
}

void recognizer_report(LRecognizer *recog)
{
    
}
