# Windows Programming Laboratory works

## Title
Window. Window handling. Basic window’s form elements

## Implemented Tasks
I did the laboratory work in Code::Blocks 13.12 IDE.

**_Basic Level_**
- Create a Windows application
- In the middle of the window is present the following text: "Done with Pride and Prejudice by Oxana
- On windows resize, text reflows and be in window's middle (vertically and horizontally)

**_Normal Level_**
- Add 2 buttons to window: one with default styles, one with custom styles (size, background, text color, font family/size)
- Add 2 text elements to window: one with default styles, one with custom styles (size, background, text color, font family/size)
    
**_Advanced Level_**
- Make elements to interact or change other elements (2 different interactions)
- Change behavior of different window actions (at least 3)

## Features
- By clicking <b>Send</b> button, the text from inputArea is sent to outputArea. 
- By clicking <b>Reset</b> button, the text from outputArea is deleted. 
- By clicking <b>Change Color</b> button, the color of the text in outputArea is changed. There
are 5 font colors and when you click more times, the color changes from those 5 defined in the
program.
- By clicking <b>Background</b> button, the background color of the outputArea is changed. There
are 5 background colors and when you click more times, the color changes from those 5 defined in
the program. 
- By clicking <b>Change font</b> button, the font of the text in the outputArea is changed. There
are 5 fonts when you click more times, the font changes from those 5 defined in the program.
- By clicking <b>Minimize</b> button, an information box appears, telling that ”Minimize button
doesn’t work. 
- By clicking <b>Maximize button</b>, the window moves to a random location on desktop. 
- By clicking <b>Close</b> button, a message appears which waits for user’s response(YES/NO) to exit
from the program. 
- Elements fit window on resize

###Program Overview

### Conclusions
During this laboratory work I learned the basis of windows programming, for example how
to create a button and a text element, how to make interactions between elements, how to change
the properties of some elements and make them with custom styles and how to change behaviour of
window actions.

During the work I also found some problems which I managed to fix and some which I couldn’t. 
The first one is that when I press Reset button, I want to clear both inputArea and outputArea, but because outputArea
is a read-only element, when I press Reset it clears the message, but also makes it colored, which is a bug.
The second problem is that I want to keep the background while I change the font and text color
and vice versa, I want to keep the text color while I change the background. I tried some methods
for this, but anyway I can keep only one thing at a time.

This laboratory work has helped me understand how a GUI can be created and what is event-driven programming.

