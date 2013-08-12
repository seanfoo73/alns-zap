Notes:

1.  The project was created in 4.2
2.  The game runs visually okay on ios5 retina
3.  A change was made to CCImage.mm (cocos2dx) in order to compile for ios 5.
		The change can be found in \alns-zap\ZapIOS\ZapIOS\libs\cocos2dx\platform\ios\CCImage.mm
		line 393-400.  The call for ios 7 is just commented out.  TODO:  Implement a way to figure out 
		what device you're on and then make the appropriate call.