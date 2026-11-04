# Practical 7: The Command Pattern

In this practical you'll split "which key was pressed" from "what the player does". Every key press becomes a **command**, commands are packed into a single number using **bit flags**, and a **mediator** hands them to everything that needs them: the player, and a HUD that shows what's happening. Your job is to add new commands, such as DODGE and DASH, and to combine them.

By the end you'll understand how games turn input into actions, why that split makes controls easy to change, and how one number can hold several commands at once.

> The project was generated with [raylib project creator](https://raysan5.itch.io/raylib-project-creator) by raysan5.

---

## Short version

If you just want to get going, this is the whole thing in a few lines:

**1. Clone**

```bash
mkdir -p ~/Projects
cd ~/Projects
git clone https://github.com/MuddyGames/command_pattern.git
```

**2. Set up raylib**

*Linux:*

```bash
cd ~/Projects/command_pattern/setup
bash setup_linux.sh
```

or make the script executable and run it directly:

```bash
cd ~/Projects/command_pattern/setup
chmod +x setup_linux.sh
./setup_linux.sh
```

*Windows* (install [raylib](https://raysan5.itch.io/raylib) first, and then use w64devkit terminal):

```bash
cd ~/Projects/command_pattern/setup
sh setup_windows.sh
```

**3. Build and run**

```bash
cd ~/Projects/command_pattern
make run                            # desktop
```

The rest of this page explains each step, and what to do when the computer says no.

---

## Step 1: Get the project

Keep all your work in a `Projects` folder in your home directory.

- **Linux:** open a terminal.
- **Windows:** open the **w64devkit** terminal (`C:\raylib\w64devkit\w64devkit.exe`).

Then type the same commands on both:

```bash
mkdir -p ~/Projects
cd ~/Projects
git clone https://github.com/MuddyGames/command_pattern.git
```

Your project now lives in `~/Projects/command_pattern`.

---

## Step 2: Install raylib and Emscripten (once only)

If you set these up for an earlier practical, skip this step. They're shared by every raylib project.

### Linux (Debian, Ubuntu, Pop!_OS or Arch)

```bash
cd ~/Projects/command_pattern/setup
bash setup_linux.sh
```

It will ask for your password to install packages. It takes a few minutes, especially on lab machines.

### Windows

Install the **[raylib installer](https://raysan5.itch.io/raylib)** and keep the default location, `C:\raylib`. Then in the **w64devkit** terminal:

```bash
cd ~/Projects/command_pattern/setup
sh setup_windows.sh
```

When it finishes, close w64devkit and open it again.

> No more MSYS2. Everything now runs in the w64devkit terminal that comes with the raylib installer.

For the manual steps, see the [drawing_primitives README](https://github.com/MuddyGames/drawing_primitives#manual-setup-only-if-the-script-didnt-work-for-you).

---

## Step 3: Build and run

Every command runs from the project folder:

```bash
cd ~/Projects/command_pattern
```

### On your desktop

The same commands work on Linux and Windows (w64devkit):

| Command | What it does |
|---|---|
| `make` | Builds the game |
| `make run` | Builds it and runs it |
| `make BUILD_MODE=DEBUG` | Builds with debug info, for stepping through in the debugger |
| `make clean` | Deletes the `build` folder, so the next build starts fresh |

A window should open with a player you can move with the keyboard or a controller. Try moving and jumping at the same time.

### In your browser

```bash
make run_web
```

This builds the web version, starts a small local web server and opens
**<http://127.0.0.1:8000/command_pattern.html>** in your browser.

Press **Ctrl+C** in the terminal to stop the server.

| Command | What it does |
|---|---|
| `make web` | Builds the web version only |
| `make run_web` | Builds it and opens it in your browser |
| `make stop_web` | Stops a web server left running from earlier |

### In VS Code

Open `projects/VSCode/main.code-workspace` (**File → Open Workspace from File…**).

- **Ctrl+Shift+B** builds.
- **F5** builds and starts the debugger. The first time, choose **Debug** in the Run and Debug drop-down.
- **Terminal → Run Task** lists Run, Run Web, Stop Web Server and Clean.

Set a breakpoint in `ExecuteCommand()` in `command.c`, then hold two keys and look at `command` in the debugger. Right-click it and choose a binary or hex view to see exactly which bits are set.

---

## How the project is laid out

```
command_pattern/
├── Makefile               build instructions
├── src/
│   ├── main.c             opens the window, runs the game loop
│   ├── input_manager.c    turns key, mouse and button presses into commands
│   ├── mediator.c         passes each command to the player and the HUD
│   ├── command.c          ExecuteCommand(): which player action each command triggers
│   ├── player.c           what the player can do (move, jump, fire…)
│   ├── hud.c              shows the active commands on screen
│   └── game.c             sets up, updates and draws the game
├── include/
│   ├── command.h          the command bit flags and command functions
│   ├── input_manager.h    input polling
│   ├── mediator.h         the mediator
│   ├── player.h           Player struct and actions
│   ├── hud.h              Hud struct
│   ├── game.h             game functions
│   └── constants.h        screen size, FPS and other constants
├── resources/             sprites and images
├── build/                 everything make creates
├── platform/              icons and web page template; leave these alone
└── setup/                 setup scripts (run once)
```

Each frame, `main.c` calls `UpdateGame()`, then `DrawGame()`. `UpdateGame()` is where input turns into action:

```c
void UpdateGame(GameData *data, float deltaTime)
{
    data->activeCommand = PollInput();                                        // keys and buttons -> command bits
    MediatorHandleCommand(data->mediator, data->activeCommand, deltaTime);    // command bits -> player actions
}
```

The mediator then passes the command to its two **colleagues**:

```c
void MediatorHandleCommand(Mediator *mediator, Command command, float deltaTime)
{
    ExecuteCommand(command, mediator->player, deltaTime);   // 1. the player acts on it
    UpdateHud(mediator->hud, command);                      // 2. the HUD shows it
}
```

Two patterns, two separations:

- **Command:** `input_manager.c` knows nothing about the player, and `player.c` knows nothing about keys. That's why you can change the controls without touching the player, or give the same commands to an AI instead of a keyboard.
- **Mediator:** the player and the HUD know nothing about each other. The mediator is the only place that knows about both. Want a sound when the player jumps? Add a third colleague to the mediator. The player and the HUD don't change.

---

## Input to action

[![](https://mermaid.ink/img/pako:eNqdlF1vmzAUhv-K5d60GknNZ4BFkZqPSvuIVrW72tiFA8cEzdiRMW2yJP99DhCW3kWDG3z8vC_nHDje41RmgGM8GAwSoQvNIf4kNrVGWqInTneg0BTW9LWQtUpEQ-WKbtbo-zwRyFwPPxP8BXYrSVWGvj2jmRRaSc5BJfhXi0wN0pouqaA5qPFK3U_GleFEPnkycLN7eze-72K9dGakS8gKqmWrup0Dh5xqqO56aG6gmSxLKjL0sqs0lC262EJaa0Dd3j_Bwgja4h5SXUjxLp-lfIWvwEw6H_t8GsBYvAM_1-XmEurtu8agwWByeKZvqKnugKZdN5r4tNDokdO8aixhmCNim5sc0KyrvMHOZX3oPsYBdW2ft9uU8wo91qIpozqgxWUGld5xMHmwgvP4BlzmsMw6Jfsb4hs7GgWZ0y0Hb0Wm17G72Vqp5FLFN4SQS5NpZ8IYc4H0JswfpYRcazI7m7jgM783Ga1sRq_OZH4uJ2Q-RL2JG4bgpteaLP63J9jCJaiSFpmZmf3JMsF6DSUkODaPGTBac53gRBwNSmstX3YixbFWNVhYyTpf45hRXplVvcnMfzwvqBmoso9uqPghZXmW5Or0qk4OIgM1k7XQOHZsu4FxvMdbHNueN4wCEo4CL_BCzx35Ft4ZikTDIAq9kU2CyHX9yDta-E_jT4Z-6PpO4DgR8T3H80MLm0kzg7Zsj4TmZDj-BWiHT38?type=png)](https://mermaid.live/edit#pako:eNqdlF1vmzAUhv-K5d60GknNZ4BFkZqPSvuIVrW72tiFA8cEzdiRMW2yJP99DhCW3kWDG3z8vC_nHDje41RmgGM8GAwSoQvNIf4kNrVGWqInTneg0BTW9LWQtUpEQ-WKbtbo-zwRyFwPPxP8BXYrSVWGvj2jmRRaSc5BJfhXi0wN0pouqaA5qPFK3U_GleFEPnkycLN7eze-72K9dGakS8gKqmWrup0Dh5xqqO56aG6gmSxLKjL0sqs0lC262EJaa0Dd3j_Bwgja4h5SXUjxLp-lfIWvwEw6H_t8GsBYvAM_1-XmEurtu8agwWByeKZvqKnugKZdN5r4tNDokdO8aixhmCNim5sc0KyrvMHOZX3oPsYBdW2ft9uU8wo91qIpozqgxWUGld5xMHmwgvP4BlzmsMw6Jfsb4hs7GgWZ0y0Hb0Wm17G72Vqp5FLFN4SQS5NpZ8IYc4H0JswfpYRcazI7m7jgM783Ga1sRq_OZH4uJ2Q-RL2JG4bgpteaLP63J9jCJaiSFpmZmf3JMsF6DSUkODaPGTBac53gRBwNSmstX3YixbFWNVhYyTpf45hRXplVvcnMfzwvqBmoso9uqPghZXmW5Or0qk4OIgM1k7XQOHZsu4FxvMdbHNueN4wCEo4CL_BCzx35Ft4ZikTDIAq9kU2CyHX9yDta-E_jT4Z-6PpO4DgR8T3H80MLm0kzg7Zsj4TmZDj-BWiHT38)

> **Flow:**
>
> 1. **Input devices:** the player presses keys or buttons.
> 2. **Input manager:** turns them into command bits, such as `MOVE_UP | FIRE`.
> 3. **Mediator:** passes the command to each colleague: the player (through `ExecuteCommand()`) and the HUD.
> 4. **Player and HUD:** the player performs the action, or several in the same frame; the HUD shows which commands are active.

---

## Commands as bit flags

Every command gets its own bit, so one number can hold any mix of them:

```c
typedef enum
{
    NONE        = 0,        // No command
    MOVE_UP     = 1 << 0,   // Binary: 000001
    MOVE_DOWN   = 1 << 1,   // Binary: 000010
    MOVE_LEFT   = 1 << 2,   // Binary: 000100
    MOVE_RIGHT  = 1 << 3,   // Binary: 001000
    JUMP        = 1 << 4,   // Binary: 010000
    FIRE        = 1 << 5,   // Binary: 100000
    // Add new commands here, e.g. DODGE, DASH
    COUNT       = 6         // How many commands there are: NOT a flag
} Command;
```

> **`COUNT` is a number, not a flag.** It's 6, which in binary is `000110`, the same bits as `MOVE_DOWN | MOVE_LEFT`. Never OR it into a command. When you add a command, increase `COUNT` by one.

### Combining commands

**Example:** the player moves **left** and **jumps** at the same time:

```c
Command combo = MOVE_LEFT | JUMP;
// Binary: 000100 | 010000 = 010100
// Result: both bits are set

if (IsCommandActive(combo, MOVE_LEFT))   // TRUE: bit 2 is set
    MoveLeft(player);

if (IsCommandActive(combo, JUMP))        // TRUE: bit 4 is set
    Jump(player);
```

[![](https://mermaid.ink/img/pako:eNqtlFtvmzAYhv-K69xsEmk5mYMVVcqBVKqaJWq7XWxMk4GPBA1wZMyaLOt_nyE0TTJN6kW5QLyfv_d5fRDe4ZgngCleCrZeocdJWCL1DL-FeCGgqigahvj7vjh6LT4shuPgMLB_j9XwbP4l-HEXTB8Hkbi6HmUlE1uKdF03dL0tTSDOCpZTZB_cE-W7_TxbnFqU4dxiOGeJgXKOMvmUVYDm92i-BsFkxsvWtg9FfzrUmXWqrPdQ1blss_6ZnrnXIy5XKMpkhZgAVIG8OOPcNFNomljcJFcINhDXEi5a-6CSgpfL6xn_BXeQyg8fB1ddqR0ffpqc9N3Wxfqo5yxriPr9azTuDqMV3XGNWxF0-3ksglZMu0W34uaYWcltDoqcZnlOe1GUQBppTfxPoD3Dd53E7GT_KUvkilrrjRbznAvaa3b1CDJ6D8i4g6Rp6sf2AZISNzXcU4j5X8jkPSDBC8QCkpIDxI2MlL15OdMOEhMw2CuEEC8y07dCbjoII4nD3APEBDexzmZin0CwhgsQBcsS9YPvGmSI5QoKCDFVn-qYmPoBQhyWz6qV1ZI_bMsYUylq0LDg9XKFacrySql6nTAJk4ypi6I4VNes_Mp58WJZiiaqs0OZgBjzupSYmqbRNmO6wxtMDdu-9B3dcx3bsT3bcomGt6pL9y8d37NdQ3d8yyK-_azh3y1fvySeRUzHND1fJ45HXA1DkkkuZvv7q73Gnv8CVgR0WA?type=png)](https://mermaid.live/edit#pako:eNqtlFtvmzAYhv-K69xsEmk5mYMVVcqBVKqaJWq7XWxMk4GPBA1wZMyaLOt_nyE0TTJN6kW5QLyfv_d5fRDe4ZgngCleCrZeocdJWCL1DL-FeCGgqigahvj7vjh6LT4shuPgMLB_j9XwbP4l-HEXTB8Hkbi6HmUlE1uKdF03dL0tTSDOCpZTZB_cE-W7_TxbnFqU4dxiOGeJgXKOMvmUVYDm92i-BsFkxsvWtg9FfzrUmXWqrPdQ1blss_6ZnrnXIy5XKMpkhZgAVIG8OOPcNFNomljcJFcINhDXEi5a-6CSgpfL6xn_BXeQyg8fB1ddqR0ffpqc9N3Wxfqo5yxriPr9azTuDqMV3XGNWxF0-3ksglZMu0W34uaYWcltDoqcZnlOe1GUQBppTfxPoD3Dd53E7GT_KUvkilrrjRbznAvaa3b1CDJ6D8i4g6Rp6sf2AZISNzXcU4j5X8jkPSDBC8QCkpIDxI2MlL15OdMOEhMw2CuEEC8y07dCbjoII4nD3APEBDexzmZin0CwhgsQBcsS9YPvGmSI5QoKCDFVn-qYmPoBQhyWz6qV1ZI_bMsYUylq0LDg9XKFacrySql6nTAJk4ypi6I4VNes_Mp58WJZiiaqs0OZgBjzupSYmqbRNmO6wxtMDdu-9B3dcx3bsT3bcomGt6pL9y8d37NdQ3d8yyK-_azh3y1fvySeRUzHND1fJ45HXA1DkkkuZvv7q73Gnv8CVgR0WA)

> **What's happening?**
>
> 1. The player presses **A** (MOVE_LEFT) and **SPACE** (JUMP) at the same time.
> 2. Each command has its own bit.
> 3. `|` (OR) combines them: `000100 | 010000 = 010100`.
> 4. Both bits are set, so both actions happen in the same frame.

> **Why not `if (command == JUMP)`?** Because `010100` is not equal to `010000`. The moment the player also moves, `==` says "no jump". Always test single bits with `IsCommandActive()`, which uses `&` (AND).

---

## The tasks

Already working in the starter code:

- [x] MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT (WASD or arrow keys, D-pad)
- [x] JUMP (SPACE, or the bottom face button)
- [x] FIRE (left mouse button, or the right trigger)
- [x] Two combos: MOVE_UP + FIRE (`MoveUpFire`) and JUMP + FIRE (`JumpFire`)
- [x] Idle "breathing" when there's no input
- [x] Frame-rate independent movement: held actions move by `speed * deltaTime`
- [x] A HUD showing the command bits, the active commands and a jump counter

### Task 1: Explore the starter code

Run it, then find:

1. in `input_manager.c`, where a key press sets a bit,
2. in `command.c`, where `ExecuteCommand()` checks each bit,
3. in `command.c`, how the two combos are checked **before** the single commands, and why they use `else if`,
4. in `player.c`, the function that finally moves the player,
5. in `command.h`, which bit each command uses,
6. in `mediator.c`, the two colleagues the command is passed to. Check that `player.c` and `hud.c` never mention each other.

Then hold **two** keys and watch the console: `PollInput()` logs the command bits each time they change, e.g. `Command Bits: 010100 (20)` for MOVE_LEFT + JUMP.

### Task 2: Add DODGE

Each new command needs a change in **four** places. Miss one and it silently does nothing.

1. **`command.h`:** give it the next free bit, and update `COUNT`:

   ```c
   DODGE = 1 << 6,   // Binary: 1000000
   COUNT = 7
   ```

2. **`player.h` / `player.c`:** write what the player does:

   ```c
   void Dodge(Player *player, float deltaTime);
   ```

   A dodge happens once per press, like `Jump()`, so move a fixed distance and ignore `deltaTime` with `(void)deltaTime;`. Something held every frame, like `MoveLeft()`, uses `speed * deltaTime` instead.

3. **`input_manager.c`:** set the bit when the key is pressed:

   ```c
   if (IsKeyPressed(KEY_LEFT_SHIFT))
       command |= DODGE;
   ```

4. **`command.c`:** in `ExecuteCommand()`, call the player function:

   ```c
   if (IsCommandActive(command, DODGE))
       Dodge(player, deltaTime);
   ```

   Put it with the movement checks, **outside** the combo `if` / `else if` / `else`, so a dodge isn't skipped when a combo runs.

5. **`hud.c` (optional):** show it on screen, by adding a line to `DrawHud()`:

   ```c
   DrawActiveCommand(hud->lastCommand, DODGE, "DODGE", &y, PURPLE);
   ```

   The HUD already shows the new bit in its `Command:` line, because it prints every bit up to `COUNT`.

> **`IsKeyPressed` or `IsKeyDown`?** `IsKeyPressed` is true **once**, on the frame the key goes down: right for a dodge. `IsKeyDown` is true **every frame** the key is held: right for movement. Use the wrong one and the dodge fires 60 times a second, or movement stutters.

### Task 3: Add DASH, with a controller button too

Add DASH the same way as DODGE, using bit 7. This time bind it to a key **and** a controller button, so either one works:

```c
if (IsKeyPressed(KEY_E) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT))
    command |= DASH;
```

`command.c` and the player don't care which device set the bit. That's the point of the pattern.

### Task 4: Combine commands

Try these combinations and check that both actions happen in the same frame:

- MOVE_RIGHT + FIRE
- MOVE_LEFT + JUMP
- MOVE_UP + DODGE

Do any of them look wrong? For example, should you be able to dodge in mid-air? Decide, and make `ExecuteCommand()` enforce it.

### Task 5: A simple combo

Make a sequence of inputs trigger something special:

- **Double-tap D** within 0.3 seconds: DODGE. Store the time of the last tap with `GetTime()` and compare.
- **Your own combo**, such as DASH + FIRE, following how `JumpFire` is handled in `ExecuteCommand()`.

Combos need memory between frames, so keep the extra state, such as the last tap time, in the input manager or the player, not in a local variable.

### Task 6: A third colleague

Give the mediator one more thing to tell. For example, a **sound** that plays on JUMP, or a **score** that goes up on FIRE:

1. Create `sound.c` / `sound.h` (or `score.c` / `score.h`) with a struct and an `Update…()` function that takes a `Command`.
2. Add a pointer to it in the `Mediator` struct, and pass it to `CreateMediator()`.
3. Call its `Update…()` in `MediatorHandleCommand()`.

The player and the HUD shouldn't change at all. If you find yourself editing `player.c`, stop and rethink.

> Sound needs `InitAudioDevice()` after `InitWindow()`, and `LoadSound()` with a file in `resources/`.

### Task 7: Stretch goals

- **Smooth jump:** `Jump()` moves the player up instantly. Make it rise and fall over time instead, using a vertical speed, gravity and `deltaTime`. Test it with `SetTargetFPS(30)`: the jump should look the same at any frame rate.
- **Record and replay:** save each frame's command in an array, then feed the array back through `MediatorHandleCommand()`. The player repeats your moves exactly, because it only ever sees commands.
- **Configurable controls:** load the key for each command from a file, so players can remap them.
- **Function pointers:** store one function pointer per command bit, so `ExecuteCommand()` loops through a table instead of a list of `if`s.

---

## Cheat sheet

```c
// Bit flags
Command c = NONE;
c |= JUMP;                     // turn a bit ON
c &= ~JUMP;                    // turn a bit OFF
c ^= JUMP;                     // flip a bit
if (c & JUMP) { }              // is the bit on? (what IsCommandActive does)
if (c == NONE) { }             // no commands at all this frame

// Keyboard and mouse
IsKeyDown(KEY_A);              // true every frame the key is held
IsKeyPressed(KEY_SPACE);       // true once, on the frame it goes down
IsKeyReleased(KEY_SPACE);      // true once, on the frame it comes up
IsMouseButtonDown(MOUSE_BUTTON_LEFT);

// Controller (0 = first controller)
IsGamepadAvailable(0);
IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT);      // D-pad left
IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);  // A on Xbox, Cross on PlayStation
GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_TRIGGER);      // how far the trigger is pulled

// Timing (for combos)
double now = GetTime();        // seconds since the game started
```

To print a command in binary while testing:

```c
char bits[COUNT + 1];
GetCommandBits(command, bits);
printf("Command: %s\n", bits);   // e.g. MOVE_LEFT | JUMP -> 010100
```

---

## When things go wrong

Don't panic. Nine times out of ten it's one of these:

| You see… | What's going on | Fix |
|---|---|---|
| `raylib.h: No such file or directory` | make can't find raylib | Run the setup script again (Step 2) |
| New command does nothing | One of the four places is missing | Check `command.h`, `player.c`, `input_manager.c` **and** `command.c` (Task 2) |
| New command does nothing during a combo | Its check is inside the combo's `else` | Move it outside the combo `if` / `else if` / `else` |
| The terminal scrolls non-stop | A `printf` or `TraceLog` runs every frame, e.g. in a held action like `MoveLeft()` | Only log when something changes, like `PollInput()` does with `previousCommand` |
| Two commands always happen together | They share the same bit | Give each command its own `1 << n` |
| Action stops working when you also move | Testing with `==` instead of `&` | Use `IsCommandActive()` |
| Dodge fires every frame while the key is held | `IsKeyDown` used for a one-off action | Use `IsKeyPressed` |
| Movement is jerky | `IsKeyPressed` used for movement | Use `IsKeyDown` |
| Controller does nothing | It isn't controller 0, or it was plugged in after the game started | Plug it in first, and check `IsGamepadAvailable(0)` |
| `undefined reference to 'Dodge'` | Declared in `player.h` but not written in `player.c` | Add the function body |
| `Emscripten not found in …` (Linux) | Emscripten isn't in `~/emsdk` | Run the setup script (Step 2) |
| `emcc: No such file or directory` (Windows) | Emscripten isn't loaded in this terminal | Close w64devkit and open it again |
| `Address already in use` | A web server from earlier is still running | `make stop_web`, or use another port: `make run_web WEB_PORT=8080` |
| Browser says it can't connect | The page opened before the server was ready | Refresh the page |
| The program still won't build | Old leftover files | `make clean`, then `make` again |

Still stuck? Read the **first** error message, not the last. The first one is usually the real problem; the rest are just complaining about it.

---

## Further reading

- [Command](https://gameprogrammingpatterns.com/command.html) in *Game Programming Patterns* by Robert Nystrom: the classic explanation, written for games
- [Command](https://refactoring.guru/design-patterns/command) and [Mediator](https://refactoring.guru/design-patterns/mediator) on Refactoring.Guru: diagrams and plain-English explanations
- [raylib cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html) and [examples](https://www.raylib.com/examples.html): see the input examples for keyboard and gamepad

---

## Before you submit

- [ ] It compiles and runs with no errors or warnings
- [ ] DODGE and DASH work, each with its own bit, and `COUNT` is updated
- [ ] DASH works from both the keyboard and a controller
- [ ] Combined commands work in the same frame
- [ ] At least one new combo works
- [ ] The mediator has a third colleague, and the player and HUD were not changed to add it
- [ ] The input manager still knows nothing about the player
- [ ] Your code is commented and tidy

Commit early and commit often. Your deadline is one command nobody can remap.

Best of luck. Now go make something that responds.