# Optimizer Tree

Windows optimizer utility, native C++ Win32.

## Build without installing anything on your own PC

1. Create a GitHub repository named `OptimizerTree`.
2. Upload this whole folder structure.
3. Open **Actions** → **Build Optimizer Tree** → **Run workflow**.
4. Wait for the Windows runner to finish.
5. Download the `OptimizerTree` artifact.
6. Inside it is `OptimizerTree.exe`.

The final app is a native Windows EXE. The end-user does not need Python or .NET Runtime.

## Local build (optional)

Use a Visual Studio Developer Command Prompt on Windows:

```bat
build.bat
```
