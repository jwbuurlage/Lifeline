class DocumentVC < UIViewController
  def viewDidLoad
    w = UIScreen.mainScreen.bounds.size.width
    h = UIScreen.mainScreen.bounds.size.height
    
    view.backgroundColor = UIColor.whiteColor
    
    @pad = Writepad.alloc.initWithFrame [[0, h/2], [w, h/2]]
    view.addSubview @pad
    @pad.delegate = self
    
    @gridView = GridView.alloc.initWithFrame [[110, 30], [100, 100]]
    @gridView.image = Image.new 10
    view.addSubview @gridView
  end
  
  def image_ready(image)
    @gridView.image = image
    @gridView.setNeedsDisplay
  end
end