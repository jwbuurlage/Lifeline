# ----------------------------------------------------------------------------
# BYN SOFTWARE, COPYRIGHT 2012
# 
# Author: Jan-Willem Buurlage et al.
# Contact: j.buurlage@students.uu.nl
# 
# Part of the Lifeline project, handwriting recognition for scientific wr-
# iting. Tailored for touchscreens.
# ----------------------------------------------------------------------------

class AppDelegate
  def application(application, didFinishLaunchingWithOptions:launchOptions)
    puts return_value(2) # test with c-lib
    
    application.statusBarStyle = UIStatusBarStyleBlackOpaque
     
    @window = UIWindow.alloc.initWithFrame(UIScreen.mainScreen.bounds)
    @window.rootViewController = LLDocumentVC.alloc.init
    @window.makeKeyAndVisible
    
    true
  end
end
