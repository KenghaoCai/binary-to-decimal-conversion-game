# binary-to-decimal-convertion-game
Random binary to correct decimal conversion game:
Keypad Inputs:
0 - 9 = Decimal inputs

A = Easy Mode - 4-bits - 15 points 
B = Medium - 6-bits - 63 points
C = Hard - 8-bits - 255 points
(My point scale might seem weird but it is just representing the number of possible numbers you might encounter for each mode.)

' * ' = Start/Restart
' # ' = Next/Skip
' D ' = Done (when you are finished inputting the desired decimal number)

Rules:
Once the game starts, you will be given 5 rounds. During each round, a random binary number will be generated within the difficulty level you have chosen. 
Next, you are given 10 seconds to input a decimal that you believe is the correct value and press ' D'. If answered correctly, 
you will be rewarded with the proper score, however, if failed, the round will be lost and no points are rewarded. 

Changing the game difficulty during a game is allowed with a consequence of losing your chance to gain any points for that round.

# technicals
The game is developed using an ATmega 328P microcontroller, and the display is controlled by an Arduino Uno. The only information passed between the two platforms are 
score information, which is from the ATmega 328P to the Arduino Uno.

# Video demo (YouTube): https://youtu.be/v7KP_-BDDy4
