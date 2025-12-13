# DLL Injection
A basic DLL injection demo project. 

## Injector
The injector injects based off window names.
It looks for a window under the Desktop with a given ClassName (given through command line arguments).
After finding the window it allocates the path of the given DLL (also given though command line args) to the process memory.
> [!TIP]
> It is advised to use absolute paths, to avoid relying on working directories.

That DLL is loaded by a thread in the remote process.

