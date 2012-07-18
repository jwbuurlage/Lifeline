class WritepadController < NSObject
  attr_reader :paths
  
  def init
    super
    
    @recog = Recognizer.new
    
    @paths ||= []
    @data ||= []
    
    @bp = nil
    
    self
  end
  
  def send 
    @recog.set_data @data
    
    @data = []
    @paths = []
    
  end
  
  def touchDown(pad, event:event)
    touch = event.touchesForView(pad).anyObject
    point = touch.locationInView pad 
    
    @data << [point.x, point.y]
    
    @bp = UIBezierPath.alloc.init
    @bp.lineWidth = 5.0
    @bp.moveToPoint point
    
    @paths << @bp
        
    @prevPoint = point
  end
  
  def touchDragInside(pad, event:event)
    touch = event.touchesForView(pad).anyObject;
    point = touch.locationInView pad;
    
    @data << [point.x, point.y]
    @bp.addLineToPoint point
    
    pad.setNeedsDisplay
  end
  
  def touchUpInside(pad, event:event)
    previousPoint = CGPointZero
  end
  
  def delegate=(delegate)
    @recog.set_delegate delegate
  end
end

class Writepad < UIControl
  def initWithFrame(frame)
    super
    
    self.backgroundColor = UIColor.grayColor
    
    @controller = WritepadController.alloc.init
    
    self.addTarget(@controller, action:"touchDown:event:", forControlEvents:UIControlEventTouchDown);
    self.addTarget(@controller, action:"touchDragInside:event:", forControlEvents:UIControlEventTouchDragInside);
    self.addTarget(@controller, action:"touchUpInside:event:", forControlEvents:UIControlEventTouchUpInside);
      
    button = UIButton.buttonWithType(UIButtonTypeRoundedRect)
    button.frame = [[10, 10], [100, 40]]
    button.setTitle("Send", forState:UIControlStateNormal)
    button.addTarget(self, action:'buttonClicked', forControlEvents:UIControlEventTouchUpInside)
    self.addSubview button
  end
  
  def delegate=(delegate)
    @controller.delegate = delegate
  end
  
  def buttonClicked
    @controller.send
  end
  
  def drawRect(rect)
    UIColor.lightGrayColor.set
    UIBezierPath.bezierPathWithRect(rect).fill;
    
    @controller.paths.each do |path|
      UIColor.blackColor.set
      path.stroke
    end
  end
end