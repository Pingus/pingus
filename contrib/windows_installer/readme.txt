This is the Pingus Windows Installer

The installer, when built in release mode, can be concatenated with a zip that you use to install and executed. The command line for doing this in DOS is:

copy /b installer.exe + pingus.zip Pingus-Setup.exe

Its not too badly written so it should be possible to tweak the code as required. The additional overhead of the installer is 20.5Kb

The things the installer does not do:
  Cancel an active install
  Allow you to uninstall Pingus

Written by Neil Mitchell, neil__mitchell@hotmail.com for help if required, or the Pingus mailing list.
