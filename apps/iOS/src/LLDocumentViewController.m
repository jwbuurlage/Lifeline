// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#import "LLDocumentViewController.h"
#import "LLWritepad.h"

@implementation LLDocumentViewController


- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
    self.view.backgroundColor = [UIColor whiteColor];
    
    CGRect padFrame = CGRectMake(0, 
                                 self.view.frame.size.height / 2, 
                                 self.view.frame.size.width, 
                                 self.view.frame.size.height / 2);
    
    CGRect textFrame = CGRectMake(20, 20, 500, 200);
    textView = [[UITextView alloc] initWithFrame:textFrame];
    textView.text = @"";
    textView.font = [UIFont boldSystemFontOfSize:24.0f];
    
    LLWritepad* writepad = [[LLWritepad alloc] initWithFrame:padFrame
                                                    delegate:self];
    
    
    [self.view addSubview:textView];
    [self.view addSubview:writepad];
}


- (void)characterFound:(char)character
{
    NSString* appendedText = [textView.text stringByAppendingFormat:@"%c", character];
    textView.text = appendedText;
}

////////////////
// C Wrappers //
////////////////

// Alternative is making LLListener a class, and calling [LLListener characterFound] here.
// Might not be shorter.

void characterFound(char char_found, void* obj)
{
    LLDocumentViewController* docView = (__bridge LLDocumentViewController*)obj;
    [docView characterFound:char_found];
}

- (Listener)c_listener
{
    Listener listener;
    listener.char_found = characterFound;
    listener.obj = (__bridge void*)self;
    return listener;
}

@end