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
    float h = self.view.frame.size.height;
    float w = self.view.frame.size.width;
    
    [super viewDidLoad];
	// Do any additional setup after loading the view.
    self.view.backgroundColor = [UIColor whiteColor];
        
    // TextView
    CGRect textFrame = CGRectMake(20, 20, 500, 200);
    textView = [[UITextView alloc] initWithFrame:textFrame];
    textView.text = @"";
    textView.font = [UIFont boldSystemFontOfSize:24.0f];
    [self.view addSubview:textView];

    // WritePad
    CGRect padFrame = CGRectMake(0, h / 2, w, h / 2);
    LLWritepad* writepad = [[LLWritepad alloc] initWithFrame:padFrame
                                                    delegate:self];
    [self.view addSubview:writepad];
    
    // DEBUG, should make this a seperate view I think
    infoDebug = [[LLInfoVC alloc] initWithNibName:nil bundle:nil];
    infoDebug.delegate = self;
    
    UIButton* but = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    [but setFrame:CGRectMake(w - 110, h/2 - 50, 100, 40)];
    [but setTitle:@"Debug" forState:UIControlStateNormal];
    [but addTarget:self action:@selector(showInfo) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:but];
        
    CGRect resultFrame = CGRectMake(10, 10, 500, 300);
    results = [[UITextView alloc] initWithFrame:resultFrame];
    results.text = @"Nothing to see here, move along.";
    results.font = [UIFont boldSystemFontOfSize:14.0f];
    [self.view addSubview:results];
}

- (void)showInfo
{
    [self presentViewController:infoDebug animated:true completion:^{ [infoDebug setImage:_image]; }];
}

- (void)bestMatch:(char)character
{
    NSString* appendedText = [textView.text stringByAppendingFormat:@"%c", character];
    textView.text = appendedText;
}

- (void)sourceImage:(LImage*)src
{
    if(_image != 0) {
        if(_image->grid != 0)
            free(_image->grid);
        free(_image);
    }

    _image = src;
}

- (void)resultSet:(LResultSet*)result
{
    results.text = @"";
    ListElement* element = result->head;
    do
    {
        LMatchData* matchData = (LMatchData *)element->data;
        
        results.text = [results.text stringByAppendingFormat:@"%c; %f \n", matchData->character, matchData->score];
        element = element->next;
    } while(element);
    
    list_destroy(result);
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