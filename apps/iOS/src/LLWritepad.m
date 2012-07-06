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
    if (self = [super initWithFrame:aFrame]) {
        controller = [[LLWritepadController alloc] initWithDelegate:delegate];
            
        // target and actions
        [self addTarget:controller action:@selector(touchDown:event:) forControlEvents:UIControlEventTouchDown];
        [self addTarget:controller action:@selector(touchDragInside:event:) forControlEvents:UIControlEventTouchDragInside];
        [self addTarget:controller action:@selector(touchUpInside:event:) forControlEvents:UIControlEventTouchUpInside];
        
        // test button
        UIButton* _testButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
        [_testButton setFrame:CGRectMake(20, 20, 120, 45)];
        [_testButton setTitle:@"Recognize" forState:UIControlStateNormal];
        [_testButton addTarget:controller action:@selector(recognize:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:_testButton];
    }
    return self;
}

- (void)drawRect:(CGRect)rect
{
    [[UIColor lightGrayColor] set];
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
    if(self = [super init]) 
    {
        paths = [[NSMutableArray alloc] init];
        
        // Set up recognizer
        recognizer.charSet = CharacterSetNumeric;
        recognizer.listener = [delegate C_LListener];
        
        pointData = (LPointData *)malloc(sizeof(LPointData));
        list_init(&(pointData->points), free);
    }
    return self;
}

/* LLWritepad Delegate */

- (void)pushPointToData:(CGPoint)point 
{
    LPoint* lpoint = LPointMake(point.x, point.y);
    list_insert_next(&(pointData->points), 0, lpoint);
}

- (void)touchDown:(LLWritepad*)pad event:(UIEvent*)event
{
    UITouch* touch = [event touchesForView:pad].anyObject;
    CGPoint point = [touch locationInView:pad];
            
    [self pushPointToData:point];
    
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
    
    [self pushPointToData:point];

    [bp addLineToPoint:point];    
    [pad setNeedsDisplay];
}

- (void)touchUpInside:(LLWritepad*)pad event:(UIEvent*)event
{
    previousPoint = CGPointZero;
}

- (void)recognize:(LLWritepad*)pad
{
    // MARK MEMLEAK -- GIVING AWAY POINTER AND ALLOCING NEW DATA   
    recognizer_set_data(&recognizer, pointData);
    recognizer_score_against(&recognizer, CharacterSetSlashes);
    
    pointData = malloc(sizeof(LPointData));
    list_init(&(pointData->points), free);

    [paths removeAllObjects];
    [pad setNeedsDisplay];
}

@end