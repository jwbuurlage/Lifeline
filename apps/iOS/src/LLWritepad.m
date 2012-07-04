// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#import "LLWritepad.h"
#import "LLListenerProtocol.h"

@implementation LLWritepadController

- (id)initWithDelegate:(id <LLListener>)delegate
{
    if([super init]) 
    {
        // Set up recognizer
        recognizer.charSet = CharacterSetNumeric;
        recognizer.listener = [delegate c_listener];
        recognizer_recognize(&recognizer);
    }
    return self;
}

// LLWritepad Delegate
- (void)touchDown:(LLWritepad*)pad event:(UIEvent*)event
{
    NSLog(@"touchDown:");
}


- (void)touchDragInside:(LLWritepad*)pad event:(UIEvent*)event 
{
    NSLog(@"touchDrag:");
}

- (void)touchUpInside:(LLWritepad*)pad event:(UIEvent*)event
{
    NSLog(@"touchUp:");
    recognizer_recognize(&recognizer);
}

- (void)strokeFinished:(LLWritepad*)pad
{
    Stroke stroke;
    float points[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
    stroke.points = points;
    stroke.length = 4;
    
    recognizer_add_stroke(&recognizer, &stroke);
}

@end

@implementation LLWritepad

- (id)initWithFrame:(CGRect)aFrame delegate:(id <LLListener>)delegate
{
    if ([super initWithFrame:aFrame]) {
        self.backgroundColor = [UIColor grayColor];
        controller = [[LLWritepadController alloc] initWithDelegate:delegate];
        
        // target and actions
        [self addTarget:controller action:@selector(touchDown:event:) forControlEvents:UIControlEventTouchDown];
        [self addTarget:controller action:@selector(touchDragInside:event:) forControlEvents:UIControlEventTouchDragInside];
        [self addTarget:controller action:@selector(touchUpInside:event:) forControlEvents:UIControlEventTouchUpInside];
        

    }
    return self;
}

@end