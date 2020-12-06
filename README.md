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
  
 ### Gameplay Related
  
* Double Jump
* Wall Jump (when colliding against a vertical wall jump can be performed again).
> Wall Jumping can sometimes feel strange due to collision feedback, we reccommend letting the character collide clearly and then fall a bit off the wall so a double jump can be performed comfortably again.
* Player health and ammo can be found in the app bar alongside FPS and VSYNC.

## Bugs

* If God Mode is activated while jumping the player will still have gravity applied. Deactivating it and reactivating on solid ground will fix it.

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
* F9 - View colliders and logic
* F10 - God Mode
* F11 - Activate/deactivate Framerate cap to 30
> Movement in God Mode is performed with the WASD keys. God Mode deactivates once the level changes.

## Developers

* Oscar Reguera - Art & Code
> https://github.com/oscarrep

* Jan Adell - Design & Code
> https://github.com/JanAdell

## Future Implementations

* Charged jump from any surface in any direction, using mouse coords as target/base.

## Found and Fixed bugs

* Parallax desynced from camera movement.
* The colliders that didn't exist/we were'nt able to register, were replaced by selected areas in tiled.
* When level changing, acceleration from the player during loading made subsequent spawn point be in the wrong position.
* Enemies not appearing on screen.
* Some enemies can go through colliders.



## Assets

* Textures for maps and player and level 1 Music. Asset pack "Warped Caves" made by **ansimuz** with music from **Pascal Belisle**.
> https://ansimuz.itch.io/warped-caves

* Level 2 Music. Techno Loop by **Furious D**.
> http://www.flashkit.com/soundfx/Instruments/Techno_l-Furious_-8051/index.php

* Checkpoint Fx
> http://freesound.org/people/GameAudio/sounds/220207/

* Enemy death sound
> https://freesound.org/people/spookymodem/sounds/249813/

## Project Repository

* You can find our latest releases and updates here!

> https://github.com/JanAdell/PlatformerGame

## Find our latest Release here

> https://github.com/JanAdell/PlatformerGame/releases

## License

This project is licensed under an unmodified MIT license, which is an OSI-certified license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.




