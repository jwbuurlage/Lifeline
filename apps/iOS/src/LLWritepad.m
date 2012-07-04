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

- (void)drawRect:(CGRect)rect
{
    [[UIColor grayColor] set];
    [[UIBezierPath bezierPathWithRect:rect] fill];
    for(UIBezierPath* path in [controller paths])
    {
        [[UIColor blackColor] set];
        [path stroke];
    }
}

@end


@implementation LLWritepadController

@synthesize paths;

- (id)initWithDelegate:(id <LLListener>)delegate
{
    if([super init]) 
    {
        paths = [[NSMutableArray alloc] init];
        
        // Set up recognizer
        recognizer.charSet = CharacterSetNumeric;
        recognizer.listener = [delegate C_LListener];
    }
    return self;
}

/* LLWritepad Delegate */

- (void)pushPointToData:(CGPoint)point 
{
    LPoint lpoint;
    lpoint.x = point.x;
    lpoint.y = point.y;

    (++pointData.pixels);
    pointData.length += 1;
}

- (void)touchDown:(LLWritepad*)pad event:(UIEvent*)event
{
    UITouch* touch = [event touchesForView:pad].anyObject;
    CGPoint point = [touch locationInView:pad];
            
    bp = [[UIBezierPath alloc] init];
    bp.lineWidth = 5.0;
    [bp moveToPoint:point];
    
    [paths addObject:bp];
    previousPoint = point;
}


- (void)touchDragInside:(LLWritepad*)pad event:(UIEvent*)event 
{
    UITouch* touch = [event touchesForView:pad].anyObject;
    CGPoint point = [touch locationInView:pad];
    [bp addLineToPoint:point];    
    [pad setNeedsDisplay];
}

- (void)touchUpInside:(LLWritepad*)pad event:(UIEvent*)event
{
    previousPoint = CGPointZero;
}

- (void)recognize:(LLWritepad*)pad
{
    
}

@end