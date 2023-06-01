# PhotoFrame-Arduino
The project consists of a digital photo frame primarily designed to display images at regular intervals. The photo frame mainly consists of a screen where the image slideshow will be displayed and a microSD card for storing the images. The user only needs to interact with the memory card, which allows them to upload a limited number of photos depending on the available card memory. The slideshow starts automatically when the photo frame is connected to a power source.

Additionally, the photo frame can also function as a simple digital thermometer to indicate the ambient temperature. This functionality is activated by pressing a button that pauses the image slideshow and displays the temperature on the screen. To resume the slideshow, the user can press the button again.![prpm](https://github.com/Raluca14/PhotoFrame-Arduino/assets/113622449/fe5feb74-1c5e-4ed4-bb24-3ce7e64eae2b)

Hardware Design

The hardware components used for this project are:

Arduino UNO compatible development board
1.18" LCD screen (ST7735)
MicroSD Card Adapter + microSD card
Temperature sensor (AHT2x)
Button
Breadboard
Wires
The LCD screen and microSD adapter use the SPI communication protocol, while the temperature sensor uses I2C.
![image](https://github.com/Raluca14/PhotoFrame-Arduino/assets/113622449/527db521-b368-45e7-ad97-cfb340c48121)

The buttons are connected to the following pins:

Button 1 - Pin 3
Button 2 - Pin 5
Button 3 - Pin 6
Button 4 - Pin 7

Software Design 

The software part of this project was developed in the Arduino IDE. A significant part of the software implementation involved correctly utilizing the libraries and functions specific to each component.

The libraries used are:

- SD.h (for working with an SD card)
- SPI.h
- Adafruit_GFX.h
- Adafruit_ST7735.h (to interact with the LCD screen, along with the above two libraries)
- Wire.h
- AHTxx.h (this library was obtained from a Github resource and, along with the previous library, was used to interact with the temperature and humidity sensor)

Loading images from the SD card to the screen is done using a special function called bmpDraw . The function takes the image file name to be converted and an Integer attribute called "filter" that identifies the filter to be applied to the current image. If no filter is selected, the image will be displayed as it appears on the SD card. The function performs preliminary checks on the image, such as verifying the format, dimensions, and bit depth. It then converts the pixels into a format accepted by the LCD screen and displays each pixel on the screen. Loading an image takes approximately one second, depending on its complexity.

At a certain point, the function extracts the pixels of the current image in RGB format, on three color channels. At this stage, color filters were implemented based on the "filter" attribute. These filters are implemented by manipulating the color channels.

In the loop function, checks are made for the states of each button. An if statement branches the program into two paths: one for reading and displaying the temperature and one for scrolling through the images. In both paths, the state of button 1 is checked (this is accompanied by the LED part explained in the User Interface). Filters can only be accessed during the image scrolling, so the button checks for filters are only performed in the corresponding image scrolling path.

