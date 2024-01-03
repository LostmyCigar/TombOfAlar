# TombOfAlar

_This read me exist incase of issues with the portfolio website_

Note: I will not be writing detailed information code implementation here. This will be more about the code structure and how we worked as a team.

Some things I did gameplay wise here and would live to talk about in person though are: 

  - **Projectile movement**
Targeting, Turning, Accelerating, Deaccelerating, etc
  - **Projectile Spawning**
Handle Input, Aim direction, Aim offsets, Multispawning, etc

## About the Game
Tomb of Alar's creation was dictated by deadlines. We decided early on as a group that we wanted a polished product and to achieve that, we set our own deadline for the game of 2 weeks. After those two weeks we wanted to have the game locked in feature wise so we could just focus on polishing what we had created thus far.

## This needs to be flexible, fast!
Creating the shooting for a twinstick shooter is quite hard designwise. Being the absolute core-mechanic of the game, it completely dictates whether the end product feels good to play or not. Because of this we wanted designers to be able to try out and iterate over multiple different designs for the shooting. This becomes a challenge of **teamwork** and **effective communication**. Designers were working with the tool as soon as it became possible and new features had to be created on demand. We needed clear communication on what features needed prioritization over others as well as me giving instruction on how to use everything. 

## The Good, 

Data assets for everything. Well, two things. Weapon behavior and Projectile behavior. This creates **a lot of flexibility**, the biggest being the possibility to easily swap data assets on demand, completely changing the behavior of projectiles during their lifetime. This built the possibility of creating very complex weapons since we can link projectile behaviors together by having the keep track of the next behavior and so on. Together with bullets themselves being able to create new bullets after a certain time or on destruction we can create almost any type of projectile imaginable (assuming we also have the movement required for it).


<img src="https://github.com/LostmyCigar/TombOfAlar/assets/60781151/3129e540-3349-48fe-91f4-d7798e39b447" width=50% height=55%>

_Here is how it looks for designers when they edit bullet behaviour_


## The Bad 
As someone who very much enjoys clean and maintainable code, this project pained me a bit. If I had to place it somewhere in my favorite triangle it had to be around here: 

<img src="https://github.com/LostmyCigar/TombOfAlar/assets/60781151/93463e52-2592-4d97-92a0-e201bcfa4599" align="center" width=30% height=25%>

A big chunk of the code is in the same few files and it would be hard for someone who hasn't worked in them to just jump in and take over.

I will not go into details of stuff I do not like with my code (This is a portfolio after all) but instead I'll move to a chapter that is closely related to mistakes. 

## and The Ugly
**_Actually Learnings, but that doesn't make a movie reference_**

_You can always skip this part and go straight into the **Vanagandr** tab, as that is the result of my learnings here._ 

I'll start off by saying that I do not regret my fast made spaghetti. It was what the project required and it works bug free (to the best of our knowledge). But towards the end of the project I felt how the code was beginning to catch up to me and would need a complete rewrite. 

In my previous projects I’ve always tried to do a more component-based structure of the code and this project solidified that approach for me even more. C++ was created for a reason. A weapon class should maybe handle when we shoot and what we shoot, but does it really need to know every single detail. Pack those down into small classes and let them handle themselves. The weapon class can handle a few abstract parent classes instead and tell them when to do their stuff.


## Working with Designers
As soon as designers could start working with shooting I created a small guide on how it all worked and how to create new “weapons” in blueprints. This was so that designers could have something to follow while working and did not require me to repeat explanations multiple times. After a while this guide became outdated, but at that point everyone working with weapons had a grasp on how they worked so that it was no longer needed. 

![ToA_BlueprintGuide](https://github.com/LostmyCigar/TombOfAlar/assets/60781151/1d67339c-cb65-413f-9918-c10dcc36388b)

