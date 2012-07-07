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
    
    // DEBUG
    gridView = [[_LLGridView alloc] initWithFrame:CGRectMake(300, 200, 200, 200)];
    [self.view addSubview:gridView];
    
    
    CGRect resultFrame = CGRectMake(500, 300, 200, 200);
    results = [[UITextView alloc] initWithFrame:resultFrame];
    results.text = @"";
    results.font = [UIFont boldSystemFontOfSize:12.0f];
    [self.view addSubview:results];
}


- (void)bestMatch:(char)character
{
    NSString* appendedText = [textView.text stringByAppendingFormat:@"%c", character];
    textView.text = appendedText;
}

- (void)sourceImage:(LImage*)src
{
    [gridView setImage:src];
    [gridView setNeedsDisplay];
}

- (void)resultSet:(LResultSet*)result
{
    results.text = @"";
    ListElement* element = result->matchData.head;
    do
    {
        LMatchData* matchData = (LMatchData *)element->data;
        
        results.text = [results.text stringByAppendingFormat:@"%c; %f \n", matchData->character, matchData->score];
        element = element->next;
    } while(element);
    
    list_destroy(&(result->matchData));
    free(result);
}


// C Wrappers for LLListener protocol
// ----------------------------------
// Alternative is making LLListener a class, and calling [LLListener characterFound] here.
// Might not be shorter / prettier.

void bestMatch(char best_char, void* obj)
{
    [(__bridge LLDocumentViewController*)obj bestMatch:best_char];
}

void sourceImage(LImage* src, void*obj)
{
    [(__bridge LLDocumentViewController*)obj sourceImage:src];
}

void resultSet(LResultSet* result, void*obj)
{
    [(__bridge LLDocumentViewController*)obj resultSet:result]; 
}

- (LListener)C_LListener
{
    LListener listener;
    listener.char_found = bestMatch;
    listener.source_image = sourceImage;
    listener.result_set = resultSet;
    listener.obj = (__bridge void*)self;
    return listener;
}

@end