# Car_number_detection
The task was to detect car number plate. The solution could be implemented two ways using image-processing approach and Computer Vision approach. For the former I used CUDA OpenCV library. I made an assumption that plate will always be rectangulare shape no matter the anlge of the picture. The project asks for the path of the image as a command line argument and processes it.

![alt text](https://github.com/NuneTadevosyan/Car_number_detection/blob/master/Imgs/car2.jpg?raw=true)

 Then I do edge detection on the image 
 
 ![alt text](https://github.com/NuneTadevosyan/Car_number_detection/blob/master/Detected_imgs/edge_detection2.png?raw=true)

 Lastly, by finding contours , detect number plate
 
 ![alt text](https://github.com/NuneTadevosyan/Car_number_detection/blob/master/Detected_imgs/Car1.png?raw=true)


For the Computer Vision approach I needed labeled and proper dataset to build a valid neural network with CuDNN , which I was not able to find on the internet.
