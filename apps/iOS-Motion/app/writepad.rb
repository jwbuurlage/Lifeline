# ----------------------------------------------------------------------------
# BYN SOFTWARE, COPYRIGHT 2012
# 
# Author: Jan-Willem Buurlage et al.
# Contact: j.buurlage@students.uu.nl
# 
# Part of the Lifeline project, handwriting recognition for scientific wr-
# iting. Tailored for touchscreens.
# ----------------------------------------------------------------------------

class LLHandwritingController
  attr_accessor :delegate
  
  def init
    @touches ||= []
    
  end
  
  def finishedStroke(pad)
    # fix up the strokes // normalize
    stroke = [1, 2, 3, 4]
    
    # send to lib
    recognizer_send_stroke(defaultRecognizer, stroke)
    
    #empty touches
    touches.replace []
  end
end

class LLWritepadControl < UIControl
  def initWithFrame(frame)
    super
    
    @controller = LLHandwritingController.alloc.init
    self.addTarget(@contorller, 
                    action:"finishedStroke:", 
                    forControlState:UIControlStateTouchFinished)
    
    self.backgroundColor = UIColor.grayColor
  end
  
  def setDelegate(delegate)
    @controller.delegate = delegate
    
  end
  
  def beginTrackingWithTouch(touch, withEvent:event)
    
  end
  
  def continueTrackingWithTouch(touch, withEvent:event)
    
  end
  
  def endTrackingWithTouch(touch, withEvent:event)
    
  end
end