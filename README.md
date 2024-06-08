# Arcane Engine

Arcane Engine is a 2D game engine created for the Programming 4 course in Digital Arts and Entertainment. The engine uses the SDL library to handle the rendering of images and text, and is also used for the input and sound systems.

This repository focusses on the Game Engine itself, however inside the package it includes a remake of the game Galaga to provide an example project.

## Features

- **Component System**: A non-inheritance based component system that allows for easy and efficient game object management.
- **Scenegraph**: A hierarchical structure for handling ownership and spatial relationships between game objects.
- **Easy to Use Input System**: Comprehensive support for keyboard, mouse and controller inputs, simplifying user interaction handling.
- **Observer Pattern**: A design pattern implementation to facilitate communication between game objects and systems, promoting a decoupled architecture.
- **Service Locator for Sound**: A global access point for sound services, making it easy to manage and play audio throughout the game.
- **State Machine**: A simple state machine system for managing game states, enhancing control over game flow and logic.
- **2D Box Overlap Handler**: Handling of 2D box collisions for that broadcasts overlap Events to both overlapped objects.
