## 1. Hooking the game's code
This first project is about hooking code into the game's executable.\
Unlike what you might expect, this part is rather simple.

---

#### Knowledge Requirement : Understanding the basics of DLLs
To understand how this works, you'll need to know what shared 
pre-compiled libraries and export tables are, and know how a
program goes from obtaining the path to a DLL to calling
functions contained by that same DLL. Look it up on google,
it takes 10 minutes to get to know about all that.

---

### The exploit
This exploit is due to Avalanche Engine, rather than Just Cause 3.\
It involves creating a malicious proxy dll that the game will load
instead of the expected one, thus allowing our DLL to execute 
any code within the game's process before passing the call to the
actual DLL and returning its result.

### Why does it work ?
It works due to a programming oversight, as the game uses microsoft's
win32 API function
[LoadLibrary](https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-loadlibrarya)
to load the 
[XInput](https://learn.microsoft.com/en-us/windows/win32/xinput/getting-started-with-xinput#introduction-to-xinput)
library for XBox controller input incorrectly, passing 
``"XInput_9_1_0.dll"`` as an argument instead of 
``"C:\\Windows\\System32\\XInput9_1_0.dll"``. This results with the
win32 API looking for that file first within the game folder, and only
after that within PATH defined folders. There's no checksum or any 
file verification involved with the loading of that DLL, so you can
make the game load whatever you want really, as long as it has a
compatible export table.

### Making it happen
All you have to do involves create a dll with compatible exports to
``XInput_9_1_0``, copying it to the game's directory, having code within your 
[XInputGetState](https://learn.microsoft.com/en-us/windows/win32/api/xinput/nf-xinput-xinputgetstate)
implementation interact with the game application, as the game will call this every frame,
and forwarding XInput calls to the actual ``XInput_9_1_0.dll``, which
your proxy dll will load.

<h4> 
Everything explained above is demonstrated by the code within
this project. It's fairly easy to understand, as well as fully 
commented, so I won't go into any further details as to how it works. 
</h4>