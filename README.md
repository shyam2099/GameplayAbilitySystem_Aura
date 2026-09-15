# Aura — Top-Down ARPG (Unreal Engine 5, Gameplay Ability System)

A top-down action RPG built in Unreal Engine 5 as a deep, hands-on implementation of Unreal's **Gameplay Ability System (GAS)**. Built while following an advanced GAS-focused course, then typed and implemented independently rather than using provided project files.

![gameplay screenshot or gif here](Content/Screenshots/gameplay.gif)


## What it does

- Player and enemy abilities/spells, implemented through GAS's Ability, Attribute Set, and Gameplay Effect framework
- Damage and status effect system (e.g. burn, slow, displace, electrocute etc)
- Cooldown and resource (mana/stamina) management per ability
- Abilities built to be **network-safe**, supporting multiplayer play on a local server rather than single-player only
- Targeting system with mouse controls
- UI Integration with health and mana bars, with skill tree UI.
- Enemy AI with their own abilites

## Tech

- **Engine:** Unreal Engine 5 (UE 5.3)
- **Language:** C++
- **Key systems:** Gameplay Ability System, Gameplay Tags, Gameplay Effects, Attribute Sets, replication/networking

## Project structure

- `Source/` — C++ implementation: ability classes, attribute sets, gameplay effects, and related gameplay logic
- `Content/` — Blueprints, UI, meshes, and other UE assets
- `Config/` — project and engine configuration
- `Data/` — data assets (e.g. ability/effect data tables, if used)


## Background

This project was built while completing an advanced Unreal Engine 5 course focused on the Gameplay Ability System. The course's structure was followed, but all code was independently written and understood rather than copied from provided project files. It was built primarily as a deliberate, hands-on way to learn a system that's notoriously difficult to pick up from documentation alone.

## Author

Shyam Hrishikesan — Senior Game Programmer (Unity/C#, Unreal/C++)
[Portfolio](https://shyam2099.github.io) · [LinkedIn](https://www.linkedin.com/in/sam-2099/)
