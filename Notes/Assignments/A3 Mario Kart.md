
### CHANGES WILL BE MADE
    but this is pretty much the full extent of the idea

**Due: Feb 15th, 6pm**

**Class next week (Feb 8/9)**
- We'll cover scenes and box 2d debug rendering to help with this
- There will be plenty of time to work on this project

**Team project**
- You can work in teams of 2
- Both members have to contribute. I won't hesitate to reduce marks (as far as 0) for either member if they're not pulling their weight
- Create a new repo and share it with your other team member and me
	- Pick either of your codebases to start

**Basic Requirements**
- Should use your component framework, collision callbacks, events, etc
- No GameObject subclasses, all logic is in new components

**Core Gameplay**
- Sync physics objects to X/Z instead of X/Y
- Turn off gravity
- Kart physics
	- Create a new component to handle inputs and physics
	- Use trig based on the angle the kart is facing
		- Remember your cos/sin to turn an angle into a dir vector?
	- Calculate amount of force needed using F=ma in 2D
		- Decide on a target velocity and calculate force needed to achieve it
		- Cap the force, so there's still acceleration
		- If target is 0 when not accelerating, don't slow down too quickly
		- Allow the kart to drive backwards
		- Make the kart driving feel good

**Camera**
- You'll need a perspective camera that follows the player around
- Create a GameObject with the following components (some new)
	- Transform
	- Camera (only holds info not in transform)
	- Follow object
		- Write a "system" for this in your game code
		- It'll read the other objects transform and use it along with an offset
		- Remember the eye/at/up parameters of our function to create a view matrix
- Leave a pointer to this GameObject in your Game/Scene class
	- You'll need it to pass the camera component to your render system

**Rendering**
- Track and Kart graphics are at the bottom of this file
	- I don't have a coin or other graphics for you
- Ground is just a flat plane
	- Track is a 1x1 quad scaled up to 150x150
- Karts are double-sided quads with a different part of the texture on each side
	- Karts are 1x1 no scale
- Other objects are up to you

**Track**
- Create some roughed in static boxes for the walls
	- They don't need to match the track, just stop player from cutting corners
	- They can overlap each other

**Gimmicks**
- Use sensors and collision callbacks/events to do the following
	- Coin pickup
		- Each one you hold raises your max target velocity
		- Show a spinning coin over the players head for a second each time you pick one up
	- Speed boost pads
		- Push you in the direction of the pad
		- If you align with the pad, you get a bigger boost
			- Watch out for negative force if still pushing forward
	- Oil slicks
		- Messes with your rotation controls
		- Causes you to lose all coins

**Debug**
- Use imgui to display debug stats like:
	- velocity
	- target velocity
	- number of coins
	- etc

**Other**
- Create a vector of "Players" in your Game/Scene, so we can be ready for multiplayer

**Assets**
![[KartBowserBoth.png]]
![[KartLuigiBoth.png]]
![[KartMarioBoth.png]]
![[KartToadBoth.png]]
![[SNES_Mario_Circuit_3_map.png|300]]