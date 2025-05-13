# Roadmap
- [X] Load sprite patterns dynamically
- [X] Load sprite patterns allocated in different pages / banks
- [X] Define animations
- [X] Score detail
- [X] Basic moves of basic players
- [X] Net border
- [X] Game play for two players
- [X] Revisar problemas del sonido
- [X] Umpire and umpire speech bubles
- [X] Main sound samples
- [X] Welcome screen
- [X] Consistent speed with 50 and 60 Hz refresh rates
- [X] Disable 50/60 Hz hotkey and speed hotkeys (NEXTREG 0x06)
- [X] Darker shadow
- [ ] Implement the auto mode, which should reuse the decisions made by the computer
- [X] Implement exit via BREAK key
  - [X] Include a confirmation message before leaving
- [X] Basic menu
- [X] Fix icon of ball on the corner
- [X] Fix umpire chair

- [ ] Playability
  - [X] Give more margin  
  - [ ] Implement new logic to position the ball and speed at hit
  - [ ] Implement lobs by the computer when opponent close to the net
  - [ ] Implement movement of computer to better target smash
  - [ ] Adjust height of lobs to make it more challenging
  - [ ] Fix no points issues
  - [ ] Fix high vert speed issues
  - [X] Fix the computer not passing the baseline

- [X] Check the sprite flickering effect in the Next => It fails!
  - [X] Fixed... sprites, when loaded, where place at x,y = 0,0. The following are potencial improvements
  - [X] Temporarily limited bottom player to avoid flickering effect
  - [ ] Move x,y and visibility information to slots and simplify physical_coords.c
  - [ ] Define union types for sprite registers
  - [ ] Use the extra sprite frames available to implement a swapping technique
  - [ ] Interrupt in last line...
  - [ ] Ensure at least one interrupt

- [X] DMA related
    - [X] Try to disable interruptions during DMA
    - [X] Try to di / ei DMA operations
    - [X] Add cycle information to DMA operations


- [ ] Court dimensions
  - [X] Reduce court horizontal lines at least 1 pixel
  - [ ] Limit how close the player can get to the net
  
- [ ] Review joystick behaviour if player 1 joystick and player 2 goes with cursor keys... (it might be necessary to use both kempston to avoid clash with keyboard, and set cursor during menu choices or read also the joystick)

- [ ] Menu
  - [X] Start with a green or black screen to hide the layer2 noise... or load directly a layer2 in memory
  - [X] Transition effect using fast in black tilemaps
  - [ ] Add the transition effect when finishing the game as well
  - [ ] Stop the CTC timer once the transition is over
  - [X] Fix issue of not clearing the menu if pressing back
  - [ ] Fix issue of going back from redefine keys directly to main menu
  - [ ] Add credits
  - [ ] Add match length to the choices (number of sets... games)
  - [ ] Show short representation of the keyset (QAOPM), with a special char for ENTER and SPACE

- [X] Better courts
  - [X] Random choice of court by default
  - [ ] Different bounce
  - [ ] Different bounce sounds
  - [ ] More ads
  
- [ ] Better sounds
  - [ ] Different sounds top and bottom player
  - [ ] Different sound for slow speed hits
  - [ ] Different sound for the bouncing of the ball depending on the surface
  - [X] Ugh sound when serving
  - [ ] Ugh sound when fast hit in general
  - [ ] Crowd roar when good point
  - [ ] Squeak sound when changing speed
  
- [X] Simplify ball distance related issues
  - [X] Treat each axis independently (y, for instance should have more margin before the player and short behind...)

- [ ] Fix speed issues when y distance is too short or zero (avoid such situation and add safeguards)
- [ ] Revisar velocidades y precisión del saque
- [ ] Lob strike
- [X] Try pre-hit option
- [X] Touch effect (crotch hit)
- [ ] Game score in the court
- [ ] Propiedades específicas del jugador (rather than constants like SERVICE_FAST_SPEED and SERVICE_SLOW_SPEED)

- [ ] Full animation of basic players (skins basic set)
- [ ] Skins set 1
- [ ] Network play
- [ ] Skins set 2
- [ ] Score animation
- [ ] Recoge pelotas
- [ ] Limpiar código y mejorar makefile

- [ ] Optimizaciones de tamaño
  - [X] Implementar un zx7 que lee de múltiples bancos para aplicarlo a layer2
  - [X] Empaquetar más los sonidos (se pueden trocear y ocupar páginas completas)
  - [ ] Probar compresión ADPCM para el sonido
