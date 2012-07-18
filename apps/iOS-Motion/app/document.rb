# ----------------------------------------------------------------------------
# BYN SOFTWARE, COPYRIGHT 2012
# 
# Author: Jan-Willem Buurlage et al.
# Contact: j.buurlage@students.uu.nl
# 
# Part of the Lifeline project, handwriting recognition for scientific wr-
# iting. Tailored for touchscreens.
# ----------------------------------------------------------------------------

class LLDocumentVC < UIViewController
  def viewDidLoad
    view.backgroundColor = UIColor.whiteColor
    
    # Writepad view
    @writepad = LLWritepadControl.alloc.initWithFrame([[0, 0], [200, 200]])
    recognizer_register_listener(defaultRecognizer, self)
  end
  
  def finishedStroke(pad)
    puts "stroke finished"
  end
end