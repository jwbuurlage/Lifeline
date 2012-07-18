class Image
    def initialize(size) 
      @data = Array.new(size*size, 0)
      @size = size
    end
    
    def size
      @size
    end
    
    def pixel(i,j)
      @data[i*size + j]
    end
    
    def set_pixel(i, j, value)
      @data[i*size + j] = value
    end
    
    def inspect
      @data.inspect
    end
end

class Recognizer
  def set_delegate(delegate)
    @delegate = delegate
  end
  
  def set_data(data)
    @data = data
    normalize
    connect
    make_image
  end

  def normalize()
    x_center = 0
    y_center = 0
    
    @data.each do |(x,y)|
      x_center += x
      y_center += y
    end
    
    x_center /= @data.length
    y_center /= @data.length
    
    @data.each_with_index do |(x, y), i|
      @data[i] = [x - x_center, y - y_center]
    end
    
    # We want to normalize the dataset, so this means taking the taking the maximum absolute value of the x and y points respectively and then dividing each component of a point by that number.
    x_max = -1000
    x_min = 1000
    y_max = -1000
    y_min = 1000
    
    @data.each do |(x,y)|
       if x > x_max; x_max = x; end
       if x < x_min; x_min = x; end
       if y > y_max; y_max = y; end
       if y < y_min; y_min = y; end       
    end
    
    x_norm = x_max.abs
    y_norm = y_max.abs
    if x_min.abs > x_max.abs; x_norm = x_min.abs; end
    if y_min.abs > y_max.abs; y_norm = y_min.abs; end
    
    @data.each_with_index do |(x,y), i|
      @data[i] = [x/x_norm, y/y_norm]
    end
    
    p @data
  end
  
  def connect()
  
  end
  
  def make_image()
    imag = Image.new(13)
      
      iv = 2.0/imag.size
      for i in 0...imag.size
        for j in 0...imag.size
          @data.each do |(x,y)|
            if ((-1 + i * iv < x and -1 + (i + 1) * iv > x) and
                (-1 + j * iv < y and -1 + (j + 1) * iv > y))
              imag.set_pixel(i,j,1) 
            end
          end
        end
      end
    @delegate.image_ready imag
  end
end
