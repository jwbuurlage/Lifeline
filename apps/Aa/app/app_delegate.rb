class AppDelegate
  def application(application, didFinishLaunchingWithOptions:launchOptions)
    application.statusBarStyle = UIStatusBarStyleBlackOpaque
          
    @window = UIWindow.alloc.initWithFrame(UIScreen.mainScreen.bounds)
    documentVC = DocumentVC.alloc.init
    @window.rootViewController = documentVC
    @window.makeKeyAndVisible
    
    true
  end
end
