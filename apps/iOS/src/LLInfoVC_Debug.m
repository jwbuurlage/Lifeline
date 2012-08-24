// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#import "../include/LLInfoVC_Debug.h"

@implementation LLInfoVC
@synthesize delegate;

- (void)viewDidLoad
{
    [super viewDidLoad];
        
    self.view.backgroundColor = [UIColor whiteColor];
    
    gridView = [[LLGridView alloc] initWithFrame:
                            CGRectMake(10, 10, self.view.frame.size.width - 20, 
                                        self.view.frame.size.width - 20)];
    [self.view addSubview:gridView];
    
    UIButton* but = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    [but setFrame:CGRectMake(10, 10, 100, 40)];
    [but setTitle:@"Hide" forState:UIControlStateNormal];
    [but addTarget:self action:@selector(actionTapped) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:but];
}

- (void)actionTapped
{
    [delegate dismissViewControllerAnimated:true completion:nil];
}

- (void)setImage:(LImage*)image
{
    [gridView setImage:image];
    [gridView setNeedsDisplay];
}

@end