xboxToVJoy
==========

Translates data from Xbox 360 Controllers using XInput to a virtual DirectInput controller using VJoy. This gives us a better DirectInput interface to an Xbox 360 Controller (or other XInput compatible controllers), so that it can be used with older games and emulators that only support DirectInput.

This has a number of important advantages over using the normal DirectInput interface to an Xbox 360 controller, or even using one of the alternative Xbox 360 controller drivers:

1. The triggers on the controller can be used simultaneously! In my opinion this is the most important improvement because not being able to use the triggers at the same time is simply inexcusable. (Thanks to Microsoft for asininely screwing that one up.)
2. Since the DInput controllers are virtual, they are always "connected". This substantially improves things for games that require all controllers to be connected when they start up and will permenantly lose their connection to the controllers if they become disconnected. (e.g. You have to change its batteries, or it disconnects accidentally.)
3. We can get access to the Guide / Home button and map it to a DirectInput button.
4. In theory it works in all recent versions of Windows. (Currently tested in 8.1)
5. Doesn't require installing alternative drivers for your Xbox 360 controller, which I was not able to get to work with a wireless Xbox 360 controller in Windows 8.
6. The code here can be tweaked to alter the behavior of a controller.

This application requires VJoy to be installed and up to four virtual controllers to be created that all have 11 buttons, one continuous POV hat switch, and the follow axes: X, Y, Z, RX, RY, RZ. The left and right triggers will be mapped to +Z and +RZ axes respectively.

When launched, it will run in the system tray. The system tray menu has an option for bringing up a window that allows each button on the virtual controller to be pressed using the mouse. This allows games (particularly emulators) to be configured to use the virtual controller, since they automatically detect button presses and will often detect an Xbox 360 controller's button pressed before the virtual controller.

This program also properly correlates the vJoy devices as listed by DirectInput with each connected Xbox controller, so that the Xbox controller for player 1 maps to the first vJoy device, player 2 maps to the second vJoy device, and so on.

**Useful tip**: combine xboxToVJoy with [devreorder](https://github.com/briankendall/devreorder) so that all of your old DirectInput games only see the vJoy devices. This way their controller bindings will always work regardless of how Windows arbitrarily changes the order of your controllers! It's the best way I know of to have your Xinput controllers "just work" with DirectInput games!!

Dependencies: Qt 5.x, vJoy 2.1.6 SDK, DirectX 8 SDK, XInput

Requirements: vJoy 2.1.6, XInput
