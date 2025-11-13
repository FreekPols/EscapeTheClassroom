---
title: An affordable and adaptable educational escape room
abstract: some abstract here
---

# Introduction
- engaging students difficult
- gamification, escape rooms
- solution in education? efforts?
- challenges: cost, setup time, adaptability
- here we outline ...
- use it yourselves, or let your students build one based on this idea: resulting in an entertaining experience and a learning experience in electronics and programming

# Background
- some info on the educational escape rooms

https://www.instructables.com/id/Escaperoom

# The design

## Outline of the escape room
- box with buttons and lcd screen
- puzzles to solve, input codes
- indication of level of completion
- hints should be possible, but not by asking teacher
- timer
- resistors
- tea box, size of ...



## Materials and setup

The entire setup is designed to be affordable and easy to assemble using commonly available components.
We used a tea box with dimensions of approximately 20 x 15 x 10 cm as the enclosure for the electronics.
However, any box of similar size can be used - it might be worthwhile to visit the local second-hand store to find a suitable box.
We disassembled the lid of the box and used a laser cutter to create holes for the buttons, leds, buzzer, and LCD screen (the source file is included in the supplementary files), see figure {numref}`figure {number} <fig_top>`.

````{figure}
:label: fig_top
```{figure} top1.*
:label: fig_top1

Top view of the tea box with holes for buttons, leds, buzzer, and LCD screen.
```
```{figure} top2.*
:label: fig_top2

Top view of the tea box with holes for buttons, leds, buzzer, and LCD screen.
```
````



For this project, we used the Arduino Uno R3 as the main microcontroller.
It is both affordable, widely available and has enough input and output pins to connect all the necessary components. 

We included four 16 mm panel mount push buttons for user input.
Using a series circuit with resistors, where each button is connected to a different resistor value, we can read the button presses using a single analog input pin on the Arduino, see {numref}`figure {number} <fig_circuit>`.
This reduces the number of required pins and simplifies the wiring.
Moreover, this system allows to easily add more buttons if needed.

```{figure} circuit.*
:label: fig_circuit

Wiring diagram of the button input circuit using resistors. [circuit](@https://www.circuit-diagram.org/editor/c/5fd81e68c4b84dc39689d8260aeaba49)
```

Four green leds are required to indicate the progress of the team (though more leds can be added, optionally using a multiplexer to reduce the number of required pins). 
A red led is used to indicate a wrong code entry, or running out of time.
A buzzer is used to sound an alarm or, when complete, a victory tune.

In this project we used the internal clock of the Arduino to keep track of time.
Optionally, a real time clock (RTC) module can be added to keep more accurate time, especially when the device is powered off and on again.

An LCD screen is used to display the timer, hints, and other information to the players.
We opted for the LCD with integrated I2C module, which reduces the number of required pins to just two (SDA and SCL), and off course power and ground.

Finally, a battery pack and on/off switch are used to power the entire setup, making it portable and easy to set up anywhere.




## Code



# Implementation



# Conclusion