Toadturle, by EmmyMalware

A classic malware, overwriting files and just fucking with the systen!

Name come from : My favorite viruses, Toadie and Kaze.3228 (kaze is the behavor of the virus)
Language : C++
Platforms: x86 (XP+), x64 (Vista+)
Encoding : ANSI, not wWinMain
Issues with compiler : I had issues with the 141_xp compiler for x64, WinSDK 10.0 for x64 btw.
What it does: At startup, it creates a Key in the registry (HKEY_CURRENT_USER\Toadturle\Infect)
It will read for the key. If it doesn't exist, it creates it.

Now for the WinMain part... 
It shows a message box using GetUserNameA to get the user, and overwrites all files in the current directory it has been run from with "I like Turtles" (hardcoded)
Then if the day is September 12th (my birthday), it will show a message box and begins to overwrite all files in the Windows dir with again, "I like Turtles"
After 1 minute, it uses RtlAdjustPrivilage to raise the shutdown to the process, and uses NtShutdownSystem to shut it down. This was a fun virus to write, took me
2 days to write.

Thank you for reading, EmmyMalware signing off at 1:33 am.

Emmy needs to sleep.