class Image
    def initialize(size) 
      @data = Array.new(size*size, 0)
      @size = size
    end
    
    def size
      @size
    end
    
    def set_pixel(i, j, value)
      @data[i * size + j] = value
    end
    
    def pixel(i, j)
      @data[i * size + j]
    end
    
    def inspect
      str = ""
      for i in 0...@size
        for j in 0...@size
          str += @data[i * @size + j].to_s
        end
      str += "\n"
      end
      str
    end
end

class Recognizer
  def initialize
    @delegate = nil
  end
  
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
  
  # Taking the average of all the points as the center of the image.
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
      puts @data[i]
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
    
    puts "x_min #{x_min}, x_max #{x_max}, y_min #{y_min}, y_max #{y_max}"
    
    
    x_norm = x_max.abs
    y_norm = y_max.abs
    if x_min.abs > x_max.abs; x_norm = x_min.abs; end
    if y_min.abs > y_max.abs; y_norm = y_min.abs; end
    
   puts "x_norm #{x_norm}, y_norm #{y_norm}"
    
    norm = x_norm
    if y_norm > x_norm; norm = y_norm; end
    
    puts "norm #{norm}"
    
    @data.each_with_index do |(x,y), i|
      @data[i] = [x/norm, y/norm]
    end
    
   p @data
  end
  
  def connect()
  
  end
  
  def make_image()
    imag = Image.new(17)
      
      iv = 2.0/imag.size
      for i in 0...imag.size
        for j in 0...imag.size
          @data.each do |(x,y)|
            if ((-1 + i * iv <= y and -1 + (i + 1) * iv >= y) and
                (-1 + j * iv <= x and -1 + (j + 1) * iv >= x))
              imag.set_pixel(i,j,1) 
            end
          end
        end
      end 
    @delegate.image_ready imag if @delegate
    p imag
  end
end
#data = [[1.0,0.0],[0.0,1.0]]
data = [[2.0, 0.0], [0.0, 2.0],[0.0,-2.0],[-2.0,0.0],[3.0,4.0],[3.0,-4.0]]
#data = [[1.0,4.0],[8.0,5.0],[7.0,13.0],[12.0,1.0],[19.0,3.0]]
recog = Recognizer.new
recog.set_data data
