class GridView < UIView
  def image=(image)
    @image = image
  end
  
  def drawRect(rect)
    # draw stuff
    n = @image.size # size
    ppr = self.frame.size.width / n.to_f # pixels per rect
    
    UIColor.whiteColor.set
    UIBezierPath.bezierPathWithRect(rect).fill
    
    UIColor.grayColor.set
    UIBezierPath.bezierPathWithRect(rect).stroke
    
    for i in 0...n
      for j in 0...n
        pix = [[ppr * j, ppr * i], [ppr, ppr]]
        if @image.pixel(i, j) == 1
          UIColor.blackColor.set
          UIBezierPath.bezierPathWithRect(pix).fill
        end
        
        UIColor.grayColor.set
        UIBezierPath.bezierPathWithRect(pix).stroke
      end
    end
  end
end