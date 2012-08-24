// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#import <UIKit/UIKit.h>
#import <LMatch.h>

@interface LLGridView : UIView
{
    LImage* representedImage;
}

-(void)setImage:(LImage*)image;

@end
