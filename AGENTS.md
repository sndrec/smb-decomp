# Codex Agent Instructions

This repository aims to build **libmkb**, a standalone C/C++ library that can simulate the physics, camera, and control logic of **Super Monkey Ball**. These are the guiding principles for all future contributions:

## Goals
- **Readability**: produce clear, well-documented code that faithfully reflects the original game's logic so that it can be easily understood.
- **Accuracy**: the resulting library should behave identically to the original game aside from unavoidable floating-point rounding differences.
- **Embeddability**: the core logic should compile as a reusable C++ library for use in custom frontends. Compiling libmkb for the browser via WebAssembly with three.js for graphics is the endgoal for an initial frontend.

## Non-Goals
- **Perfect matching** of the original binary is not required if it harms readability or embeddability.
- **Full completeness** (e.g. menus, graphics, character animation) is not a concern. Focus on physics, camera, and controls.

Codex agents should follow these principles when decompiling, refactoring, or adding functionality.
