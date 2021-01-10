# Platformer Game: Charged

Repository dedicated to the releases of the development subject (GDDV-CITM)

In this repository we will be updating our platformer assignment for the Development subject, "Charged".

## Description

Charged is a 2D platformer in which vertical mobility is all. 

## How to Play

Simply try to reach the end of the level though jumping. Some colliders might feel off so we reccommend, if stuck, to check the debug keys in order to be able to print colliders on screen and see in a more easier manner the laydout we're colliding against.

## Key Features

### Code Related

* **Configuration loaded though XML** files:
  * Player animation.
  * Asset pathing.
  * Window configuration.
* **Map Loading** from TMX:
  * Colliders taken from object groups defined in Tiled.
  * Spawn positions for the player defined in Tiled as an object and loaded into the game through tmx.
* **Entity Loading** from XML:
  * Both animations, speed, size and position are taken from there.
* **Enemy Pathfinding**: Enemies approach the player at a certain distance.
* **Save File Detection**: Game allows the "Continue" option when a save file is detected only.
* **Menuing and settings**: Fullscreen and Volume can be modified from settings menu.
 
  
 ### Gameplay Related
  
* Double Jump
* Wall Jump (when colliding against a vertical wall jump can be performed again).
> Wall Jumping can sometimes feel strange due to collision feedback, we reccommend letting the character collide clearly and then fall a bit off the wall so a double jump can be performed comfortably again.
* Player **health** can be seen as a portrait of the character on the topleft corner.
* Player **ammo** can be seen under player health.
* Level **collectibles** can be seen on the topright corner. These pickups also work as a way for the player to recover **1 life and 5 bullets**.
* Level **timer** which shows time spent on current level.


## Bugs

* If **God Mode is activated while jumping** the player will still have gravity applied. Deactivating it and reactivating on solid ground will fix it, otherwise the player will fall into the void;
* Game **chrashes** sometimes if a collision is happening at the same time as the player collides with the end of the level.
* Entities might sometimes **travel through solid ground** if the player is detected on the opposite side.

## Controls

* A Key - Left Horizontal Movement
* D Key - Right Horizontal Movement
* O Key - Shoot in the direction the player is looking at
* Spacebar - Jump

### Debug Keys

* F1 - Go to first level
* F2 - Go to second level
* F3 - Go back to the start of current level
* F4 - Teleport to last checkpoint
* F5 - Save
* F6 - Load
* F8 - See UI boxes
* F9 - View colliders, pathfinding and logic
* F10 - God Mode
* F11 - Activate/deactivate Framerate cap (30 FPS)
> Movement in God Mode is performed with the WASD keys. God Mode deactivates once the level changes.

## Developers

* Oscar Reguera - Art & Code
> https://github.com/oscarrep

* Jan Adell - Design & Code
> https://github.com/JanAdell

## Found and Fixed bugs

* Parallax desynced from camera movement.
* The colliders that didn't exist/we were'nt able to register, were replaced by selected areas in tiled.
* When level changing, acceleration from the player during loading made subsequent spawn point be in the wrong position.
* Enemies not appearing on screen.
* Some enemies can go through colliders.
* Continue button didn't work as intended after saving the game in one session.


## Assets

* Textures for maps and player and level 1 Music. Asset pack "Warped Caves" made by **ansimuz** with music from **Pascal Belisle**.
> https://ansimuz.itch.io/warped-caves

* Level 2 Music. Techno Loop by **Furious D**.
> http://www.flashkit.com/soundfx/Instruments/Techno_l-Furious_-8051/index.php

* Checkpoint Fx
> http://freesound.org/people/GameAudio/sounds/220207/

* Enemy death sound
> https://freesound.org/people/spookymodem/sounds/249813/

* Font used was Squarified, which was turned into an Atlas and made into an xml document with **rTexPacker**
> https://raylibtech.itch.io/rtexpacker

* Graphical user interface sound effects taken from the Dev Tones section of RCP Tones (click_04 used as clicking sound, and digi_plink as hover sound)
> https://rcptones.com/dev_tones/

## Project Repository

* You can find our latest releases and updates here!

> https://github.com/JanAdell/PlatformerGame

## Find our latest Release here

> https://github.com/JanAdell/PlatformerGame/releases

## License

This project is licensed under an unmodified MIT license, which is an OSI-certified license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.




