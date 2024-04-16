### This project is a container for technical demonstrations on hacking the game
From hooking code into the game's process to displaying on-screen info 
and interacting with the game environment, these demonstrate all I know
about hacking this game.

## Contents

| Project                                   | Description                                                                                                                                                                                                                                                      |
|-------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| 0. Shared Code                            | Code used within other projects.                                                                                                                                                                                                                                 |
| 1. Hooking the game                       | Rerouting the game's script thread to third-party code.                                                                                                                                                                                                          |
| 2. Intercepting threads                   | Intercepting the creation and destruction of game threads.                                                                                                                                                                                                       |
| 3. Taking over the game's window          | Internally manipulating the game's window.                                                                                                                                                                                                                       |
| 3. Intercepting win32 API window messages | Hooking the game window's [window procedure](https://learn.microsoft.com/en-us/windows/win32/winmsg/window-procedures) to proxy, block and modify the [window messages](https://learn.microsoft.com/en-us/windows/win32/learnwin32/window-messages) it receives. |
