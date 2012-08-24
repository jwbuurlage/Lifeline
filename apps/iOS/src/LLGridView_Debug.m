// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#import "LLGridView_Debug.h"

@implementation LLGridView

-(void)setImage:(LImage*)image
{    
    representedImage = image;
}

-(void)drawRect:(CGRect)rect
{      
    if(representedImage == 0)
        return;
    
    int n = representedImage->size;

    float pixelsPerRect = [self frame].size.width / (float)n;
    
    [[UIColor whiteColor] set];
    [[UIBezierPath bezierPathWithRect:rect] fill];

    [[UIColor grayColor] set];
    [[UIBezierPath bezierPathWithRect:rect] stroke];
    
    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            CGRect rect = CGRectMake(pixelsPerRect * j, pixelsPerRect * i, pixelsPerRect, pixelsPerRect);
            
            switch (representedImage->grid[i*n + j]) {
                case 1: [[UIColor blackColor] set]; break;
                case 2: [[UIColor redColor] set]; break;
                case 3: [[UIColor yellowColor] set]; break;
                default: [[UIColor whiteColor] set]; break;
            }
                
            [[UIBezierPath bezierPathWithRect:rect] fill];
            
            [[UIColor grayColor] set];
            [[UIBezierPath bezierPathWithRect:rect] stroke];
        }
    }
}

@end